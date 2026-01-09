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

def process_root_file(file_path):
    """处理单个ROOT文件，返回所有通道的RMS值和高斯拟合参数，并生成PDF"""
    try:
        # 打开ROOT文件
        with uproot.open(file_path) as file:
            # 获取EventData树
            tree = file["EventData"]

            # 输出entry数
            print(f"文件 {file_path} 中的entry数: {tree.num_entries}")
            
            # 读取数据
            data = tree.arrays(["ABCChannelNumber", "Charge", "Sign", "Gain"], library="np")
            
            # 获取唯一通道号
            unique_channels = np.unique(data["ABCChannelNumber"])
            
            # 计算每个通道的RMS和高斯拟合参数
            channel_rms = {}
            channel_sigma = {}
            
            # 创建PDF文件
            base_name = os.path.splitext(os.path.basename(file_path))[0]
            pdf_path = f"{base_name}_channel_distributions.pdf"
            
            with PdfPages(pdf_path) as pdf:
                print(f"为文件 {base_name} 生成PDF: {pdf_path}")
                
                for channel in tqdm(unique_channels, desc=f"处理通道 ({base_name})"):
                    # 选择当前通道的Charge值
                    mask = (data["ABCChannelNumber"] == channel) & (data["Sign"] == 1) & (data["Gain"] == 1)
                    charges = data["Charge"][mask]
                    
                    # if channel > 3:
                    #     break
                    if len(charges) > 1:
                        # 计算RMS
                        rms = np.std(charges)
                        channel_rms[channel] = rms
                        
                        # 高斯拟合
                        mean, sigma = fit_gaussian(charges)
                        channel_sigma[channel] = sigma
                        
                        # 创建图形
                        fig, ax = plt.subplots(figsize=(10, 6))
                        
                        # 绘制分布图
                        plot_channel_distribution(charges, channel, mean, sigma, ax)
                        
                        # 保存到PDF
                        pdf.savefig(fig, bbox_inches='tight')
                        plt.close(fig)
                    else:
                        print(f"通道 {channel} 数据点不足，跳过拟合")
            

                # 保存所有RMS值到CSV
                df = pd.DataFrame({
                    'Channel': list(channel_rms.keys()),
                    'RMS': list(channel_rms.values()),
                    'Sigma': list(channel_sigma.values())
                })
                df.to_csv(f'{base_name}_channel_rms_values.csv', index=False)
            return channel_rms, channel_sigma
    
    except Exception as e:
        print(f"处理文件 {os.path.basename(file_path)} 时出错: {str(e)}")
        return {}, {}

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
        channel_rms, channel_sigma = process_root_file(file_path)
        
        # 收集所有RMS值
        all_rms_values.extend(channel_sigma.values())
    
    print(f"总共处理了 {len(all_rms_values)} 个通道的RMS值")
    
    # 绘制分布图
    plot_rms_distribution(all_rms_values)

if __name__ == "__main__":
    main()