#!bin/bash
#selections=("tau21" "tau21_deta4" "tau21_deta5" "tau21_deta6" "tau21_04" "tau21_04_deta4" "tau21_04_deta5" "tau21_04_deta6" "tau21_05" "tau21_05_deta4" "tau21_05_deta5" "tau21_05_deta6" "tau21_06" "tau21_06_deta4" "tau21_06_deta5" "tau21_06_deta6" "tau21_07" "tau21_07_deta4" "tau21_07_deta5" "tau21_07_deta6")
#selections=("VV")
#selections=("invM1000_tau21HP")
#selections=("invM1000" "invM1000_tau21HP" "invM1000_tau21_045" "invM1200" "invM1200_tau21HP" "invM1200_tau21_045" "invM1500" "invM1500_tau21HP" "invM1500_tau21_045")
selections=("VV" "VBF" "invM500" "invM1000" "invM1500" "invM2000" "invM2500" "invM3000" "invM3500" "invM4000")
echo ${#selections[@]}
selNumber=${#selections[@]}
echo $selNumber
dir=CHS
#dir=pt30etaAK4
#dir=mjjVBFSelHigh
#dir=pt20AK4
#dir=moreMjjTau21
echo $dir
#for i in {0..$selNumber}
#mkdir input/$dir/
for ((i=0;i<$selNumber;i+=1))
do
    cp -p MiniTrees/DataUHH/$dir/dijetUHH_13TeV_miniTree_${selections[$i]}.root MiniTrees/DataUHH/
    cp -p MiniTrees/SignalUHH/$dir/dijetUHH_13TeV_*on_${selections[$i]}_Interpolated??00_miniTree.root MiniTrees/SignalUHH/

done
mkdir plots/$dir/out/
python rerunLast3_selectedMasses_cuts_newStrategy.py
for ((i=0;i<$selNumber;i+=1))
do
    cp -p workspaces/CMS_jj_*ton_${selections[$i]}_??00_13TeV.root workspaces/$dir/
    cp -p workspaces/CMS_jj_bkg_VV_${selections[$i]}_??00_13TeV.root workspaces/$dir/
    
    cp -p datacards/CMS_jj_*on_${selections[$i]}_??00_13TeV__invMass*.* datacards/$dir/

    cp -p plots/sigmodel_graviton_${selections[$i]}1200__invMass*.* plots/$dir/
    cp -p plots/sigmodel_graviton_${selections[$i]}2000__invMass*.* plots/$dir/
    cp -p plots/sigmodel_graviton_${selections[$i]}4000__invMass*.* plots/$dir/
    cp -p plots/fitDiagnosticsgraviton${selections[$i]}1200_invMass*.root plots/$dir/
    cp -p plots/fitDiagnosticsgraviton${selections[$i]}2000_invMass*.root plots/$dir/
    cp -p plots/fitDiagnosticsgraviton${selections[$i]}4000_invMass*.root plots/$dir/
    cp -p plots/sigmodel_radion_${selections[$i]}1200__invMass*.* plots/$dir/
    cp -p plots/sigmodel_radion_${selections[$i]}2000__invMass*.* plots/$dir/
    cp -p plots/sigmodel_radion_${selections[$i]}4000__invMass*.* plots/$dir/
    cp -p plots/fitDiagnosticsradion${selections[$i]}1200_invMass*.root plots/$dir/
    cp -p plots/fitDiagnosticsradion${selections[$i]}2000_invMass*.root plots/$dir/
    cp -p plots/fitDiagnosticsradion${selections[$i]}4000_invMass*.root plots/$dir/
    cp -p LimitTxt/graviton__invMass_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    cp -p LimitTxt/graviton__invMass_afterVBFsel_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    cp -p LimitTxt/radion__invMass_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    cp -p LimitTxt/radion__invMass_afterVBFsel_${selections[$i]}_new_combined_Limit.txt LimitTxt/$dir/
    cp -p Limits/brazilianFlag_graviton__invMass_${selections[$i]}_new_combined_13TeV.* Limits/$dir/
    cp -p Limits/brazilianFlag_graviton__invMass_afterVBFsel_${selections[$i]}_new_combined_13TeV.* Limits/$dir/
    cp -p Limits/brazilianFlag_radion__invMass_${selections[$i]}_new_combined_13TeV.* Limits/$dir/
    cp -p Limits/brazilianFlag_radion__invMass_afterVBFsel_${selections[$i]}_new_combined_13TeV.* Limits/$dir/
    cp -p Limits/CMS_jj_??00_*on_${selections[$i]}_13TeV__invMass*_asymptoticCLs_new.root Limits/$dir/
    cp *.out plots/$dir/out/

    rm workspaces/CMS_jj_graviton_${selections[$i]}_1200_13TeV.root 
    rm workspaces/CMS_jj_graviton_${selections[$i]}_2000_13TeV.root 
    rm workspaces/CMS_jj_graviton_${selections[$i]}_4000_13TeV.root 
    rm workspaces/CMS_jj_radion_${selections[$i]}_1200_13TeV.root 
    rm workspaces/CMS_jj_radion_${selections[$i]}_2000_13TeV.root 
    rm workspaces/CMS_jj_radion_${selections[$i]}_4000_13TeV.root 
    rm workspaces/CMS_jj_bkg_VV_${selections[$i]}_1200_13TeV.root 
    rm workspaces/CMS_jj_bkg_VV_${selections[$i]}_2000_13TeV.root 
    rm workspaces/CMS_jj_bkg_VV_${selections[$i]}_4000_13TeV.root 
    rm datacards/CMS_jj_graviton_${selections[$i]}_1200_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_graviton_${selections[$i]}_1200_13TeV__invMass_afterVBFsel_limit1_submit.src
    rm datacards/CMS_jj_graviton_${selections[$i]}_2000_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_graviton_${selections[$i]}_2000_13TeV__invMass_afterVBFsel_limit1_submit.src 
    rm datacards/CMS_jj_graviton_${selections[$i]}_4000_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_graviton_${selections[$i]}_4000_13TeV__invMass_afterVBFsel_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_1200_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_1200_13TeV__invMass_afterVBFsel_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_2000_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_2000_13TeV__invMass_afterVBFsel_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_4000_13TeV__invMass_limit1_submit.src 
    rm datacards/CMS_jj_radion_${selections[$i]}_4000_13TeV__invMass_afterVBFsel_limit1_submit.src 
    rm datacards/CMS_jj_graviton_${selections[$i]}_1200_13TeV__invMass.txt 
    rm datacards/CMS_jj_graviton_${selections[$i]}_1200_13TeV__invMass_afterVBFsel.txt
    rm datacards/CMS_jj_graviton_${selections[$i]}_2000_13TeV__invMass.txt
    rm datacards/CMS_jj_graviton_${selections[$i]}_2000_13TeV__invMass_afterVBFsel.txt
    rm datacards/CMS_jj_graviton_${selections[$i]}_4000_13TeV__invMass.txt
    rm datacards/CMS_jj_graviton_${selections[$i]}_4000_13TeV__invMass_afterVBFsel.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_1200_13TeV__invMass.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_1200_13TeV__invMass_afterVBFsel.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_2000_13TeV__invMass.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_2000_13TeV__invMass_afterVBFsel.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_4000_13TeV__invMass.txt
    rm datacards/CMS_jj_radion_${selections[$i]}_4000_13TeV__invMass_afterVBFsel.txt 
    rm plots/sigmodel_graviton_${selections[$i]}1200__invMass*.* 
    rm plots/sigmodel_graviton_${selections[$i]}2000__invMass*.* 
    rm plots/sigmodel_graviton_${selections[$i]}4000__invMass*.* 
    rm plots/fitDiagnosticsgraviton${selections[$i]}1200_invMass*.root 
    rm plots/fitDiagnosticsgraviton${selections[$i]}2000_invMass*.root 
    rm plots/fitDiagnosticsgraviton${selections[$i]}4000_invMass*.root 
    rm plots/sigmodel_radion_${selections[$i]}1200__invMass*.* 
    rm plots/sigmodel_radion_${selections[$i]}2000__invMass*.* 
    rm plots/sigmodel_radion_${selections[$i]}4000__invMass*.* 
    rm plots/fitDiagnosticsradion${selections[$i]}1200_invMass*.root 
    rm plots/fitDiagnosticsradion${selections[$i]}2000_invMass*.root 
    rm plots/fitDiagnosticsradion${selections[$i]}4000_invMass*.root 
    rm LimitTxt/graviton__invMass_${selections[$i]}_new_combined_Limit.txt 
    rm LimitTxt/graviton__invMass_afterVBFsel_${selections[$i]}_new_combined_Limit.txt 
    rm LimitTxt/radion__invMass_${selections[$i]}_new_combined_Limit.txt 
    rm LimitTxt/radion__invMass_afterVBFsel_${selections[$i]}_new_combined_Limit.txt 
    rm Limits/brazilianFlag_graviton__invMass_${selections[$i]}_new_combined_13TeV.* 
    rm Limits/brazilianFlag_graviton__invMass_afterVBFsel_${selections[$i]}_new_combined_13TeV.* 
    rm Limits/brazilianFlag_radion__invMass_${selections[$i]}_new_combined_13TeV.* 
    rm Limits/brazilianFlag_radion__invMass_afterVBFsel_${selections[$i]}_new_combined_13TeV.* 
    rm Limits/CMS_jj_??00_*on_${selections[$i]}_13TeV__invMass*_asymptoticCLs_new.root
    rm higgs*.root
    rm *.out
done
