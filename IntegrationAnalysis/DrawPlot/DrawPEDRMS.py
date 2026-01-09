import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import glob
import os
from matplotlib.ticker import MultipleLocator


def plot_all_sigma_distribution():
    """读取所有CSV文件并绘制Sigma分布图"""
    
    # 获取所有CSV文件
    csv_files = glob.glob("*_channel_rms_values.csv")
    
    if not csv_files:
        print("没有找到CSV文件")
        return
    
    print(f"找到 {len(csv_files)} 个CSV文件")
    
    # 读取所有数据
    all_sigma_values = []
    file_info = []  # 存储文件信息用于图例
    
    for csv_file in csv_files:
        try:
            # 读取CSV文件
            df = pd.read_csv(csv_file)
            
            # 提取Sigma值
            sigma_values = df['Sigma'].dropna().values

            for sigma in sigma_values:
                if sigma == 0:
                    print(f"Sigma = 0, 文件名={csv_file}, 通道号 = {df['Channel'][sigma_values.tolist().index(sigma)]}")

                if sigma > 10:
                    print(f"Sigma = {sigma}, 文件名={csv_file}, 通道号 = {df['Channel'][sigma_values.tolist().index(sigma)]}")
            
            if len(sigma_values) > 0:
                all_sigma_values.extend(sigma_values)
                
                # 获取文件名（不含扩展名）
                file_base = os.path.splitext(os.path.basename(csv_file))[0]
                file_info.append({
                    'name': file_base,
                    'count': len(sigma_values),
                    'mean': np.mean(sigma_values),
                    'std': np.std(sigma_values)
                })
                
                print(f"文件 {file_base}: {len(sigma_values)} 个Sigma值, 均值={np.mean(sigma_values):.2f}, 标准差={np.std(sigma_values):.2f}")
            else:
                file_base = os.path.splitext(os.path.basename(csv_file))[0]
                print(f"文件 {file_base}: 没有有效的Sigma数据")
            
        except Exception as e:
            print(f"读取文件 {csv_file} 时出错: {e}")
    
    if not all_sigma_values:
        print("没有有效的Sigma数据")
        return
    
    # 创建图形
    plt.figure(figsize=(8, 6))

    bin_edges = np.arange(-0.1, 15, 0.1)
    
    # 绘制直方图
    n, bins, patches = plt.hist(all_sigma_values, bins=bin_edges, 
                               color='red', edgecolor='red',histtype='step', density=False)
    
    # 计算总体统计信息
    total_mean = np.mean(all_sigma_values)
    total_median = np.median(all_sigma_values)
    total_std = np.std(all_sigma_values)
    
    # 添加统计线
    # plt.axvline(total_mean, color='red', linestyle='dashed', linewidth=2, 
    #            label=f'Mean: {total_mean:.2f}')
    # plt.axvline(total_median, color='green', linestyle='dashed', linewidth=2,
    #            label=f'Median: {total_median:.2f}')
    
    # 设置图表属性
    # plt.title('Distribution of Sigma Values for All Channels', fontsize=16, fontweight='bold')
    plt.xlabel('PED sigma [ADCu]', fontsize=14)
    plt.ylabel('Count', fontsize=14)
    ax = plt.gca()
    ax.set_yscale('log')
    ax.xaxis.set_major_locator(MultipleLocator(5))
    ax.xaxis.set_minor_locator(MultipleLocator(1))
    ax.tick_params(axis='both', which='both', direction='in', labelsize='14')
    ax.tick_params(axis='both', which='major', length=10)
    ax.tick_params(axis='both', which='minor', length=5)
    plt.grid(True, linestyle='--', alpha=0.7)
    # plt.legend(fontsize=12)
    
    # 添加统计信息文本框
    stats_text = f"Channel Number: {len(all_sigma_values)}\n"
    stats_text += f"Mean: {total_mean:.2f}\n"
    stats_text += f"Median: {total_median:.2f}\n"
    stats_text += f"Std: {total_std:.2f}\n"
    stats_text += f"Min: {np.min(all_sigma_values):.2f}\n"
    stats_text += f"Max: {np.max(all_sigma_values):.2f}"
    
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.8)
    plt.text(0.5, 0.98, stats_text, transform=plt.gca().transAxes, fontsize=14,
             verticalalignment='top', bbox=props)
    
    # 调整布局
    # plt.tight_layout()
    
    # 保存图像
    plt.savefig('all_channels_sigma_distribution.png', dpi=300, bbox_inches='tight')
    plt.savefig('all_channels_sigma_distribution.pdf', bbox_inches='tight')
    
    # 显示图像
    plt.show()
    
    # 输出详细统计信息
    print(f"\n=== 总体统计信息 ===")
    print(f"总通道数: {len(all_sigma_values)}")
    print(f"Sigma均值: {total_mean:.2f}")
    print(f"Sigma中位数: {total_median:.2f}")
    print(f"Sigma标准差: {total_std:.2f}")
    print(f"Sigma范围: [{np.min(all_sigma_values):.2f}, {np.max(all_sigma_values):.2f}]")
    
    # 输出各文件统计信息
    print(f"\n=== 各文件统计 ===")
    for info in file_info:
        print(f"{info['name']}: {info['count']}通道, 均值={info['mean']:.2f}, 标准差={info['std']:.2f}")

if __name__ == "__main__":
    plot_all_sigma_distribution()