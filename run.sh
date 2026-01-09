#!/bin/bash
configFile=/junofs/users/wudr/SPMTElec/spmtOffline/setup.sh
prefix="root://junoeos01.ihep.ac.cn/"

inputfile="paths.txt"

# raw_directory=$1

# this directory should be changed to the real directory for saving the png figures in DQM disk
export png_directory="/eos/juno/user/wudr/DQM/images/"

# this directory should be changed to the real directory for saving the root files
export root_directory="/eos/juno/user/wudr/DQM/root/"

cur=$(dirname $(realpath $BASH_SOURCE[0]))
export binary_list_output_directory="$cur/binaryLists/"

if [ ! -d "$binary_list_output_directory" ]; then
	mkdir -p "$binary_list_output_directory"
fi

if [ ! -d "$cur/jobs" ]; then
	mkdir "$cur/jobs"
fi

source $configFile

# list all binary files
rm $binary_list_output_directory/*txt

# 放到循环里
while IFS= read -r raw_directory; do
	eos ls $raw_directory/*.bin >& fileList

	while read line; do
		file_name=$(basename $line)
		gcuId=$(echo "$file_name" | awk -F '[_.-]' '{print $1}')
		runNum=$(echo "$file_name" | awk -F '[_.-]' '{print $4}')
		fileNum=$(echo "$file_name" | awk -F '[_.-]' '{print $5}')

	# echo "$fileNum:$raw_directory$line"
		output_file="${binary_list_output_directory}/${gcuId}_${runNum}.txt"
	# echo "$fileNum:$line" >> "$output_file"
		echo "$fileNum:$raw_directory$line" >> "$output_file"
	done < fileList
	# 放到循环里
done < inputfile
# rm fileList

for file in "$binary_list_output_directory"/*.txt; do
	sort -t: -k1,1n "$file" | cut -d: -f2- > "${file}_tmp"
	mv "${file}_tmp" "$file"
done

echo "Files are sorted and written to $binary_list_output_directory"

# submit jobs
make
