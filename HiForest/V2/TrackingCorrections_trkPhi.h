#ifndef TrackingCorrections2_h
#define TrackingCorrections2_h
#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TString.h"
#include "TLine.h"
using namespace std;

class TrackingCorrections2
{
public:
   vector<TFile*> sample_;
   vector<TString> centBin_;
   TH1D * ptBin_;
   TH1D * etaBin_;
   TH1D * jetBin_;
   
   // setup
   TString corrSetName_;
   TString trkCorrModule_;
   Int_t ptRebinFactor_;
   Int_t smoothLevel_;
   Bool_t weightSamples_;
   Bool_t isLeadingJet_;
   
   vector<TString> levelName_;
   vector<vector<TString> > levelInput_;
   vector<TString> matName_;
   vector<Float_t> ptHatMin_;
   vector<Float_t> sampleWeight_;
   
   Int_t numCentBins_;
   Int_t numPtBins_;
   Int_t numEtaBins_;
   Int_t numJEtBins_;
   Int_t numLevels_;
   
   vector<vector<vector<vector<TH3F*> > > > inputHists_;
   vector<vector<vector<TH3F*> > > combInputHists_;
   vector<vector<TH3F*> > correctionHists_;
   
   TrackingCorrections2(TString name="trkCorrHisAna_djuqv1",TString mod="hitrkEffAnalyzer");
   void AddSample(TString file, Float_t pthat) {
      sample_.push_back(new TFile(file));
      //cout << "sameple" << sample_.size() << ": " << sample_.back()->GetName() << endl;
      ptHatMin_.push_back(pthat);
   }
   void Init();
   Float_t GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * outCorr=0);
   TH2D * ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend);
   void Write();
   TH1 * InspectCorr(Int_t lv, Int_t centBeg, Int_t centEnd, Int_t jetBegBin, Int_t jetEndBin,Int_t mode=0,Int_t begbin=0, Int_t endbin=-1);
};

TrackingCorrections2::TrackingCorrections2(TString name, TString mod) :
corrSetName_(name),
trkCorrModule_(mod),
ptRebinFactor_(1),
smoothLevel_(0),
isLeadingJet_(true)
{
   centBin_.push_back("0to1");
   centBin_.push_back("2to3");
   centBin_.push_back("4to11");
   centBin_.push_back("12to19");
   if (name=="QM2011") centBin_.push_back("20to35");
   else if (name=="RAA2012") {
      centBin_.push_back("20to27");
      centBin_.push_back("28to35");
   }
   else centBin_.push_back("20to39");
   numCentBins_ = centBin_.size();
   
   levelName_.push_back("Eff");
   levelName_.push_back("Fak");
   levelName_.push_back("Mul");
   levelName_.push_back("Sec");
   numLevels_ = levelName_.size();
   
   levelInput_.resize(numLevels_);
   levelInput_[0].push_back("heff3D"); levelInput_[0].push_back("hsim3D");
   levelInput_[1].push_back("hfak3D"); levelInput_[1].push_back("hrec3D");
   levelInput_[2].push_back("hmul3D"); levelInput_[2].push_back("hsim3D");
   levelInput_[3].push_back("hsec3D"); levelInput_[3].push_back("hrec3D");
   
   matName_.push_back("Num");
   matName_.push_back("Den");   
}

