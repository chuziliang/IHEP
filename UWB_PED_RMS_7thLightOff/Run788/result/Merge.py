import pandas as pd
import glob

# 获取所有txt文件的文件名
txt_files = glob.glob("*_788_0.txt")

# 读取所有txt文件内容并合并
dataframes = [pd.read_csv(file) for file in txt_files]
combined_df = pd.concat(dataframes)

# 将合并后的数据写入到csv文件
combined_df.to_csv("output_run788.csv", index=False)

print("文件合并完成，已写入到 output.csv 文件中")