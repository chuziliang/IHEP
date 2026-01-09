import os

# 循环执行命令10次
for i in range(9):
    command = 'hep_sub job_{}.sh -argu "%{{ProcId}}" -n 200 -mem 8000 \n'.format(str(i))
    os.system(command)
    print(f'第{i+1}次执行命令: {command}')

print('所有命令执行完毕')