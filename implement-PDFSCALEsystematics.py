from ROOT import *
import fileinput
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
parser.add_option("-d", "--directory", dest="directory", action="store",default="testInvertedSelection/JMSJMR",
                              help="set datacard directory")
parser.add_option("-n", "--name", dest="name", action="store",default="JMSJMR",
                              help="set datacard name")
(opts, args) = parser.parse_args(argv)  
path = opts.path
outpath=opts.outpath
directory=opts.directory 
name=opts.name

prefixDCin = outpath+"datacards/"+directory+"/CMS_jj_"
prefixDCout = outpath+"datacards/"+directory+"/CMS_jj_"

#prefix = "EXOVVSystematics/dijet"

#purities = ["LP","HP"]
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
    if "graviton" in signal:
        massesInSystematics = [1200,1400,1600,1800,2000,2500,3000,3500,4000] 
    if "radion" in signal:
        massesInSystematics = [1200,1400,1600,1800,2000,2500,3500,4000,4500]
    for category in categories:
        if signal.find("graviton")  !=-1: label = "graviton"
        if signal.find("radion")    !=-1: label = "radion"
    #if signal.find("WZ")      !=-1: label = "WprimeWZ"
    #if signal.find("ZprimeWW")!=-1: label = "ZprimeWW"
        fname_PDFacc = path+"PDFacc/PDFaccsys_"+label+"_"+category+".txt"
        fname_PDFxs = path+"PDFxs/PDFxssys_"+label+"_"+category+".txt"
        fname_SCALEacc = path+"SCALEacc/SCALEaccsys_"+label+"_"+category+".txt"
        fname_SCALExs = path+"SCALExs/SCALExssys_"+label+"_"+category+".txt"
        PDFaccUP={} 
        PDFaccDOWN={}
        PDFxsUP={}        
        PDFxsDOWN={}
        SCALEaccUP={}
        SCALEaccDOWN={}
        SCALExsUP={}
        SCALExsDOWN={}
        print "For %s_%s :" %(signal,category)       
        print "Input systematics cards:"
        print fname_PDFacc
        print fname_PDFxs
        print fname_SCALEacc
        print fname_SCALExs
        
        with open(fname_PDFacc,"r") as PDFacc:
            for l in PDFacc:
                print l
                if l.find('mass') != -1: continue
                for m in masses_interpolated:
              #print " find mass "+str(m)+ " found : "+str( l.find("%i"%m))
                    if not l.find("%i"%m) != -1: continue
                    PDFaccup = 1 + float(l.split(' ')[1])/100. 
                    PDFaccdown = 1 - float(l.split(' ')[1])/100.
                    PDFaccUP[m] = PDFaccup
                    PDFaccDOWN[m] = PDFaccdown
                    
        with open(fname_PDFxs,"r") as PDFxs:
            for l in PDFxs:
                if l.find('mass') != -1: continue
                for m in masses_interpolated:
                    if not l.find("%i"%m) != -1: continue
                    PDFxsup = 1 + float(l.split(' ')[1])/100.
                    PDFxsdown =1 - float(l.split(' ')[1])/100.
                    PDFxsUP[m] = PDFxsup
                    PDFxsDOWN[m] = PDFxsdown      
        
        with open(fname_SCALEacc,"r") as SCALEacc:
            for l in SCALEacc:
                if l.find('mass') != -1: continue
                for m in masses_interpolated:
                    if not l.find("%i"%m) != -1: continue
                    SCALEaccup = 1 + float(l.split(' ')[1])/100. 
                    SCALEaccdown = 1 + float(l.split(' ')[2])/100.
                    SCALEaccUP[m] = SCALEaccup
                    SCALEaccDOWN[m] = SCALEaccdown
                    
        with open(fname_SCALExs,"r") as SCALExs:
            for l in SCALExs:
                if l.find('mass') != -1: continue
                for m in masses_interpolated:
                    if not l.find("%i"%m) != -1: continue
                    SCALExsup = 1 + float(l.split(' ')[1])/100.
                    SCALExsdown =1 + float(l.split(' ')[2])/100.
                    SCALExsUP[m] = SCALExsup
                    SCALExsDOWN[m] = SCALExsdown
        
        for m in masses_interpolated:
            if not m in massesInSystematics:
                print "THIS MASSPOINT IS NOT IN LIST!! Extrapolating to lower: "
                ref = m - 100
                PDFaccUP[m] = PDFaccUP[ref]
                PDFaccDOWN[m] = PDFaccDOWN[ref]
                PDFxsUP[m]   = PDFxsUP[ref]
                PDFxsDOWN[m] = PDFxsDOWN[ref]
                SCALEaccUP[m]   = SCALEaccUP[ref]
                SCALEaccDOWN[m] = SCALEaccDOWN[ref]
                SCALExsUP[m]   = SCALExsUP[ref]
                SCALExsDOWN[m] = SCALExsDOWN[ref]
                print " PDFaccUP[%i]=PDFaccUP[%i]=%f" %(m,ref,PDFaccUP[m])
            
          
        for m in masses_interpolated:
            print ""
            print "Mass = %i: " %(m)
            print " PDFacc UP/DOWN =    %.4f     %.4f  " %(PDFaccUP[m],PDFaccDOWN[m])
            print " PDFxs UP/DOWN =    %.4f     %.4f  " %(PDFxsUP[m],PDFxsDOWN[m])
            print " SCALEacc UP/DOWN =    %.4f     %.4f  " %(SCALEaccUP[m],SCALEaccDOWN[m])
            print " SCALExs UP/DOWN =    %.4f     %.4f  " %( SCALExsUP[m],SCALExsDOWN[m])
            fname_datacard_in = prefixDCin + "%s_%s_%i"%(signal,name,m)+"_13TeV__"+category+".txt"
            fname_datacard_out = prefixDCout + "%s_%s_%i"%(signal,name,m)+"_13TeV__"+category+".txt"
            print "Input datacard:  %s" %fname_datacard_in
            print "Output datacard: %s"  %fname_datacard_out
            lines = []
            try:
                with open(fname_datacard_in) as infile:
                    for line in infile:
                        if not (line.find("CMS_mass_scale_j_13TeV")!=-1 or line.find("CMS_mass_res_j_13TeV")!=-1 or line.find("CMS_scale_j_13TeV")!=-1 or line.find("CMS_res_j_13TeV")!=-1 ):
                            lines.append(line)
                    PDFacc="\nCMS_PDF_acceptance_j_13TeV       lnN      %s/%s           -                          \n"%(PDFaccUP[m],PDFaccDOWN[m])
                    PDFxs="CMS_PDF_XS_j_13TeV         lnN      %s/%s           -                          \n"%(PDFxsUP[m],PDFxsDOWN[m])
                    SCALEacc="CMS_SCALE_acceptance_j_13TeV            lnN      %s/%s           -       \n"%(SCALEaccUP[m],SCALEaccDOWN[m])
                    SCALExs="CMS_SCALE_XS_j_13TeV              lnN      %s/%s           -   \n"%(SCALExsUP[m],SCALExsDOWN[m])
                    lines.append(PDFacc)
#                    lines.append(PDFxs)
                    lines.append(SCALEacc)
#                    lines.append(SCALExs)
                            
                    with open(fname_datacard_out, 'w') as outfile:
                        for line in lines:
                            outfile.write(line)
                    print "PRINTED TO: %s"  %fname_datacard_out
            except EnvironmentError: # parent of IOError, OSError *and* WindowsError where available
                print 'oops, datacard not found!'
