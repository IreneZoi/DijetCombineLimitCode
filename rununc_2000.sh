#!bin/bash

selections=("correctPUPPIres")
uncertainty=("none") # "lumi_13TeV" "CMS_eff_vtag_tau21_sf_13TeV" "CMS_pu_13TeV" "CMS_sig_p1_jes_13TeV" "CMS_sig_p2_jer_13TeV" "CMS_PDF_acceptance_j_13TeV" "CMS_SCALE_acceptance_j_13TeV" "CMS_mass_scale_j_13TeV" "CMS_mass_res_j_13TeV" "CMS_scale_j_13TeV" "CMS_res_j_13TeV" "CMS_L1pre_j_13TeV")
#uncertainty=("CMS_sig_p1_jes_13TeV")
echo ${#selections[@]}
selNumber=${#selections[@]}
echo $selNumber
uncNumber=${#uncertainty[@]}

dir=testInvertedSelection/${selections[$0]}
echo $dir



for ((i=0;i<$selNumber;i+=1))
do
    for ((j=0;i<$uncNumber;j+=1))
    do
	echo python runAllUHH_selectedMasses_cuts_newStrategy_unc.py -n ${selections[$0]} -u ${uncertainty[$j]}
	python runAllUHH_selectedMasses_cuts_newStrategy_unc_2000.py -n ${selections[$0]} -u ${uncertainty[$j]}
	
	cp -p datacards/CMS_jj_*on_${selections[$i]}_${uncertainty[$j]}_??00_13TeV__invMass*.* datacards/$dir/

#    cp -p plots/sigmodel_*on_${selections[$i]}_${uncertainty[$j]}??00__invMass*.* plots/$dir/
#    cp -p plots/fitDiagnostics*on${selections[$i]}_${uncertainty[$j]}??00_invMass*.root plots/$dir/
#    cp -p *.out plots/$dir/
#    cp -p *.root plots/$dir/
   
#    cp -p LimitTxt/*on__invMass*_${selections[$i]}_${uncertainty[$j]}_new_combined_Limit.txt LimitTxt/$dir/
    
#    cp -p Limits/CMS_jj_??00_*on_${selections[$i]}_${uncertainty[$j]}_13TeV__invMass*_asymptoticCLs_new.root Limits/$dir/
#    cp -p Limits/brazilianFlag_*on__invMass*_${selections[$i]}_${uncertainty[$j]}_new_combined_13TeV.* Limits/$dir/

#    rm datacards/CMS_jj_*on_${selections[$i]}_${uncertainty[$j]}_??00_13TeV__invMass*.* 

#    rm plots/sigmodel_*on_${selections[$i]}_${uncertainty[$j]}??00__invMass*.* 
#    rm plots/fitDiagnostics*on${selections[$i]}_${uncertainty[$j]}*00_invMass*.root 
#    rm *.out 
#    rm *.root
  
#    rm LimitTxt/*on__invMass*_${selections[$i]}_${uncertainty[$j]}_new_combined_Limit.txt 

#    rm Limits/CMS_jj_??00_*on_${selections[$i]}_${uncertainty[$j]}_13TeV__invMass*_asymptoticCLs_new.root
#    rm Limits/brazilianFlag_*on__invMass*_${selections[$i]}_${uncertainty[$j]}_new_combined_13TeV.* 
    done
done