from ROOT import *
#import fileinput
from optparse import OptionParser
import sys

argv = sys.argv
parser = OptionParser()   
parser.add_option("-b", "--batch", dest="batch", default=False,action="store_true",
                              help="set batch mode")
parser.add_option("-s", "--signal", dest="signal", default="BulkWW",action="store",
                              help="set signal. only in batch mode")
parser.add_option("-m", "--mass", dest="mass", default=1200,action="store",
                              help="set mass. only in batch mode")
parser.add_option("-p", "--path", dest="path",action="store", default="/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/forSystematics/",
                              help="set input path")
parser.add_option("-o", "--outpath", dest="outpath", action="store",default="/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/",
                              help="set output path")
parser.add_option("-d", "--directory", dest="directory", action="store",default="datacards/testInvertedSelection/JMSJMR",
                              help="set datacard directory")
parser.add_option("-n", "--name", dest="name", action="store",default="JMSJMR",
                              help="set datacard name")
(opts, args) = parser.parse_args(argv)  
path = opts.path
outpath=opts.outpath
directory=opts.directory 
name=opts.name
mass=opts.mass

prefixDCin = outpath+directory+"/CMS_jj_"
prefixDCout = outpath+directory+"/CMS_jj_"

categories=["invMass","invMass_afterVBFsel"]#,"invMass_combined"]
#channels = ["WW","WZ","ZZ","VV"]
label =""
signals=["graviton","radion"]#["BulkWW","BulkZZ","WZ","ZprimeWW"]
masses_interpolated =[m*100 for m in range(12,45+1)] #irene ?
massesInSystematics = [1200,1400,1600,1800,2000,2500,3000,3500,4000,4500] #irene ?

if opts.batch:
    masses_interpolated = [int(opts.mass)]
    signals = [opts.signal]
    label = opts.signal
    print "test "+label
    print "test 2 " +opts.signal+ " "+ str(opts.mass)

for signal in signals:
#    if "graviton" in signal:
 #       massesInSystematics = [1200,1400,1600,1800,2000,2500,3000,3500,4000] 
  #  if "radion" in signal:
   #     massesInSystematics = [1200,1400,1600,1800,2000,2500,3500,4000,4500]
    for category in categories:
        if signal.find("graviton")  !=-1: label = "graviton"
        if signal.find("radion")    !=-1: label = "radion"
    #if signal.find("WZ")      !=-1: label = "WprimeWZ"
    #if signal.find("ZprimeWW")!=-1: label = "ZprimeWW"
        fname_JMS = path+"JMS/JMSsys_"+label+"_"+category+".txt"
        fname_JMR = path+"JMR/JMRsys_"+label+"_"+category+".txt"
        fname_JES = path+"JES/JESsys_"+label+"_"+category+".txt"
        fname_JER = path+"JER/JERsys_"+label+"_"+category+".txt"
        JMSUP={}
        JMSDOWN={}
        JMRUP={}
        JMRDOWN={}
        jesUP={}
        jesDOWN={}
        jerUP={}
        jerDOWN={}
        print "For %s_%s :" %(signal,category)       
        print "Input systematics cards:"
        print fname_JMS
        print fname_JMR
        print fname_JES
        print fname_JER
        
        with open(fname_JMS,"r") as JMS:
            for l in JMS:
                print l
                if l.find('mass') != -1: continue
    #            for m in masses_interpolated:
              #print " find mass "+str(m)+ " found : "+str( l.find("%i"%m))
                if not l.find("%s"%mass) != -1: continue
                JMSup = 1 + float(l.split(' ')[1])/100. 
                JMSdown = 1 + float(l.split(' ')[2])/100.
                JMSUP[mass] = JMSup
                JMSDOWN[mass] = JMSdown
                    
        with open(fname_JMR,"r") as JMR:
            for l in JMR:
                if l.find('mass') != -1: continue
#                for m in masses_interpolated:
                if not l.find("%s"%mass) != -1: continue
                jmrup = 1 + float(l.split(' ')[1])/100.
                jmrdown =1 + float(l.split(' ')[2])/100.
                JMRUP[mass] = jmrup
                JMRDOWN[mass] = jmrdown      
        
        with open(fname_JES,"r") as JES:
            for l in JES:
                if l.find('mass') != -1: continue
