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

def process_root_file(file_path):
    """处理单个ROOT文件，返回所有通道的Mean值和高斯拟合参数，并生成PDF"""
    try:
        # 打开ROOT文件
        with uproot.open(file_path) as file:
            # 获取EventData树
            tree = file["EventData"]
            
            # 读取数据
            data = tree.arrays(["ABCChannelNumber", "Charge", "Sign", "Gain"], library="np")
            
            # 获取唯一通道号
            unique_channels = np.unique(data["ABCChannelNumber"])
            
            # 计算每个通道的Mean和高斯拟合参数
            channel_mean = {}
            channel_mean_err = {}

            for channel in unique_channels:
                # 选择当前通道的Charge值
                mask = (data["ABCChannelNumber"] == channel) & (data["Sign"] == 0) & (data["Gain"] == 0)
                charges = data["Charge"][mask]
                if len(charges) > 1:
                    # 计算Mean
                    mean_val = np.mean(charges)
                    mean_err = np.std(charges) / np.sqrt(len(charges))
                    channel_mean[channel] = mean_val
                    channel_mean_err[channel] = mean_err
            
            return channel_mean, channel_mean_err
    
    except Exception as e:
        print(f"处理文件 {os.path.basename(file_path)} 时出错: {str(e)}")
        return {}, {}

def plot_mean_comparison(channel_mean_root, channel_mean_root_err, channel_mean_lp2i):
    """在同一张图中绘制root文件和LP2i文件的Mean值对比，并添加比值子图"""
    if not channel_mean_root or not channel_mean_lp2i:
        print("没有有效数据可绘制")
        return
    
    # 创建包含两个子图的图形
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), sharex=True)
    fig.subplots_adjust(hspace=0)
    
    # 处理root文件数据
    root_channels = list(channel_mean_root.keys())
    root_means = list(channel_mean_root.values())
    sorted_indices_root = np.argsort(root_channels)
    sorted_root_channels = np.array(root_channels)[sorted_indices_root]
    sorted_root_means = np.array(root_means)[sorted_indices_root]
    
    # 处理LP2i文件数据
    lp2i_channels = list(channel_mean_lp2i.keys())
    lp2i_means = list(channel_mean_lp2i.values())
    sorted_indices_lp2i = np.argsort(lp2i_channels)
    sorted_lp2i_channels = np.array(lp2i_channels)[sorted_indices_lp2i]
    sorted_lp2i_means = np.array(lp2i_means)[sorted_indices_lp2i]
    
    # 第一个子图：原始Mean值对比
    ax1.plot(sorted_root_channels, sorted_root_means, 'bo-', markersize=3, linewidth=1, alpha=0.7, label='JUNO')
    ax1.plot(sorted_lp2i_channels, sorted_lp2i_means, 'ro-', markersize=3, linewidth=1, alpha=0.7, label='LP2i')
    
    # 添加统计信息
    root_overall_mean = np.mean(root_means)
    root_overall_std = np.std(root_means)
    lp2i_overall_mean = np.mean(lp2i_means)
    lp2i_overall_std = np.std(lp2i_means)
    
    # ax1.axhline(root_overall_mean, color='blue', linestyle='--', linewidth=1, 
    #             label=f'Root Overall Mean: {root_overall_mean:.2f}')
    # ax1.axhline(lp2i_overall_mean, color='red', linestyle='--', linewidth=1,
    #             label=f'LP2i Overall Mean: {lp2i_overall_mean:.2f}')
    
    # 设置第一个子图属性
    # ax1.set_title('Mean Value Comparison: Root File vs LP2i File', fontsize=16)
    # ax1.set_xlabel('Channel Number', fontsize=12)
    ax1.set_ylabel('PED Mean [ADCu]', fontsize=12)
    ax1.grid(True, linestyle='--', alpha=0.7)
    ax1.legend(fontsize="large")
    
    # 第二个子图：1 - root_means/lp2i_means 比值
    common_channels = sorted(set(root_channels) & set(lp2i_channels))
    ratio_values = []
    ratio_errors = []
    
    for channel in common_channels:
        if channel in channel_mean_root and channel in channel_mean_lp2i:
            root_val = channel_mean_root[channel]
            lp2i_val = channel_mean_lp2i[channel]
            if lp2i_val != 0:  # 避免除零错误
                ratio = 100.*(1 - (root_val / lp2i_val))
                error = 100.*channel_mean_root_err[channel]/lp2i_val  # 1%的相对误差
                ratio_values.append(ratio)
                ratio_errors.append(error)
            else:
                ratio_values.append(0)
                ratio_errors.append(0)
    
    if ratio_values:
        # 绘制比值数据点
        ax2.errorbar(common_channels, ratio_values, yerr=ratio_errors, 
                    fmt='bo', markersize=4, capsize=3, alpha=0.7, label='Diff. = 1 - JUNO/LP2i')
        
        # 计算平均比值和误差
        mean_ratio = np.mean(ratio_values)
        std_ratio = np.std(ratio_values)
        mean_error = np.mean(ratio_errors)
        
        # 添加平均线和误差带
        ax2.axhline(0, color='red', linestyle='--', linewidth=2)
        ax2.axhline(0 + 15, color='orange', linestyle=':', linewidth=2)
        ax2.axhline(0 - 15, color='orange', linestyle=':', linewidth=2, label='15% limit')
        
        # 添加统计信息文本框
        textstr = f'Mean Diff.: {mean_ratio:.2f}%\nStd Deviation: {std_ratio:.2f}%\nN Channels: {len(common_channels)}'
        props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
        ax2.text(0.02, 0.97, textstr, transform=ax2.transAxes, fontsize=10,
                verticalalignment='top', bbox=props)
    
    # 设置第二个子图属性
    # ax2.set_title('Ratio: 1 - Root_Mean / LP2i_Mean', fontsize=14)
    ax2.set_xlabel('ABC Channel', fontsize=12)
    ax2.set_ylabel('Diff. [%]', fontsize=12)
    ax2.set_ylim(-17, 23)
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.legend(fontsize="large")
    
    # plt.tight_layout()
    
    # 保存并显示
    plt.savefig('mean_comparison_with_ratio.pdf', dpi=300, bbox_inches='tight')
    # plt.show()

def main():
    # 获取ROOT文件列表
    root_files = ["/junofs/users/chuziliang125/IntegrationAnalysis/result/GU8005_PED.root"]
    LP2i_file = "/junofs/users/chuziliang125/ABCCalibFactor/database_all/ABC_database_75/Pedestal_info.txt"

    # 读取LP2i文件数据
    try:
        LP2i_data = pd.read_csv(LP2i_file, sep='\s+', skiprows=1, usecols=['Channel', 'Mean_HG_ping'])
        # 创建LP2i数据的字典
        channel_mean_lp2i = {}
        for index, row in LP2i_data.iterrows():
            channel_mean_lp2i[int(row['Channel'])] = float(row['Mean_HG_ping'])
        print(f"成功读取LP2i文件，包含 {len(channel_mean_lp2i)} 个通道")
    except Exception as e:
        print(f"读取LP2i文件时出错: {str(e)}")
        channel_mean_lp2i = {}
    
    print(f"开始处理 {len(root_files)} 个ROOT文件...")
    for file_path in tqdm(root_files, desc="处理文件"):
        # 处理每个文件
        channel_mean_root, channel_mean_root_err = process_root_file(file_path)
        print(f"成功处理root文件，包含 {len(channel_mean_root)} 个通道")
    
    # 绘制对比图
    plot_mean_comparison(channel_mean_root, channel_mean_root_err, channel_mean_lp2i)

if __name__ == "__main__":
    main()