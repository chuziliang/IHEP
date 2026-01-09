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

def plot_comparison(channel_data, channel_data_err, channel_ref, channel_ref_err, title):
    """在同一张图中绘制root文件和LP2i文件的Mean值对比，并添加比值子图"""
    if not channel_data or not channel_ref:
        print("没有有效数据可绘制")
        return
    
    # 创建包含两个子图的图形
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 6), sharex=True)
    fig.subplots_adjust(hspace=0)
    
    # 处理data文件数据
    channels = list(channel_data.keys())
    data = list(channel_data.values())
    data_errs = list(channel_data_err.values())
    sorted_indices = np.argsort(channels)
    sorted_channels = np.array(channels)[sorted_indices]
    sorted_data = np.array(data)[sorted_indices]
    sorted_data_errs = np.array(data_errs)[sorted_indices]
    
    # 处理ref文件数据
    ref = list(channel_ref.values())
    ref_errs = list(channel_ref_err.values())
    sorted_ref = np.array(ref)[sorted_indices]
    sorted_ref_errs = np.array(ref_errs)[sorted_indices]
    
    # 第一个子图：原始Mean值对比
    ax1.errorbar(sorted_channels, sorted_data, yerr=sorted_data_errs, fmt='bo-', markersize=3, linewidth=1, alpha=0.7, label='Tested electronics')
    ax1.errorbar(sorted_channels, sorted_ref, yerr=sorted_ref_errs, fmt='ro-', markersize=3, linewidth=1, alpha=0.7, label='Ref')
    
    # 添加统计信息
    data_overall_mean = np.mean(data)
    data_overall_std = np.std(data)
    ref_overall_mean = np.mean(ref)
    ref_overall_std = np.std(ref)
    
    # ax1.axhline(data_overall_mean, color='blue', linestyle='--', linewidth=1, 
    #             label=f'JUNO Overall Mean: {data_overall_mean:.2f}')
    # ax1.axhline(ref_overall_mean, color='red', linestyle='--', linewidth=1,
    #             label=f'ref Overall Mean: {ref_overall_mean:.2f}')
    
    # 设置第一个子图属性
    # ax1.set_title('Mean Value Comparison: JUNO File vs ref File', fontsize=16)
    # ax1.set_xlabel('Channel Number', fontsize=12)
    if title == "Gain":
        ax1.set_ylabel(r'Gain [10$^{6}$]', fontsize=12)
    else:
        ax1.set_ylabel(f'Dark count rate [Hz]', fontsize=12)
    ax1.grid(True, linestyle='--', alpha=0.7)
    ax1.legend(fontsize="large")
    
    # 第二个子图：1 - data/ref 比值
    ratio_values = []
    ratio_errors = []
    
    for channel in sorted_channels:
        data_val = channel_data[channel]
        data_err = channel_data_err[channel]
        ref_val = channel_ref[channel]
        ref_err = channel_ref_err[channel]
        if ref_val != 0:  # 避免除零错误
            ratio = 100.*(1 - (data_val / ref_val))
            error = 100.*(data_val / ref_val)*np.sqrt((data_err/data_val)**2 + (ref_err/ref_val)**2)  # 1%的相对误差
            ratio_values.append(ratio)
            ratio_errors.append(error)
        else:
            ratio_values.append(0)
            ratio_errors.append(0)
    
    if ratio_values:
        # 绘制比值数据点
        ax2.errorbar(sorted_channels, ratio_values, yerr=ratio_errors, 
                    fmt='bo', markersize=4, capsize=3, alpha=0.7, label='Diff. = 1 - Tested/Ref')
        
        # 计算平均比值和误差
        mean_ratio = np.mean(ratio_values)
        std_ratio = np.std(ratio_values)
        mean_error = np.mean(ratio_errors)
        
        # 添加平均线和误差带
        ax2.axhline(0, color='red', linestyle='--', linewidth=2)
        ax2.axhline(0 + 30, color='orange', linestyle=':', linewidth=2)
        ax2.axhline(0 - 30, color='orange', linestyle=':', linewidth=2, label='30% limit')
        
        # 添加统计信息文本框
        textstr = f'Mean Diff.: {mean_ratio:.2f}%\nStd Deviation: {std_ratio:.2f}%\nN Channels: {len(sorted_channels)}'
        props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)
        ax2.text(0.02, 0.97, textstr, transform=ax2.transAxes, fontsize=10,
                verticalalignment='top', bbox=props)
    
    # 设置第二个子图属性
    # ax2.set_title('Ratio: 1 - Root_Mean / LP2i_Mean', fontsize=14)
    ax2.set_xlabel('ABC Channel', fontsize=12)
    ax2.set_ylabel('Diff. [%]', fontsize=12)
    ax2.set_ylim(-33, 33)
    ax2.grid(True, linestyle='--', alpha=0.7)
    ax2.legend(fontsize="large")
    
    # plt.tight_layout()
    
    # 保存并显示
    plt.savefig(f'{title}_comparison_with_ratio.pdf', dpi=300, bbox_inches='tight')
    # plt.show()

