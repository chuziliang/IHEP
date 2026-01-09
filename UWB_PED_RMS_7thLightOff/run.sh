#!/bin/bash

raw_directory=/eos/juno/user/wudr/DQM/root/
job_directory="/junofs/users/chuziliang125/UWB_PED_RMS_7thLightOff/10th_LightOn/job/"

eos ls ${raw_directory}*1435_0.root >& fileList

while read line; do
	file_name=$(basename $line)
	gcuId=$(echo "$file_name" | awk -F '[_.-]' '{print $1}')
	runNum=$(echo "$file_name" | awk -F '[_.-]' '{print $4}')
	fileNum=$(echo "$file_name" | awk -F '[_.-]' '{print $5}')

# echo "$fileNum:$raw_directory$line"
	output_file="${job_directory}/${gcuId}_${runNum}.sh"
# echo "$fileNum:$line" >> "$output_file"
	cat << EOF > ${output_file}
#!/bin/bash -e
eos cp ${raw_directory}${line} \$TMPDIR
root -l -q '/junofs/users/chuziliang125/UWB_PED_RMS_7thLightOff/GetPEDRMS.cc("\$TMPDIR/${file_name}")'
rm \$TMPDIR/${file_name}

EOF
    chmod +x ${output_file}
    hep_sub ${output_file}
done < fileList

# rm fileList

# for file in "$job_directory"/*.txt; do
# 	sort -t: -k1,1n "$file" | cut -d: -f2- > "${file}_tmp"
# 	mv "${file}_tmp" "$file"
# done