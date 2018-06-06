#!bin/bash
selections=("VV" "VBF" "invM500" "invM1000" "invM1500" "invM2000" "invM2500" "invM3000" "invM3500" "invM4000")
channels=("graviton" "radion")
masses=("1200" "2000" "4000")
bins=("_invMass" "_invMass_afterVBFsel")
echo ${#selections[@]}
selNumber=${#selections[@]}
echo $selNumber

echo ${#channels[@]}
chanNumber=${#channels[@]}
echo $chanNumber

echo ${#masses[@]}
massNumber=${#masses[@]}
echo $massNumber

echo ${#bins[@]}
binNumber=${#bins[@]}
echo $binNumber

dir=mjjVBFSelHigh
echo $dir
cd Limits

for ((i=0;i<$selNumber;i+=1))
do
    for ((j=0;j<$chanNumber;j+=1))
    do 
	for ((k=0;k<$massNumber;k+=1))
	do
	    for((l=0;l<$binNumber;l+=1))
	    do
		echo ${selections[$i]}  ${channels[$j]}  ${masses[$k]}  ${bins[$l]}
		output=CMS_jj_${channels[$j]}_${masses[$k]}_13TeV_${bins[$l]}_${selections[$i]}_signif.out		
		combine ../datacards/$dir/CMS_jj_${channels[$j]}_${selections[$i]}_${masses[$k]}_13TeV_${bins[$l]}.txt -M ProfileLikelihood --signif -v2 -m ${masses[$k]} -n ${channels[$j]}${selections[$i]}${masses[$k]}${bins[$l]}_signif --rMax 100 --rMin 0.000001 &> $output
		awk '/Significance: / {print $2}' $output >> ../datacards/$dir/easy_$output
		awk '/p-value/ {print $3}' $output >> ../datacards/$dir/easy_$output | sed -i 's|)| |g' ../datacards/$dir/easy_$output

	    done
	done
    done
done

cd ..