void TrackingCorrections2::Init()
{
   cout << "==============================================" << endl;
   cout << " correction set: " << corrSetName_ << " module: " << trkCorrModule_ << endl;
   //cout << " isLeadingJet: " << isLeadingJet_ << " inputMethod: " << inputMethod << ", ptRebinFactor: " << ptRebinFactor_ << endl;
   cout << " Setup - smoothLevel: " << smoothLevel_ << endl;
   cout << "==============================================" << endl;
   // =============================
   // Setup Inputs
   // =============================
   for (Int_t i=0; i<sample_.size(); ++i) cout << sample_[i]->GetName() << endl;
   
   if (sample_.size()==0||!sample_[0]) {
      cout << "No input correction file" << endl;
      exit(1);
   }
   
   // =============================
   // Get x,y,z bins
   // =============================
   TH3F* h3tmp = (TH3F*)sample_[0]->Get(trkCorrModule_+"/heff3D_cbin"+centBin_[0]);
   if (!h3tmp) {
      cout << "bad input: " << trkCorrModule_+"/heff3D_cbin"+centBin_[0] << " in " << sample_[0]->GetName() << endl;
      exit(1);
   }
   
   etaBin_ = (TH1D*)h3tmp->Project3D("x");
   ptBin_  = (TH1D*)h3tmp->Project3D("y");
   jetBin_ = (TH1D*)h3tmp->Project3D("z");
   
   numPtBins_ = ptBin_->GetNbinsX();
   numEtaBins_ = etaBin_->GetNbinsX();
   numJEtBins_ = jetBin_->GetNbinsX();
   
	cout << endl << "Pt " << numPtBins_ << " bins:";
	for (Int_t i=1; i<=numPtBins_+1; ++i) cout << ptBin_->GetBinLowEdge(i) << " ";
	cout << endl << "Eta " << numEtaBins_ << " bins:";
	for (Int_t i=1; i<=numEtaBins_+1; ++i) cout << etaBin_->GetBinLowEdge(i) << " ";
	cout << endl << "JEt " << numJEtBins_ << " bins:";
	for (Int_t i=1; i<=numJEtBins_+1; ++i) cout << jetBin_->GetBinLowEdge(i) << " ";
	cout << endl;
   
   // =============================
   // allocate memory for input histograms
   // =============================
   cout << "Levels: " << numLevels_ << " Samples: " << sample_.size() << " cbins: " << numCentBins_ << endl;
   vector<vector<vector<vector<TH3F*> > > > v(numLevels_,
                                              vector<vector<vector<TH3F*> > > (sample_.size(),
                                                                               vector<vector<TH3F*> > (numCentBins_,
                                                                                                       vector<TH3F*>(2)
                                                                                                       )
                                                                               )
                                              );
   inputHists_ = v;
   
   // =============================
   // load corrections
   // =============================
   for (Int_t lv=0; lv<numLevels_; ++lv) {
      cout << "Load " << levelName_[lv] << " Histograms for ptHatMin";
      for (Int_t s=0; s<sample_.size(); ++s) {
         cout << " " << ptHatMin_[s];
         for (Int_t c=0; c<numCentBins_; ++c) {
            for (Int_t m=0; m<2; ++m) {
               TString hname(Form("%s/%s_cbin%s",trkCorrModule_.Data(),levelInput_[lv][m].Data(),centBin_[c].Data()));
               TH3F * h3 = (TH3F*)sample_[s]->Get(hname);
               if (!h3) {
                  cout << hname << " not found in correction file" << endl;
                  exit(1);
               }
               TH3F * hsave = (TH3F*)h3->Clone(Form("h%s_f%.0f_c%d_%d",(corrSetName_+levelName_[lv]).Data(),ptHatMin_[s],c,m));
               inputHists_[lv][s][c][m] = hsave;
               //cout << inputHists_[lv][s][c][m]->GetName() << ": " << inputHists_[lv][s][c][m] << endl;
            }
         }
      }
      cout << endl;
   }

   // =============================
   // set sample weights
   // =============================
   sampleWeight_.reserve(ptHatMin_.size());
   for (Int_t s=0; s<ptHatMin_.size(); ++s) { // merge pt hat samples with weight
      if ( fabs(ptHatMin_[s]-30)<0.1 ) sampleWeight_[s] = 1.079e-02;
      if ( fabs(ptHatMin_[s]-50)<0.1 ) sampleWeight_[s] = 1.021e-03;
      if ( fabs(ptHatMin_[s]-80)<0.1 ) sampleWeight_[s] = 9.913e-05;
      if ( fabs(ptHatMin_[s]-120)<0.1 ) sampleWeight_[s] = 1.128e-05;
      if ( fabs(ptHatMin_[s]-170)<0.1 ) sampleWeight_[s] = 1.470e-06;
      if ( fabs(ptHatMin_[s]-200)<0.1 ) sampleWeight_[s] = 5.310e-07;
      if ( fabs(ptHatMin_[s]-250)<0.1 ) sampleWeight_[s] = 1.192e-07;
      if ( fabs(ptHatMin_[s]-300)<0.1 ) sampleWeight_[s] = 3.176e-08;
      cout << "sample pthat: " << ptHatMin_[s] << " weight: " << sampleWeight_[s] << endl;
   }

   // =============================
   // weight inputs
   // =============================
   vector<vector<vector<TH3F*> > > vci(numLevels_,
                                      vector<vector<TH3F*> > (numCentBins_,
                                                              vector<TH3F*>(2)
                                                              )
                                      );
   combInputHists_ = vci;

   for (Int_t lv=0; lv<numLevels_; ++lv) {
      for (Int_t m=0; m<2; ++m) {
         for (Int_t c=0; c<numCentBins_; ++c) {
            // declare new histograms
            combInputHists_[lv][c][m] = (TH3F*)h3tmp->Clone(Form("combInputHist_lv%d_c%d_%d",lv,c,m));
            combInputHists_[lv][c][m]->Reset();
            for (Int_t ijet=1; ijet<=numJEtBins_; ++ijet) {
               for (Int_t ipt=1; ipt<=numPtBins_; ++ipt) {
                  for (Int_t ieta=1; ieta<=numEtaBins_; ++ieta) {
                     Double_t content[2] = {0,0};
                     Double_t error[2] = {0,0};
                     for (Int_t s=0; s<ptHatMin_.size(); ++s) { // merge pt hat samples with weight
                        if (!weightSamples_) {
                           content[m]+=inputHists_[lv][s][c][m]->GetBinContent(ieta,ipt,ijet);
                           error[m] = sqrt(pow(error[m],2)+pow(inputHists_[lv][s][c][m]->GetBinError(ieta,ipt,ijet),2));
                        } else {
                           content[m]+=inputHists_[lv][s][c][m]->GetBinContent(ieta,ipt,ijet)*sampleWeight_[s];
                           error[m] = sqrt(pow(error[m],2)+pow(inputHists_[lv][s][c][m]->GetBinError(ieta,ipt,ijet)*sampleWeight_[s],2));
                        }
                     } // end of for pt hat loop
                     combInputHists_[lv][c][m]->SetBinContent(ieta,ipt,ijet, content[m]);
                     combInputHists_[lv][c][m]->SetBinError(ieta,ipt,ijet, error[m]);
                  } // end for each correction bin
               }
            }
         }
      }
   }

   // =============================
   // correction histograms
   // =============================
   vector<vector<TH3F*> > vc(numLevels_,vector<TH3F*>(numCentBins_));
   correctionHists_ = vc;
   for (Int_t lv=0; lv<numLevels_; ++lv) {
      for (Int_t c=0; c<numCentBins_; ++c) {
         correctionHists_[lv][c] = (TH3F*)combInputHists_[lv][c][0]->Clone(Form("correctionHists_lv%d_c%d",lv,c));
         correctionHists_[lv][c]->Divide(combInputHists_[lv][c][1]);
      }
   }
}

