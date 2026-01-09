#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
脚本用于读取当前目录下所有CSV文件，筛选sigma值大于10和小于1.5的文件名和通道
"""

import os
import glob
import pandas as pd

def filter_sigma_values():
    """
    读取当前目录下所有CSV文件，筛选sigma值大于10和小于1.5的通道
    """
    # 获取当前目录下所有CSV文件
    csv_files = glob.glob("*.csv")
    
    if not csv_files:
        print("当前目录下未找到CSV文件")
        return
    
    print(f"找到 {len(csv_files)} 个CSV文件")
    
    # 存储筛选结果
    high_sigma_results = []  # sigma > 10
    low_sigma_results = []   # sigma < 1.5
    
    for csv_file in csv_files:
        try:
            # 读取CSV文件
            df = pd.read_csv(csv_file)
            
            # 检查必要的列是否存在
            if 'Sigma' not in df.columns or 'Channel' not in df.columns:
                print(f"警告: 文件 {csv_file} 缺少必要的列 (Channel 或 Sigma)")
                continue
            
            # 筛选sigma值大于10的通道
            high_sigma = df[df['Sigma'] > 10]
            if not high_sigma.empty:
                for _, row in high_sigma.iterrows():
                    high_sigma_results.append({
                        '文件名': csv_file,
                        '通道': row['Channel'],
                        'Sigma值': row['Sigma'],
                        'RMS值': row.get('RMS', 'N/A')
                    })
            
            # 筛选sigma值小于1.5的通道
            low_sigma = df[df['Sigma'] < 1.5]
            if not low_sigma.empty:
                for _, row in low_sigma.iterrows():
                    low_sigma_results.append({
                        '文件名': csv_file,
                        '通道': row['Channel'],
                        'Sigma值': row['Sigma'],
                        'RMS值': row.get('RMS', 'N/A')
                    })
            
            print(f"处理文件: {csv_file} - 总通道数: {len(df)}, "
                  f"Sigma>10: {len(high_sigma)}, Sigma<1.5: {len(low_sigma)}")
                  
        except Exception as e:
            print(f"错误: 处理文件 {csv_file} 时出错: {e}")
    
    # 输出结果
    print("\n" + "="*80)
    print("筛选结果汇总")
    print("="*80)
    
    # 输出Sigma值大于10的结果
    if high_sigma_results:
        print(f"\nSigma值大于10的通道 (共 {len(high_sigma_results)} 个):")
        print("-" * 60)
        print(f"{'文件名':<25} {'通道':<8} {'Sigma值':<12} {'RMS值':<12}")
        print("-" * 60)
        for result in high_sigma_results:
            print(f"{result['文件名']:<25} {result['通道']:<8} {result['Sigma值']:<12.3f} {result['RMS值']:<12.3f}")
    else:
        print("\n未找到Sigma值大于10的通道")
    
    # 输出Sigma值小于1.5的结果
    if low_sigma_results:
        print(f"\nSigma值小于1.5的通道 (共 {len(low_sigma_results)} 个):")
        print("-" * 60)
        print(f"{'文件名':<25} {'通道':<8} {'Sigma值':<12} {'RMS值':<12}")
        print("-" * 60)
        for result in low_sigma_results:
            print(f"{result['文件名']:<25} {result['通道']:<8} {result['Sigma值']:<12.3f} {result['RMS值']:<12.3f}")
    else:
        print("\n未找到Sigma值小于1.5的通道")
    
    # 保存结果到文件
    save_results_to_file(high_sigma_results, low_sigma_results)
    
    return high_sigma_results, low_sigma_results

def save_results_to_file(high_sigma_results, low_sigma_results):
    """
    将筛选结果保存到文本文件
    """
    with open("sigma_filter_results.txt", "w", encoding="utf-8") as f:
        f.write("Sigma值筛选结果\n")
        f.write("="*50 + "\n\n")
        
        if high_sigma_results:
            f.write("Sigma值大于10的通道:\n")
            f.write("-"*40 + "\n")
            for result in high_sigma_results:
                f.write(f"文件: {result['文件名']}, 通道: {result['通道']}, "
                       f"Sigma: {result['Sigma值']:.3f}, RMS: {result['RMS值']:.3f}\n")
            f.write("\n")
        
        if low_sigma_results:
            f.write("Sigma值小于1.5的通道:\n")
            f.write("-"*40 + "\n")
            for result in low_sigma_results:
                f.write(f"文件: {result['文件名']}, 通道: {result['通道']}, "
                       f"Sigma: {result['Sigma值']:.3f}, RMS: {result['RMS值']:.3f}\n")
    
    print(f"\n详细结果已保存到: sigma_filter_results.txt")

def analyze_sigma_distribution():
    """
    分析所有文件的Sigma值分布
    """
    csv_files = glob.glob("*.csv")
    all_sigma_values = []
    
    for csv_file in csv_files:
        try:
            df = pd.read_csv(csv_file)
            if 'Sigma' in df.columns:
                all_sigma_values.extend(df['Sigma'].tolist())
        except:
            pass
    
    if all_sigma_values:
        import numpy as np
        sigma_array = np.array(all_sigma_values)
        print(f"\nSigma值分布统计:")
        print(f"总样本数: {len(sigma_array)}")
        print(f"最小值: {sigma_array.min():.3f}")
        print(f"最大值: {sigma_array.max():.3f}")
        print(f"平均值: {sigma_array.mean():.3f}")
        print(f"标准差: {sigma_array.std():.3f}")
        print(f"中位数: {np.median(sigma_array):.3f}")
        
        # 统计不同范围的Sigma值数量
        ranges = [
            ("<1.0", (sigma_array < 1.0).sum()),
            ("1.0-1.5", ((sigma_array >= 1.0) & (sigma_array < 1.5)).sum()),
            ("1.5-5.0", ((sigma_array >= 1.5) & (sigma_array < 5.0)).sum()),
            ("5.0-10.0", ((sigma_array >= 5.0) & (sigma_array < 10.0)).sum()),
            (">=10.0", (sigma_array >= 10.0).sum())
        ]
        
        print(f"\nSigma值分布:")
        for range_name, count in ranges:
            percentage = (count / len(sigma_array)) * 100
            print(f"{range_name}: {count} 个 ({percentage:.1f}%)")

def main():
    """
    主函数
    """
    print("开始处理CSV文件...")
    print("当前工作目录:", os.getcwd())
    
    # 筛选Sigma值
    high_sigma, low_sigma = filter_sigma_values()
    
    # 分析Sigma值分布
    analyze_sigma_distribution()
    
    print("\n处理完成!")

if __name__ == "__main__":
    main()