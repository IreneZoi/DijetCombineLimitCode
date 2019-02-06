#!bin/bash
#selections=("tau21" "tau21_deta4" "tau21_deta5" "tau21_deta6" "tau21_04" "tau21_04_deta4" "tau21_04_deta5" "tau21_04_deta6" "tau21_05" "tau21_05_deta4" "tau21_05_deta5" "tau21_05_deta6" "tau21_06" "tau21_06_deta4" "tau21_06_deta5" "tau21_06_deta6" "tau21_07" "tau21_07_deta4" "tau21_07_deta5" "tau21_07_deta6")
selections=("Primes")
#selections=( "invM1000" )
echo ${#selections[@]}
selNumber=${#selections[@]}
echo $selNumber
#dir=CHS
#dir=pt30etaAK4
#dir=mjjVBFSelHigh
dir=testInvertedSelection/${selections[$0]}
echo $dir

mkdir input/$dir/
#for ((i=0;i<$selNumber;i+=1))
#do
#    echo ${selections[$i]}
#    echo root -b -q "PrepareRootFile.C(\"${selections[$i]}\")"
#    root -b -q "PrepareRootAllFile.C(\"${selections[$i]}\",\"$dir\")" 
#done


for ((i=0;i<$selNumber;i+=1))
do
    cp -p input/*on_${selections[$i]}_??00.root  input/$dir/
done

python runAllUHH_selectedMasses_cuts_newStrategy_allSignals.py

for ((i=0;i<$selNumber;i+=1))
do

   rm input/*on_${selections[$i]}_??00.root   
done

mkdir MiniTrees/DataUHH/$dir/
mkdir MiniTrees/SignalUHH/$dir/
mkdir workspaces/$dir/
mkdir datacards/$dir/
mkdir plots/$dir/
mkdir LimitTxt/$dir/
mkdir Limits/$dir/

for ((i=0;i<$selNumber;i+=1))
do
    cp -p MiniTrees/DataUHH/dijetUHH_13TeV_miniTree_${selections[$i]}.root MiniTrees/DataUHH/$dir/
    cp -p MiniTrees/SignalUHH/dijetUHH_13TeV_*on_${selections[$i]}_Interpolated??00_miniTree.root MiniTrees/SignalUHH/$dir/
    cp -p workspaces/CMS_jj_*on_${selections[$i]}_??00_13TeV.root workspaces/$dir/
    cp -p workspaces/CMS_jj_*prime_${selections[$i]}_??00_13TeV.root workspaces/$dir/
    cp -p workspaces/CMS_jj_bkg_VV_${selections[$i]}_??00_13TeV.root workspaces/$dir/

    cp -p datacards/CMS_jj_*on_${selections[$i]}_??00_13TeV__invMass*.* datacards/$dir/
    cp -p datacards/CMS_jj_*prime_${selections[$i]}_??00_13TeV__invMass*.* datacards/$dir/

    cp -p plots/sigmodel_*on_${selections[$i]}??00__invMass*.* plots/$dir/
    cp -p plots/fitDiagnostics*on${selections[$i]}??00_invMass*.root plots/$dir/
    cp -p plots/sigmodel_*prime_${selections[$i]}??00__invMass*.* plots/$dir/
    cp -p plots/fitDiagnostics*prime${selections[$i]}??00_invMass*.root plots/$dir/
    cp -p *.out plots/$dir/
    cp -p *.root plots/$dir/
   
    cp -p LimitTxt/*on__invMass*_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    cp -p LimitTxt/*prime__invMass*_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    
    cp -p Limits/CMS_jj_??00_*on_${selections[$i]}_13TeV__invMass*_asymptoticCLs_new.root Limits/$dir/
    cp -p Limits/brazilianFlag_*on__invMass*_${selections[$i]}_new_combined_13TeV.* Limits/$dir/

    cp -p Limits/CMS_jj_??00_*prime_${selections[$i]}_13TeV__invMass*_asymptoticCLs_new.root Limits/$dir/
    cp -p Limits/brazilianFlag_*prime__invMass*_${selections[$i]}_new_combined_13TeV.* Limits/$dir/


#    rm input/*on_${selections[$i]}_Interpolated??00.root  

#    rm MiniTrees/DataUHH/dijetUHH_13TeV_miniTree_${selections[$i]}.root
#    rm MiniTrees/SignalUHH/dijetUHH_13TeV_*on_${selections[$i]}_Interpolated??00_miniTree.root

#    rm workspaces/CMS_jj_*on_${selections[$i]}_??00_13TeV.root 

#    rm datacards/CMS_jj_*on_${selections[$i]}_??00_13TeV__invMass*.* 

#    rm plots/sigmodel_*on_${selections[$i]}??00__invMass*.* 
#    rm plots/fitDiagnostics*on${selections[$i]}*00_invMass*.root 
#    rm *.out 
#    rm *.root
  
#    rm LimitTxt/*on__invMass*_${selections[$i]}_new_combined_Limit.txt 

#    rm Limits/CMS_jj_??00_*on_${selections[$i]}_13TeV__invMass*_asymptoticCLs_new.root
#    rm Limits/brazilianFlag_*on__invMass*_${selections[$i]}_new_combined_13TeV.* 
#done
