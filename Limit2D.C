#include "TH2.h"
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;
#define Channels 2
#define Categories 1
#define dirs 2
#define namen 2
#define MassPoints 3

//less /nfs/dust/cms/user/abenecke/scripts/plots/Limit_2d.C


bool print=true; 
TString dir = "/nfs/dust/cms/user/zoiirene/LimitCode/CMSSW_8_1_0/src/DijetCombineLimitCode/LimitTxt/";



void Limit2D(){

  TString channel[Channels];
  channel[0] = "graviton";
  channel[1] = "radion";

  TString category[Categories];
  //  category[0] = "invMass";
  //category[1] = "invMass_afterVBFsel";
  category[0] = "invMass_combined";
    
  TString Mass[MassPoints];
  Mass[0] = "1200";
  Mass[1] = "2000";
  Mass[2] = "4000";
  
  TString names[namen];
  TString directory[dirs];

  //  directory[0]="testInvertedSelection/";
  directory[0]="testInvertedSelection/";
  directory[1]="testInvertedSelection/JMSJMR/";

  //  directory[1]="testInvertedSelectionPtAK4/";
  //directory[2]="testInvertedSelectionPtAK4_100/";
 
  //  names[0] = "noVBF_VBF_comb";
  names[0] = "updatedJEC";
  names[1] = "JMSJMR";
  //names[1] = "updatedJEC_AK4Pt50";
  //names[2] = "updatedJEC_AK4Pt100";


  std::vector<TString> mass = {"1200","2000","4000"};
  std::vector<TString> channels = {"graviton","radion"};
  std::vector<TString> testing = {"JECJER","JMSJMR"};//,"P_{T_{1}}^{AK4}>100GeV"};
  


  typedef std::map<std::pair<int, int>, double > Maptype;

  Maptype Limit_graviton[Categories];
  Maptype Limit_radion[Categories];
 
  double limit[MassPoints];   
  TString filename;
 
  //get limits from file
  for(int i=0;i<channels.size();i++)
    {
      for(int k=0;k<Categories;k++)
	{
	  for(int j=0;j<dirs;j++)
	    {
	      filename = dir+directory[j]+channel[i]+"__"+category[k]+"_"+names[j]+"_new_combined_Limit.txt";
	      //	      if(k==0) filename = dir+channel[i]+"__"+category[1]+"_"+cut_tau21[j]+"_new_combined_Limit.txt";
	      cout << filename << endl;
	      ifstream stream(filename);
	      std::string line;
	      if(!stream.is_open())	      
		{
		  limit[0]=0;
		  limit[1]=0;
		  limit[2]=0;
		  cout << " File " << filename << " not opened" << endl;
		}
	      else
		{
		  std::getline(stream,line);
		  if(print) cout << " first line " << line << endl; 
		  while(!stream.eof())
		    stream  >> limit[0]  >> limit[1] >> limit[2];
		  
		}
	      cout << "Limit for channel " << channel[i] << " category  " << category[1] << " and cuts " << names[j]<< " : " << endl;
	      
	      for(int l=0;l<MassPoints;l++)
		{
		  cout << " Mass " << Mass[l] << " -> " << limit[l] << endl;
		  
		  if(i==0)
		    {
		      Limit_graviton[k].insert( std::make_pair(std::make_pair( j,l ), limit[l]) );
		      cout << "Limit for "<< category[k] << " channel graviton[" << j  << "]["<< l << "] : "<< Limit_graviton[k][std::make_pair( j,l )]<<endl;
		      
		    }
		  if(i==1)
		    {
		      Limit_radion[k].insert( std::make_pair(std::make_pair( j,l ), limit[l]) );
		      cout << "Limit for "<< category[k] << " channel radion[" << j  << "]["<< l << "] : "<< Limit_radion[k][std::make_pair( j,l )] <<endl;;
		    }
		}//masses

	    }//dirs=namen
	}//categories
    }//channels



  //histograms and canvas
 
  for(int i=0;i<channels.size();i++)
    {
 
      for(int k=0;k<Categories;k++)
	{


	  TH2F *limit_hist = new TH2F("limit_hist","Limit hist",testing.size(),0,2,mass.size(),0,3);

	cout << " tau21cut.size() " << testing.size() << " and detacut.size() " << mass.size() << endl; 

       	for(int l=0;l<testing.size();l++)
	  {
	  for(int m=0;m<mass.size();m++)
	    {
	      if(channel[i]=="graviton")  limit_hist->Fill(testing[l],mass[m],Limit_graviton[k].find(std::make_pair(l,m))->second);
	      if(channel[i]=="radion")    limit_hist->Fill(testing[l],mass[m],Limit_radion[k][std::make_pair(l,m)]);
	  
	    }//mass
	  }//testing

	//save result
	TCanvas *c1= new TCanvas("c1","c1",10,10,600,600);
	c1->Clear();
	c1->cd();
	gPad->SetTickx();
	gPad->SetTicky();
	gStyle->SetOptStat(0);
	c1->SetTopMargin(0.05);
	c1->SetBottomMargin(0.15); 
	c1->SetRightMargin(0.15);
	c1->SetLeftMargin(0.1);
	limit_hist->SetTitle("");
	limit_hist->GetXaxis()->SetTitle("");
	limit_hist->GetYaxis()->SetTitle("");
	limit_hist->Draw("colz");
	limit_hist->Draw("same TEXT");

	// TPaveText *pt = new TPaveText(.625,.16,.84,.26,"nbNDC");
	// pt->SetFillColor(0);
	// if(decays[i]=="HT")pt->AddText("T ' #rightarrow H t");
	// if(decays[i]=="ZT")pt->AddText("T ' #rightarrow Z t");
	// if(decays[i]=="WB")pt->AddText("T ' #rightarrow W b");
	// pt->Draw();


	c1->Print(dir+"Limit_all_"+channel[i]+"_"+category[k]+".pdf");





	}//categories
    }//channels

}//limits 2D
