#!/bin/bash

file_list="file_list_2.txt"  # 文件名列表文件

# 指定要检查的文件夹目录列表
directories=(
  "/eos/juno/Commissioning_Dryrun/2023/0619/"
  "/eos/juno/Commissioning_Dryrun/2023/0621/"
  "/eos/juno/Commissioning_Dryrun/2023/0412/"
  "/eos/juno/Commissioning_Dryrun/2023/0413/"
  "/eos/juno/Commissioning_Dryrun/2023/0418/"
  "/eos/juno/Commissioning_Dryrun/2023/0425/"
  "/eos/juno/Commissioning_Dryrun/2023/0905/"
)

# 读取文件名列表文件的内容到数组
mapfile -t filenames < "$file_list"

for filename in "${filenames[@]}"; do
  file_exists=0  # 标志变量，初始值为0，表示文件不存在

 # 遍历文件夹目录列表，检查是否存在文件名在列表中的文件
  for directory in "${directories[@]}"; do
    if [[ $(eos stat "$directory/$filename") ]]; then
      file_exists=1  # 文件存在，将标志变量设置为1
      echo "文件 $filename 存在于目录 $directory" >> test.txt
      break
    fi
  done

  # 如果文件不存在于任何一个文件夹下，输出文件不存在的消息
  if [[ $file_exists -eq 0 ]]; then
    echo "文件 $filename 不存在" >> test.txt
  fi
done