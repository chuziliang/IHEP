import uproot
import numpy as np
import matplotlib.pyplot as plt
import os
from tqdm import tqdm
from scipy.optimize import curve_fit
import scipy.stats as stats
from matplotlib.backends.backend_pdf import PdfPages
import pandas as pd
import argparse
import sys

def gaussian(x, amplitude, mean, sigma):
    """高斯函数定义"""
    return amplitude * np.exp(-(x - mean)**2 / (2 * sigma**2))

def fit_gaussian(charges):
    """用高斯函数拟合charges数组，返回mean和sigma"""
    if len(charges) < 10:  # 需要足够的数据点进行拟合
        return np.mean(charges), np.std(charges)
    
    try:
        # 创建直方图数据
        # hist, bin_edges = np.histogram(charges, bins=1024, range=(0, 1024), density=False)
        # bin_centers = (bin_edges[:-1] + bin_edges[1:]) / 2
        
        # # 初始参数估计
        # initial_amplitude = np.max(hist)
        # initial_mean = np.mean(charges)
        # initial_sigma = np.std(charges)

        # bounds = ([0, -np.inf, 0], [np.inf, np.inf, np.inf])
        
        # # 高斯拟合
        # popt, _ = curve_fit(gaussian, bin_centers, hist, 
        #                    p0=[initial_amplitude, initial_mean, initial_sigma],
        #                    bounds=bounds,maxfev=10000)

        mu, sigma = stats.norm.fit(charges[charges<200])
        
        return mu, sigma  # 返回mean和sigma
        
    except Exception as e:
        print(f"高斯拟合失败: {str(e)}")
        return np.mean(charges), np.std(charges)

def plot_channel_distribution(charges, channel, fit_mean, fit_sigma, ax):
    """绘制单个通道的分布图和拟合结果"""
    if len(charges) == 0:
        return
    
    # 绘制直方图
    # n, bins, patches = ax.hist(charges, bins=range(0, 1024, 1), range=(0, 1024), alpha=0.7, color='skyblue', density=False)
    hist, bin_edges = np.histogram(charges, bins=range(0, 1024, 1), range=(0, 1024), density=False)
    ax.step(bin_edges[:-1], hist, where='post', linewidth=2, color='blue')
    
    # 生成拟合曲线
    x = np.linspace(np.min(charges), np.max(charges), 1000)
    y = len(charges)*stats.norm.pdf(x, fit_mean, fit_sigma)
    
    # 绘制拟合曲线
    ax.plot(x, y, 'r-', linewidth=2, label=f'Gaussian Fit: μ={fit_mean:.2f}, σ={fit_sigma:.2f}')
    
    # 添加统计信息
    actual_mean = np.mean(charges)
    actual_std = np.std(charges)
    ax.axvline(actual_mean, color='green', linestyle='--', linewidth=1, 
              label=f'Actual Mean: {actual_mean:.2f}')
    ax.axvline(actual_mean + actual_std, color='orange', linestyle=':', linewidth=1)
    ax.axvline(actual_mean - actual_std, color='orange', linestyle=':', linewidth=1,
              label=f'Actual Std: {actual_std:.2f}')
    
    # 设置图表属性
    ax.set_title(f'Channel {channel} - Charge Distribution', fontsize=12)
    ax.set_xlabel('Charge Value', fontsize=10)
    ax.set_ylabel('Probability Density', fontsize=10)
    ax.set_yscale('log')
    ax.set_ylim(0.1, 10*np.max(hist))
    ax.grid(True, linestyle='--', alpha=0.7)
    ax.legend(fontsize=8)
    
    # 添加文本框显示统计信息
    textstr = f'Number of Data Points: {len(charges)}\nFit μ: {fit_mean:.2f}\nFit σ: {fit_sigma:.2f}\nActual μ: {actual_mean:.2f}\nActual σ: {actual_std:.2f}'
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
    ax.text(0.05, 0.95, textstr, transform=ax.transAxes, fontsize=8,
            verticalalignment='top', bbox=props)

