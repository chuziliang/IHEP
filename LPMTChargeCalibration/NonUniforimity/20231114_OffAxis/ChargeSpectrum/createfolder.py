import os

def create_folder_from_txt(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()
        for line in lines:
            folder_name = line.strip()
            if not os.path.exists(folder_name):
                os.makedirs(folder_name)
                print(f"文件夹 {folder_name} 创建成功。")
            else:
                print(f"文件夹 {folder_name} 已存在。")

if __name__ == "__main__":
    txt_file = "/junofs/users/chuziliang125/LPMTChargeCalibration/NonUniforimity/20231114_OffAxis/folder_path.txt"
    create_folder_from_txt(txt_file)