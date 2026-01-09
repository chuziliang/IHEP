#!/bin/bash

for i in {0..9}
do
    echo "/junofs/users/chuziliang125/PED_analysis20230816/AnalysisV10/analysis /junofs/users/chuziliang125/PED_analysis20230816/AnalysisV10/DataList.txt $i" > "LightOffAnalysis_$i.sh"
done