def plot_time_sequence(time_sequence, ax, title="Time Sequence Plot", xlabel="Index", ylabel="Time Value", figsize=(10, 6)):
    """
    绘制时间序列变化图
    
    参数:
        time_sequence: 时间序列数据（列表或数组）
        title: 图表标题（默认"Time Sequence Plot"）
        xlabel: x轴标签（默认"Index"）
        ylabel: y轴标签（默认"Time Value"）
        figsize: 图表大小（默认(10, 6)）
    """
    
    # 绘制时间序列
    ax.plot(time_sequence[0:5000], marker='o', markersize=4, linestyle='-', linewidth=1)
    
    # 添加标签和标题
    ax.set_title(title, fontsize=14)
    ax.set_xlabel(xlabel, fontsize=12)
    ax.set_ylabel(ylabel, fontsize=12)
    
    # 添加网格线
    ax.grid(True, linestyle='--', alpha=0.7)
    
    # 自动调整布局
    # plt.tight_layout()
    
    # 显示图表
    # plt.show()

def process_root_file(file_path):
    """处理单个ROOT文件，返回所有通道的RMS值和高斯拟合参数，并生成PDF"""

    max_coarse_time = pow(2,26)

    # 根据file_path得到GCU号
    gcu_id = os.path.basename(file_path).split('_')[0]

    # 读取文档BELLOW2GCUABCHVS.map，根据GCU号获得ABC号
    with open('BELLOW2GCUABCHVS.map', 'r') as f:
        for line in f:
            line = line.split()
            # print(line[6])
            if gcu_id in line[6]:
                abc_id = line[1]
                # 去掉开头的0
                abc_id = abc_id.lstrip('0')
                break

    # 根据ABC号确定文档名称
    Calib_File_name = f'/junofs/users/chuziliang125/ABCCalibFactor/database_all/ABC_database_{abc_id}/calib_info.txt'
    PED_File_name = f'/junofs/users/chuziliang125/ABCCalibFactor/database_all/ABC_database_{abc_id}/Pedestal_info.txt'

    try:
        # 打开ROOT文件
        with uproot.open(file_path) as file:
            # 获取EventData树
            tree = file["EventData"]
            
            # 读取数据
            data = tree.arrays(["ABCChannelNumber", "Charge", "Sign", "Gain", "CoarseTime"], library="np")
            
            # 获取唯一通道号
            unique_channels = np.unique(data["ABCChannelNumber"])
            
            # 计算每个通道的RMS和高斯拟合参数
            channel_rms = {}
            channel_sigma = {}
            channel_mean = {}
            channle_gain = {}
            channel_time = {}
            channel_rate = {}
            channel_rate_err = {}
            channel_gain_err = {}
            channel_PED_mean = {}
            channel_PED_sigma = {}
            channel_P1 = {}
            channel_P0 = {}
            channel_P1_err = {}
            channel_P0_err = {}

            # 读取文档，获得每个通道的刻度参数
            try:
                LP2i_data = pd.read_csv(Calib_File_name, sep='\s+', skiprows=1, usecols=['Channel', 'P1_HG_ping','P0_HG_ping','P1_err_HG_ping','P0_err_HG_ping'])
                # 创建LP2i数据的字典
                for index, row in LP2i_data.iterrows():
                    channel_P1[int(row['Channel'])] = float(row['P1_HG_ping'])
                    channel_P0[int(row['Channel'])] = float(row['P0_HG_ping'])
                    channel_P1_err[int(row['Channel'])] = float(row['P1_err_HG_ping'])
                    channel_P0_err[int(row['Channel'])] = float(row['P0_err_HG_ping'])
                print(f"成功读取LP2i文件")
            except Exception as e:
                print(f"读取LP2i文件时出错: {str(e)}")

            try:
                LP2i_PED_data = pd.read_csv(PED_File_name, sep='\s+', skiprows=1, usecols=['Channel', 'Mean_HG_ping','Sigma_HG_ping'])
                # 创建LP2i数据的字典
                for index, row in LP2i_PED_data.iterrows():
                    channel_PED_mean[int(row['Channel'])] = float(row['Mean_HG_ping'])
                    channel_PED_sigma[int(row['Channel'])] = float(row['Sigma_HG_ping'])
                print(f"成功读取PED文件")
            except Exception as e:
                print(f"读取PED文件时出错: {str(e)}")
            
            # 创建PDF文件
            base_name = os.path.splitext(os.path.basename(file_path))[0]
            pdf_path = f"{base_name}_channel_distributions.pdf"
            time_pdf_path = f"{base_name}_time_distributions.pdf"
            
            with PdfPages(pdf_path) as pdf, PdfPages(time_pdf_path) as time_pdf:
                print(f"为文件 {base_name} 生成PDF: {pdf_path} 和 {time_pdf_path}")
                
                for channel in tqdm(unique_channels, desc=f"处理通道 ({base_name})"):
                    # 选择当前通道的Charge值
                    mask = (data["ABCChannelNumber"] == channel)
                    charges = data["Charge"][mask]
                    time = data["CoarseTime"][mask]
                    sign = data["Sign"][mask]
                    gain = data["Gain"][mask]

                    # 统计time的循环次数
                    cycle_count = 0
                    # prev_3 = time[0]
                    # prev_2 = time[1]
                    # prev_1 = time[2]

                    # after_1 = time[4]
                    # after_2 = time[5]
                    # after_3 = time[6]

                    # threshold = max_coarse_time * 0.9
                    
                    for i,v in enumerate(time[3:len(time)-3]):
                        # 回绕条件：当前值突然变小，且前值接近最大值，当前值接近0
                        if time[i] < time[i-1] and time[i-1]>time[i-2] and time[i-2]>time[i-3] and time[i+1]>time[i] and time[i+2]>time[i+1] and time[i+3]>time[i+2]:
                            cycle_count += 1
                        # prev = current
                    
                    # # 计算相邻元素的差值
                    # diffs = np.diff(time)
                    
                    # # 找到所有负差值的位置（可能回绕点）
                    # negative_diff_indices = np.where(diffs < 0)[0]
                    
                    # # 验证这些点是否是真的回绕（前值接近max，后值接近0）
                    # prev_values = time[negative_diff_indices]
                    # next_values = time[negative_diff_indices + 1]
                    
                    # # 应用回绕条件过滤
                    # is_wrap = (prev_values > 0.9 * max_coarse_time) & (next_values < 0.1 * max_coarse_time)
                    # cycles = np.sum(is_wrap)

                    # 计算时间间隔
                    channel_time[channel] = 25.e-9*max_coarse_time*cycle_count + 25.e-9*(time[-1]-time[0])

                    print(f"通道 {channel} 回绕次数: {cycle_count}")
                    print(f"通道 {channel} 时间间隔: {channel_time[channel]:.2e} 秒")
                    print(f"通道 {channel} 事件数: {len(charges)}")

                    # channel_rate[channel] = len(charges)/channel_time[channel]
                    channel_rate[channel] = len(charges)/600
                    channel_rate_err[channel] = np.sqrt(len(charges))/600
                    
                    # if channel > 3:
                    #     break
                    if len(charges) > 1:
                        # 计算RMS
                        rms = np.std(charges)
                        channel_rms[channel] = rms
                        
                        # 高斯拟合
                        mean, sigma = fit_gaussian(charges[(sign == 0) & (gain == 0)])
                        channel_sigma[channel] = sigma
                        channel_mean[channel] = mean
                        channle_gain[channel] = ((mean-channel_PED_mean[channel]-channel_P0[channel])/channel_P1[channel])/0.16 # 10e6 
                        mean_err = sigma/np.sqrt(len(charges[(sign == 0) & (gain == 0)]))

                        numerator_err_2  =  (mean_err**2 +  channel_P0_err[channel]**2)
                        numerator = (mean-channel_PED_mean[channel]-channel_P0[channel])

                        gain_err = channle_gain[channel]*np.sqrt(numerator_err_2/numerator**2 + channel_P1_err[channel]**2/channel_P1[channel]**2)

                        channel_gain_err[channel] = gain_err
                        
                        # 创建图形
                        fig, ax = plt.subplots(figsize=(10, 6))
                        
                        # 绘制分布图
                        plot_channel_distribution(charges, channel, mean, sigma, ax)
                        
                        # 保存到PDF
                        pdf.savefig(fig, bbox_inches='tight')
                        plt.close(fig)

                        fig_time, ax_time = plt.subplots(figsize=(10, 6))
                        
                        # 绘制分布图
                        plot_time_sequence(time,ax_time)
                        
                        # 保存到PDF
                        time_pdf.savefig(fig_time, bbox_inches='tight')
                        plt.close(fig_time)
            

                # 保存所有RMS值到CSV
                df = pd.DataFrame({
                    'Channel': list(channel_rms.keys()),
                    'RMS': list(channel_rms.values()),
                    'Sigma': list(channel_sigma.values()),
                    'Mean': list(channel_mean.values()),
                    'Rate': list(channel_rate.values()),
                    'PED_Mean': list(channel_PED_mean.values()),
                    'PED_Sigma': list(channel_PED_sigma.values()),
                    'P1': list(channel_P1.values()),
                    'P0': list(channel_P0.values()),
                    'P1_err': list(channel_P1_err.values()),
                    'P0_err': list(channel_P0_err.values()),
                    'Time': list(channel_time.values()),
                    'Gain': list(channle_gain.values()),
                    'Gain_err': list(channel_gain_err.values()),
                    'Rate_err': list(channel_rate_err.values()),
                })
                df.to_csv(f'{base_name}_channel_rms_values.csv', index=False)
            # return channel_rms, channel_sigma
    
    except Exception as e:
        print(f"处理文件 {os.path.basename(file_path)} 时出错: {str(e)}")
        # return {}, {}

