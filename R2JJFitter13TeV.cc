/** \macro H2GGFitter.cc
 *
 * $Id: R2JJFitter.cc,v 1.14 2013/06/27 10:18:27 hinzmann Exp $
 *
 * Software developed for the CMS Detector at LHC
 *
 *
 *  \author Serguei Ganjour - CEA/IRFU/SPP, Saclay
 *  \modified by Maxime Gouzevitch for the Dijet Bump Search - IPNL, Lyon 
 *
 * Macro is implementing the unbinned maximum-likelihood model for 
 * the Higgs to gamma gamma analysis. PDF model and RooDataSets 
 * are stored in the workspace which is feeded to  HiggsAnalysis/CombinedLimit tools:
 * 
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit
 * http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/HiggsAnalysis/CombinedLimit/data/lhc-hcg/cms-jj-1fb/
 * 
 * The analysis root trees produced in a simple format 
 *
 *     TFile file(filename,"RECREATE", "X->jj input tree for unbinned maximum-likelihood fit");
 *     TTree* outTree  = new TTree("XTojj","X->jj input tree for unbinned maximum-likelihood fit");
 *     Float_t mass;
 *     Int_t CAT3;
 *     Float_t weight;
 *
 *     outTree->Branch("mass",&mass,"mass/F");
 *     outTree->Branch("weight",&weight,"weight/F");
 *     outTree->Branch("CAT4",&CAT4,"CAT4/I");
 *     {
 *       .............
 *       outTree->Fill();
 *     }
 *
 *     file.Write();
 *     file.Close();
 *     delete outTree;
 *
 * are used as input files. They have to be produced for 
 * data and Monte Carlo signal and background data sets 
 * after all analysis selections to be applied. It is recommended to put   
 * loose kinematical cuts on pt1 and pt2 (20 GeV) since further selections 
 * are possible based on RooDataSets. 
 * It is recommended to use Root 5.28/00 (CMSSW_4_1_3).
 *
 *
 */
// Loading:  .L H2GGFitter.cc
// Running:  runfits("jj120-shapes-combined-Unbinned.root")  
//                

/*
#include <cstring>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <unistd.h>
#include <errno.h>
#include <iomanip>
// ROOT headers
#include "TCanvas.h"
#include "TAxis.h"
#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TIterator.h"

#include "TLatex.h"
#include "TString.h"
#include "TLegend.h"
#include "TGraphAsymmErrors.h"


// RooFit headers
#include "RooAbsPdf.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
#include "RooHistPdf.h"
#include "RooMsgService.h"
#include "RooNLLVar.h"
#include "RooPlot.h"
#include "RooRandom.h"
#include "RooRealVar.h"
#include "RooWorkspace.h"
#include "TStyle.h"

// RooStats headers
#include "RooStats/HLFactory.h"

#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooAbsData.h"
#include "RooPlot.h"
#include "RooGaussian.h"
#include "RooProduct.h"
#include "RooExtendPdf.h"
#include "RooBernstein.h"
#include "RooFitResult.h"
#include "RooMinimizer.h"
#include "RooCmdArg.h"
#include "RooConstVar.h"
#include "RooRealVar.h"
*/
//#include "HiggsCSandWidth.h"
//#include "HiggsCSandWidth.cc"
//#include "RooPower.h"

using namespace RooFit;
using namespace RooStats ;

static const Int_t NCAT = 12; //for VV and qV analysis together this should be 6--> Now 21!
Double_t MMIN = 1000;
Double_t MMAX = 2500;
std::string filePOSTfix="";
double signalScaler=2460.00*1.00/10000.; // assume signal cross section of 0.01pb=10fb and 1263.890/pb of luminosity (The factor 10000. is the number of gen events that is set to 10000. for all samples in the interpolation script
double scaleFactorHP=0.992; // tau21 and jet mass scale factors data/MC
double scaleFactorLP=1.023; // tau21 and jet mass scale factors data/MC
double scaleFactorHPherwig=1.0; // tau21 and jet mass scale factors Herwig/Pythia
double scaleFactorLPherwig=1.3; // tau21 and jet mass scale factors Herwig/Pythia

void AddSigData(RooWorkspace*, Float_t);
void AddBkgData(RooWorkspace*);
void SigModelFit(RooWorkspace*, Float_t);
RooFitResult*  BkgModelFitBernstein(RooWorkspace*, Bool_t);
void MakePlots(RooWorkspace*, Float_t, RooFitResult* , TString signalname);
void MakeSigWS(RooWorkspace* w, const char* filename);
void MakeBkgWS(RooWorkspace* w, const char* filename);
void SetConstantParams(const RooArgSet* params);

RooArgSet* defineVariables()
{
  // define variables of the input ntuple
  RooRealVar* mgg  = new RooRealVar("mgg13TeV","M(jet-jet)",MMIN,MMAX,"GeV");
  RooRealVar* evWeight   = new RooRealVar("evWeight","Reweightings",0,100,"");
  RooRealVar* normWeight  = new RooRealVar("normWeight","Additionnal Weight",0,10000000,"");
  RooCategory* categories = new RooCategory("categories","event category NCAT") ;
  // categories->defineType("highPureVV",0);
//   categories->defineType("mediumPureVV",1);
//   categories->defineType("lowPureVV",2);
//   categories->defineType("highPureqV",3);
//   categories->defineType("mediumPureqV",4);
//   categories->defineType("lowPureqV",5);
  
  categories->defineType("highPureVV",0);
  categories->defineType("lowPureVV",1);
  categories->defineType("noPureVV",2);
  categories->defineType("highPureWW",3);
  categories->defineType("lowPureWW",4);
  categories->defineType("noPureWW",5);
  categories->defineType("highPureWZ",6);
  categories->defineType("lowPureWZ",7);
  categories->defineType("noPureWZ",8);
  categories->defineType("highPureZZ",9);
  categories->defineType("lowPureZZ",10);
  categories->defineType("noPureZZ",11);
  categories->defineType("highPureqV",12);
  categories->defineType("lowPureqV",13);
  categories->defineType("noPureqV",14);
  categories->defineType("highPureqW",15);
  categories->defineType("lowPureqW",16);
  categories->defineType("noPureqW",17);
  categories->defineType("highPureqZ",18);
  categories->defineType("lowPureqZ",19);
  categories->defineType("noPureqZ",20);
  RooArgSet* ntplVars = new RooArgSet(*mgg, *categories, *evWeight, *normWeight);
 
  return ntplVars;
}



