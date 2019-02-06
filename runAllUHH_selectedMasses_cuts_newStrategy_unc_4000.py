# First insall combine according to this
# https://cms-hcomb.gitbooks.io/combine/content/part1/#for-end-users-that-dont-need-to-commit-or-do-any-development

import os
import sys
from optparse import OptionParser
argv = sys.argv
parser = OptionParser()
parser.add_option("-u", "--uncertainty", dest="uncertainty", action="store",default="all",
                              help="set datacard directory")
parser.add_option("-n", "--name", dest="name", action="store",default="correctPUPPIres",
                              help="set datacard name")
(opts, args) = parser.parse_args(argv)
cut = opts.name
uncertainty = opts.uncertainty

categories=["_invMass","_invMass_afterVBFsel"]
steps=[2,3]#,4,5]
masses=[4000]
signals=["graviton","radion"]

if 1 in steps:
    for mass in masses:
        for category in categories:
            for signal in signals:
                os.system('cp datacards/CMS_jj_'+str(signal)+'_'+str(cut)+'_'+str(mass)+'_13TeV_'+str(category)+'.txt  datacards/CMS_jj_'+str(signal)+'_'+str(cut)+'_'+str(mass)+'_13TeV_'+str(category)+'_'+str(uncertainty)+'.txt')
                datafile ='datacards/CMS_jj_'+str(signal)+'_'+str(cut)+'_'+str(mass)+'_13TeV_'+str(category)+'_'+str(uncertainty)+'.txt'
                if (uncertainty !="lumi_13TeV"):
                    os.system('sed -i -e  \'s/lumi_13TeV/\#lumi_13TeV/g\' '+str(datafile)) 
                if (uncertainty !="CMS_eff_vtag_tau21_sf_13TeV"):
                    os.system('sed -i -e  \'s/CMS_eff_vtag_tau21_sf_13TeV/\#CMS_eff_vtag_tau21_sf_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_pu_13TeV"):
                    os.system('sed -i -e  \'s/CMS_pu_13TeV/\#CMS_pu_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_PDF_acceptance_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_PDF_acceptance_j_13TeV/\#CMS_PDF_acceptance_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_SCALE_acceptance_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_SCALE_acceptance_j_13TeV/\#CMS_SCALE_acceptance_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_mass_scale_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_mass_scale_j_13TeV/\#CMS_mass_scale_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_mass_res_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_mass_res_j_13TeV/\#CMS_mass_res_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_scale_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_scale_j_13TeV/\#CMS_scale_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_res_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_res_j_13TeV/\#CMS_res_j_13TeV/g\' '+str(datafile))
                if (uncertainty !="CMS_L1pre_j_13TeV"):
                    os.system('sed -i -e  \'s/CMS_L1pre_j_13TeV/\#CMS_L1pre_j_13TeV/g\' '+str(datafile))
            os.system('python Limits/CombineDatacardsUHH_cuts_unc.py '+str(mass)+' '+str(cut)+' '+str(uncertainty))

# or uncertainty == "CMS_sig_p1_jes_13TeV" or uncertainty == "CMS_sig_p2_jer_13TeV" or uncertainty == "CMS_bkg_fit__invMass_13TeV_norm" or uncertainty == "CMS_bkg_fit_slope1__invMass_13TeV"):




 #                   os.system('sed  \'s/lumi_13TeV/\#lumi_13TeV/g\' datacards/CMS_jj_'+str(signal)+'_'+str(cut)+'_'+str(mass)+'_13TeV_'+str(category)+'.txt > datacards/CMS_jj_'+str(signal)+'_'+str(cut)+'_'+str(mass)+'_13TeV_'+str(category)+'_'+str(uncertainty)+'.txt')


categories=["_invMass","_invMass_afterVBFsel","_invMass_combined"]

if 2 in steps:
    for mass in masses:
        os.system('python Limits/CalcAsympLimitsUHH_cuts_unc.py '+str(mass)+' '+str(cut)+' '+str(uncertainty))

if 3 in steps:
    os.system('python Limits/brazilianFlag_theoryUncBand_selectedMasses_cuts_unc_4000.py '+str(cut)+' '+str(uncertainty))
