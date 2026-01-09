file1 = '/junofs/users/chuziliang125/GCUIP/ethers_DAQ服务器'  # 第一个文件路径
file2 = '/junofs/users/chuziliang125/GCUIP/ethers_工控机'  # 第二个文件路径

with open(file1, 'r') as f1, open(file2, 'r') as f2:
    lines1 = f1.readlines()  # 读取第一个文件的所有行
    lines2 = f2.readlines()  # 读取第二个文件的所有行

# 遍历两个文件的行，并比较它们
for i, (line1, line2) in enumerate(zip(lines1, lines2), start=1):
    line1 = line1.strip()  # 去除行尾的换行符和空格
    line2 = line2.strip()

    if line1 != line2:
        print(f"行号 {i}:")
        print(f"DAQ服务器: {line1}")
        print(f"工控机: {line2}")
        print()