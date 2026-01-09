import os

def count_files(folder_path):
    file_count = 0

    # 遍历文件夹下的所有子文件夹和文件
    for root, dirs, files in os.walk(folder_path):
        filename = "folder_path.txt"
        with open(filename, "a") as file:
            file.write(root)
            file.write("\n")

# 输入文件夹路径
folder_path = input("请输入文件夹路径：")

# 调用函数统计文件个数
count_files(folder_path)