def plot_rms_distribution(all_rms_values):
    """绘制所有通道RMS值的分布图"""
    if not all_rms_values:
        print("没有有效数据可绘制")
        return
    
    plt.figure(figsize=(10, 6))
    
    # 绘制直方图
    plt.hist(all_rms_values, bins=50, alpha=0.7, color='skyblue', edgecolor='black')
    
    # 添加统计信息
    mean_rms = np.mean(all_rms_values)
    median_rms = np.median(all_rms_values)
    plt.axvline(mean_rms, color='red', linestyle='dashed', linewidth=1, label=f'Mean RMS: {mean_rms:.2f}')
    plt.axvline(median_rms, color='green', linestyle='dashed', linewidth=1, label=f'Median RMS: {median_rms:.2f}')
    
    # 设置图表属性
    plt.title('All Channels RMS Value Distribution', fontsize=14)
    plt.xlabel('RMS Value', fontsize=12)
    plt.ylabel('Number of Channels', fontsize=12)
    plt.grid(True, linestyle='--', alpha=0.7)
    plt.legend()
    plt.tight_layout()
    
    # 保存并显示
    plt.savefig('channel_rms_distribution.png', dpi=300)
    plt.show()

def main():
    # 获取ROOT文件列表
    root_files = []
    
    # 添加命令行参数解析
    parser = argparse.ArgumentParser(description='Process ROOT files and generate channel distribution analysis')
    parser.add_argument('--file-list', '-f', type=str, required=True, 
                       help='Path to text file containing list of ROOT files (one per line)')
    parser.add_argument('--line-number', '-l', type=int, default=None,
                       help='Specific line number to process (1-indexed). If not provided, process all lines')
    
    args = parser.parse_args()
    
    # 读取文件列表
    try:
        with open(args.file_list, 'r') as f:
            root_files = [line.strip() for line in f if line.strip() and not line.startswith('#')]
    except FileNotFoundError:
        print(f"错误: 找不到文件列表 {args.file_list}")
        sys.exit(1)
    
    # 如果指定了行号，只处理该行
    if args.line_number is not None:
        if args.line_number < 1 or args.line_number > len(root_files):
            print(f"错误: 行号 {args.line_number} 超出范围 (1-{len(root_files)})")
            sys.exit(1)
        root_files = [root_files[args.line_number - 1]]
        print(f"处理第 {args.line_number} 行: {root_files[0]}")
    
    if not root_files:
        print("文件列表为空")
        return

    all_rms_values = []
    
    print(f"开始处理 {len(root_files)} 个ROOT文件...")
    for file_path in tqdm(root_files, desc="处理文件"):
        # 处理每个文件
        process_root_file(file_path)
        
        # 收集所有RMS值
        # all_rms_values.extend(channel_sigma.values())
    
    print(f"总共处理了 {len(all_rms_values)} 个通道的RMS值")
    
    # 绘制分布图
    # plot_rms_distribution(all_rms_values)s

if __name__ == "__main__":
    main()