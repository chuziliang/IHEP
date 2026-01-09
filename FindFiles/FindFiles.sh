#!/bin/bash

output_file="test.txt"  # 输出文件名

# 指定要处理的目录列表
directories=(
  "/eos/juno/Commissioning_Dryrun/2024/1117/"
)

# 遍历目录列表，使用find命令查找每个目录及其子目录下的所有文件，并将结果追加到txt文件中
for directory in "${directories[@]}"; do
  # eos ls "$directory"log*.txt \; >> "$output_file"
  # eos ls -d "$directory"log*.txt >> "$output_file"
  eos ls "$directory"*.bin >> "$output_file"

done

