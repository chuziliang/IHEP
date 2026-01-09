import os

def count_files_in_directory(directory):
    folder_file_counts = {}
    for root, dirs, files in os.walk(directory):
        folder_path = root
        file_count = len(files)
        folder_file_counts[folder_path] = file_count
    return folder_file_counts

directory_path = '.'  # 可以修改为你要统计的目录路径
file_counts = count_files_in_directory(directory_path)

sorted_counts = sorted(file_counts.items(), key=lambda item: item[1], reverse=True)

for folder_path, count in sorted_counts:
    print(f"目录 '{folder_path}' 下的文件数量为: {count}")