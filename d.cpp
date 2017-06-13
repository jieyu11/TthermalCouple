#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <TMinuit.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TLegend.h>
#include <TLatex.h>
#include <map>
using namespace std;
 
#include "drawPlot.h"
// http://www.cplusplus.com/forum/articles/9645/
template <typename T>
string NumberToString ( T Number )
{
   ostringstream ss; 
   ss << Number;
   return ss.str();
}
double StringToNumber ( const string &Text )
{
  stringstream ss(Text);
  double result;
  return ss >> result ? result : 0;
}

int main(int argc, char* argv[])
{

  if (argc<2) return 0;
  string sin = "";
  for (int it = 1; it < argc; it++){

    if(string(argv[it]) == "-In"){ sin = string(argv[it+1]); }
    else {cout<<"format not recognized."<<endl; return 2;}
    it += 1;
  } 

  cout<<"working on: "<<sin<<endl;

  TFile * f1 = new TFile("result.root", "recreate"); 
  TTree * tree0 = new TTree("atree", "thermal couple data");
  //float Tambient, TthermalIn, TthermalOut;
  //tree0->Branch("Tambient", &Tambient, "Tambient/F");
  //tree0->Branch("TthermalIn", &TthermalIn, "TthermalIn/F");
  //tree0->Branch("TthermalOut", &TthermalOut, "TthermalOut/F");

  float T1, T2, T3, T4;
  int year, month, date, hour, minute, second;
  tree0->Branch("T1", &T1, "T1/F");
  tree0->Branch("T2", &T2, "T2/F");
  tree0->Branch("T3", &T3, "T3/F");
  tree0->Branch("T4", &T4, "T4/F");
  tree0->Branch("year", &year, "year/I");
  tree0->Branch("month", &month, "month/I");
  tree0->Branch("date", &date, "date/I");
  tree0->Branch("hour", &hour, "hour/I");
  tree0->Branch("minute", &minute, "minute/I");
  tree0->Branch("second", &second, "second/I");

  int iT1 = -1, iT2 = -1, iT3 = -1, iT4 = -1;
  int iDate = -1, iTime = -1;
  ifstream fin(sin.c_str());
  string sline;
  int iline = 0;
  while(getline(fin, sline)){
    if (fin.eof()) break;

    if (sline.find("===") != string::npos) continue;

    size_t p0 = sline.find(",");
    vector<string> swords;
    while(p0 != string::npos){
      string aword = sline.substr(0,p0);
      aword.erase(std::remove(aword.begin(), aword.end(), ' '), aword.end());
      aword.erase(std::remove(aword.begin(), aword.end(), '"'), aword.end());
      swords.push_back( aword );
      sline = sline.substr(p0 + 1);
      p0 = sline.find(",");
    }

    if(swords.size() < 6) continue;
    //// "NO    ","Date      ","Time      ","  T1  ","T1 Type","  T2  ","T2 Type","  T3  ","T3 Type","  T4  ","T4 Type","T1-T2 ","T3-T4 ","Unit"
    //// "======","==========","==========","======","=======","======","=======","======","=======","======","=======","======","======","===="
    //// "0001  ","6/2/2017  ","16:03:37  ","-39.3 ","K      ","-34.6 ","K      ","16.9  ","K      ","21.8  ","K      ","-4.7  ","-4.9  ","°C  "

    if (iT2 < 0 || iT3 < 0 || iT4 < 0 || iDate < 0 || iTime < 0){
      int iw=0;
      for (auto aword : swords){
        if      (aword=="T1") iT1 = iw;
        else if (aword=="T2") iT2 = iw;
        else if (aword=="T3") iT3 = iw;
        else if (aword=="T4") iT4 = iw;
        else if (aword=="Date") iDate = iw;
        else if (aword=="Time") iTime = iw;
        iw++;
      }
    }else{
      T1 = StringToNumber( swords.at(iT1) );
      T2 = StringToNumber( swords.at(iT2) );
      T3 = StringToNumber( swords.at(iT3) );
      T4 = StringToNumber( swords.at(iT4) );

      size_t k0 = swords.at(iDate).find("/");
      size_t k1 = swords.at(iDate).rfind("/");
      if(k0 != string::npos && k1 != string::npos){
        month = int( StringToNumber( swords.at(iDate).substr(0, k0) ) );
        date   = int( StringToNumber( swords.at(iDate).substr(k0+1, k1 - k0 - 1) ) );
        year  = int( StringToNumber( swords.at(iDate).substr(k1+1) ) );
      }else{
        cout<<"ERROR: Date not found !!!! NOT filled! "<<endl;
      }

      k0 = swords.at(iTime).find(":");
      k1 = swords.at(iTime).rfind(":");
      if(k0 != string::npos && k1 != string::npos){
        hour   = int( StringToNumber( swords.at(iTime).substr(0, k0) ) );
        minute = int( StringToNumber( swords.at(iTime).substr(k0+1, k1 - k0 - 1) ) );
        second = int( StringToNumber( swords.at(iTime).substr(k1+1) ) );
      }else{
        cout<<"ERROR: Time not found !!!! NOT filled! "<<endl;
      }
      if(iline %100 == 0){
  cout<<"T: "<< T1 << " "<< T2 << " "<< T3 <<" " << T4<<endl;
  cout<<"Date: "<<year<<" "<< month << " "<< date<< endl;
  cout<<"Time: "<<hour<<" "<<minute << " "<<second <<endl;
      }
      iline ++;
      tree0 -> Fill();
    }
  }

  int NEntry = tree0 ->GetEntries();
  TH1F * ha = new TH1F("Tin_thermalCouple","", NEntry, 0., float(NEntry)/60.);
  TH1F * hb = new TH1F("Tout_thermalCouple","", NEntry, 0., float(NEntry)/60.);
  for (int iEntry=0; iEntry < NEntry; iEntry++){
    tree0->GetEntry(iEntry);
    ha -> SetBinContent(iEntry+1, T1); //T thermal In
    hb -> SetBinContent(iEntry+1, T2); //T thermal Out
    if (iEntry < 10)cout<<iEntry<<" Tin "<<T1<<" out "<<T2<<endl;
  }
  tree0->Write();
  ha->Write();
  hb->Write();

  drawPlot(ha, hb);
  delete tree0;
  delete f1;

  return 0;
}