def main():
    # 获取ROOT文件列表
    Data_files = "/junofs/users/chuziliang125/IntegrationAnalysis/ComparePM/GU8158_SPE_channel_rms_values.csv"
    Ref_file = "/junofs/users/chuziliang125/IntegrationAnalysis/ComparePM/GU8159_SPE_channel_rms_values.csv"

    # 读取Data文件数据
    try:
        Data_data = pd.read_csv(Data_files, sep=',', usecols=['Channel', 'Gain', 'Rate', 'Gain_err', 'Rate_err'])
        # 创建Data数据的字典 
        channel_Gain_data = {}
        channel_Rate_data = {}
        channel_Gain_err_data = {}
        channel_Rate_err_data = {}
        for index, row in Data_data.iterrows():
            channel_Gain_data[int(row['Channel'])] = float(row['Gain'])
            channel_Rate_data[int(row['Channel'])] = float(row['Rate'])
            channel_Gain_err_data[int(row['Channel'])] = float(row['Gain_err'])
            channel_Rate_err_data[int(row['Channel'])] = float(row['Rate_err'])
        print(f"成功读取Data文件，包含 {len(channel_Gain_data)} 个通道")
    except Exception as e:
        print(f"读取Data文件时出错: {str(e)}")
        channel_Gain_data = {}
        channel_Rate_data = {}
        channel_Gain_err_data = {}
        channel_Rate_err_data = {}
    
    
    # 读取Ref文件数据
    try:
        Ref_data = pd.read_csv(Ref_file, sep=',', usecols=['Channel', 'Gain', 'Rate', 'Gain_err', 'Rate_err'])
        # 创建Ref数据的字典 
        channel_Gain_ref = {}
        channel_Rate_ref = {}
        channel_Gain_ref_err = {}
        channel_Rate_ref_err = {}
        for index, row in Ref_data.iterrows():
            channel_Gain_ref[int(row['Channel'])] = float(row['Gain'])
            channel_Rate_ref[int(row['Channel'])] = float(row['Rate'])
            channel_Gain_ref_err[int(row['Channel'])] = float(row['Gain_err'])
            channel_Rate_ref_err[int(row['Channel'])] = float(row['Rate_err'])
        print(f"成功读取Ref文件，包含 {len(channel_Gain_ref)} 个通道")
    except Exception as e:
        print(f"读取Ref文件时出错: {str(e)}")
        channel_Gain_ref = {}
        channel_Rate_ref = {}
        channel_Gain_ref_err = {}
        channel_Rate_ref_err = {}
    
    
    # 绘制对比图
    plot_comparison(channel_Gain_data, channel_Gain_err_data, channel_Gain_ref, channel_Gain_ref_err, 'Gain')
    plot_comparison(channel_Rate_data, channel_Rate_err_data, channel_Rate_ref, channel_Rate_ref_err, 'Rate')

if __name__ == "__main__":
    main()