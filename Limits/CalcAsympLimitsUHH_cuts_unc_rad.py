import array, math
import os, sys

#channels=["graviton","radion"]
channels=["radion"]
masses=[int(sys.argv[1])]
cut=str(sys.argv[2]) #irene
uncertainty=str(sys.argv[3])
fullToys=False

for chan in channels:
    print "chan =",chan
    
#    bins=["_invMass_combined"]
    bins=["_invMass","_invMass_afterVBFsel","_invMass_combined"]
    
    if fullToys:
      points=[]
      for p in range(1,10):
          points+=[float(p/10.)]
          points+=[float(p/10.+0.05)]
          points+=[float(p/1.)]
          points+=[float(p/1.+0.5)]
          points+=[float(p*10.)]
          points+=[float(p*10.+5.)]
    else:
      points=[0.1]
    for bin in bins:

        for mass in masses:
          print "mass =",mass
          directory="datacards/"
          for point in points:

            outputname = directory+"CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_limit"+str(int(point*10))+"_submit.src"
            logname = directory+"CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_limit"+str(int(point*10))+"_submit.out"
            outputfile = open(outputname,'w')
            outputfile.write('#!/bin/bash\n')
            outputfile.write("cd ${CMSSW_BASE}/src/DijetCombineLimitCode; eval `scramv1 run -sh`\n")
            if fullToys:
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --clsAcc 0 -T 100 -i 30 --singlePoint "+str(point)+" -s 10000"+str(int(point*100))+" --saveHybridResult --saveToys -m "+str(mass) + " -n "+chan+str(bin)+" &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_toy"+str(int(point*10))+"_fullCLs.out\n")
              outputfile.write("hadd -f grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root higgsCombine" + chan + str(bin)+".HybridNew.mH"+str(int(mass))+".10000*.root\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_obs_fullCLs.out\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.5 &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_50_fullCLs.out\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.16 &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_16_fullCLs.out\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.84 &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_84_fullCLs.out\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.025 &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_025_fullCLs.out\n")
              outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+".txt -M HybridNew --frequentist --grid grid_mX"+str(mass)+"_" + chan + "_13TeV_"+bin+".root -m "+str(mass) + " -n "+chan+str(bin)+" --expectedFromGrid 0.975 &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_975_fullCLs.out\n")
            else:
                if uncertainty != "CMS_sig_p1_jes_13TeV" and uncertainty != "CMS_sig_p2_jer_13TeV"  and uncertainty != "CMS_bkg_fit_slope1__invMass_13TeV":
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M Asymptotic -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --freezeParameters CMS_sig_p1_jes_13TeV,CMS_sig_p2_jer_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs.out\n")
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M MaxLikelihoodFit --signif -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(mass)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --plots --out ${CMSSW_BASE}/src/DijetCombineLimitCode/plots --freezeParameters CMS_sig_p1_jes_13TeV,CMS_sig_p2_jer_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_MaxLikelihoodFit.out\n")
                    outputfile.write("mv higgsCombine"+chan+str(cut)+str(bin)+"_"+str(uncertainty)+".Asymptotic.mH"+str(int(mass))+".root Limits/CMS_jj_"+str(mass)+"_"+chan+"_"+str(cut)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs_new.root")
                if uncertainty == "CMS_sig_p1_jes_13TeV":
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M Asymptotic -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --freezeParameters CMS_sig_p2_jer_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs.out\n")
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M MaxLikelihoodFit --signif -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(mass)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --plots --out ${CMSSW_BASE}/src/DijetCombineLimitCode/plots --freezeParameters CMS_sig_p2_jer_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_MaxLikelihoodFit.out\n")
                    outputfile.write("mv higgsCombine"+chan+str(cut)+str(bin)+"_"+str(uncertainty)+".Asymptotic.mH"+str(int(mass))+".root Limits/CMS_jj_"+str(mass)+"_"+chan+"_"+str(cut)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs_new.root")
                if uncertainty == "CMS_sig_p2_jer_13TeV":
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M Asymptotic -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --freezeParameters CMS_sig_p1_jes_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs.out\n")
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M MaxLikelihoodFit --signif -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(mass)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --plots --out ${CMSSW_BASE}/src/DijetCombineLimitCode/plots --freezeParameters CMS_sig_p1_jes_13TeV,CMS_bkg_fit_slope1__invMass_13TeV  &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_MaxLikelihoodFit.out\n")
                    outputfile.write("mv higgsCombine"+chan+str(cut)+str(bin)+"_"+str(uncertainty)+".Asymptotic.mH"+str(int(mass))+".root Limits/CMS_jj_"+str(mass)+"_"+chan+"_"+str(cut)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs_new.root")
                if  uncertainty == "CMS_bkg_fit_slope1__invMass_13TeV" or uncertainty == "CMS_bkg_fit_slope1__invMass_afterVBFsel_13TeV":
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M Asymptotic -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --freezeParameters CMS_sig_p1_jes_13TeV,CMS_sig_p2_jer_13TeV &>CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs.out\n")
                    outputfile.write("combine datacards/CMS_jj_"+chan+"_"+str(cut)+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+".txt -M MaxLikelihoodFit --signif -v2 -m "+str(mass) + " -n "+chan+str(cut)+str(mass)+str(bin)+"_"+str(uncertainty)+" --rMax 100 --rMin 0.000001 --plots --out ${CMSSW_BASE}/src/DijetCombineLimitCode/plots --freezeParameters CMS_sig_p1_jes_13TeV,CMS_sig_p2_jer_13TeV &>CMS_jj_"+chan+"_"+str(mass)+"_13TeV_"+bin+"_"+str(uncertainty)+"_MaxLikelihoodFit.out\n")
                    outputfile.write("mv higgsCombine"+chan+str(cut)+str(bin)+"_"+str(uncertainty)+".Asymptotic.mH"+str(int(mass))+".root Limits/CMS_jj_"+str(mass)+"_"+chan+"_"+str(cut)+"_13TeV_"+bin+"_"+str(uncertainty)+"_asymptoticCLs_new.root")

            outputfile.close()
            
  
            command="rm "+logname
            print command
            os.system(command)
            if fullToys:
              command="""bsub -q 8nh -o """+logname+" source "+outpu
              # command="""qsub -q all.q """+outputname+" source "+outputname
              # submitJobsOnT3batch.sh
            else:
                command="chmod 755 ./"+outputname+";./"+outputname
            print command
            os.system(command)
            os.system("chmod 755 ./"+outputname+";./"+outputname)
