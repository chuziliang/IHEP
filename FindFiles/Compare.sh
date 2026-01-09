#!/bin/bash

# 读取指定的txt文档，文档中每一行为一个文件路径
txt_file="test.txt"  # 请将这里替换为实际的txt文件名

while read file_path; do
    eos ls -s "$file_path"
done < "$txt_file"