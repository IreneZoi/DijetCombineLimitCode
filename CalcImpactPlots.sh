#!bin/bash
masses=(1200 2000 4000)
#models=("WZ" "BulkWW" "BulkZZ" "ZprimeWW")
category=("_invMass" "_invMass_afterVBFsel" "_invMass_combined") #"WWLP" "ZZHP" "ZZLP" "VVnew")
models=("graviton" "radion")
#category=("qVnew")
name="correctPUPPIres"

for mass in ${masses[@]}
do
    for model in ${models[@]}
    do 
        for c in ${category[@]}
        do
        #mass=1400
        #model="WZ"
        #cat="WZLP"
        datacard="datacards/CMS_jj_${model}_${name}_${mass}_13TeV_${c}.txt"
        workspace="datacards/CMS_jj_${model}_${name}_${mass}_13TeV_${c}.root"

        text2workspace.py $datacard -m $mass

        combineTool.py -M Impacts -d $workspace -m $mass --doInitialFit --robustFit 1

        combineTool.py -M Impacts -d $workspace -m $mass --robustFit 1 --doFits

        combineTool.py -M Impacts -d $workspace -m $mass -o impacts.json

        plotImpacts.py -i impacts.json -o impacts

        mv ./impacts.pdf Impacts_${model}_${mass}_${c}_${name}.pdf
        done
    done
done

