#!/bin/bash

# 定义变量
output_file="errorjobs.txt"
i=1

# 循环读取文件，并将内容追加到输出文件中
while [ -f "job$i.sh.err.10411388.$i" ]; do

  num_lines=$(wc -l < "job$i.sh.err.10411388.$i")

  # 判断行数是否超过2行
  if [ $num_lines -gt 0 ]; then
    echo "job_$i.sh"
  fi
  i=$((i+1))
done
