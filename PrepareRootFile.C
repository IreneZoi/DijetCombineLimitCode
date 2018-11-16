#include "TH1.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;
#define MassPoints 10
#define Channels 2

bool print=true; 
bool drawlegend=true;

void DrawWithGen(TH1F *hGen, TH1F *hRec, bool drawlegend, bool logy, TString channel, TString name, TString selections, float xmin, float xmax,  float ymin, float ymax);
void Draw2(TH1F *hRec1, TString label1, TH1F *hRec2, TString label2, bool drawlegend, bool logy, TString channel, TString name, TString selections, float xmin, float xmax,  float ymin, float ymax);

void Draw( TH1F *hRec, bool drawlegend, bool logy, TString channel, TString name, TString selections, float xmin, float xmax,  float ymin, float ymax);


void PrepareRootFile(TString cut, TString directory, TString legend = "yes", TString status = "blinded"){
  cout << " cut " << cut << endl;
  // input 

  TString base_dir = "/nfs/dust/cms/user/zoiirene/DiBoson/OutputBatch/"+directory+"/"; //ForFit/";
  if(print) std::cout << "base dir " << base_dir <<std::endl;
  TString output_dir = "/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/input/";

  TString channel[Channels];
  channel[0] = "graviton";
  channel[1] = "radion";
  TString Channel[Channels];
  Channel[0] = "Graviton";
  Channel[1] = "Radion";

  Int_t Mass[MassPoints];
  if(print) std::cout << "MassPoints " << MassPoints <<std::endl;
  TString ss_mass[MassPoints];
  if(print) std::cout << "ssv " << std::endl;

  int i,j;
  for(i=0; i<MassPoints;i++)
    {
      if(i < 5)
        Mass[i]=1200+(200*i);
      else
        Mass[i]=2000+(500*(i-4));

      if(print) std::cout << "mass " << i << ": "<< Mass[i] <<std::endl;
      ss_mass[i].Form("%d",Mass[i]);
      if(print) std::cout << "mass ts"<< ss_mass[i]  <<std::endl;
    }

  TString signal_file[Channels*MassPoints];
  for(j=0;j<Channels;j++)
    {
      for(i=0;i<MassPoints;i++)
	{
	  signal_file[i+j*MassPoints] = "uhh2.AnalysisModuleRunner.MC.VBF_"+Channel[j]+ss_mass[i]+"ToWW_inv_newJEC_PUPPI.root";
	  if(print) std::cout << "signal_file  " << i+j*MassPoints << " : " << "uhh2.AnalysisModuleRunner.MC.VBF_"+Channel[j]+ss_mass[i]+"ToWW_inv_newJEC_PUPPI.root"  <<std::endl;
	}
    }
  
  TString data_file = "uhh2.AnalysisModuleRunner.Data.DATA_JetHT_Run2016.root";
  if(print) std::cout << "data_file  " << data_file  <<std::endl;
  TString qcd_file = "uhh2.AnalysisModuleRunner.MC.MC_QCD_HT.root";
  if(print) std::cout << "radion_file  " << qcd_file  <<std::endl;


  TString input_data = base_dir+data_file;
  TString input_qcd = base_dir+qcd_file;

  TFile * f_data = new TFile(input_data); 
  TFile * f_qcd = new TFile(input_qcd); 


  TString input_signal[Channels*MassPoints];
  TFile * f_signal[Channels*MassPoints]; 

  for(j=0;j<Channels*MassPoints;j++)
    {
      input_signal[j] = base_dir+signal_file[j];
      f_signal[j] = new TFile(input_signal[j]); 
    }

  TDirectoryFile*d_WTopJet[Channels*MassPoints];
  TH1F* h_WTopJet_invM_VV[Channels*MassPoints];
  TDirectoryFile*d_WTopJet_withVBF_VV[Channels*MassPoints];
  TH1F* h_WTopJet_invM_withVBF_VV[Channels*MassPoints];

  TDirectoryFile*d_WTopJet_data;
  TH1F* h_WTopJet_invM_VV_data;
  TDirectoryFile*d_WTopJet_withVBF_VV_data;
  TH1F* h_WTopJet_invM_withVBF_VV_data;

  TDirectoryFile*d_WTopJet_qcd;
  TH1F* h_WTopJet_invM_VV_qcd;
  TDirectoryFile*d_WTopJet_withVBF_VV_qcd;
  TH1F* h_WTopJet_invM_withVBF_VV_qcd;

  TString selectionVV="Wtopjets_withVBF_VVMass_inverted";
  TString selectionVBF="Wtopjets_withVBF_VVMass";


  string s8="invM800";
  string s9="invM900";
  string s10="invM1000";
  string de35="de35";
  string de4="de4";
  string de45="de45";
  string de5="de5";
  string de55="de55";
  string de6="de6";
  string CUT=cut.Data();
  if(print)  cout << "CUT= " << CUT << endl;
  if(print)  cout << "s8= " << s8 << endl;
  if(print)  cout << "CUT find= " << CUT.find(s8) << endl;
  if(CUT.find(s8)!= std::string::npos)
    {
      selectionVBF="Wtopjets_VBF_"+s8;
      if(CUT.find(de35)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de35;
      if(CUT.find(de4)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de4;
      if(CUT.find(de45)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de45;
      if(CUT.find(de5)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de5;
      if(CUT.find(de55)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de55;
      if(CUT.find(de6)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s8+"_"+de6;
    }
  if(CUT.find(s9)!= std::string::npos)
    {
      selectionVBF="Wtopjets_VBF_"+s9;
      if(CUT.find(de35)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de35;
      if(CUT.find(de4)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de4;
      if(CUT.find(de45)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de45;
      if(CUT.find(de5)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de5;
      if(CUT.find(de55)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de55;
      if(CUT.find(de6)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s9+"_"+de6;
    }
  if(CUT.find(s10)!= std::string::npos)
    {
      selectionVBF="Wtopjets_VBF_"+s10;
     if(CUT.find(de35)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de35;
      if(CUT.find(de4)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de4;
      if(CUT.find(de45)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de45;
      if(CUT.find(de5)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de5;
      if(CUT.find(de55)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de55;
      if(CUT.find(de6)!= std::string::npos)
	selectionVBF="Wtopjets_VBF_"+s10+"_"+de6;
    }

  if(print)  cout << "selectionVV= " << selectionVV << endl;
  //  selectionVBF="Wtopjets_withVBF_"+cut;
  if(print)  cout << "selectionVBF= " << selectionVBF<< endl;

  f_data->cd();
  
  d_WTopJet_data =(TDirectoryFile*)f_data->Get(selectionVV);
  h_WTopJet_invM_VV_data = (TH1F*)d_WTopJet_data->Get("invMass_rootfile");                                                                                    
  if(print) std::cout << " histo data VV" <<std::endl;
  d_WTopJet_withVBF_VV_data =(TDirectoryFile*)f_data->Get(selectionVBF);
  h_WTopJet_invM_withVBF_VV_data = (TH1F*)d_WTopJet_withVBF_VV_data->Get("invMass_rootfile");                       
  if(print) std::cout << " histo data VV VBF" <<std::endl;
  
  f_qcd->cd();
  
  d_WTopJet_qcd =(TDirectoryFile*)f_qcd->Get(selectionVV);
  h_WTopJet_invM_VV_qcd = (TH1F*)d_WTopJet_qcd->Get("invMass_rootfile");                                                                                    
  if(print) std::cout << " histo QCD VV" <<std::endl;
  d_WTopJet_withVBF_VV_qcd =(TDirectoryFile*)f_qcd->Get(selectionVBF);
  h_WTopJet_invM_withVBF_VV_qcd = (TH1F*)d_WTopJet_withVBF_VV_qcd->Get("invMass_rootfile");                       
  if(print) std::cout << " histo QCD VV VBF" <<std::endl;
  
  

  for(i=0; i<Channels*MassPoints; i++)
    {
      f_signal[i]->cd();
      
      d_WTopJet[i] =(TDirectoryFile*)f_signal[i]->Get(selectionVV);
      h_WTopJet_invM_VV[i] = (TH1F*)d_WTopJet[i]->Get("invMass_rootfile");                                                                                    
      if(print) std::cout << " histo VV " <<std::endl;
      d_WTopJet_withVBF_VV[i] =(TDirectoryFile*)f_signal[i]->Get(selectionVBF);
      h_WTopJet_invM_withVBF_VV[i] = (TH1F*)d_WTopJet_withVBF_VV[i]->Get("invMass_rootfile");                       
      if(print) std::cout << " histo VBF" <<std::endl;
    }



  TFile *output[Channels*MassPoints];

  for(j=0;j<Channels;j++)
    {
      for(i=0;i<MassPoints;i++)
	{
	  output[i+j*MassPoints] = new TFile(output_dir+channel[j]+"_"+cut+"_"+ss_mass[i]+".root","RECREATE");
	  h_WTopJet_invM_VV[i+j*MassPoints]->Write(channel[j]+"_invMass");
	  h_WTopJet_invM_VV_data->Write("data_invMass");
	  h_WTopJet_invM_VV_qcd->Write("qcd_invMass");
	  
	  h_WTopJet_invM_withVBF_VV[i+j*MassPoints]->Write(channel[j]+"_invMass_afterVBFsel");
	  h_WTopJet_invM_withVBF_VV_qcd->Write("data_invMass_afterVBFsel");
	  h_WTopJet_invM_withVBF_VV_qcd->Write("qcd_invMass_afterVBFsel");
      
	  output[i+j*MassPoints]->Close();
	}
    }

}//Prepare root file