Float_t TrackingCorrections2::GetCorr(Float_t pt, Float_t eta, Float_t jet, Float_t cent, Double_t * outCorr)
{
   // Find Correction Bin
   Int_t ptBin = ptBin_->FindBin(pt);
   Int_t etaBin = etaBin_->FindBin(eta);
   Int_t jetBin = jetBin_->FindBin(jet);
   
   Int_t bin = -1;   
   // Get the corresponding centrality bin
   if (cent<2) {
      bin = 0;
   } else if (cent<4) {
      bin = 1;
   } else if (cent<12) {
      bin = 2;
   } else if (cent<20) {
      bin = 3;   
   } else {
      bin = 4;
   }
   
   Double_t corr[4]={1,1,1,1};
   for (Int_t lv=0; lv<numLevels_; ++lv) {
      corr[lv] = correctionHists_[lv][bin]->GetBinContent(etaBin,ptBin,jetBin);
      if (lv==0&&corr[lv]<0.001) { // if eff==0, no correction, b/c too few statistics
       corr[lv] = 1;
      }
      if (outCorr) outCorr[lv] = corr[lv];
   }
   
   Double_t eff = corr[0];
   Double_t fake = corr[1];
   Double_t mul = corr[2];
   Double_t sec = corr[3];
   return (1-fake)*(1-sec)/(eff*(1+mul));
}

