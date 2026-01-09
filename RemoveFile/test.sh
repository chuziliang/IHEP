if [[ $(eos stat /eos/juno/Commissioning_Dryrun/2023/0413/8181_230417-2106_1708.root) ]]; then
      file_exists=1  # 文件存在，将标志变量设置为1
      echo "文件 $filename 存在于目录 $directory"
      break
fi