#!bin/bash/

awk '/Significance: / {print $2}' Limits/CMS_jj_radion_1200_13TeV__invMass_signif.out >> prova.txt
awk '/p-value/ {print $3}' Limits/CMS_jj_radion_1200_13TeV__invMass_signif.out >> prova.txt | sed -i 's|/\)|/\ |g' prova.txt 