void runfits(const Float_t mass=2000, int signalsample = 1, Bool_t dobands = false)
{

//******************************************************************//
//  Running mode  corresponds to the following cases
//         - full run set:
//         - create signal and background data sets 
//         - make and fit signal and background  models 
//         - write signal and background workspaces in root files
//         - write data card

//*******************************************************************//


  TString signalname;
  if (signalsample==0)
  { signalname="RS1ZZ";
  }
  if (signalsample==1)
  { signalname="RS1WW";
  }
  if (signalsample==2)
  { signalname="WZ";
  }
  if (signalsample==3)
  { signalname="qW";
  }
  if (signalsample==4)
  { signalname="qZ";
  }
  if (signalsample==5)
  { signalname="BulkWW";
  }
  if (signalsample==6)
  { signalname="BulkZZ";
  }
  TString fileBaseName("CMS_jj_"+signalname+TString::Format("_%.0f_13TeV", mass));

  vector<string> cat_names;
  cat_names.push_back("CMS_jj_VVHP");
  cat_names.push_back("CMS_jj_VVLP");
  cat_names.push_back("CMS_jj_VVNP");
  cat_names.push_back("CMS_jj_WWHP");
  cat_names.push_back("CMS_jj_WWLP");
  cat_names.push_back("CMS_jj_WWNP");
  cat_names.push_back("CMS_jj_WZHP");
  cat_names.push_back("CMS_jj_WZLP");
  cat_names.push_back("CMS_jj_WZNP");
  cat_names.push_back("CMS_jj_ZZHP");
  cat_names.push_back("CMS_jj_ZZLP");
  cat_names.push_back("CMS_jj_ZZNP");
  cat_names.push_back("CMS_jj_qVHP");
  cat_names.push_back("CMS_jj_qVLP");
  cat_names.push_back("CMS_jj_qVNP");
  cat_names.push_back("CMS_jj_qWHP");
  cat_names.push_back("CMS_jj_qWLP");
  cat_names.push_back("CMS_jj_qWNP");
  cat_names.push_back("CMS_jj_qZHP");
  cat_names.push_back("CMS_jj_qZLP");
  cat_names.push_back("CMS_jj_qZNP");


  TString fileBkgName(TString::Format("CMS_jj_bkg_13TeV", mass));
  TString card_name("Xvv_models_Bkg_13TeV.rs");
  HLFactory hlf("HLFactory", card_name, false);
  RooWorkspace* w = hlf.GetWs();
  RooFitResult* fitresults;

  w->var("mgg13TeV")->setMin(MMIN);
  w->var("mgg13TeV")->setMax(MMAX);

// Add data to the workspace

  cout << "CREATE SIGNAL" << endl;
  
  AddSigData(w, mass, signalsample,cat_names);

  cout << "CREATE BACKGROUND" << endl;

  AddBkgData(w,cat_names);
  
// Add the signal and background models to the workspace.
// Inside this function you will find a discription our model.
// Fit data with models

  cout << "FIT SIGNAL" << endl;

  SigModelFit(w, mass, signalname,cat_names);
    
  cout << "FIT BACKGROUND" << endl;

  fitresults = BkgModelFitBernstein(w, dobands,cat_names);
      
// Make statistical treatment
// Setup the limit on Higgs production

  cout << "CREATE SIGNAL WS" << endl;
  
  MakeSigWS(w, fileBaseName, signalname,cat_names);

  cout << "CREATE BACKGROUND WS" << endl;
  
  MakeBkgWS(w, fileBkgName,cat_names);

  cout << "CREATE DATACARD" << endl;

  if((signalsample==0)||(signalsample==1)||(signalsample==2)||(signalsample==5)||(signalsample==6))
  {
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 0, signalname, signalsample, cat_names, mass);
    if(NCAT>1)
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 1, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 2, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 3, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 4, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 5, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 6, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 7, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 8, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 9, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 10, signalname, signalsample, cat_names, mass);
    MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 11, signalname, signalsample, cat_names, mass);

  }
  if((signalsample==3)||(signalsample==4))
  {
    if(NCAT>3)
      
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 12, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 13, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 14, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 15, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 16, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 17, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 18, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 19, signalname, signalsample, cat_names, mass);
      MakeDataCard_1Channel(w, fileBaseName, fileBkgName, 20, signalname, signalsample, cat_names, mass);
  }

//   cout << "MAKE PLOTS" << endl;
//
// // Make plots for data and fit results
//   MakePlots(w, mass, fitresults, signalname,cat_names);
     
     cout << "DONE!" << endl;


    return;



}



void AddSigData(RooWorkspace* w, Float_t mass, int signalsample, std::vector<string> cat_names) {

  Int_t ncat = NCAT;
  TString inDir   = "./MiniTrees/Signal_VV_13TeV/";

  Float_t MASS(mass);
  Float_t sqrts(8);


//****************************//
// Signal Data Set
//****************************//

  // Variables
  RooArgSet* ntplVars = defineVariables();
  RooRealVar weightVar("weightVar","",1,0,1000);

//signal300_tree_radcut.root
  int iMass = abs(mass);       
  TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_RS1ZZOUT%d_miniTree.root", iMass)));  
  if (signalsample==1) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_RS1WWOUT%d_miniTree.root", iMass)));
  }
  if (signalsample==2) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_WZOUT%d_miniTree.root", iMass)));
  }
  if (signalsample==3) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_QstarQWOUT%d_miniTree.root", iMass)));
  }
  if (signalsample==4) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_QstarQZOUT%d_miniTree.root", iMass)));
  }
  if (signalsample==5) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_BulkWWOUT%d_miniTree.root", iMass)));
  }
  if (signalsample==6) {
    sigFile1.Close();
    TFile sigFile1(inDir+TString(Form("dijetVV_13TeV_BulkZZOUT%d_miniTree.root", iMass)));
  }

  TTree* sigTree1 = (TTree*) sigFile1.Get("TCVARS");


// add weight var into the list of ntuple variables
  weightVar.setVal(1.);
  ntplVars->add(RooArgList(weightVar));

// common preselection cut
  TString mainCut("1");


//****************************//
// Signal  Data Set
//****************************//
// Create non scaled signal dataset composed with  different productions 
// according to their cross sections

  RooDataSet sigScaled("sigScaled","dataset",sigTree1,*ntplVars,mainCut,"evWeight");

