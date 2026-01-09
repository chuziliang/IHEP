import paramiko
import os
import stat

# 远程服务器信息
remote_host = "10.3.192.121"
remote_user = "run"
remote_password = "run@147852369"
remote_folder_path = "/home/run/SPMT_zhangsh/tcp_spmt/single_SPMT_v2/data/"

# 本地保存路径
local_folder_path = "GU8113/"

# 创建SSH对象
ssh = paramiko.SSHClient()
# 允许连接不在know_hosts文件中的主机
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
# 连接服务器
ssh.connect(remote_host, username=remote_user, password=remote_password)

# 创建SFTP客户端对象
sftp = ssh.open_sftp()

# 定义递归复制函数
def copy_files_recursive(remote_path, local_path):
    try:
        remote_files = sftp.listdir_attr(remote_path)
        for file_attr in remote_files:
            remote_file_path = os.path.join(remote_path, file_attr.filename)
            local_file_path = os.path.join(local_path, file_attr.filename)

            if file_attr.filename.endswith('8113.root'):
                print(remote_file_path)
                sftp.get(remote_file_path, local_file_path)
    except Exception as e:
        print(f"Error occurred: {e}")

# 调用递归复制函数
for i in range(1203,1289):
    remote_folder_path_root = remote_folder_path + str(i) +"/ROOT/"
    copy_files_recursive(remote_folder_path_root, local_folder_path)

# 关闭SFTP和SSH连接
sftp.close()
ssh.close()