TH2D* TrackingCorrections2::ProjectPtEta(TH3F * h3, Int_t zbinbeg, Int_t zbinend)
{
   h3->GetZaxis()->SetRange(zbinbeg,zbinend);
   TH2D * h2 = (TH2D*)h3->Project3D(Form("yx_%d_%d",zbinbeg,zbinend));
   return h2;
}

void TrackingCorrections2::Write()
{
   for (Int_t lv=0; lv<numLevels_; ++lv) {
      for (Int_t s=0; s<sample_.size(); ++s) {
         for (Int_t c=0; c<numCentBins_; ++c) {
            for (Int_t m=0; m<2; ++m) {
               inputHists_[lv][s][c][m]->Write();
            }
         }
      }
   }
}

TH1 * TrackingCorrections2::InspectCorr(Int_t lv, Int_t centBeg, Int_t centEnd, Int_t jetBegBin, Int_t jetEndBin, Int_t mode, Int_t begbin, Int_t endbin)
{
   Int_t rebinFactor=1;
   TH3F *hNum=0, *hDen=0;
   TH2D *hNum2D=0, *hDen2D=0, *hCorr2D=0;
   TH1D *hNum1D=0, *hDen1D=0, *hCorr1D=0;
   TString inspName(Form("%s_%s_Lv%d_c%d_%d_j%d_%d_%d",(corrSetName_).Data(),trkCorrModule_.Data(),lv,centBeg,centEnd,jetBegBin,jetEndBin,mode));
   if (mode>0) inspName+=Form("_bin_%d_%d",begbin,endbin);
   cout << inspName << endl;
   
   hNum = (TH3F*)combInputHists_[lv][centBeg][0]->Clone(inspName+"hNum");
   hDen = (TH3F*)combInputHists_[lv][centBeg][1]->Clone(inspName+"hDen");
   
   if (mode<=2) {
      hNum->GetZaxis()->SetRange(jetBegBin,jetEndBin);
      hDen->GetZaxis()->SetRange(jetBegBin,jetEndBin);
      hNum2D = (TH2D*)hNum->Project3D("yx");
      hDen2D = (TH2D*)hDen->Project3D("yx");
      
      // ===============================================
      // 2D Inspection
      // ===============================================
      if (mode==0) {
         hCorr2D = (TH2D*)hNum2D->Clone(inspName+"Corr2D");
         hCorr2D->Divide(hNum2D,hDen2D);
         return hCorr2D;
      }
      
      // ===============================================
      // 1D Pt
      // ===============================================
      if (mode==1) {
         hNum1D = hNum2D->ProjectionX(inspName+"Num_px",begbin,endbin);
         hDen1D = hDen2D->ProjectionX(inspName+"Den_px",begbin,endbin);
      } else if (mode==2) {
         hNum1D = hNum2D->ProjectionY(inspName+"Num_py",begbin,endbin);
         hDen1D = hDen2D->ProjectionY(inspName+"Den_py",begbin,endbin);
      }
      
      // ===============================================
      // 1D Eta
      // ===============================================
      hNum1D->Rebin(rebinFactor);
      hDen1D->Rebin(rebinFactor);
      hCorr1D = (TH1D*)hNum1D->Clone(inspName+"Corr1D");
      hCorr1D->Divide(hNum1D,hDen1D);
   } else if (mode==3) {
      // ===============================================
      // 1D Jet
      // ===============================================
      //cout << "xaxis: " << begbin << "," << endbin << endl;
      //cout << "yaxis: " << jetBegBin << "," << jetEndBin << endl;
      hNum->GetYaxis()->SetRange(jetBegBin,jetEndBin); // set pt range
      hDen->GetYaxis()->SetRange(jetBegBin,jetEndBin); // set pt range
      hNum->GetXaxis()->SetRange(begbin,endbin); // set eta range
      hDen->GetXaxis()->SetRange(begbin,endbin); // set eta range
      hNum1D = (TH1D*)hNum->Project3D("z");
      hDen1D = (TH1D*)hDen->Project3D("z");
      hNum1D->Rebin(rebinFactor);
      hDen1D->Rebin(rebinFactor);
      hCorr1D = (TH1D*)hNum1D->Clone(inspName+"Corr1Dz");
      hCorr1D->Divide(hNum1D,hDen1D);
   }
   return hCorr1D;
}
#endif // TrackingCorrections2_h