//  RooRealVar *scaleWeightVar1 = (RooRealVar*) (*ntplVars)["evWeight"] ;
//  RooRealVar *scaleWeightVar2 = (RooRealVar*) (*ntplVars)["normWeight"] ;
//  RooFormulaVar *scaleWeightVar3 = new RooFormulaVar( "scaleWeight3", "", "@0*@1", 
//						      RooArgList(*scaleWeightVar1, *scaleWeightVar2));

  sigScaled.Print("v");

  RooDataSet* sigToFit[NCAT];
  for (int c = 0; c < ncat; ++c) {
    sigToFit[c] =  (RooDataSet*) sigScaled.reduce(*w->var("mgg13TeV"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*sigToFit[c],Rename(TString::Format("Sig_%s",cat_names.at(c).c_str())));
  }

          
// Create full signal data set without categorization
  RooDataSet* sigToFitAll  = (RooDataSet*) sigScaled.reduce(*w->var("mgg13TeV"),mainCut);
  w->import(*sigToFitAll,Rename("Sig"));

// Create weighted signal dataset composed with  different 
// production processes according to their cross sections
// no common preselection cut applied yet 

  RooRealVar *weightVar1 = (RooRealVar*) (*ntplVars)["evWeight"] ;
  RooRealVar *weightVar2 = (RooRealVar*) (*ntplVars)["normWeight"] ;
  RooFormulaVar *weightVar3 = new RooFormulaVar( "weight3", "", "@0*@1", RooArgList(*weightVar1, *weightVar2));

  weightVar.setVal(1.);
  ntplVars->setRealValue("normWeight", 1.);
  RooDataSet sigWeightedTmp1("sigData","dataset",sigTree1,*ntplVars,mainCut,"weightVar");
  RooRealVar *weightX = (RooRealVar*) sigWeightedTmp1.addColumn(*weightVar3) ;
  RooDataSet sigWeighted("sigData","dataset",
			 RooArgList((*ntplVars)["mgg13TeV"],
				    (*ntplVars)["categories"],*weightX),
			 Import(sigWeightedTmp1),WeightVar(*weightX));

  cout << "---- nX:  " << sigWeighted.sumEntries() << endl; 
  for (int c = 0; c < ncat; ++c) {
    Float_t nExpEvt = sigWeighted.reduce(*w->var("mgg13TeV"),TString::Format("categories==%d",c))->sumEntries();
    cout << TString::Format("nEvt exp.  %s : ",cat_names.at(c).c_str()) << nExpEvt << endl; 
  }

  sigWeighted.Print("v");


// apply a common preselection cut;
// split into 4  categories;

  RooDataSet* signal[NCAT];
  for (int c = 0; c < ncat; ++c) {
    signal[c] =  (RooDataSet*) sigWeighted.reduce(*w->var("mgg13TeV"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*signal[c],Rename(TString::Format("SigWeight_%s",cat_names.at(c).c_str())));
  }
// Create full weighted signal data set without categorization
  RooDataSet* signalAll  = (RooDataSet*) sigWeighted.reduce(*w->var("mgg13TeV"),mainCut);
  w->import(*signalAll, Rename("SigWeight"));

}


void AddBkgData(RooWorkspace* w, std::vector<string> cat_names) {

  Int_t ncat = NCAT;
  TString inDir   = "./MiniTrees/Data_VV_13TeV/";

// common preselection cut
  TString mainCut("1");

  Float_t minMassFit(MMIN),maxMassFit(MMAX); 


//****************************//
// CMS Data Set
//****************************//
// retrieve the data tree;
// no common preselection cut applied yet; 

  TFile dataFile(inDir+"dijetVV_13TeV_miniTree.root");   
  TTree* dataTree     = (TTree*) dataFile.Get("TCVARS");

  // Variables
  RooArgSet* ntplVars = defineVariables();

  RooDataSet Data("Data","dataset",dataTree,*ntplVars,"","normWeight");

// apply a common preselection cut;
// split into NCAT  categories;

  
  RooDataSet* dataToFit[NCAT];
  for (int c = 0; c < ncat; ++c) {
// Real data
    dataToFit[c]   = (RooDataSet*) Data.reduce(*w->var("mgg13TeV"),mainCut+TString::Format(" && categories==%d",c));
    w->import(*dataToFit[c],Rename(TString::Format("Data_%s",cat_names.at(c).c_str())));
  }

// Create full data set without categorization
  RooDataSet* data    = (RooDataSet*) Data.reduce(*w->var("mgg13TeV"),mainCut);
  w->import(*data, Rename("Data"));
  data->Print("v");

}



void SigModelFit(RooWorkspace* w, Float_t mass, TString signalname, std::vector<string> cat_names) {

  Int_t ncat = NCAT;
  Float_t MASS(mass);

//******************************************//
// Fit signal with model pdfs
//******************************************//
// retrieve pdfs and datasets from workspace to fit with pdf models



  RooDataSet* sigToFit[NCAT];
  RooAbsPdf* jjSig[NCAT];

  Float_t minMassFit(MMIN),maxMassFit(MMAX); 


// Fit Signal 

  for (int c = 0; c < ncat; ++c) {
    cout << "---------- category = " << c << endl;
    sigToFit[c]   = (RooDataSet*) w->data(TString::Format("SigWeight_%s",cat_names.at(c).c_str()));
    jjSig[c]     = (RooAbsPdf*)  w->pdf(signalname+"_jj"+TString::Format("_%s",cat_names.at(c).c_str()));

    cerr << ("jj_"+signalname+TString::Format("_sig_m0_%s",cat_names.at(c).c_str())) << endl;
    ((RooRealVar*) w->var("jj_"+signalname+TString::Format("_sig_m0_%s",cat_names.at(c).c_str())))->setVal(MASS);
  
    cout << " Mass = " << MASS << endl;
      


    jjSig[c]     ->fitTo(*sigToFit[c],Range(mass*0.8,mass*1.5),SumW2Error(kTRUE),PrintEvalErrors(-1));
// IMPORTANT: fix all pdf parameters to constant
    w->defineSet(TString::Format("SigPdfParam_%s",cat_names.at(c).c_str()), RooArgSet(*w->var("jj_"+signalname+TString::Format("_sig_m0_%s",cat_names.at(c).c_str())),
								   *w->var("jj_"+signalname+TString::Format("_sig_sigma_%s",cat_names.at(c).c_str())),
								   *w->var("jj_"+signalname+TString::Format("_sig_alpha_%s",cat_names.at(c).c_str())),
								   *w->var("jj_"+signalname+TString::Format("_sig_n_%s",cat_names.at(c).c_str())), 
								   *w->var("jj_"+signalname+TString::Format("_sig_gsigma_%s",cat_names.at(c).c_str())),
								   *w->var("jj_"+signalname+TString::Format("_sig_frac_%s",cat_names.at(c).c_str()))) );
    SetConstantParams(w->set(TString::Format("SigPdfParam_%s",cat_names.at(c).c_str())));
  }


}



RooFitResult* BkgModelFitBernstein(RooWorkspace* w, Bool_t dobands, std::vector<string> cat_names) {

  Int_t ncat = NCAT;

//******************************************//
// Fit background with model pdfs
//******************************************//

// retrieve pdfs and datasets from workspace to fit with pdf models


  RooDataSet* data[NCAT];
  RooBernstein* bkg_fit[NCAT];
  RooFitResult* fitresult[NCAT];;
  RooPlot* plotbkg_fit[NCAT];

// dobands and dosignal
  RooDataSet* signal[NCAT];
  RooAbsPdf*  jjSig[NCAT];


  Float_t minMassFit(MMIN),maxMassFit(MMAX); 

// Fit data with background pdf for data limit

  RooRealVar* mgg     = w->var("mgg13TeV");  
  mgg->setUnit("GeV");
  
  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);


  for (int c = 0; c < ncat; ++c) {
    data[c]   = (RooDataSet*) w->data(TString::Format("Data_%s",cat_names.at(c).c_str()));

    ((RooRealVar*) w->var(TString::Format("bkg_fit_slope1_%s",cat_names.at(c).c_str())))->setConstant(true);
    cout << "---------------- Parameter 1 set to const" << endl;

    ((RooRealVar*) w->var(TString::Format("bkg_fit_slope3_%s",cat_names.at(c).c_str())))->setConstant(true);
    cout << "---------------- Parameter 3 set to const" << endl;

    
    RooFormulaVar *p1mod = new RooFormulaVar(TString::Format("p1mod_%s",cat_names.at(c).c_str()),"","@0",*w->var(TString::Format("bkg_fit_slope1_%s",cat_names.at(c).c_str())));
    RooFormulaVar *p2mod = new RooFormulaVar(TString::Format("p2mod_%s",cat_names.at(c).c_str()),"","@0",*w->var(TString::Format("bkg_fit_slope2_%s",cat_names.at(c).c_str())));
    RooFormulaVar *p3mod = new RooFormulaVar(TString::Format("p3mod_%s",cat_names.at(c).c_str()),"","@0",*w->var(TString::Format("bkg_fit_slope3_%s",cat_names.at(c).c_str())));
     


    RooFormulaVar *sqrtS = new RooFormulaVar(TString::Format("sqrtS_%s",cat_names.at(c).c_str()),"","@0",*w->var("sqrtS"));
    RooFormulaVar *x = new RooFormulaVar(TString::Format("x_%s",cat_names.at(c).c_str()),"","@0/@1",RooArgList(*mgg, *sqrtS));


    
    // FOR COMBINE IN CMSSW_6_1
    //RooAbsPdf* bkg_fitTmp0 = new RooGenericPdf(TString::Format("DijetBackground_%d",c), "pow(1-@0, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    // alternative 3-parameter function
//    ////RooAbsPdf* bkg_fitTmp0 = new RooGenericPdf(TString::Format("DijetBackground_%d",c), "exp(-(@0-@1)/(1+@2*(@0-@1)+@3*(@0-@1)*(@0-@1)))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    //w->factory(TString::Format("bkg_fit_norm_%s[4000.0,0.0,10000000]",cat_names.at(c).c_str()));
    //RooExtendPdf bkg_fitTmp(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()),"",*bkg_fitTmp0,*w->var(TString::Format("bkg_fit_norm_%s",cat_names.at(c).c_str())));
    //fitresult[c] = bkg_fitTmp.fitTo(*data[c], Strategy(1),Minos(kFALSE), Range(minMassFit,maxMassFit),SumW2Error(kTRUE), Save(kTRUE),PrintEvalErrors(-1));
    //w->import(bkg_fitTmp);

    // FOR COMBINE IN CMSSW_7_1
    // RooAbsPdf* bkg_fitTmp = new RooGenericPdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()), "pow(1, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    RooAbsPdf* bkg_fitTmp = new RooGenericPdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()), "pow(1-@0, @1)/pow(@0, @2+@3*log(@0))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    // alternative 3-parameter function
//    //RooAbsPdf* bkg_fitTmp1 = new RooGenericPdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()), "exp(-(@0-@1)/(1+@2*(@0-@1)+@3*(@0-@1)*(@0-@1)))", RooArgList(*x, *p1mod, *p2mod, *p3mod));
    //w->factory(TString::Format("bkg_fit_%s_norm[4000.0,0.0,10000000]",cat_names.at(c).c_str()));
    RooAbsReal* bkg_fitTmp2  = new RooRealVar(TString::Format("bkg_fit_%s_norm",cat_names.at(c).c_str()),"",4000.0,0.0,10000000);
    w->import(*bkg_fitTmp);
    w->import(*bkg_fitTmp2);


//************************************************//
// Plot jj background fit results per categories 
//************************************************//
// Plot Background Categories 
//****************************//

    TCanvas* ctmp = new TCanvas("ctmp","jj Background Categories",0,0,500,500);
    Int_t nBinsMass(80);
    plotbkg_fit[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotbkg_fit[c],LineColor(kWhite),MarkerColor(kWhite));    

    bkg_fitTmp.plotOn(plotbkg_fit[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange"),PrintEvalErrors(-1)); 
    data[c]->plotOn(plotbkg_fit[c]);    

    plotbkg_fit[c]->Draw();  

//********************************************************************************//

    if (dobands) {

      RooAbsPdf *cpdf; cpdf = bkg_fitTmp0;
      TGraphAsymmErrors *onesigma = new TGraphAsymmErrors();
      TGraphAsymmErrors *twosigma = new TGraphAsymmErrors();
      
      RooRealVar *nlim = new RooRealVar(TString::Format("nlim%d",c),"",0.0,0.0,10.0);
      nlim->removeRange();
      
      RooCurve *nomcurve = dynamic_cast<RooCurve*>(plotbkg_fit[c]->getObject(1));
      
      for (int i=1; i<(plotbkg_fit[c]->GetXaxis()->GetNbins()+1); ++i) {
	double lowedge = plotbkg_fit[c]->GetXaxis()->GetBinLowEdge(i);
	double upedge  = plotbkg_fit[c]->GetXaxis()->GetBinUpEdge(i);
	double center  = plotbkg_fit[c]->GetXaxis()->GetBinCenter(i);
	
	double nombkg = nomcurve->interpolate(center);
	nlim->setVal(nombkg);
	mgg->setRange("errRange",lowedge,upedge);
	RooAbsPdf *epdf = 0;
	epdf = new RooExtendPdf("epdf","",*cpdf,*nlim,"errRange");
	
	RooAbsReal *nll = epdf->createNLL(*(data[c]),Extended());
	RooMinimizer minim(*nll);
	minim.setStrategy(0);
	double clone = 1.0 - 2.0*RooStats::SignificanceToPValue(1.0);
	double cltwo = 1.0 - 2.0*RooStats::SignificanceToPValue(2.0);
	
	minim.migrad();
	minim.minos(*nlim);
	// printf("errlo = %5f, errhi = %5f\n",nlim->getErrorLo(),nlim->getErrorHi());
	
	onesigma->SetPoint(i-1,center,nombkg);
	onesigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	minim.setErrorLevel(0.5*pow(ROOT::Math::normal_quantile(1-0.5*(1-cltwo),1.0), 2)); // the 0.5 is because qmu is -2*NLL
	// eventually if cl = 0.95 this is the usual 1.92!      
	
	
	minim.migrad();
	minim.minos(*nlim);
	
	twosigma->SetPoint(i-1,center,nombkg);
	twosigma->SetPointError(i-1,0.,0.,-nlim->getErrorLo(),nlim->getErrorHi());
	
	
	delete nll;
	delete epdf;
	
      }
      mgg->setRange("errRange",minMassFit,maxMassFit);
      
      twosigma->SetLineColor(kGreen);
      twosigma->SetFillColor(kGreen);
      twosigma->SetMarkerColor(kGreen);
      twosigma->Draw("L3 SAME");
      
      onesigma->SetLineColor(kYellow);
      onesigma->SetFillColor(kYellow);
      onesigma->SetMarkerColor(kYellow);
      onesigma->Draw("L3 SAME");
      
      plotbkg_fit[c]->Draw("SAME"); 
     
    }

  }
  return fitresult;


}


void SetConstantParams(const RooArgSet* params) {

  TIterator* iter(params->createIterator());
  for (TObject *a = iter->Next(); a != 0; a = iter->Next()) {
    RooRealVar *rrv = dynamic_cast<RooRealVar *>(a);
    if (rrv) { rrv->setConstant(true); std::cout << " " << rrv->GetName(); }
  }  

}

void MakePlots(RooWorkspace* w, Float_t mass, RooFitResult* fitresults, TString signalname, std::vector<string> cat_names) {

  cout << "Start plotting" << endl; 

  Int_t ncat = NCAT;

// retrieve data sets from the workspace
  RooDataSet* dataAll         = (RooDataSet*) w->data("Data");
  RooDataSet* signalAll       = (RooDataSet*) w->data("Sig");

  RooDataSet* data[21];  
  RooDataSet* signal[21];
  RooAbsPdf*  jjGaussSig[21];
  RooAbsPdf*  jjCBSig[21];
  RooAbsPdf*  jjSig[21];
  RooAbsPdf*  bkg_fit[21];  
//  RooAbsPdf*  bkg_fit2[21];  

  for (int c = 0; c < ncat; ++c) {
    data[c]         = (RooDataSet*) w->data(TString::Format("Data_%s",cat_names.at(c).c_str()));
//    signal[c]       = (RooDataSet*) w->data(TString::Format("Sig_%s",cat_names.at(c).c_str()));
    signal[c]       = (RooDataSet*) w->data(TString::Format("SigWeight_%s",cat_names.at(c).c_str()));
    jjGaussSig[c]  = (RooAbsPdf*)  w->pdf(TString::Format("jjGaussSig_%s",cat_names.at(c).c_str()));
    jjCBSig[c]     = (RooAbsPdf*)  w->pdf(TString::Format("jjCBSig_%s",cat_names.at(c).c_str()));
    jjSig[c]       = (RooAbsPdf*)  w->pdf(signalname+"_jj"+TString::Format("_%s",cat_names.at(c).c_str()));
    bkg_fit[c]       = (RooAbsPdf*)  w->pdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()));
//    bkg_fit2[c]      = (RooAbsPdf*)  w->pdf(TString::Format("bkg_fit2_%s",cat_names.at(c).c_str()));
  }

// retrieve mass observable from the workspace
  RooRealVar* mgg     = w->var("mgg13TeV");  
  mgg->setUnit("GeV");

// retrieve pdfs after the fits
// Signal Model

  RooAbsPdf* jjGaussSigAll  = w->pdf("jjGaussSig"+signalname);
  RooAbsPdf* jjCBSigAll     = w->pdf("jjCBSig"+signalname);
  RooAbsPdf* jjSigAll       = w->pdf(signalname+"_jj");

//  RooAbsPdf* bkg_fitAll       = w->pdf("bkg_fit");
  RooAbsPdf* bkg_fitAll       = w->pdf("bkg_fitAll");
  
  cout << "Progress plotting" << endl;
 
//****************************//
// Plot jj Fit results
//****************************//


  Float_t minMassFit(MMIN),maxMassFit(MMAX); 
  Float_t MASS(mass);

  Int_t nBinsMass(100);
  
  RooPlot* plotjjAll = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
  signalAll->plotOn(plotjjAll,PrintEvalErrors(-1));
 
  gStyle->SetOptTitle(0);
  jjSigAll->plotOn(plotjjAll,PrintEvalErrors(-1));
  jjSigAll->plotOn(plotjjAll,Components("jjGaussSig"+signalname),LineStyle(kDashed),LineColor(kGreen),PrintEvalErrors(-1));
  jjSigAll->plotOn(plotjjAll,Components("jjCBSig"+signalname),LineStyle(kDashed),LineColor(kRed),PrintEvalErrors(-1));

  jjSigAll->paramOn(plotjjAll, ShowConstants(true), Layout(0.15,0.55,0.9), Format("NEU",AutoPrecision(2)));
  plotjjAll->getAttText()->SetTextSize(0.03);

  TCanvas* c1 = new TCanvas("c1","jj",0,0,500,500);
  c1->cd(1);
  plotjjAll->Draw();  


  cout << "Progress plotting" << endl;

//********************************************//
// Plot jj signal fit results per categories 
//********************************************//
// Plot Signal Categories 
//****************************//

  TLatex *text = new TLatex();
  text->SetNDC();
  text->SetTextSize(0.04);

//  TCanvas* c2 = new TCanvas("c2","jj Categories",0,0,1000,1000);

//  c2->Divide(3,3);
  RooPlot* plotjj[21];
  for (int c = 0; c < ncat; ++c) {
    plotjj[c] = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
    signal[c]->plotOn(plotjj[c],LineColor(kWhite),MarkerColor(kWhite),PrintEvalErrors(-1));    

    jjSig[c]  ->plotOn(plotjj[c],PrintEvalErrors(-1));
    jjSig[c]  ->plotOn(plotjj[c],Components("jjGaussSig"+signalname+TString::Format("_%s",cat_names.at(c).c_str())),LineStyle(kDashed),LineColor(kGreen),PrintEvalErrors(-1));
    jjSig[c]  ->plotOn(plotjj[c],Components("jjCBSig"+signalname+TString::Format("_%s",cat_names.at(c).c_str())),LineStyle(kDashed),LineColor(kRed),PrintEvalErrors(-1));
    

    jjSig[c]  ->paramOn(plotjj[c]);
    signal[c]  ->plotOn(plotjj[c],PrintEvalErrors(-1));

        
    TCanvas* dummy = new TCanvas("dummy", "dummy",0, 0, 400, 400);
    TH1F *hist = new TH1F("hist", "hist", 400, minMassFit, maxMassFit);
 
    plotjj[c]->SetTitle("");      
    plotjj[c]->SetMinimum(0.0);
    plotjj[c]->SetMaximum(1.40*plotjj[c]->GetMaximum());
    plotjj[c]->GetXaxis()->SetTitle("m_{jj} (GeV)");

    TCanvas* ctmp = new TCanvas("ctmp","jj Background Categories",0,0,500,500);
    plotjj[c]->Draw();  
//    hist->Draw("same");
    
    plotjj[c]->Draw("SAME");  
    TLegend *legmc = new TLegend(0.62,0.75,0.92,0.9);
    legmc->AddEntry(plotjj[c]->getObject(5),"Simulation","LPE");
    legmc->AddEntry(plotjj[c]->getObject(1),"Parametric Model","L");
    legmc->AddEntry(plotjj[c]->getObject(3),"Crystal Ball component","L");
    legmc->AddEntry(plotjj[c]->getObject(2),"Gaussian Outliers","L");
    
    legmc->SetBorderSize(0);
    legmc->SetFillStyle(0);
    legmc->Draw();    
    
    float effS = effSigma(hist);
//    text->DrawLatex(0.65,0.4, TString::Format("#sigma_{eff} = %.2f GeV",effS));
//    cout<<"effective sigma [" << c << "] = " << effS <<endl;
    
    TLatex *lat  = new TLatex(minMassFit+1.5,0.85*plotjj[c]->GetMaximum(),"#scale[1.0]{CMS Preliminary}");
    lat->Draw();
    TLatex *lat2 = new TLatex(minMassFit+1.5,0.75*plotjj[c]->GetMaximum(),cat_names.at(c).c_str());
    lat2->Draw();
    TLatex *lat3 = new TLatex(minMassFit+1.5,0.55*plotjj[c]->GetMaximum(),TString::Format("#scale[0.8]{#sigma_{eff} = %.2f GeV}",effS));
    lat3->Draw();

    int iMass = abs(mass);

    //ctmp->SaveAs("plots/sigmodel_"+signalname+TString::Format("%d_%s.png", iMass, cat_names.at(c).c_str()));
    ctmp->SaveAs("plots/sigmodel_"+signalname+TString::Format("%d_%s.pdf", iMass, cat_names.at(c).c_str()));


  }


//************************************************//
// Plot jj background fit results per categories 
//************************************************//
// Plot Background Categories 
//****************************//

  TCanvas* c4 = new TCanvas("c4","jj Background Categories",0,0,2000,2000);
  c4->Divide(3,7);

  RooPlot* plotbkg_fit[21];
  for (int c = 0; c < ncat; ++c) {
    plotbkg_fit[c] = mgg->frame(Range(minMassFit,maxMassFit),Bins(nBinsMass));
    data[c]->plotOn(plotbkg_fit[c],LineColor(kWhite),MarkerColor(kWhite));    
    bkg_fit[c]->plotOn(plotbkg_fit[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange"),PrintEvalErrors(-1)); 
    data[c]->plotOn(plotbkg_fit[c]);    
    bkg_fit[c]->paramOn(plotbkg_fit[c],Layout(0.4,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotbkg_fit[c]->getAttText()->SetTextSize(0.03);
    c4->cd(c+1);
    plotbkg_fit[c]->Draw();  
    gPad->SetLogy(1);
    plotbkg_fit[c]->SetAxisRange(0.1,plotbkg_fit[c]->GetMaximum()*1.5,"Y");
  }

  //c4->SaveAs(TString::Format("plots/backgrounds_log.png",c));
  c4->SaveAs(TString::Format("plots/backgrounds_log.pdf",c));
  //c4->SaveAs(TString::Format("plots/backgrounds_log.eps",c));
  //c4->SaveAs(TString::Format("plots/backgrounds_log.C",c));



  TCanvas* c5 = new TCanvas("c5","jj Background Categories",0,0,2000,2000);
  c5->Divide(3,7);

  RooPlot* plotbkg_fit[21];
  for (int c = 0; c < ncat; ++c) {
    plotbkg_fit[c] = mgg->frame(nBinsMass);
    data[c]->plotOn(plotbkg_fit[c],LineColor(kWhite),MarkerColor(kWhite));    
    bkg_fit[c]->plotOn(plotbkg_fit[c],LineColor(kBlue),Range("fitrange"),NormRange("fitrange"),PrintEvalErrors(-1)); 
    data[c]->plotOn(plotbkg_fit[c]);    
    bkg_fit[c]->paramOn(plotbkg_fit[c], ShowConstants(true), Layout(0.4,0.9,0.9), Format("NEU",AutoPrecision(4)));
    plotbkg_fit[c]->getAttText()->SetTextSize(0.03);
    c5->cd(c+1);
    plotbkg_fit[c]->Draw();  
  }

  //c5->SaveAs(TString::Format("plots/backgrounds.png",c));
  c5->SaveAs(TString::Format("plots/backgrounds.pdf",c));
  //c5->SaveAs(TString::Format("plots/backgrounds.eps",c));
  //c5->SaveAs(TString::Format("plots/backgrounds.C",c));

}


void MakeSigWS(RooWorkspace* w, const char* fileBaseName, TString signalname, std::vector<string> cat_names) {

  TString wsDir   = "workspaces/"+filePOSTfix;
  Int_t ncat = NCAT;

//**********************************************************************//
// Write pdfs and datasets into the workspace 
// for statistical tests. 
// 
// Suffix to attribute belonging to given the category
// "catN"    - N shower shape, pseudo-rapidity, pt or Njets categories 
//  N=0-3    - default shower shape, pseudo-rapidity 4 categories
//  N=0-3    - Pt<40 GeV or 0-jet (Central Jet Veto)
//  N=4-7    - Pt>40 GeV or 1-jet (one-jet tagging)
//**********************************************************************//

//********************************//
// Retrieve P.D.F.s
//********************************//

  RooAbsPdf* jjSigPdf[NCAT];

// (1) import signal P.D.F.s

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");


  for (int c = 0; c < ncat; ++c) {
    jjSigPdf[c] = (RooAbsPdf*)  w->pdf(signalname+"_jj"+TString::Format("_%s",cat_names.at(c).c_str()));
    wAll->import(*w->pdf(signalname+"_jj"+TString::Format("_%s",cat_names.at(c).c_str())));
  }

// (2) Systematics on energy scale and resolution

  wAll->factory("CMS_sig_p1_jes_13TeV[0.0,-5.0,5.0]");
  wAll->factory("CMS_jj_sig_p1_jes_13TeV[0.01,0.01,0.01]");
  wAll->factory("sum::CMS_sig_p1_jes_sum_13TeV(1.0,prod::CMS_sig_p1_jes_prod_13TeV(CMS_sig_p1_jes_13TeV, CMS_jj_sig_p1_jes_13TeV))");
    for (int c = 0; c < ncat; ++c) {
wAll->factory("prod::CMS_jj_"+signalname+"_sig_m0_"+TString::Format("%s_13TeV",cat_names.at(c).c_str())+"(jj_"+signalname+"_sig_m0_"+TString::Format("%s",cat_names.at(c).c_str())+", CMS_sig_p1_jes_sum_13TeV)");
    }

// (3) Systematics on resolution: create new sigmas


  wAll->factory("CMS_sig_p2_jer_13TeV[0.0,-5.0,5.0]");
  wAll->factory("CMS_jj_sig_p2_jer_13TeV[0.1,0.1,0.1]");
  wAll->factory("sum::CMS_sig_p2_jer_sum_13TeV(1.0,prod::CMS_sig_p2_jer_prod_13TeV(CMS_sig_p2_jer_13TeV, CMS_jj_sig_p2_jer_13TeV))");

    for (int c = 0; c < ncat; ++c) {
wAll->factory("prod::CMS_jj_"+signalname+"_sig_sigma_"+TString::Format("%s_13TeV",cat_names.at(c).c_str())+"(jj_"+signalname+"_sig_sigma_"+TString::Format("%s",cat_names.at(c).c_str())+", CMS_sig_p2_jer_sum_13TeV)");
    }

    for (int c = 0; c < ncat; ++c) {
wAll->factory("prod::CMS_jj_"+signalname+"_sig_gsigma_"+TString::Format("%s_13TeV",cat_names.at(c).c_str())+"(jj_"+signalname+"_sig_gsigma_"+TString::Format("%s",cat_names.at(c).c_str())+", CMS_sig_p2_jer_sum_13TeV)");
    }

// (4) do reparametrization of signal
  for (int c = 0; c < ncat; ++c) {
    wAll->factory(
		  "EDIT::"+signalname+"_jj"+TString::Format("_sig_%s(",cat_names.at(c).c_str())+signalname+"_jj"+TString::Format("_%s,",cat_names.at(c).c_str()) +
		  " jj_"+signalname+TString::Format("_sig_m0_%s=CMS_jj_",cat_names.at(c).c_str())+signalname+TString::Format("_sig_m0_%s_13TeV, ", cat_names.at(c).c_str()) +
		  " jj_"+signalname+TString::Format("_sig_sigma_%s=CMS_jj_",cat_names.at(c).c_str())+signalname+TString::Format("_sig_sigma_%s_13TeV, ", cat_names.at(c).c_str()) +
		  " jj_"+signalname+TString::Format("_sig_gsigma_%s=CMS_jj_",cat_names.at(c).c_str())+signalname+TString::Format("_sig_gsigma_%s_13TeV)", cat_names.at(c).c_str())
		  );
  }

  TString filename(wsDir+TString(fileBaseName)+".root");
  wAll->writeToFile(filename);
  cout << "Write signal workspace in: " << filename << " file" << endl;

  return;
}


void MakeBkgWS(RooWorkspace* w, const char* fileBaseName, std::vector<string> cat_names) {

  TString wsDir   = "workspaces/"+filePOSTfix;
  Int_t ncat = NCAT;  

//**********************************************************************//
// Write pdfs and datasets into the workspace 
// for statistical tests. 
// 
// Suffix to attribute belonging to given the category
// "catN"    - N shower shape, pseudo-rapidity, pt or Njets categories 
//  N=0-3    - default shower shape, pseudo-rapidity 4 categories
//  N=0-3    - Pt<40 GeV or 0-jet (Central Jet Veto)
//  N=4-7    - Pt>40 GeV or 1-jet (one-jet tagging)
//**********************************************************************//


//********************************//
// Retrieve the datasets and PDFs
//********************************//

  RooDataSet* data[NCAT];
  RooExtendPdf* bkg_fitPdf[NCAT];

// (1) import everything

  cout << "Start importing everything" << endl;

  RooWorkspace *wAll = new RooWorkspace("w_all","w_all");

  for (int c = 0; c < ncat; ++c) {
 
    cout << "For category " << c << endl;
    data[c]      = (RooDataSet*) w->data(TString::Format("Data_%s",cat_names.at(c).c_str()));
    ((RooRealVar*) data[c]->get()->find("mgg13TeV"))->setBins(MMAX-MMIN) ;
    RooDataHist* dataBinned = data[c]->binnedClone();
    bkg_fitPdf[c] = (RooExtendPdf*)  w->pdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str()));
    //   wAll->import(*data[c], Rename(TString::Format("data_obs_%s",cat_names.at(c).c_str())));
    wAll->import(*dataBinned, Rename(TString::Format("data_obs_%s",cat_names.at(c).c_str())));
   wAll->import(*w->pdf(TString::Format("bkg_fit_%s",cat_names.at(c).c_str())));
   wAll->import(*w->function(TString::Format("bkg_fit_%s_norm",cat_names.at(c).c_str())));

   double mean = (wAll->var(TString::Format("bkg_fit_%s_norm",cat_names.at(c).c_str())))->getVal();
   double min = (wAll->var(TString::Format("bkg_fit_%s_norm",cat_names.at(c).c_str())))->getMin();
   double max = (wAll->var(TString::Format("bkg_fit_%s_norm",cat_names.at(c).c_str())))->getMax();
   wAll->factory(TString::Format("CMS_bkg_fit_%s_13TeV_norm[%g,%g,%g]", cat_names.at(c).c_str(), mean, min, max));

   double mean = (wAll->var(TString::Format("bkg_fit_slope1_%s",cat_names.at(c).c_str())))->getVal();
   double min = (wAll->var(TString::Format("bkg_fit_slope1_%s",cat_names.at(c).c_str())))->getMin();
   double max = (wAll->var(TString::Format("bkg_fit_slope1_%s",cat_names.at(c).c_str())))->getMax();

   wAll->factory(TString::Format("CMS_bkg_fit_slope1_%s_13TeV[%g,%g,%g]", cat_names.at(c).c_str(), mean, min, max));


   double mean = (wAll->var(TString::Format("bkg_fit_slope2_%s",cat_names.at(c).c_str())))->getVal();
   double min = (wAll->var(TString::Format("bkg_fit_slope2_%s",cat_names.at(c).c_str())))->getMin();
   double max = (wAll->var(TString::Format("bkg_fit_slope2_%s",cat_names.at(c).c_str())))->getMax();
   wAll->factory(TString::Format("CMS_bkg_fit_slope2_%s_13TeV[%g,%g,%g]", cat_names.at(c).c_str(), mean, min, max));


   double mean = (wAll->var(TString::Format("bkg_fit_slope3_%s",cat_names.at(c).c_str())))->getVal();
   double min = (wAll->var(TString::Format("bkg_fit_slope3_%s",cat_names.at(c).c_str())))->getMin();
   double max = (wAll->var(TString::Format("bkg_fit_slope3_%s",cat_names.at(c).c_str())))->getMax();

   wAll->factory(TString::Format("CMS_bkg_fit_slope3_%s_13TeV[%g,%g,%g]", cat_names.at(c).c_str(), mean, mean, mean));

    cout << "Done For category " << c << endl;    
  }
  
  
  cout << "Imported" << endl;

// (2) do reparametrization of background

  for (int c = 0; c < ncat; ++c) {
      wAll->factory(
		    TString::Format("EDIT::CMS_bkg_fit_%s_13TeV(bkg_fit_%s,",cat_names.at(c).c_str(),cat_names.at(c).c_str()) +
		    TString::Format(" bkg_fit_%s_norm=CMS_bkg_fit_%s_13TeV_norm,", cat_names.at(c).c_str(),cat_names.at(c).c_str())+
		    TString::Format(" bkg_fit_slope1_%s=CMS_bkg_fit_slope1_%s_13TeV,", cat_names.at(c).c_str(),cat_names.at(c).c_str())+
		    TString::Format(" bkg_fit_slope2_%s=CMS_bkg_fit_slope2_%s_13TeV,", cat_names.at(c).c_str(),cat_names.at(c).c_str())+
		    TString::Format(" bkg_fit_slope3_%s=CMS_bkg_fit_slope3_%s_13TeV)", cat_names.at(c).c_str(),cat_names.at(c).c_str())
		    );
  } 


  TString filename(wsDir+TString(fileBaseName)+".root");
  wAll->writeToFile(filename);
  cout << "Write background workspace in: " << filename << " file" << endl;

  std::cout << "observation ";
  for (int c = 0; c < ncat; ++c) {
    std::cout << "  " << (wAll->data(TString::Format("data_obs_%s",cat_names.at(c).c_str())))->sumEntries();
    (wAll->data(TString::Format("data_obs_%s",cat_names.at(c).c_str())))->Print();
  }
  std::cout << std::endl;
  
  for (int c = 0; c < ncat; ++c) {
    printf("CMS_bkg_fit_slope1_%s_13TeV  param  %.4f  %.3f   # Mean and absolute uncertainty on background slope\n",
	   cat_names.at(c).c_str(), (wAll->var(TString::Format("CMS_bkg_fit_slope1_%s_13TeV",cat_names.at(c).c_str())))->getVal(), 10);
  }

  return;
}


Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}



void MakeDataCard_1Channel(RooWorkspace* w, const char* fileBaseName, const char* fileBkgName, int iChan, TString signalname, int signalsample, std::vector<string> cat_names, double mass) {

  TString cardDir = "datacards/"+filePOSTfix;
  Int_t ncat = NCAT;
  TString wsDir   = "../workspaces/"+filePOSTfix;
//**********************//
// Retrieve the datasets
//**********************//

  cout << "Start retrieving dataset" << endl;

  RooDataSet* data[NCAT];
  RooDataSet* signal[NCAT];
  for (int c = 0; c < NCAT; ++c) {
    data[c]        = (RooDataSet*) w->data(TString::Format("Data_%s",cat_names.at(c).c_str()));
    signal[c]      = (RooDataSet*) w->data(TString::Format("SigWeight_%s",cat_names.at(c).c_str()));
  }

//*****************************//
// Print Expected event yields
//*****************************//

  cout << "======== Expected Events Number =====================" << endl;  
  cout << "#Events data:        " <<  w->data("Data")->sumEntries()  << endl;
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events data %s:   ",cat_names.at(c).c_str()) << data[c]->sumEntries()  << endl;
  }
  cout << ".........Expected Signal ............................" << endl;  
  cout << "#Events Signal:      " << w->data("SigWeight")->sumEntries()  << endl;
  Float_t siglikeErr[NCAT];
  for (int c = 0; c < ncat; ++c) {
    cout << TString::Format("#Events Signal %s: ",cat_names.at(c).c_str()) << signal[c]->sumEntries() << endl;
    siglikeErr[c]=0.6*signal[c]->sumEntries();
  }
  cout << "====================================================" << endl;  

//*************************//
// Print Data Crd int file
//*************************//

  cout << "Start writing datacard" << endl;

  TString filename(cardDir+TString(fileBaseName)+Form("_%s.txt",cat_names[iChan].c_str()));
  ofstream outFile(filename);

  double scaleFactor=signalScaler;
  // // Herwig HP+HP
  // if(((signalsample==0)||(signalsample==1))&&(iChan==0))
  //     scaleFactor*=(scaleFactorHP*scaleFactorHP/scaleFactorHPherwig/scaleFactorHPherwig);
  // // Herwig HP+LP
  // if(((signalsample==0)||(signalsample==1))&&(iChan==1))
  //     scaleFactor*=(scaleFactorHP*scaleFactorLP/scaleFactorHPherwig/scaleFactorLPherwig);
  // Pythia HP+HP
  if(((signalsample==0)||(signalsample==1)||(signalsample==2)||(signalsample==5)||(signalsample==6))&&(iChan==0 ||iChan==3 ||iChan==6 ||iChan==9))
      scaleFactor*=(scaleFactorHP*scaleFactorHP);
  // Pythia HP+LP
  if(((signalsample==0)||(signalsample==1)||(signalsample==2)||(signalsample==5)||(signalsample==6))&&(iChan==1||iChan==4||iChan==7||iChan==10))
      scaleFactor*=(scaleFactorHP*scaleFactorLP);
  
  //SINGLE Q
  // Pythia HP
  if(((signalsample==3)||(signalsample==4))&&(iChan==12 ||iChan==15 ||iChan==18 ))
      scaleFactor*=(scaleFactorHP);
  // Pythia LP
  if(((signalsample==3)||(signalsample==4))&&(iChan==13||iChan==16||iChan==19))
      scaleFactor*=(scaleFactorLP);

  outFile << "# Fully Hadronic VV analysis" << endl;
  outFile << "imax 1" << endl;
  if(signalsample<3){
  outFile << "jmax 3" << endl;
  } else if(signalsample<5){
  outFile << "jmax 2" << endl;
  } else {
  outFile << "jmax 2" << endl;
  }
  outFile << "kmax *" << endl;
  outFile << "---------------" << endl;

  outFile << Form("shapes data_obs %s ", cat_names[iChan].c_str()) << wsDir+TString(fileBkgName)+".root" << Form(" w_all:data_obs_%s", cat_names[iChan].c_str()) << endl;
  outFile << Form("shapes bkg_fit_jj %s ", cat_names[iChan].c_str()) <<  wsDir+TString(fileBkgName)+".root" << Form(" w_all:CMS_bkg_fit_%s_13TeV", cat_names[iChan].c_str()) << endl;
  if(signalsample<3){
  outFile << Form("shapes RS1WW_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_RS1WW_%.0f_13TeV.root", mass) << Form(" w_all:RS1WW_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  outFile << Form("shapes RS1ZZ_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_RS1ZZ_%.0f_13TeV.root", mass) << Form(" w_all:RS1ZZ_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  outFile << Form("shapes WZ_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_WZ_%.0f_13TeV.root", mass) << Form(" w_all:WZ_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  } else if(signalsample<5){
  outFile << Form("shapes qW_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_qW_%.0f_13TeV.root", mass) << Form(" w_all:qW_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  outFile << Form("shapes qZ_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_qZ_%.0f_13TeV.root", mass) << Form(" w_all:qZ_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  } else {
  outFile << Form("shapes BulkWW_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_BulkWW_%.0f_13TeV.root", mass) << Form(" w_all:BulkWW_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  outFile << Form("shapes BulkZZ_jj %s ", cat_names[iChan].c_str()) << wsDir+TString::Format("CMS_jj_BulkZZ_%.0f_13TeV.root", mass) << Form(" w_all:BulkZZ_jj_sig_%s", cat_names[iChan].c_str()) << endl;
  }
  outFile << "---------------" << endl;
  outFile << Form("bin          %s", cat_names[iChan].c_str()) << endl;
  outFile <<  "observation   "  <<  Form("%.10lg",data[iChan]->sumEntries()) << endl;
  outFile << "------------------------------" << endl;
  if(signalsample<3){
  outFile << "bin                      "<< Form("%s       %s      %s      %s      ", cat_names[iChan].c_str(), cat_names[iChan].c_str(), cat_names[iChan].c_str(), cat_names[iChan].c_str()) << endl;
  outFile << "process                 RS1WW_jj RS1ZZ_jj WZ_jj     bkg_fit_jj     " << endl;
  outFile << "process                 -2 -1 0        1          " << endl;
  if(signalname=="RS1ZZ")
      outFile <<  "rate                      " 
	  << "  0  " << signal[iChan]->sumEntries()*scaleFactor << "  0  " << 1 << endl;
  if(signalname=="RS1WW")
      outFile <<  "rate                      " 
	  << "  " << signal[iChan]->sumEntries()*scaleFactor << "  0  0  " << 1 << endl;
  if(signalname=="WZ")
      outFile <<  "rate                      " 
	  << "  0  0  " << signal[iChan]->sumEntries()*scaleFactor << "  " << 1 << endl;
  outFile << "--------------------------------" << endl;
  outFile << "# signal scaled by " << signalScaler << " to a cross section of 10/fb and also scale factor of " << scaleFactor/signalScaler << " are applied." << endl;
  
  outFile << "lumi_13TeV       lnN  1.05  1.05  1.05    - " << endl;
  if((iChan==0)||(iChan==3)){
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  1.15  1.15  1.15      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s         lnN  1.185  1.197  1.191      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  } else {
  // anti-correlated the high purity (1.076*1.076) and low purity (0.54*1.076) categories
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  0.58  0.58  0.58      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s         lnN  1.185  1.197  1.191      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  }
  outFile << "CMS_scale_j_13TeV         lnN  1.120  1.120  1.120      - # jet energy scale" << endl;
  outFile << "CMS_res_j_13TeV         lnN  1.040  1.040  1.040      - # jet energy resolution" << endl;
  outFile << "CMS_pu_13TeV         lnN  1.030  1.030  1.030      - # pileup" << endl;
  } else if(signalsample<5) {
  outFile << "bin                      "<< Form("%s      %s      %s      ", cat_names[iChan].c_str(), cat_names[iChan].c_str(), cat_names[iChan].c_str()) << endl;
  outFile << "process                 qW_jj qZ_jj     bkg_fit_jj     " << endl;
  outFile << "process                 -1 0        1          " << endl;
  if(signalname=="qZ")
      outFile <<  "rate                      " 
	  << "  0  " << signal[iChan]->sumEntries()*scaleFactor << "  " << 1 << endl;
  if(signalname=="qW")
      outFile <<  "rate                      " 
	  << "  " << signal[iChan]->sumEntries()*scaleFactor << "  0  " << 1 << endl;
  outFile << "--------------------------------" << endl;
  outFile << "# signal scaled by " << signalScaler << " to a cross section of 10/fb and also scale factor of " << scaleFactor/signalScaler << " are applied." << endl;
  
  outFile << "lumi_13TeV       lnN  1.026  1.026    - " << endl;
  if((iChan==0)||(iChan==3)){
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  1.076  1.076      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s          lnN  1.093  1.099      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  } else {
  // anti-correlated the high purity (1.076) and low purity (0.54) categories
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  0.54  0.54      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s          lnN  1.093  1.099      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  }
  outFile << "CMS_scale_j_13TeV         lnN  1.060  1.060      - # jet energy scale" << endl;
  outFile << "CMS_res_j_13TeV         lnN  1.020  1.020      - # jet energy resolution" << endl;
  outFile << "CMS_pu_13TeV         lnN  1.030  1.030      - # pileup" << endl;
  } else {
  outFile << "bin                      "<< Form("%s       %s      %s      ", cat_names[iChan].c_str(), cat_names[iChan].c_str(), cat_names[iChan].c_str()) << endl;
  outFile << "process                 BulkWW_jj BulkZZ_jj     bkg_fit_jj     " << endl;
  outFile << "process                 -1 0        1          " << endl;
  if(signalname=="BulkZZ")
      outFile <<  "rate                      " 
	  << "  0  " << signal[iChan]->sumEntries()*scaleFactor << "  " << 1 << endl;
  if(signalname=="BulkWW")
      outFile <<  "rate                      " 
	  << "  " << signal[iChan]->sumEntries()*scaleFactor << "  0  " << 1 << endl;
  outFile << "--------------------------------" << endl;
  outFile << "# signal scaled by " << signalScaler << " to a cross section of 10/fb and also scale factor of " << scaleFactor/signalScaler << " are applied." << endl;
  
  outFile << "lumi_13TeV       lnN  1.026  1.026    - " << endl;
  if((iChan==0)||(iChan==3)){
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  1.15  1.15      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s          lnN  1.185  1.197      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  } else {
  // anti-correlated the high purity (1.076*1.076) and low purity (0.54*1.076) categories
  outFile << "CMS_eff_vtag_tau21_sf_13TeV         lnN  0.58  0.58      - # tau21 efficiency" << endl;
//  outFile << Form("CMS_eff_vtag_mass_sf_%s          lnN  1.185  1.197      - # jet mass efficiency",cat_names[iChan].c_str()) << endl;
  }
  outFile << "CMS_scale_j_13TeV         lnN  1.02  1.02      - # jet energy scale" << endl;
  outFile << "CMS_res_j_13TeV         lnN  1.10  1.10      - # jet energy resolution" << endl;
  outFile << "CMS_pu_13TeV         lnN  1.030  1.030      - # pileup" << endl;
  } 
  outFile << "# Parametric shape uncertainties, entered by hand." << endl;
  outFile << Form("CMS_sig_p1_jes_13TeV    param   0.0   1.0   # dijet mass shift due to JES uncertainty") << endl;
  outFile << Form("CMS_sig_p2_jer_13TeV     param   0.0   1.0   # dijet mass resolution shift due to JER uncertainty") << endl;
 
  outFile << Form("CMS_bkg_fit_%s_13TeV_norm           flatParam  # Normalization uncertainty on background slope",cat_names[iChan].c_str()) << endl;

  outFile << Form("CMS_bkg_fit_slope1_%s_13TeV         flatParam  # Mean and absolute uncertainty on background slope",cat_names[iChan].c_str()) << endl;

  outFile << Form("CMS_bkg_fit_slope2_%s_13TeV         flatParam  # Mean and absolute uncertainty on background slope",cat_names[iChan].c_str()) << endl;

  outFile << Form("CMS_bkg_fit_slope3_%s_13TeV         flatParam  # Mean and absolute uncertainty on background slope",cat_names[iChan].c_str()) << endl;

  outFile.close();

  cout << "Write data card in: " << filename << " file" << endl;

  return;
}


Double_t effSigma(TH1 *hist) {

  TAxis *xaxis = hist->GetXaxis();
  Int_t nb = xaxis->GetNbins();
  if(nb < 10) {
    std::cout << "effsigma: Not a valid histo. nbins = " << nb << std::endl;
    return 0.;
  }

  Double_t bwid = xaxis->GetBinWidth(1);
  if(bwid == 0) {
    std::cout << "effsigma: Not a valid histo. bwid = " << bwid << std::endl;
    return 0.;
  }
  Double_t xmax = xaxis->GetXmax();
  Double_t xmin = xaxis->GetXmin();
  Double_t ave = hist->GetMean();
  Double_t rms = hist->GetRMS();

  Double_t total=0.;
  for(Int_t i=0; i<nb+2; i++) {
    total+=hist->GetBinContent(i);
  }
  if(total < 100.) {
    std::cout << "effsigma: Too few entries " << total << std::endl;
    return 0.;
  }
  Int_t ierr=0;
  Int_t ismin=999;

  Double_t rlim=0.683*total;
  Int_t nrms=rms/(bwid);    // Set scan size to +/- rms
  if(nrms > nb/10) nrms=nb/10; // Could be tuned...

  Double_t widmin=9999999.;
  for(Int_t iscan=-nrms;iscan<nrms+1;iscan++) { // Scan window centre
    Int_t ibm=(ave-xmin)/bwid+1+iscan;
    Double_t x=(ibm-0.5)*bwid+xmin;
    Double_t xj=x;
    Double_t xk=x;
    Int_t jbm=ibm;
    Int_t kbm=ibm;
    Double_t bin=hist->GetBinContent(ibm);
    total=bin;
    for(Int_t j=1;j<nb;j++){
      if(jbm < nb) {
        jbm++;
        xj+=bwid;
        bin=hist->GetBinContent(jbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
      if(kbm > 0) {
        kbm--;
        xk-=bwid;
        bin=hist->GetBinContent(kbm);
        total+=bin;
        if(total > rlim) break;
      }
      else ierr=1;
    }
    Double_t dxf=(total-rlim)*bwid/bin;
    Double_t wid=(xj-xk+bwid-dxf)*0.5;
    if(wid < widmin) {
      widmin=wid;
      ismin=iscan;
    }
  }
  if(ismin == nrms || ismin == -nrms) ierr=3;
  if(ierr != 0) std::cout << "effsigma: Error of type " << ierr << std::endl;

  return widmin;
}

void R2JJFitter13TeV(double mass, std::string postfix="", int signalsamples=0)
{
    filePOSTfix=postfix;
    if(signalsamples==0)
    {
    runfits(mass, 0);
    runfits(mass, 1);
    //runfits(mass, 2);
    } else if(signalsamples==1)
    {
    runfits(mass, 3);
    runfits(mass, 4);
    } else if(signalsamples==5)
    {
    runfits(mass, 5);
    // runfits(mass, 6);
    }
    else {
    runfits(mass, 2);
    }
}