#                for m in masses_interpolated:
                if not l.find("%s"%mass) != -1: continue
                jesup = 1 + float(l.split(' ')[1])/100. 
                jesdown = 1 + float(l.split(' ')[2])/100.
                jesUP[mass] = jesup
                jesDOWN[mass] = jesdown
                    
        with open(fname_JER,"r") as JER:
            for l in JER:
                if l.find('mass') != -1: continue
                #for m in masses_interpolated:
                if not l.find("%s"%mass) != -1: continue
                jerup = 1 + float(l.split(' ')[1])/100.
                jerdown =1 + float(l.split(' ')[2])/100.
                jerUP[mass] = jerup
                jerDOWN[mass] = jerdown
        
#        for m in masses_interpolated:
 #           if not m in massesInSystematics:
  #              print "THIS MASSPOINT IS NOT IN LIST!! Extrapolating to lower: "
  #              ref = mass - 100
  #              JMSUP[m] = JMSUP[ref]
  #              JMSDOWN[m] = JMSDOWN[ref]
  #              JMRUP[m]   = JMRUP[ref]
  #              JMRDOWN[m] = JMRDOWN[ref]
  #              jesUP[m]   = jesUP[ref]
  #              jesDOWN[m] = jesDOWN[ref]
  #              jerUP[m]   = jerUP[ref]
  #              jerDOWN[m] = jerDOWN[ref]
   #             print " JMSUP[%i]=JMSUP[%i]=%f" %(m,ref,JMSUP[m])
            
          
    #    for m in masses_interpolated:
        print ""
        print "Mass = %s: " %(mass)
        print " JMS UP/DOWN =    %.4f     %.4f  " %(JMSUP[mass],JMSDOWN[mass])
        print " JMR UP/DOWN =    %.4f     %.4f  " %(JMRUP[mass],JMRDOWN[mass])
        print " JES UP/DOWN =    %.4f     %.4f  " %(jesUP[mass],jesDOWN[mass])
        print " JER UP/DOWN =    %.4f     %.4f  " %( jerUP[mass],jerDOWN[mass])
        fname_datacard_in = prefixDCin + "%s_%s_%s"%(signal,name,mass)+"_13TeV__"+category+".txt"
        fname_datacard_out = prefixDCout + "%s_%s_%s"%(signal,name,mass)+"_13TeV__"+category+".txt"
        print "Input datacard:  %s" %fname_datacard_in
        print "Output datacard: %s"  %fname_datacard_out
        lines = []
        try:
            with open(fname_datacard_in) as infile:
                for line in infile:
                    if not (line.find("CMS_mass_scale_j_13TeV")!=-1 or line.find("CMS_mass_res_j_13TeV")!=-1 or line.find("CMS_scale_j_13TeV")!=-1 or line.find("CMS_res_j_13TeV")!=-1 ):
                        lines.append(line)
                jms="\nCMS_mass_scale_j_13TeV       lnN      %s/%s           -                          \n"%(JMSUP[mass],JMSDOWN[mass])
                jmr="CMS_mass_res_j_13TeV         lnN      %s/%s           -                          \n"%(JMRUP[mass],JMRDOWN[mass])
                jes="CMS_scale_j_13TeV            lnN      %s/%s           -  # jet energy scale      \n"%(jesUP[mass],jesDOWN[mass])
                jer="CMS_res_j_13TeV              lnN      %s/%s           -  # jet energy resolution \n"%(jerUP[mass],jerDOWN[mass])
                L1pre="CMS_L1pre_j_13TeV             lnN     1.02/0.98            -                           \n"
                lines.append(jms)
                lines.append(jmr)
                lines.append(jes)
                lines.append(jer)
                lines.append(L1pre)
                with open(fname_datacard_out, 'w') as outfile:
                    for line in lines:
                        outfile.write(line)
                    print "PRINTED TO: %s"  %fname_datacard_out
        except EnvironmentError: # parent of IOError, OSError *and* WindowsError where available
            print 'oops, datacard not found!'
