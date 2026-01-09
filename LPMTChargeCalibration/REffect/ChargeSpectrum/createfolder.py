import os

EvtPosition=(-0.8,-0.6,-0.4,-0.2,0.0,0.2,0.4,0.6,0.8)

def create_folder_from_txt():
    for line in EvtPosition:
        if not os.path.exists(line):
            os.makedirs(line)
            print(f"文件夹 {line} 创建成功。")
        else:
            print(f"文件夹 {line} 已存在。")

if __name__ == "__main__":
    create_folder_from_txt()