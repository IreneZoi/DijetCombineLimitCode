from ROOT import *
import ROOT
import array, math
import os,sys

mass=sys.argv[1]
cut=sys.argv[2]
unc=sys.argv[3]
signals=["graviton","radion"]
directory="datacards/"

for signal in signals:
  card=directory+"CMS_jj_"+signal+"_"+cut+"_"+mass+"_13TeV__invMass"
  command="combineCards.py " +card+"_"+unc+".txt " +card+"_afterVBFsel_"+unc+".txt > " +card+"_combined_"+unc+".txt"
  print command
  os.system( command  )
