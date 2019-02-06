#include "TH2.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
#define Channels 2
#define Categories 3
#define dirs 3
#define namen 3
#define MassPoints 3

//less /nfs/dust/cms/user/abenecke/scripts/plots/Limit_2d.C


bool print=true; 
TString dir = "/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/LimitTxt/";



void Uncertainty_newFormat(){

  TString channel[Channels];
  channel[0] = "graviton";
  channel[1] = "radion";

  TString category[Categories];
  category[0] = "invMass";
  category[1] = "invMass_afterVBFsel";
  category[2] = "invMass_combined";
    
  TString Mass[MassPoints];
  Mass[0] = "1200";
  Mass[1] = "2000";
  Mass[2] = "4000";
  
  TString names = "correctPUPPIres";
  TString directory = "testInvertedSelection/";

  std::vector<TString> mass = {"1200","2000","4000"};
  std::vector<TString> channels = {"graviton","radion"};
  std::vector<TString> uncertainty = {"all", "none", "lumi_13TeV", "CMS_eff_vtag_tau21_sf_13TeV", "CMS_pu_13TeV", "CMS_sig_p1_jes_13TeV", "CMS_sig_p2_jer_13TeV", "CMS_PDF_acceptance_j_13TeV", "CMS_SCALE_acceptance_j_13TeV", "CMS_mass_scale_j_13TeV", "CMS_mass_res_j_13TeV", "CMS_scale_j_13TeV", "CMS_res_j_13TeV", "CMS_L1pre_j_13TeV"};
  std::vector<TString> uncertaintyLatex = {"all", "none", "lumi\\_13TeV", "CMS\\_eff\\_vtag\\_tau21\\_sf\\_13TeV", "CMS\\_pu\\_13TeV", "CMS\\_sig\\_p1\\_jes\\_13TeV", "CMS\\_sig\\_p2\\_jer\\_13TeV", "CMS\\_PDF\\_acceptance\\_j\\_13TeV", "CMS\\_SCALE\\_acceptance\\_j\\_13TeV", "CMS\\_mass\\_scale\\_j\\_13TeV", "CMS\\_mass\\_res\\_j\\_13TeV", "CMS\\_scale\\_j\\_13TeV", "CMS\\_res\\_j\\_13TeV", "CMS\\_L1pre\\_j\\_13TeV"};
  std::vector<TString> description = {" ", " ", " ", " (vtag tau21 sf) ", " (pile up) ", " (JEC scale on shape) ", " (JER on shape) ", " ", " ", " (SD mass scale) ", " (SD mass resolution) ", " (JEC scale on yield) ", " (JER on yield) ", " (L1 prefiring) "};
  


  typedef std::map<int, Float_t > Maptype;

  Maptype Limit_graviton[Categories][MassPoints];
  Maptype Limit_radion[Categories][MassPoints];
  Maptype SigmaUp_graviton[Categories][MassPoints];
  Maptype SigmaUp_radion[Categories][MassPoints];
  Maptype SigmaDown_graviton[Categories][MassPoints];
  Maptype SigmaDown_radion[Categories][MassPoints];
 
  double limit[MassPoints];   
  double Up[MassPoints];   
  double Down[MassPoints];   
  string lines[3];
  TString filename;
  TString subdir;
  TString unc;
  TString comment;
  TString fileend;
  //get limits from file
  for(int i=0;i<channels.size();i++)
    {

      cout << channels[i] << endl;
      for(int k=0;k<Categories;k++)
	{
	  cout << category[k] << endl;
	  for(int j =0; j<uncertainty.size(); j++)
	    {
	      cout << uncertainty[j] << endl;
              for(int l=0;l<MassPoints;l++)
                {
		  cout << Mass[l] << endl;

		  if (j ==0)
		    {
		      subdir = "Unc/";
		      unc =  "_";
		      fileend = "Limit";
		      if(l==1) fileend="Limit_"+Mass[l];
		      if(l==2) fileend="Limit_"+Mass[l];
		    }
	      else 
		{
		  subdir = "Unc/";//"Unc/";
		  unc = "_"+uncertainty[j]+"_";
		  fileend = "Limit_Unc";
		  if(l==1) fileend="Limit_Unc_"+Mass[l];
		  if(l==2) fileend="Limit_Unc"+Mass[l];
		}
	      filename = dir+subdir+channels[i]+"__"+category[k]+"_"+names+unc+"new_combined_"+fileend+".txt";
	      //	      if(k==0) filename = dir+channel[i]+"__"+category[1]+"_"+cut_tau21[j]+"_new_combined_Limit.txt";
	      cout << filename << endl;
	      ifstream stream(filename);
	      std::string line;
	      if(!stream.is_open())	      
		{
		  limit[l]=0;
		  //		  limit[1]=0;
		  //limit[2]=0;
		  Up[l]=0;
		  //		  Up[1]=0;
		  //Up[2]=0;
		  Down[l]=0;
		  //		  Down[1]=0;
		  //		  Down[2]=0;
		  
		  cout << "*********************  File " << filename << " not opened" << endl;
		}
	      else
		{
		  std::getline(stream,line);
		  if(print) cout << " first line " << line << endl; 
		  //
		  //		  while(!stream.eof())
		  //{
		      std::getline(stream, lines[0], '\n');
		      cout << lines[0] << endl;
		      std::getline(stream, lines[1], '\n');
		      cout << lines[1] << endl;
		      std::getline(stream, lines[2], '\n');;
		      cout << lines[2] << endl;
		      //		      stream  >> limit[0]  >> limit[1] >> limit[2] ;
		      //cout <<  limit[0]  <<" " << limit[1] << " " << limit[2] << endl;
		      //std::getline(stream,line);

		      //stream  >> Up[0]  >> Up[1] >> Up[2] ;
		      //std::getline(stream,line);
		      
		      //stream  >> Down[0]  >> Down[1] >> Down[2] ;
		      //}
		  cout << " going to split " << endl;
		  
		  stringstream ssin0(lines[0]);
		  while (ssin0.good() ){
		    ssin0 >> limit[l];
		    ssin0 >> comment;
		    ssin0 >> comment;
		    ssin0 >> comment;
		  }
		  cout <<  "limit " << l << " " <<limit[l] << " " << comment << " " << comment << " " << comment << endl;  
		  
		  stringstream ssin1(lines[1]);
		  while (ssin1.good() ){
		    ssin1 >> Up[l];
		    ssin1 >> comment;
		    ssin1 >> comment;
		    ssin1 >> comment;
		  }	
		  cout <<  "Up " << l << " " <<Up[l] << " " << comment << " " << comment << " " << comment << endl;  

		 
		  stringstream ssin2(lines[2]);
		  while (ssin2.good() ){
		    ssin2 >> Down[l];
		    ssin2 >> comment;
		    ssin2 >> comment;
		    ssin2 >> comment;
		  }
		  cout <<  "Down " << l << " " <<Down[l] << " " << comment << " " << comment << " " << comment << endl;  
		     
		}


	    
	      cout << "Limit for channel " << channels[i] << " category  " << category[k] << " and cuts " << uncertainty[j]<< " : " << endl;
	      //	      for(int l=0;l<MassPoints;l++)
	      //{
		  cout << " Mass " << Mass[l] << " -> " << limit[l] << " 1 sigma up " << Up[l] << " 1 sigma down " << Down[l] << endl;
		  
		  if(i==0)
		    {
		      cout << channels[i] << endl;
		      Limit_graviton[k][l].insert( std::make_pair(j , limit[l]) );
		      cout << "Limit for "<< category[k] << " channel graviton[" << l  << "]["<< j << "] : "<< Limit_graviton[k][l][j]<<endl;
		     
		      SigmaUp_graviton[k][l].insert( std::make_pair(j , Up[l]) );
		      cout << "Sigma Up for "<< category[k] << " channel graviton[" << l  << "]["<< j << "] : "<< SigmaUp_graviton[k][l][j]<<endl;
		      SigmaDown_graviton[k][l].insert( std::make_pair(j , Down[l]) );

	
		      
		    }
		  if(i==1)
		    {
		      Limit_radion[k][l].insert( std::make_pair(j , limit[l]) );
		      SigmaUp_radion[k][l].insert( std::make_pair(j , Up[l]) );
		      SigmaDown_radion[k][l].insert( std::make_pair(j , Down[l]) );

		      cout << "Limit for "<< category[k] << " channel radion[" << l  << "]["<< j << "] : "<< Limit_radion[k][l][j]<<endl;


		    }
		}//masses
		  
	    }//uncertainty
	}//categories
    }//channels
      

  //tables
  Float_t diff_l[uncertainty.size()-1];
  Float_t diff_u[uncertainty.size()-1];
  Float_t diff_d[uncertainty.size()-1];
  std::ofstream Table;
  for(int i=0;i<channels.size();i++)
    {

      for(int k=0;k<Categories;k++)
        {
          for(int m=0;m<mass.size();m++)
            {
	      TString filename = "/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/LimitTxt/UncertaintyImpact_"+channels[i]+"_"+Mass[m]+"_"+category[k]+".txt";
	      cout << filename << endl;
	      Table.open(filename);
	      Table << "\\begin{tabular}{|c | c | c | c |}\n";
	      Table << "\\midrule \n";
	      Table << "\\scriptsize{uncertainty} & \\scriptsize{limit (unc-none)/none } & \\scriptsize{$\\sigma_{up}$ (unc-none)/none } &  \\scriptsize{$\\sigma_{down}$ (unc-none)/none}  \\\\ \n";
	      Table << "\\midrule \n";

	      if(channels[i]=="graviton") 
		{
		  diff_l[0]= (Limit_graviton[k][m][0]-Limit_graviton[k][m][1])/Limit_graviton[k][m][1];
		  diff_d[0]= (SigmaDown_graviton[k][m][0]-SigmaDown_graviton[k][m][1])/SigmaDown_graviton[k][m][1];
		  diff_u[0]= (SigmaUp_graviton[k][m][0]-SigmaUp_graviton[k][m][1])/SigmaUp_graviton[k][m][1];
		}
	      if(channels[i]=="radion") 
		{
		  diff_l[0]= (Limit_radion[k][m][0]-Limit_radion[k][m][1])/Limit_radion[k][m][1];
		  diff_d[0]= (SigmaDown_radion[k][m][0]-SigmaDown_radion[k][m][1])/SigmaDown_radion[k][m][1];
		  diff_u[0]= (SigmaUp_radion[k][m][0]-SigmaUp_radion[k][m][1])/SigmaUp_radion[k][m][1];
		  
		}
	      cout << channels[i] << " " << category[k] << " " << Mass[m] << " " << uncertainty[0] << " " << diff_l[0] << " " << diff_d[0] << " " << diff_u[0] << endl;

	      Table << "\\scriptsize{"<< uncertainty[0] <<description[0] <<"} & \\scriptsize{"<<  setprecision(1) << diff_l[0] << "} & \\scriptsize{"<< setprecision(1) << diff_u[0] << "} & \\scriptsize{"<< setprecision(1) <<  diff_d[0] << "} \\\\ \n";
              Table <<"\\hline \n";


	      for (Int_t n=2;n<uncertainty.size();n++) 
		{
		  if(channels[i]=="graviton") 
		    {
		      diff_l[n-1]= (Limit_graviton[k][m][n]-Limit_graviton[k][m][1])/Limit_graviton[k][m][1];
		      diff_d[n-1]= (SigmaDown_graviton[k][m][n]-SigmaDown_graviton[k][m][1])/SigmaDown_graviton[k][m][1];
		      cout << " graviton diff sigma up " << SigmaDown_graviton[k][m][n] << " - "<<SigmaDown_graviton[k][m][1] << " = " << diff_d[n-1] << endl;
		      diff_u[n-1]= (SigmaUp_graviton[k][m][n]-SigmaUp_graviton[k][m][1])/SigmaUp_graviton[k][m][1];
		    }
		  if(channels[i]=="radion") 
		    {
		      diff_l[n-1]= (Limit_radion[k][m][n]-Limit_radion[k][m][1])/Limit_radion[k][m][1];
		      diff_d[n-1]= (SigmaDown_radion[k][m][n]-SigmaDown_radion[k][m][1])/SigmaDown_radion[k][m][1];
		      diff_u[n-1]= (SigmaUp_radion[k][m][n]-SigmaUp_radion[k][m][1])/SigmaUp_radion[k][m][1];
		      
		    }
		  cout << channels[i] << " " << category[k] << " " << Mass[m] << " "<< uncertainty[n-1] << " "  << diff_l[n-1] << " " << diff_d[n-1] << " " << diff_u[n-1] << endl;
		  Table << "\\scriptsize{"<< uncertaintyLatex[n] << description[n]<<"} & \\scriptsize{"<<  setprecision(1) << diff_l[n-1] << "} & \\scriptsize{"<< setprecision(1) << diff_u[n-1] << "} & \\scriptsize{"<< setprecision(1) <<  diff_d[n-1] << "} \\\\ \n";
		  Table <<"\\hline \n";


		}//unc
	      Table <<"\\end{tabular}";

	      Table.close();
	      Table.close();

	      
	    }//mass
	}//cat
    }//chan



  //histograms and canvas
  Float_t x[uncertainty.size()]; 
  Float_t xu[uncertainty.size()]; 
  Float_t xd[uncertainty.size()]; 
  Float_t y[uncertainty.size()]; 
  Float_t yu[uncertainty.size()]; 
  Float_t yd[uncertainty.size()]; 


  for(int i=0;i<channels.size();i++)
    {
 
      for(int k=0;k<Categories;k++)
	{
	  for(int m=0;m<mass.size();m++)
	    {


	      TCanvas *c1= new TCanvas("c1","c1",10,10,800,600);

	

	      c1->Clear();
	      c1->cd();
	      gPad->SetTickx();
	      gPad->SetTicky();
	      gStyle->SetOptStat(0);
	      c1->SetTopMargin(0.05);
	      c1->SetBottomMargin(0.15); 
	      c1->SetRightMargin(0.15);
	      c1->SetLeftMargin(0.1);


	      TH1F * hist = new TH1F("hist",channels[i]+"_"+category[k]+"_"+Mass[m],uncertainty.size(),0,uncertainty.size());
	      for (Int_t n=0;n<uncertainty.size();n++) 
		{
		  x[n] = hist->GetXaxis()->GetBinCenter(n+1);
		  xu[n]=0;
		  xd[n] =0;	
	    
		  hist->GetXaxis()->SetBinLabel(n+1,uncertainty[n]);
		  
		  if(channels[i]=="graviton") 
		    {
		      y[n]= Limit_graviton[k][m][n];
		      yd[n]= SigmaDown_graviton[k][m][n];
		      yu[n]= SigmaUp_graviton[k][m][n];
		    }
		  if(channels[i]=="radion") 
		    {
		      y[n]= Limit_radion[k][m][n];
		      yd[n]= SigmaDown_radion[k][m][n];
		      yu[n]= SigmaUp_radion[k][m][n];
		    }
		  
		  hist->Fill(x[n],y[n]);
		  hist->SetBinError(n+1,0);		  
		  cout << x[n] << " " << y[n] << " " << uncertainty[n] << endl;	  
		}

	      if(k ==0)
		{
		  if(m<2)	      hist->GetYaxis()->SetRangeUser(0.,0.2);
		  else	      hist->GetYaxis()->SetRangeUser(0.,0.3);
		}
	      if(k >0)
		{
		  hist->GetYaxis()->SetRangeUser(0.,0.05);
		  if(i==0 && m==2 )hist->GetYaxis()->SetRangeUser(0.,0.1);		  
		}
	      hist->Draw("p");	
	      //save result




	      TGraphAsymmErrors *limit_hist;
	      
	      limit_hist = new TGraphAsymmErrors(uncertainty.size(),x,y,xd,xu,yd,yu);
	

	      limit_hist->SetTitle("");

	      //limit_hist->GetXaxis()->SetTitle("");
	      //limit_hist->GetYaxis()->SetTitle("");
	      limit_hist->SetMarkerStyle(21);	

	      limit_hist->GetHistogram()->SetMaximum(0.2);   // along          
	      limit_hist->GetHistogram()->SetMinimum(0.);  //   Y     

	      limit_hist->Draw("psame");
	      //	limit_hist->Draw("same TEXT");
	      
	      // TPaveText *pt = new TPaveText(.625,.16,.84,.26,"nbNDC");
	      // pt->SetFillColor(0);
	      // if(decays[i]=="HT")pt->AddText("T ' #rightarrow H t");
	// if(decays[i]=="ZT")pt->AddText("T ' #rightarrow Z t");
	// if(decays[i]=="WB")pt->AddText("T ' #rightarrow W b");
	// pt->Draw();


	      c1->Print(dir+"Limit_uncertainties_"+channels[i]+"_"+category[k]+"_"+Mass[m]+".pdf");



	    }//mass
	  
	  
	}//categories
    }//channels
      
}//limits 2D
