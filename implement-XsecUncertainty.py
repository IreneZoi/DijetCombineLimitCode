import os,commands, os.path
import sys
sys.argv.append( '-b-' )
from ROOT import *
import time
from array import *
import fileinput


path = "/shome/thaarres/EXOVVAnalysisRunII/LimitCode/CMSSW_7_1_5/src/DijetCombineLimitCode/Systematics/dijet/"
prefixDCin = "datacards/CMS_jj_"
prefixDCout = "datacards/CMS_jj_"
prefix = "EXOVVSystematics/dijet/"


purities = ["LP","HP"]
channels = ["WW","WZ","ZZ"]
signals=["BulkWW","BulkZZ","WZ","ZprimeWW"]
masses_interpolated =[m*100 for m in range(10,42+1)]

# masses_interpolated =[2000]
# channels=["WW"]
# purities = ["HP"]

for purity in purities:
  ii = -1
  for signal in signals:
      ii += 1
      for ch in channels:
        dict = {}
        chnl = "_" + ch + "_"
        fname = prefix+signal+"_signal_xsec_uncertainty.txt"
        UNCERTAINTY=[]

        with open(fname,"r") as unc:
          for l in unc:
            if l.find('mass') != -1: continue
            # if not l.split(' ')[0].find(signalsLabel[ii]) != -1: continue
            for m in masses_interpolated:
              if l.find("%i"%m) != -1:
                XsecUnc = 1 + float(l.split(' ')[1])
                dict[m] = XsecUnc
              if m < 1200.:
                if l.find("1200") != -1:
                  XsecUnc = 1 + float(l.split(' ')[1])
                  dict[m] = XsecUnc
                
        print "For %s_%s_%s :" %(signal,purity,ch)        
        for m in masses_interpolated:
          print "   Mass = %i , xsec unc. = %.3f " %(m, dict[m])   
          fname_datacard_in = prefixDCin + "%s_%i"%(signal,m)+"_13TeV_CMS_jj_"+ch+purity+".txt"
          fname_datacard_out = prefixDCout + "%s_%i"%(signal,m)+"_13TeV_CMS_jj_"+ch+purity+".txt"
          print "Input datacard:  %s" %fname_datacard_in
          print "Output datacard: %s"  %fname_datacard_out
          lines = []   
          try:
            with open(fname_datacard_in) as infile:
              for line in infile:
                if not line.find("CMS_xww_XS_")!=-1:
                  lines.append(line)
              
              if signal.find("BulkWW")  !=-1: label = "BulkG_WW"
              if signal.find("BulkZZ")  !=-1: label = "BulkG_ZZ"
              if signal.find("WZ")      !=-1: label = "Wprime"
              if signal.find("ZprimeWW")!=-1: label = "Zprime"
              xu="\nCMS_xww_XS_%s_13TeV lnN      %s      %s      -\n"%(label,dict[m],dict[m])
              lines.append(xu)
              with open(fname_datacard_out, 'w') as outfile:
                for line in lines:
                  outfile.write(line)
          except EnvironmentError: # parent of IOError, OSError *and* WindowsError where available
              print 'oops, datacard not found!'