import os
import ROOT

# 定义要读取的文件夹路径
folder_path = "/junofs/users/chuziliang125/FindFiles/GU8113/"

# 创建一个用于保存所有画布的pdf文件
canvas=ROOT.TCanvas()
canvas.Print("output.pdf[")

# 遍历文件夹下的所有.root文件
for root_file_name in os.listdir(folder_path):
    if root_file_name.endswith(".root"):
        root_file_path = os.path.join(folder_path, root_file_name)

        # 打开.root文件
        root_file = ROOT.TFile.Open(root_file_path)

        # 获取名为ADCMEAN的TCanvas
        canvas = root_file.Get("ADCMEAN/JUNO_SPMT_MEAN_8113")

        if canvas:
            # canvas.Title(root_file_name)
            Title = "Title:"+str(root_file_name).split(".")[0]
            print(Title)
            canvas.Print("output.pdf",str(Title))
            # output_pdf.Append(canvas)

        # 关闭.root文件
        root_file.Close()

# 关闭pdf文件
canvas.Print("output.pdf]")
