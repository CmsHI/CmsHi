// -*- C++ -*-
//
// Package:    DiMuonOnia2DPlots
// Class:      DiMuonOnia2DPlots
// 
/**\class DiMuonOnia2DPlots DiMuonOnia2DPlots.cc DiMuonAna/DiMuonOnia2DPlots/src/DiMuonOnia2DPlots.cc
   
 Description: [one line class summary]
 
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dilep PING, Vineet Kumar, Prashant Shukla
//         Created:  Wed May 12 13:45:14 CEST 2010
// $Id DiMuonOnia2DPlots.cc,v 1.9 2013/02/03 05:45:47 kumarv Exp $
//
//
// system include files
#include <memory>
#include <map>
#include <string>
#include <TLorentzVector.h>
#include <TNtuple.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>

// user include files   
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TFile.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include <DataFormats/VertexReco/interface/VertexFwd.h>
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/CompositeCandidate.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "TMath.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

using std::cout;
using std::endl;

using namespace pat;
using namespace reco;
using namespace edm;
using namespace std;
using namespace HepMC;
using namespace muon;

//
// class declaration
//

class DiMuonOnia2DPlots : public edm::EDAnalyzer {
   public:
  explicit DiMuonOnia2DPlots(const edm::ParameterSet&);
  ~DiMuonOnia2DPlots();

  const CentralityBins *cbins_;
  CentralityProvider *centrality_;    
 

  
  bool fisCuts;  
  std::string fOutputFileName;
  
  //std::string fGenLevel;
  //std::string fHLTPathName;
  
  std::string fIsGenInfo;
  std::string fIsPATInfo;
  std::string fHLTFilterName;  
  std::string fMotherID;  
 
  std::string fIsRecoInfo;
  std::string fIsPATWTInfo;
  std::string fIsCentInfo;


//edm::InputTag fHLTFilterName;
  

  TFile *In;

  TH1F *Centrality;
  TFile *fOutputFile ;
  
  TTree *SingleMuonTree;
  TTree *SingleGenMuonTree;
  TTree *EventTree;


  TTree *SingleRecoMuonTree;
  TTree *SinglePATMuonTree;
  TTree *SingleWTPATMuonTree;

  int bin, gbin, rbin;   
  

private:
  
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
  virtual bool matchPATMuon(const pat::Muon *pMuon);
  virtual void FillTree(const edm::Event&, const edm::EventSetup&);
  virtual void FillGenTree(const edm::Event&, const edm::EventSetup&);
  virtual void FillEvTree(const edm::Event&, const edm::EventSetup&);

  virtual void FillPATTree(const edm::Event&, const edm::EventSetup&);
  virtual void FillRecoTree(const edm::Event&, const edm::EventSetup&);
  virtual void FillWTPATTree(const edm::Event&, const edm::EventSetup&);

  math::XYZPoint RefVtx;
  double nPV;

  //========================= Ev Tree variables ========================================================//
                                                                                                                                         
  int eventNbEv,runNbEv,lumiBlockEv,vertexNbEv;
  double vertexTrkEv,vertexXEv,vertexYEv,vertexZEv,vertexRhoEv;
  double nPVEv;
  int rbinEv;
  bool isVtxFakeEv;

  int Sum_Y, Sum_No;
  //int NSG=0,NSG_no=0;
  int muonCounts;
  
  int GenParSize,GenParNo;
  double GenParPx[1000], GenParPy[1000], GenParPz[1000],GenParPt[1000], GenParPhi[1000], GenParEta[1000], GenParCharge[1000];
  double GenParRap[1000], GenParId[1000], GenParStatus[1000],GenParMomId[1000], GenParMomStatus[1000];

  double GenvertexXEv,GenvertexYEv,GenvertexZEv;

  bool eventTrigger0,eventTrigger1,eventTrigger2,eventTrigger3,eventTrigger4,eventTrigger5;
  bool eventTrigger6,eventTrigger7,eventTrigger8,eventTrigger9,eventTrigger10;

  int SingleMuSize,GlobalMuSize,GlobalMuNo;

  double SingleMuPx[1000][3], SingleMuPy[1000][3], SingleMuPz[1000][3],SingleMuPt[1000][3], SingleMuPhi[1000][3], SingleMuEta[1000][3]; 
  double SingleMuCharge[1000][3];

  double SingleMu_nchi2In[1000], SingleMu_dxy[1000],SingleMu_dz[1000], SingleMu_nchi2Gl[1000],SingleMu_nValidMuHits[1000];
  double SingleMu_found[1000], SingleMu_pixeLayers[1000], SingleMu_arbitrated[1000];
  
  int SingleMu_global[1000],SingleMu_tracker[1000],SingleMu_sta[1000],SingleMu_sta_noglb[1000];

  int SingleMu_trigger1[1000],SingleMu_trigger2[1000],SingleMu_trigger3[1000],SingleMu_trigger4[1000],SingleMu_trigger5[1000];
  int SingleMu_trigger6[1000],SingleMu_trigger7[1000],SingleMu_trigger8[1000],SingleMu_trigger9[1000],SingleMu_trigger10[1000];


  double GlobalMuPx[500000][3], GlobalMuPy[500000][3], GlobalMuPz[500000][3],GlobalMuPt[500000][3], GlobalMuPhi[500000][3], GlobalMuEta[500000][3]; 
  double GlobalMuCharge[500000][3];

  double GlobalMu_nchi2In[500000][2], GlobalMu_dxy[500000][2],GlobalMu_dz[500000][2], GlobalMu_nchi2Gl[500000][2],GlobalMu_nValidMuHits[500000][2];
  double GlobalMu_found[500000][2], GlobalMu_pixeLayers[500000][2], GlobalMu_arbitrated[500000][2];
  
  int GlobalMu_global[500000][2],GlobalMu_tracker[500000][2],GlobalMu_sta[500000][2],GlobalMu_sta_noglb[500000][2];

  int GlobalMu_trigger1[500000][2],GlobalMu_trigger2[500000][2],GlobalMu_trigger3[500000][2],GlobalMu_trigger4[500000][2],GlobalMu_trigger5[500000][2];
  int GlobalMu_trigger6[500000][2],GlobalMu_trigger7[500000][2],GlobalMu_trigger8[500000][2],GlobalMu_trigger9[500000][2],GlobalMu_trigger10[500000][2];

  
  double GlobalMu2Px[500000][3], GlobalMu2Py[500000][3], GlobalMu2Pz[500000][3],GlobalMu2Pt[500000][3], GlobalMu2Phi[500000][3], GlobalMu2Eta[500000][3]; 
  double GlobalMu2Charge[500000][3];

  double GlobaldiMuPx[500000][2], GlobaldiMuPy[500000][2], GlobaldiMuPz[500000][2],GlobaldiMuPt[500000][2], GlobaldiMuPhi[500000][2], GlobaldiMuRap[500000][2],
    GlobaldiMuMass[500000][2];
  double GlobaldiMuCharge[500000][2];

  double VrtxProb[500000];


  //================= DiMu Tree Variables =================================================================//
  

  
  int eventNb,runNb,lumiBlock,vertexNb;
  double vertexTrk,vertexX,vertexY,vertexZ,vertexRho;
  int Jpsi_eventTrigger1,Jpsi_eventTrigger2,Jpsi_eventTrigger3,Jpsi_eventTrigger4,Jpsi_eventTrigger5,Jpsi_eventTrigger6;
  

  int Npix, NpixelTracks, Ntracks; 
  double SumET_HF, SumET_HFplus, SumET_HFminus, SumET_HFplusEta4, SumET_HFminusEta4; 
  double SumET_EB, SumET_ET, SumET_EE, SumET_EEplus, SumET_EEminus, SumET_ZDC, SumET_ZDCplus, SumET_ZDCminus;
  


  //1.) J/psi variables RECO                                                                                                                                     
  
  double JpsiCharge,JpsiNo , JpsiMass , JpsiPt , JpsiRap ,JpsiVprob ;
  double JpsiPx , JpsiPy , JpsiPz ;
  
  

  double RecoCtau,RecoCtauErr,RecoCtauTrue;  
  
 
  //2.) muon variables RECO
                                                                                                                                                                                  
  double muPosPx, muPosPy, muPosPz,  muPosEta,  muPosPhi;
  double muNegPx, muNegPy, muNegPz,  muNegEta,  muNegPhi;

  //3.) cut variables

  //(i). Positive Muon                                                                                                            
  double muPos_nchi2In, muPos_dxy, muPos_dz, muPos_nchi2Gl;
  int muPos_found, muPos_trackerLayers, muPos_pixeLayers, muPos_nValidMuHits, muPos_arbitrated, muPos_stationTight;
  bool muPos_matches, muPos_tracker, muPos_global,muPos_sta;  
  
  //Posative muon triggers
  int muPos_Trigger1,muPos_Trigger2,muPos_Trigger3,muPos_Trigger4,muPos_Trigger5;
  int muPos_Trigger6,muPos_Trigger7,muPos_Trigger8,muPos_Trigger9,muPos_Trigger10,muPos_Trigger11,muPos_Trigger12,muPos_Trigger13;


  //(ii).Negative Muon                                                                                                             
  double muNeg_nchi2In, muNeg_dxy, muNeg_dz, muNeg_nchi2Gl;
  int muNeg_found, muNeg_trackerLayers, muNeg_pixeLayers, muNeg_nValidMuHits, muNeg_arbitrated, muNeg_stationTight;
  bool muNeg_matches, muNeg_tracker,muNeg_global,muNeg_sta;  

  //Negative muon triggers
  int muNeg_Trigger1,muNeg_Trigger2,muNeg_Trigger3,muNeg_Trigger4,muNeg_Trigger5;
  int muNeg_Trigger6,muNeg_Trigger7,muNeg_Trigger8,muNeg_Trigger9,muNeg_Trigger10,muNeg_Trigger11,muNeg_Trigger12,muNeg_Trigger13;

  bool isVtxFake;


  //====================== DiMuon Gen Variables ==============================================================================//  
  int GeventNb,GrunNb,GlumiBlock;
  double GenvertexX,GenvertexY,GenvertexZ;
 
  int Gen_Npix, Gen_NpixelTracks, Gen_Ntracks;
 
  double Gen_SumET_HF, Gen_SumET_HFplus, Gen_SumET_HFminus, Gen_SumET_HFplusEta4, Gen_SumET_HFminusEta4; 
  double Gen_SumET_EB, Gen_SumET_ET, Gen_SumET_EE, Gen_SumET_EEplus, Gen_SumET_EEminus, Gen_SumET_ZDC, Gen_SumET_ZDCplus, Gen_SumET_ZDCminus;



  //Gen JPsi Variables
  double GenJpsiMassP, GenJpsiPtP, GenJpsiRapP;
  double GenJpsiPxP, GenJpsiPyP, GenJpsiPzP;


  //Gen JPsi Variables
  
  double GenJpsiMass, GenJpsiPt, GenJpsiRap;
  double GenJpsiPx, GenJpsiPy, GenJpsiPz;



  //2.) muon variables Gen                                         
                                                                                                                 
  double GenmuPosPx, GenmuPosPy, GenmuPosPz,  GenmuPosEta, GenmuPosPhi;
  double GenmuNegPx, GenmuNegPy, GenmuNegPz,  GenmuNegEta, GenmuNegPhi;

  //================================ PAT Muon Variables =================================================//
  int PATMuSize;
  int eventNbPAT,runNbPAT,lumiBlockPAT,rbinPAT;
  

  int PATMu_global[1000],PATMu_tracker[1000],PATMu_sta[1000],PATMu_sta_noglb[1000];
  double PATMuPx[1000], PATMuPy[1000], PATMuPz[1000],PATMuPt[1000], PATMuPhi[1000], PATMuEta[1000],PATMuCharge[1000];





  //================================ Reco Muon Variables =================================================//
  int RecoMuSize;
  int eventNbReco,runNbReco,lumiBlockReco,rbinReco;
  
  int RecoMu_global[1000],RecoMu_tracker[1000],RecoMu_sta[1000],RecoMu_sta_noglb[1000];
  double RecoMuPx[1000], RecoMuPy[1000], RecoMuPz[1000],RecoMuPt[1000], RecoMuPhi[1000], RecoMuEta[1000],RecoMuCharge[1000];





  //================================WT PAT Muon Variables =================================================//
  int WTPATMuSize;
  int eventNbWTPAT,runNbWTPAT,lumiBlockWTPAT,rbinWTPAT;

  double PAT_vertexTrkEv,PAT_vertexXEv,PAT_vertexYEv,PAT_vertexZEv,PAT_vertexRhoEv;
  
  int PAT_Npix, PAT_NpixelTracks, PAT_Ntracks;
 
  double PAT_SumET_HF, PAT_SumET_HFplus, PAT_SumET_HFminus, PAT_SumET_HFplusEta4, PAT_SumET_HFminusEta4; 
  double PAT_SumET_EB, PAT_SumET_ET, PAT_SumET_EE, PAT_SumET_EEplus, PAT_SumET_EEminus, PAT_SumET_ZDC, PAT_SumET_ZDCplus, PAT_SumET_ZDCminus;



  int PAT_eventTrigger1,PAT_eventTrigger2,PAT_eventTrigger3,PAT_eventTrigger4,PAT_eventTrigger5,PAT_eventTrigger6;
  

  int PAT_vertexNbEv;
  bool PAT_isVtxFakeEv;
  
  int PAT_GenParSize,PAT_GenParNo;
  double PAT_GenvertexXEv,PAT_GenvertexYEv,PAT_GenvertexZEv;
  double PAT_GenParPx[1000], PAT_GenParPy[1000], PAT_GenParPz[1000],PAT_GenParPt[1000], PAT_GenParPhi[1000], PAT_GenParEta[1000], PAT_GenParCharge[1000];
  double PAT_GenParRap[1000], PAT_GenParId[1000], PAT_GenParStatus[1000],PAT_GenParMomId[1000], PAT_GenParMomStatus[1000];

  int WTPATMu_global[1000],WTPATMu_tracker[1000],WTPATMu_sta[1000],WTPATMu_sta_noglb[1000];
  double WTPATMuPx[1000], WTPATMuPy[1000], WTPATMuPz[1000],WTPATMuPt[1000], WTPATMuPhi[1000], WTPATMuEta[1000],WTPATMuCharge[1000];

  int WTPATMu_Trigger1[1000],WTPATMu_Trigger2[1000],WTPATMu_Trigger3[1000],WTPATMu_Trigger4[1000],WTPATMu_Trigger5[1000];
  int WTPATMu_Trigger6[1000],WTPATMu_Trigger7[1000],WTPATMu_Trigger8[1000],WTPATMu_Trigger9[1000],WTPATMu_Trigger10[1000];
  int WTPATMu_Trigger11[1000],WTPATMu_Trigger12[1000];


  double WTPATMu_found[1000],WTPATMu_nchi2In[1000],WTPATMu_arbitrated[1000],WTPATMu_stationTight[1000];
  double WTPATMu_trackerLayers[1000], WTPATMu_pixeLayers[1000], WTPATMu_dxy[1000],WTPATMu_dz[1000];
  double  WTPATMu_nValidMuHits[1000], WTPATMu_nchi2Gl[1000];




};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//


DiMuonOnia2DPlots::DiMuonOnia2DPlots(const edm::ParameterSet& iConfig):

  centrality_(0),
  cbins_(0),
  

  fisCuts(iConfig.getUntrackedParameter<bool>("IsCuts")),
  fOutputFileName(iConfig.getUntrackedParameter<string>("OutputFileName")),
  fIsGenInfo(iConfig.getUntrackedParameter<string>("IsGenInfo")), 
  fIsPATInfo(iConfig.getUntrackedParameter<string>("IsPATInfo")), 
  fHLTFilterName(iConfig.getUntrackedParameter<string>("HLTFilterName")),
  fIsRecoInfo(iConfig.getUntrackedParameter<string>("IsRecoInfo")), 
  fIsPATWTInfo(iConfig.getUntrackedParameter<string>("IsPATWTInfo")), 
  fIsCentInfo(iConfig.getUntrackedParameter<string>("IsCentInfo")), 
  fMotherID(iConfig.getUntrackedParameter<string>("MotherID"))
  
				    //fHLTFilterName(iConfig.getUntrackedParameter<edm::InputTag>("HLTFilterName"))

{ 

  //now do what ever initialization is needed

}


DiMuonOnia2DPlots::~DiMuonOnia2DPlots()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------

void DiMuonOnia2DPlots::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  

  //cout << " -----------------------------analyze------------------------- " << endl;
  

  using namespace edm;
  using namespace std;

  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    
    centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup);
    bin = centrality_->getBin();
  }
  else{
    bin=-99999;
  }

  Centrality->Fill(bin);
  
  //cout<<" Cent: "<<bin<<endl;

  //cout<<"  Sum_Y end  "<<Sum_Y<<"  Sum_No end "<<Sum_No<<endl;
  //cout<<" muonCounts end "<<muonCounts<<endl;

  
  //very big tree for detailed study
  //FillEvTree(iEvent,iSetup);
  //EventTree->Fill();
  
  if(!strcmp(fIsGenInfo.c_str(),"TRUE")){FillGenTree(iEvent,iSetup);}
  if(!strcmp(fIsPATInfo.c_str(),"TRUE")){FillTree(iEvent,iSetup);}


  //================ New Trees ========================//
  //FillPATTree(iEvent,iSetup);
  //SinglePATMuonTree->Fill();
  
  
  if(!strcmp(fIsPATWTInfo.c_str(),"TRUE")){
    FillWTPATTree(iEvent,iSetup);
    SingleWTPATMuonTree->Fill();
  }


  if(!strcmp(fIsRecoInfo.c_str(),"TRUE")){
    FillRecoTree(iEvent,iSetup);
    SingleRecoMuonTree->Fill();
  }

}

// ------------ method called once each job just before starting event loop  ------------
void 
DiMuonOnia2DPlots::beginJob()
{

  //cout << " ----------------1-------------------------------------- " << endl;
  edm::Service<TFileService> fs;
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" );

  Sum_Y=0, Sum_No=0;
  //int NSG=0,NSG_no=0;                                                                                                              
  muonCounts=0;
  
  cout<<"begin job"<<endl;

  
  EventTree = new TTree("EventTree","EventTree");

  EventTree->Branch("eventNbEv",             &eventNbEv,             "eventNbEv/I");
  EventTree->Branch("runNbEv",               &runNbEv,               "runNbEv/I");
  EventTree->Branch("lumiBlockEv",           &lumiBlockEv,           "lumiBlockEv/I");







  EventTree->Branch("GenParNo",           &GenParNo,           "GenParNo/I");
  EventTree->Branch("GenParSize",           &GenParSize,           "GenParSize/I");
  EventTree->Branch("GenvertexXEv",         &GenvertexXEv,         "GenvertexXEv/D");
  EventTree->Branch("GenvertexYEv",         &GenvertexYEv,          "GenvertexYEv/D");
  EventTree->Branch("GenvertexZEv",         &GenvertexZEv,          "GenvertexZEv/D");

  EventTree->Branch("GenParPx",      GenParPx,        "GenParPx[GenParSize]/D");
  EventTree->Branch("GenParPy",      GenParPy,        "GenParPy[GenParSize]/D");
  EventTree->Branch("GenParPz",      GenParPz,        "GenParPz[GenParSize]/D");
  EventTree->Branch("GenParPt",      GenParPt,        "GenParPt[GenParSize]/D");

  EventTree->Branch("GenParEta",     GenParEta,       "GenParEta[GenParSize]/D");
  EventTree->Branch("GenParPhi",     GenParPhi,       "GenParPhi[GenParSize]/D");
  EventTree->Branch("GenParRap",     GenParRap,       "GenParRap[GenParSize]/D");
  EventTree->Branch("GenParCharge",  GenParCharge,    "GenParCharge[GenParSize]/D");
  EventTree->Branch("GenParId",     GenParId,       "GenParId[GenParSize]/D");
  EventTree->Branch("GenParStatus",     GenParStatus,       "GenParStatus[GenParSize]/D");

  EventTree->Branch("GenParMomId",     GenParMomId,       "GenParMomId[GenParSize]/D");
  EventTree->Branch("GenParMomStatus",     GenParMomStatus,       "GenParMomStatus[GenParSize]/D");
  

  EventTree->Branch("nPVEv",                 &nPVEv,             "nPVEv/D");
  EventTree->Branch("rbinEv",                 &rbinEv,                  "rbinEv/I");

  EventTree->Branch("vertexNbEv",            &vertexNbEv,            "vertexNbEv/I");
  EventTree->Branch("vertexTrkEv",           &vertexTrkEv,            "vertexTrkEv/D");
  EventTree->Branch("isVtxFakeEv",          &isVtxFakeEv,            "isVtxFakeEv/O");
  EventTree->Branch("vertexXEv",            &vertexXEv,            "vertexXEv/D");
  EventTree->Branch("vertexYEv",            &vertexYEv,            "vertexYEv/D");
  EventTree->Branch("vertexZEv",             &vertexZEv,            "vertexZEv/D");
  EventTree->Branch("vertexRhoEv",           &vertexRhoEv,        "vertexRhoEv/D");


  EventTree->Branch("eventTrigger0",             &eventTrigger0,             "eventTrigger0/O");
  EventTree->Branch("eventTrigger1",             &eventTrigger1,             "eventTrigger1/O");
  EventTree->Branch("eventTrigger2",             &eventTrigger2,             "eventTrigger2/O");
  EventTree->Branch("eventTrigger3",             &eventTrigger3,             "eventTrigger3/O");
  EventTree->Branch("eventTrigger4",             &eventTrigger4,             "eventTrigger4/O");
  EventTree->Branch("eventTrigger5",             &eventTrigger5,             "eventTrigger5/O");
  EventTree->Branch("eventTrigger6",             &eventTrigger6,             "eventTrigger6/O");
  EventTree->Branch("eventTrigger7",             &eventTrigger7,             "eventTrigger7/O");
  EventTree->Branch("eventTrigger8",             &eventTrigger8,             "eventTrigger8/O");
  EventTree->Branch("eventTrigger9",             &eventTrigger9,             "eventTrigger9/O");
  EventTree->Branch("eventTrigger10",             &eventTrigger10,             "eventTrigger10/O");


  EventTree->Branch("SingleMuSize",    &SingleMuSize,           "SingleMuSize/I");
  EventTree->Branch("SingleMuPx",      SingleMuPx,        "SingleMuPx[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuPy",      SingleMuPy,        "SingleMuPy[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuPz",      SingleMuPz,        "SingleMuPz[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuPt",      SingleMuPt,        "SingleMuPt[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuEta",     SingleMuEta,       "SingleMuEta[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuPhi",     SingleMuPhi,       "SingleMuPhi[SingleMuSize][3]/D");
  EventTree->Branch("SingleMuCharge",  SingleMuCharge,    "SingleMuCharge[SingleMuSize][3]/D");
  
  EventTree->Branch("SingleMu_nchi2In",  SingleMu_nchi2In, "SingleMu_nchi2In[SingleMuSize]/D");
  EventTree->Branch("SingleMu_nchi2Gl",  SingleMu_nchi2Gl, "SingleMu_nchi2Gl[SingleMuSize]/D");
  EventTree->Branch("SingleMu_dxy",  SingleMu_dxy, "SingleMu_dxy[SingleMuSize]/D");
  EventTree->Branch("SingleMu_dz",  SingleMu_dz, "SingleMu_dz[SingleMuSize]/D");

  EventTree->Branch("SingleMu_found",  SingleMu_found, "SingleMu_found[SingleMuSize]/D");
  EventTree->Branch("SingleMu_pixeLayers",  SingleMu_pixeLayers, "SingleMu_pixeLayers[SingleMuSize]/D");
  EventTree->Branch("SingleMu_arbitrated",  SingleMu_arbitrated, "SingleMu_arbitrated[SingleMuSize]/D");
  EventTree->Branch("SingleMu_nValidMuHits",  SingleMu_nValidMuHits, "SingleMu_nValidMuHits[SingleMuSize]/D");

  EventTree->Branch("SingleMu_global",  SingleMu_global, "SingleMu_global[SingleMuSize]/I");
  EventTree->Branch("SingleMu_tracker", SingleMu_tracker, "SingleMu_tracker[SingleMuSize]/I");
  EventTree->Branch("SingleMu_sta", SingleMu_sta, "SingleMu_sta[SingleMuSize]/I");
  EventTree->Branch("SingleMu_sta_noglb", SingleMu_sta_noglb, "SingleMu_sta_noglb[SingleMuSize]/I");
  
  
  EventTree->Branch("SingleMu_trigger1",  SingleMu_trigger1, "SingleMu_trigger1[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger2",  SingleMu_trigger2, "SingleMu_trigger2[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger3",  SingleMu_trigger3, "SingleMu_trigger3[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger4",  SingleMu_trigger4, "SingleMu_trigger4[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger5",  SingleMu_trigger5, "SingleMu_trigger5[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger6",  SingleMu_trigger6, "SingleMu_trigger6[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger7",  SingleMu_trigger7, "SingleMu_trigger7[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger8",  SingleMu_trigger8, "SingleMu_trigger8[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger9",  SingleMu_trigger9, "SingleMu_trigger9[SingleMuSize]/I");
  EventTree->Branch("SingleMu_trigger10",  SingleMu_trigger10, "SingleMu_trigger10[SingleMuSize]/I");

  EventTree->Branch("GlobalMuNo",           &GlobalMuNo,           "GlobalMuNo/I");
  EventTree->Branch("GlobalMuSize",         &GlobalMuSize,           "GlobalMuSize/I");
  EventTree->Branch("GlobalMuPx",      GlobalMuPx,        "GlobalMuPx[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuPy",      GlobalMuPy,        "GlobalMuPy[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuPz",      GlobalMuPz,        "GlobalMuPz[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuPt",      GlobalMuPt,        "GlobalMuPt[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuEta",     GlobalMuEta,       "GlobalMuEta[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuPhi",     GlobalMuPhi,       "GlobalMuPhi[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMuCharge",  GlobalMuCharge,    "GlobalMuCharge[GlobalMuSize][3]/D");
  
  EventTree->Branch("GlobalMu2Px",      GlobalMu2Px,        "GlobalMu2Px[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Py",      GlobalMu2Py,        "GlobalMu2Py[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Pz",      GlobalMu2Pz,        "GlobalMu2Pz[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Pt",      GlobalMu2Pt,        "GlobalMu2Pt[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Eta",     GlobalMu2Eta,       "GlobalMu2Eta[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Phi",     GlobalMu2Phi,       "GlobalMu2Phi[GlobalMuSize][3]/D");
  EventTree->Branch("GlobalMu2Charge",  GlobalMu2Charge,    "GlobalMu2Charge[GlobalMuSize][3]/D");
  
  EventTree->Branch("GlobaldiMuPx",      GlobaldiMuPx,        "GlobaldiMuPx[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuPy",      GlobaldiMuPy,        "GlobaldiMuPy[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuPz",      GlobaldiMuPz,        "GlobaldiMuPz[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuPt",      GlobaldiMuPt,        "GlobaldiMuPt[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuRap",     GlobaldiMuRap,       "GlobaldiMuRap[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuPhi",     GlobaldiMuPhi,       "GlobaldiMuPhi[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuCharge",  GlobaldiMuCharge,    "GlobaldiMuCharge[GlobalMuSize][2]/D");
  EventTree->Branch("GlobaldiMuMass",      GlobaldiMuMass,        "GlobaldiMuMass[GlobalMuSize][2]/D");

  EventTree->Branch("VrtxProb",  &VrtxProb, "VrtxProb[GlobalMuSize]/D");



  EventTree->Branch("GlobalMu_nchi2In",  GlobalMu_nchi2In, "GlobalMu_nchi2In[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_nchi2Gl",  GlobalMu_nchi2Gl, "GlobalMu_nchi2Gl[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_dxy",  GlobalMu_dxy, "GlobalMu_dxy[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_dz",  GlobalMu_dz, "GlobalMu_dz[GlobalMuSize][2]/D");

  EventTree->Branch("GlobalMu_found",  GlobalMu_found, "GlobalMu_found[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_pixeLayers",  GlobalMu_pixeLayers, "GlobalMu_pixeLayers[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_arbitrated",  GlobalMu_arbitrated, "GlobalMu_arbitrated[GlobalMuSize][2]/D");
  EventTree->Branch("GlobalMu_nValidMuHits",  GlobalMu_nValidMuHits, "GlobalMu_nValidMuHits[GlobalMuSize][2]/D");

  EventTree->Branch("GlobalMu_global",  GlobalMu_global, "GlobalMu_global[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_tracker", GlobalMu_tracker, "GlobalMu_tracker[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_sta", GlobalMu_sta, "GlobalMu_sta[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_sta_noglb", GlobalMu_sta_noglb, "GlobalMu_sta_noglb[GlobalMuSize][2]/I");
  
  
  EventTree->Branch("GlobalMu_trigger1",  GlobalMu_trigger1, "GlobalMu_trigger1[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger2",  GlobalMu_trigger2, "GlobalMu_trigger2[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger3",  GlobalMu_trigger3, "GlobalMu_trigger3[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger4",  GlobalMu_trigger4, "GlobalMu_trigger4[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger5",  GlobalMu_trigger5, "GlobalMu_trigger5[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger6",  GlobalMu_trigger6, "GlobalMu_trigger6[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger7",  GlobalMu_trigger7, "GlobalMu_trigger7[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger8",  GlobalMu_trigger8, "GlobalMu_trigger8[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger9",  GlobalMu_trigger9, "GlobalMu_trigger9[GlobalMuSize][2]/I");
  EventTree->Branch("GlobalMu_trigger10",  GlobalMu_trigger10, "GlobalMu_trigger10[GlobalMuSize][2]/I");

  cout << " ------------------------------------------------------ " << endl;

  SingleMuonTree = new TTree("SingleMuonTree","SingleMuonTree");
  
  // Event variables
  SingleMuonTree->Branch("eventNb",             &eventNb,             "eventNb/I");
  SingleMuonTree->Branch("runNb",               &runNb,               "runNb/I");
  SingleMuonTree->Branch("lumiBlock",           &lumiBlock,           "lumiBlock/I");
  SingleMuonTree->Branch("nPV",                 &nPV,             "nPV/D");
  SingleMuonTree->Branch("vertexNb",            &vertexNb,            "vertexNb/I");
  SingleMuonTree->Branch("rbin",                 &rbin,                  "rbin/I");
  SingleMuonTree->Branch("vertexTrk",           &vertexTrk,            "vertexTrk/D");
  SingleMuonTree->Branch("vertexX",            &vertexX,            "vertexX/D");
  SingleMuonTree->Branch("vertexY",            &vertexY,            "vertexY/D");
  SingleMuonTree->Branch("vertexZ",             &vertexZ,            "vertexZ/D");
  SingleMuonTree->Branch("vertexRho",           &vertexRho,        "vertexRho/D");
  SingleMuonTree->Branch("isVtxFake",          &isVtxFake,            "isVtxFake/O");

  SingleMuonTree->Branch("Jpsi_eventTrigger1",             &Jpsi_eventTrigger1,             "Jpsi_eventTrigger1/I");
  SingleMuonTree->Branch("Jpsi_eventTrigger2",             &Jpsi_eventTrigger2,             "Jpsi_eventTrigger2/I");
  SingleMuonTree->Branch("Jpsi_eventTrigger3",             &Jpsi_eventTrigger3,             "Jpsi_eventTrigger3/I");
  SingleMuonTree->Branch("Jpsi_eventTrigger4",             &Jpsi_eventTrigger4,             "Jpsi_eventTrigger4/I");
  SingleMuonTree->Branch("Jpsi_eventTrigger5",             &Jpsi_eventTrigger5,             "Jpsi_eventTrigger5/I");
  SingleMuonTree->Branch("Jpsi_eventTrigger6",             &Jpsi_eventTrigger6,             "Jpsi_eventTrigger6/I");




  SingleMuonTree->Branch("Npix",&Npix,"Npix/I");
  SingleMuonTree->Branch("NpixelTracks",&NpixelTracks,"NpixelTracks/I");
  SingleMuonTree->Branch("Ntracks", &Ntracks, "Ntracks/I");
  SingleMuonTree->Branch("SumET_HF",&SumET_HF,"SumET_HF/D");
  SingleMuonTree->Branch("SumET_HFplus",&SumET_HFplus,"SumET_HFplus/D");
  SingleMuonTree->Branch("SumET_HFminus",&SumET_HFminus,"SumET_HFminus/D");

  SingleMuonTree->Branch("SumET_HFplusEta4",&SumET_HFplusEta4,"SumET_HFplusEta4/D");
  SingleMuonTree->Branch("SumET_HFminusEta4",&SumET_HFminusEta4,"SumET_HFminusEta4/D");
  
  SingleMuonTree->Branch("SumET_ET",&SumET_ET,"SumET_ET/D");
  SingleMuonTree->Branch("SumET_EE",&SumET_EE,"SumET_EE/D");
  SingleMuonTree->Branch("SumET_EB",&SumET_EB,"SumET_EB/D");
  SingleMuonTree->Branch("SumET_EEplus",&SumET_EEplus,"SumET_EEplus/D");
  SingleMuonTree->Branch("SumET_EEminus",&SumET_EEminus,"SumET_EEminus/D");
  SingleMuonTree->Branch("SumET_ZDC",&SumET_ZDC,"SumET_ZDC/D");
  SingleMuonTree->Branch("SumET_ZDCplus",&SumET_ZDCplus,"SumET_ZDCplus/D");
  SingleMuonTree->Branch("SumET_ZDCminus",&SumET_ZDCminus,"SumET_ZDCminus/D");










  //SingleMuonTree->Branch("RecJPsiSize",   &RecJPsiSize,  "RecJPsiSize/I");  

  // Jpsi Variables dimuon variable.

  //RecJPsiSize  (if we want to put array
  //SingleMuonTree->Branch("JpsiCharge", JpsiCharge,  "JpsiCharge[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiMass",   JpsiMass,  "JpsiMass[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiPt",     JpsiPt,    "JpsiPt[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiRap",    JpsiRap,   "JpsiRap[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiPx",     JpsiPx,    "JpsiPx[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiPy",     JpsiPy,    "JpsiPy[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiPz",     JpsiPz,    "JpsiPz[RecJPsiSize]/D");
  //SingleMuonTree->Branch("JpsiVprob",  JpsiVprob, "JpsiVprob[RecJPsiSize]/D");


  //RecJPsiSize                                                                                                                                                                                    
  SingleMuonTree->Branch("JpsiCharge", &JpsiCharge,  "JpsiCharge/D");
  SingleMuonTree->Branch("JpsiNo", &JpsiNo,  "JpsiNo/D");
  SingleMuonTree->Branch("JpsiMass",   &JpsiMass,  "JpsiMass/D");
  SingleMuonTree->Branch("JpsiPt",     &JpsiPt,    "JpsiPt/D");
  SingleMuonTree->Branch("JpsiRap",    &JpsiRap,   "JpsiRap/D");
  SingleMuonTree->Branch("JpsiPx",     &JpsiPx,    "JpsiPx/D");
  SingleMuonTree->Branch("JpsiPy",     &JpsiPy,    "JpsiPy/D");
  SingleMuonTree->Branch("JpsiPz",     &JpsiPz,    "JpsiPz/D");
  SingleMuonTree->Branch("JpsiVprob",  &JpsiVprob, "JpsiVprob/D");
  SingleMuonTree->Branch("RecoCtau",   &RecoCtau,   "RecoCtau/D");
  SingleMuonTree->Branch("RecoCtauErr", &RecoCtauErr,   "RecoCtauErr/D");
  SingleMuonTree->Branch("RecoCtauTrue", &RecoCtauTrue,   "RecoCtauTrue/D");



  //muon variable
  SingleMuonTree->Branch("muPosPx",    &muPosPx,   "muPosPx/D");
  SingleMuonTree->Branch("muPosPy",    &muPosPy,   "muPosPy/D");
  SingleMuonTree->Branch("muPosPz",    &muPosPz,   "muPosPz/D");
  SingleMuonTree->Branch("muPosEta",    &muPosEta,  "muPosEta/D");
  SingleMuonTree->Branch("muPosPhi",    &muPosPhi,  "muPosPhi/D");
  
  SingleMuonTree->Branch("muNegPx",    &muNegPx,   "muNegPx/D");
  SingleMuonTree->Branch("muNegPy",    &muNegPy,   "muNegPy/D");
  SingleMuonTree->Branch("muNegPz",    &muNegPz,   "muNegPz/D");
  SingleMuonTree->Branch("muNegEta",   &muNegEta,   "muNegEta/D");
  SingleMuonTree->Branch("muNegPhi",   &muNegPhi,   "muNegPhi/D");
 

  //muPos_trackerLayers muPos_stationTight muNeg_trackerLayers muNeg_stationTight
  //1). Positive Muon    
                                                                                                                                                                      
  SingleMuonTree->Branch("muPos_nchi2In", &muPos_nchi2In, "muPos_nchi2In/D");
  SingleMuonTree->Branch("muPos_dxy", &muPos_dxy, "muPos_dxy/D");
  SingleMuonTree->Branch("muPos_dz", &muPos_dz, "muPos_dz/D");
  SingleMuonTree->Branch("muPos_nchi2Gl", &muPos_nchi2Gl, "muPos_nchi2Gl/D");
  SingleMuonTree->Branch("muPos_found", &muPos_found, "muPos_found/I");

  SingleMuonTree->Branch("muPos_trackerLayers", &muPos_trackerLayers, "muPos_trackerLayers/I");
  SingleMuonTree->Branch("muPos_pixeLayers", &muPos_pixeLayers, "muPos_pixeLayers/I");


  SingleMuonTree->Branch("muPos_nValidMuHits", &muPos_nValidMuHits, "muPos_nValidMuHits/I");
  SingleMuonTree->Branch("muPos_arbitrated", &muPos_arbitrated, "muPos_arbitrated/I");
  SingleMuonTree->Branch("muPos_stationTight", &muPos_stationTight, "muPos_stationTight/I");

  SingleMuonTree->Branch("muPos_matches", &muPos_matches, "muPos_matches/O");
  SingleMuonTree->Branch("muPos_tracker", &muPos_tracker, "muPos_tracker/O");
  SingleMuonTree->Branch("muPos_global", &muPos_global, "muPos_global/O");
  SingleMuonTree->Branch("muPos_sta", &muPos_sta, "muPos_sta/O");
  

  SingleMuonTree->Branch("muPos_Trigger1", &muPos_Trigger1, "muPos_Trigger1/I");
  SingleMuonTree->Branch("muPos_Trigger2", &muPos_Trigger2, "muPos_Trigger2/I");
  SingleMuonTree->Branch("muPos_Trigger3", &muPos_Trigger3, "muPos_Trigger3/I");
  SingleMuonTree->Branch("muPos_Trigger4", &muPos_Trigger4, "muPos_Trigger4/I");
  SingleMuonTree->Branch("muPos_Trigger5", &muPos_Trigger5, "muPos_Trigger5/I");
  SingleMuonTree->Branch("muPos_Trigger6", &muPos_Trigger6, "muPos_Trigger6/I");
  SingleMuonTree->Branch("muPos_Trigger7", &muPos_Trigger7, "muPos_Trigger7/I");
  SingleMuonTree->Branch("muPos_Trigger8", &muPos_Trigger8, "muPos_Trigger8/I");
  SingleMuonTree->Branch("muPos_Trigger9", &muPos_Trigger9, "muPos_Trigger9/I");
  SingleMuonTree->Branch("muPos_Trigger10", &muPos_Trigger10, "muPos_Trigger10/I");

  SingleMuonTree->Branch("muPos_Trigger11", &muPos_Trigger11, "muPos_Trigger11/I");
  SingleMuonTree->Branch("muPos_Trigger12", &muPos_Trigger12, "muPos_Trigger12/I");
  SingleMuonTree->Branch("muPos_Trigger13", &muPos_Trigger13, "muPos_Trigger13/I");





  //2). Negative Muon     
                                                                                                                                                                     
  SingleMuonTree->Branch("muNeg_nchi2In", &muNeg_nchi2In, "muNeg_nchi2In/D");
  SingleMuonTree->Branch("muNeg_dxy", &muNeg_dxy, "muNeg_dxy/D");
  SingleMuonTree->Branch("muNeg_dz", &muNeg_dz, "muNeg_dz/D");
  SingleMuonTree->Branch("muNeg_nchi2Gl", &muNeg_nchi2Gl, "muNeg_nchi2Gl/D");
  SingleMuonTree->Branch("muNeg_found", &muNeg_found, "muNeg_found/I");
  SingleMuonTree->Branch("muNeg_trackerLayers", &muNeg_trackerLayers, "muNeg_trackerLayers/I");
  SingleMuonTree->Branch("muNeg_pixeLayers", &muNeg_pixeLayers, "muNeg_pixeLayers/I");
  SingleMuonTree->Branch("muNeg_nValidMuHits", &muNeg_nValidMuHits, "muNeg_nValidMuHits/I");
  SingleMuonTree->Branch("muNeg_arbitrated", &muNeg_arbitrated, "muNeg_arbitrated/I");
  SingleMuonTree->Branch("muNeg_stationTight", &muNeg_stationTight, "muNeg_stationTight/I");
  SingleMuonTree->Branch("muNeg_matches", &muNeg_matches, "muNeg_matches/O");
  SingleMuonTree->Branch("muNeg_tracker", &muNeg_tracker, "muNeg_tracker/O");
  SingleMuonTree->Branch("muNeg_global", &muNeg_global, "muNeg_global/O");
  SingleMuonTree->Branch("muNeg_sta", &muNeg_sta, "muNeg_sta/O");

  SingleMuonTree->Branch("muNeg_Trigger1", &muNeg_Trigger1, "muNeg_Trigger1/I");
  SingleMuonTree->Branch("muNeg_Trigger2", &muNeg_Trigger2, "muNeg_Trigger2/I");
  SingleMuonTree->Branch("muNeg_Trigger3", &muNeg_Trigger3, "muNeg_Trigger3/I");
  SingleMuonTree->Branch("muNeg_Trigger4", &muNeg_Trigger4, "muNeg_Trigger4/I");
  SingleMuonTree->Branch("muNeg_Trigger5", &muNeg_Trigger5, "muNeg_Trigger5/I");
  SingleMuonTree->Branch("muNeg_Trigger6", &muNeg_Trigger6, "muNeg_Trigger6/I");
  SingleMuonTree->Branch("muNeg_Trigger7", &muNeg_Trigger7, "muNeg_Trigger7/I");
  SingleMuonTree->Branch("muNeg_Trigger8", &muNeg_Trigger8, "muNeg_Trigger8/I");
  SingleMuonTree->Branch("muNeg_Trigger9", &muNeg_Trigger9, "muNeg_Trigger9/I");
  SingleMuonTree->Branch("muNeg_Trigger10", &muNeg_Trigger10, "muNeg_Trigger10/I");

  SingleMuonTree->Branch("muNeg_Trigger11", &muNeg_Trigger11, "muNeg_Trigger11/I");
  SingleMuonTree->Branch("muNeg_Trigger12", &muNeg_Trigger12, "muNeg_Trigger12/I");
  SingleMuonTree->Branch("muNeg_Trigger13", &muNeg_Trigger13, "muNeg_Trigger13/I");


  SingleGenMuonTree = new TTree("SingleGenMuonTree","SingleGenMuonTree");
  // Event variables                                                              
                                                         
  SingleGenMuonTree->Branch("GeventNb",   &GeventNb,       "GeventNb/I");
  SingleGenMuonTree->Branch("GrunNb",     &GrunNb,         "GrunNb/I");
  SingleGenMuonTree->Branch("GlumiBlock", &GlumiBlock,     "GlumiBlock/I");
 

  SingleGenMuonTree->Branch("GenvertexX",            &GenvertexX,            "GenvertexX/D");
  SingleGenMuonTree->Branch("GenvertexY",            &GenvertexY,            "GenvertexY/D");
  SingleGenMuonTree->Branch("GenvertexZ",             &GenvertexZ,            "GenvertexZ/D");


  SingleGenMuonTree->Branch("Gen_Npix",&Gen_Npix,"Gen_Npix/I");
  SingleGenMuonTree->Branch("Gen_NpixelTracks",&Gen_NpixelTracks,"Gen_NpixelTracks/I");
  SingleGenMuonTree->Branch("Gen_Ntracks", &Gen_Ntracks, "Gen_Ntracks/I");
  
  SingleGenMuonTree->Branch("Gen_SumET_HF",&Gen_SumET_HF,"Gen_SumET_HF/D");
  SingleGenMuonTree->Branch("Gen_SumET_HFplus",&Gen_SumET_HFplus,"Gen_SumET_HFplus/D");
  SingleGenMuonTree->Branch("Gen_SumET_HFminus",&Gen_SumET_HFminus,"Gen_SumET_HFminus/D");
  SingleGenMuonTree->Branch("Gen_SumET_HFplusEta4",&Gen_SumET_HFplusEta4,"Gen_SumET_HFplusEta4/D");
  SingleGenMuonTree->Branch("Gen_SumET_HFminusEta4",&Gen_SumET_HFminusEta4,"Gen_SumET_HFminusEta4/D");
  SingleGenMuonTree->Branch("Gen_SumET_ET",&Gen_SumET_ET,"Gen_SumET_ET/D");
  SingleGenMuonTree->Branch("Gen_SumET_EE",&Gen_SumET_EE,"Gen_SumET_EE/D");
  SingleGenMuonTree->Branch("Gen_SumET_EB",&Gen_SumET_EB,"Gen_SumET_EB/D");
  SingleGenMuonTree->Branch("Gen_SumET_EEplus",&Gen_SumET_EEplus,"Gen_SumET_EEplus/D");
  SingleGenMuonTree->Branch("Gen_SumET_EEminus",&Gen_SumET_EEminus,"Gen_SumET_EEminus/D");
  SingleGenMuonTree->Branch("Gen_SumET_ZDC",&Gen_SumET_ZDC,"Gen_SumET_ZDC/D");
  SingleGenMuonTree->Branch("Gen_SumET_ZDCplus",&Gen_SumET_ZDCplus,"Gen_SumET_ZDCplus/D");
  SingleGenMuonTree->Branch("Gen_SumET_ZDCminus",&Gen_SumET_ZDCminus,"Gen_SumET_ZDCminus/D");





  //Gen Jpsi Variables Parent  
       
  SingleGenMuonTree->Branch("GenJpsiMassP",   &GenJpsiMassP,  "GenJpsiMassP/D");

  SingleGenMuonTree->Branch("GenJpsiPtP",     &GenJpsiPtP,    "GenJpsiPtP/D");
  SingleGenMuonTree->Branch("GenJpsiRapP",    &GenJpsiRapP,   "GenJpsiRapP/D");
  SingleGenMuonTree->Branch("GenJpsiPxP",     &GenJpsiPxP,    "GenJpsiPxP/D");
  SingleGenMuonTree->Branch("GenJpsiPyP",     &GenJpsiPyP,    "GenJpsiPyP/D");
  SingleGenMuonTree->Branch("GenJpsiPzP",     &GenJpsiPzP,    "GenJpsiPzP/D");
  


  //Gen Jpsi Variables                                                                                                                                                        
  SingleGenMuonTree->Branch("GenJpsiMass",   &GenJpsiMass,  "GenJpsiMass/D");
  SingleGenMuonTree->Branch("GenJpsiPt",     &GenJpsiPt,    "GenJpsiPt/D");
  SingleGenMuonTree->Branch("GenJpsiRap",    &GenJpsiRap,   "GenJpsiRap/D");
  SingleGenMuonTree->Branch("GenJpsiPx",     &GenJpsiPx,    "GenJpsiPx/D");
  SingleGenMuonTree->Branch("GenJpsiPy",     &GenJpsiPy,    "GenJpsiPy/D");
  SingleGenMuonTree->Branch("GenJpsiPz",     &GenJpsiPz,    "GenJpsiPz/D");
  SingleGenMuonTree->Branch("gbin",          &gbin,             "gbin/I");

  //muon variable
  SingleGenMuonTree->Branch("GenmuPosPx",    &GenmuPosPx,   "GenmuPosPx/D");
  SingleGenMuonTree->Branch("GenmuPosPy",    &GenmuPosPy,   "GenmuPosPy/D");
  SingleGenMuonTree->Branch("GenmuPosPz",    &GenmuPosPz,   "GenmuPosPz/D");
  SingleGenMuonTree->Branch("GenmuPosEta",    &GenmuPosEta,   "GenmuPosEta/D");
  SingleGenMuonTree->Branch("GenmuPosPhi",    &GenmuPosPhi,   "GenmuPosPhi/D");
  
  SingleGenMuonTree->Branch("GenmuNegPx",    &GenmuNegPx,   "GenmuNegPx/D");
  SingleGenMuonTree->Branch("GenmuNegPy",    &GenmuNegPy,   "GenmuNegPy/D");
  SingleGenMuonTree->Branch("GenmuNegPz",    &GenmuNegPz,   "GenmuNegPz/D");
  SingleGenMuonTree->Branch("GenmuNegEta",    &GenmuNegEta,   "GenmuNegEta/D");
  SingleGenMuonTree->Branch("GenmuNegPhi",    &GenmuNegPhi,   "GenmuNegPhi/D");



  //===================== PAT Muon Tree ======================================//
  SinglePATMuonTree = new TTree("SinglePATMuonTree","SinglePATMuonTree");
  
  SinglePATMuonTree->Branch("eventNbPAT",             &eventNbPAT,             "eventNbPAT/I");
  SinglePATMuonTree->Branch("runNbPAT",               &runNbPAT,               "runNbPAT/I");
  SinglePATMuonTree->Branch("lumiBlockPAT",           &lumiBlockPAT,           "lumiBlockPAT/I");
  SinglePATMuonTree->Branch("rbinPAT",                 &rbinPAT,                  "rbinPAT/I");
  
  SinglePATMuonTree->Branch("PATMuSize",    &PATMuSize,           "PATMuSize/I");
  SinglePATMuonTree->Branch("PATMuPx",      PATMuPx,        "PATMuPx[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuPy",      PATMuPy,        "PATMuPy[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuPz",      PATMuPz,        "PATMuPz[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuPt",      PATMuPt,        "PATMuPt[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuEta",     PATMuEta,       "PATMuEta[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuPhi",     PATMuPhi,       "PATMuPhi[PATMuSize]/D");
  SinglePATMuonTree->Branch("PATMuCharge",  PATMuCharge,    "PATMuCharge[PATMuSize]/D");

  SinglePATMuonTree->Branch("PATMu_global",  PATMu_global, "PATMu_global[PATMuSize]/I");
  SinglePATMuonTree->Branch("PATMu_tracker", PATMu_tracker, "PATMu_tracker[PATMuSize]/I");
  SinglePATMuonTree->Branch("PATMu_sta", PATMu_sta, "PATMu_sta[PATMuSize]/I");
  SinglePATMuonTree->Branch("PATMu_sta_noglb", PATMu_sta_noglb, "PATMu_sta_noglb[PATMuSize]/I");



  //===================== Reco Muon Tree ======================================//
  SingleRecoMuonTree = new TTree("SingleRecoMuonTree","SingleRecoMuonTree");
  
  SingleRecoMuonTree->Branch("eventNbReco",             &eventNbReco,             "eventNbReco/I");
  SingleRecoMuonTree->Branch("runNbReco",               &runNbReco,               "runNbReco/I");
  SingleRecoMuonTree->Branch("lumiBlockReco",           &lumiBlockReco,           "lumiBlockReco/I");
  SingleRecoMuonTree->Branch("rbinReco",                 &rbinReco,                  "rbinReco/I");
  
  SingleRecoMuonTree->Branch("RecoMuSize",    &RecoMuSize,           "RecoMuSize/I");
  SingleRecoMuonTree->Branch("RecoMuPx",      RecoMuPx,        "RecoMuPx[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuPy",      RecoMuPy,        "RecoMuPy[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuPz",      RecoMuPz,        "RecoMuPz[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuPt",      RecoMuPt,        "RecoMuPt[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuEta",     RecoMuEta,       "RecoMuEta[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuPhi",     RecoMuPhi,       "RecoMuPhi[RecoMuSize]/D");
  SingleRecoMuonTree->Branch("RecoMuCharge",  RecoMuCharge,    "RecoMuCharge[RecoMuSize]/D");

  SingleRecoMuonTree->Branch("RecoMu_global",  RecoMu_global, "RecoMu_global[RecoMuSize]/I");
  SingleRecoMuonTree->Branch("RecoMu_tracker", RecoMu_tracker, "RecoMu_tracker[RecoMuSize]/I");
  SingleRecoMuonTree->Branch("RecoMu_sta", RecoMu_sta, "RecoMu_sta[RecoMuSize]/I");
  SingleRecoMuonTree->Branch("RecoMu_sta_noglb", RecoMu_sta_noglb, "RecoMu_sta_noglb[RecoMuSize]/I");



 //===================== WTPAT Muon Tree ======================================//
  SingleWTPATMuonTree = new TTree("SingleWTPATMuonTree","SingleWTPATMuonTree");
  
  SingleWTPATMuonTree->Branch("eventNbWTPAT",             &eventNbWTPAT,             "eventNbWTPAT/I");
  SingleWTPATMuonTree->Branch("runNbWTPAT",               &runNbWTPAT,               "runNbWTPAT/I");
  SingleWTPATMuonTree->Branch("lumiBlockWTPAT",           &lumiBlockWTPAT,           "lumiBlockWTPAT/I");
  SingleWTPATMuonTree->Branch("rbinWTPAT",                 &rbinWTPAT,                  "rbinWTPAT/I");
  


  SingleWTPATMuonTree->Branch("PAT_Npix",&PAT_Npix,"PAT_Npix/I");
  SingleWTPATMuonTree->Branch("PAT_NpixelTracks",&PAT_NpixelTracks,"PAT_NpixelTracks/I");
  SingleWTPATMuonTree->Branch("PAT_Ntracks", &PAT_Ntracks, "PAT_Ntracks/I");

  SingleWTPATMuonTree->Branch("PAT_SumET_HF",&PAT_SumET_HF,"PAT_SumET_HF/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_HFplus",&PAT_SumET_HFplus,"PAT_SumET_HFplus/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_HFminus",&PAT_SumET_HFminus,"PAT_SumET_HFminus/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_HFplusEta4",&PAT_SumET_HFplusEta4,"PAT_SumET_HFplusEta4/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_HFminusEta4",&PAT_SumET_HFminusEta4,"PAT_SumET_HFminusEta4/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_ET",&PAT_SumET_ET,"PAT_SumET_ET/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_EE",&PAT_SumET_EE,"PAT_SumET_EE/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_EB",&PAT_SumET_EB,"PAT_SumET_EB/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_EEplus",&PAT_SumET_EEplus,"PAT_SumET_EEplus/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_EEminus",&PAT_SumET_EEminus,"PAT_SumET_EEminus/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_ZDC",&PAT_SumET_ZDC,"PAT_SumET_ZDC/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_ZDCplus",&PAT_SumET_ZDCplus,"PAT_SumET_ZDCplus/D");
  SingleWTPATMuonTree->Branch("PAT_SumET_ZDCminus",&PAT_SumET_ZDCminus,"PAT_SumET_ZDCminus/D");








  SingleWTPATMuonTree->Branch("PAT_vertexNbEv",            &PAT_vertexNbEv,            "PAT_vertexNbEv/I");
  SingleWTPATMuonTree->Branch("PAT_vertexTrkEv",           &PAT_vertexTrkEv,            "PAT_vertexTrkEv/D");
  SingleWTPATMuonTree->Branch("PAT_isVtxFakeEv",          &PAT_isVtxFakeEv,            "PAT_isVtxFakeEv/O");
  SingleWTPATMuonTree->Branch("PAT_vertexXEv",            &PAT_vertexXEv,            "PAT_vertexXEv/D");
  SingleWTPATMuonTree->Branch("PAT_vertexYEv",            &PAT_vertexYEv,            "PAT_vertexYEv/D");
  SingleWTPATMuonTree->Branch("PAT_vertexZEv",             &PAT_vertexZEv,            "PAT_vertexZEv/D");
  SingleWTPATMuonTree->Branch("PAT_vertexRhoEv",           &PAT_vertexRhoEv,        "PAT_vertexRhoEv/D");


  SingleWTPATMuonTree->Branch("PAT_eventTrigger1",             &PAT_eventTrigger1,             "PAT_eventTrigger1/I");
  SingleWTPATMuonTree->Branch("PAT_eventTrigger2",             &PAT_eventTrigger2,             "PAT_eventTrigger2/I");
  SingleWTPATMuonTree->Branch("PAT_eventTrigger3",             &PAT_eventTrigger3,             "PAT_eventTrigger3/I");
  SingleWTPATMuonTree->Branch("PAT_eventTrigger4",             &PAT_eventTrigger4,             "PAT_eventTrigger4/I");
  SingleWTPATMuonTree->Branch("PAT_eventTrigger5",             &PAT_eventTrigger5,             "PAT_eventTrigger5/I");
  SingleWTPATMuonTree->Branch("PAT_eventTrigger6",             &PAT_eventTrigger6,             "PAT_eventTrigger6/I");


  SingleWTPATMuonTree->Branch("PAT_GenParNo",           &PAT_GenParNo,           "PAT_GenParNo/I");
  SingleWTPATMuonTree->Branch("PAT_GenParSize",           &PAT_GenParSize,           "PAT_GenParSize/I");
  SingleWTPATMuonTree->Branch("PAT_GenvertexXEv",         &PAT_GenvertexXEv,         "PAT_GenvertexXEv/D");
  SingleWTPATMuonTree->Branch("PAT_GenvertexYEv",         &PAT_GenvertexYEv,          "PAT_GenvertexYEv/D");
  SingleWTPATMuonTree->Branch("PAT_GenvertexZEv",         &PAT_GenvertexZEv,          "PAT_GenvertexZEv/D");

  SingleWTPATMuonTree->Branch("PAT_GenParPx",      PAT_GenParPx,        "PAT_GenParPx[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParPy",      PAT_GenParPy,        "PAT_GenParPy[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParPz",      PAT_GenParPz,        "PAT_GenParPz[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParPt",      PAT_GenParPt,        "PAT_GenParPt[PAT_GenParSize]/D");

  SingleWTPATMuonTree->Branch("PAT_GenParEta",     PAT_GenParEta,       "PAT_GenParEta[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParPhi",     PAT_GenParPhi,       "PAT_GenParPhi[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParRap",     PAT_GenParRap,       "PAT_GenParRap[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParCharge",  PAT_GenParCharge,    "PAT_GenParCharge[PAT_GenParSize]/D");
  
  SingleWTPATMuonTree->Branch("PAT_GenParId",     PAT_GenParId,       "PAT_GenParId[PAT_GenParSize]/D");
 
   SingleWTPATMuonTree->Branch("PAT_GenParStatus", PAT_GenParStatus,    "PAT_GenParStatus[PAT_GenParSize]/D");
  

  SingleWTPATMuonTree->Branch("PAT_GenParMomId",     PAT_GenParMomId,       "PAT_GenParMomId[PAT_GenParSize]/D");
  SingleWTPATMuonTree->Branch("PAT_GenParMomStatus",     PAT_GenParMomStatus,       "PAT_GenParMomStatus[PAT_GenParSize]/D");

  SingleWTPATMuonTree->Branch("WTPATMuSize",    &WTPATMuSize,           "WTPATMuSize/I");
  SingleWTPATMuonTree->Branch("WTPATMuPx",      WTPATMuPx,        "WTPATMuPx[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuPy",      WTPATMuPy,        "WTPATMuPy[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuPz",      WTPATMuPz,        "WTPATMuPz[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuPt",      WTPATMuPt,        "WTPATMuPt[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuEta",     WTPATMuEta,       "WTPATMuEta[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuPhi",     WTPATMuPhi,       "WTPATMuPhi[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMuCharge",  WTPATMuCharge,    "WTPATMuCharge[WTPATMuSize]/D");
  
  
  SingleWTPATMuonTree->Branch("WTPATMu_found",  WTPATMu_found, "WTPATMu_found[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_nchi2In",  WTPATMu_nchi2In, "WTPATMu_nchi2In[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_arbitrated",  WTPATMu_arbitrated, "WTPATMu_arbitrated[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_stationTight",  WTPATMu_stationTight, "WTPATMu_stationTight[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_trackerLayers",  WTPATMu_trackerLayers, "WTPATMu_trackerLayers[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_pixeLayers",  WTPATMu_pixeLayers, "WTPATMu_pixeLayers[WTPATMuSize]/D");
  
  SingleWTPATMuonTree->Branch("WTPATMu_dxy",  WTPATMu_dxy, "WTPATMu_dxy[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_dz",  WTPATMu_dz, "WTPATMu_dz[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_nValidMuHits",  WTPATMu_nValidMuHits, "WTPATMu_nValidMuHits[WTPATMuSize]/D");
  SingleWTPATMuonTree->Branch("WTPATMu_nchi2Gl",  WTPATMu_nchi2Gl, "WTPATMu_nchi2Gl[WTPATMuSize]/D");


  SingleWTPATMuonTree->Branch("WTPATMu_Trigger1",  WTPATMu_Trigger1, "WTPATMu_Trigger1[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger2",  WTPATMu_Trigger2, "WTPATMu_Trigger2[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger3",  WTPATMu_Trigger3, "WTPATMu_Trigger3[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger4",  WTPATMu_Trigger4, "WTPATMu_Trigger4[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger5",  WTPATMu_Trigger5, "WTPATMu_Trigger5[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger6",  WTPATMu_Trigger6, "WTPATMu_Trigger6[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger7",  WTPATMu_Trigger7, "WTPATMu_Trigger7[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger8",  WTPATMu_Trigger8, "WTPATMu_Trigger8[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger9",  WTPATMu_Trigger9, "WTPATMu_Trigger9[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger10",  WTPATMu_Trigger10, "WTPATMu_Trigger10[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger11",  WTPATMu_Trigger11, "WTPATMu_Trigger11[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_Trigger12",  WTPATMu_Trigger12, "WTPATMu_Trigger12[WTPATMuSize]/I");

  SingleWTPATMuonTree->Branch("WTPATMu_global",  WTPATMu_global, "WTPATMu_global[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_tracker", WTPATMu_tracker, "WTPATMu_tracker[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_sta", WTPATMu_sta, "WTPATMu_sta[WTPATMuSize]/I");
  SingleWTPATMuonTree->Branch("WTPATMu_sta_noglb", WTPATMu_sta_noglb, "WTPATMu_sta_noglb[WTPATMuSize]/I");


 


  cout<<"Tree booked "<<endl;


  //Histograms       
  Centrality = new TH1F("Centrality","Centrality", 110,-10,100);
   
  //h_ZetaGen_ = genParticleDir.make<TH1D>("generatedZeta","#eta of generated Z",100,-5.,5.); 

  // Write comments in a file
}



// ------------ method called once each job just after ending the event loop  ------------
void DiMuonOnia2DPlots::endJob() 
{
  cout<<"End Job"<<endl;
  fOutputFile->cd();
  
  //EventTree->Write();
  //SinglePATMuonTree->Write();
  
  







  if(!strcmp(fIsGenInfo.c_str(),"TRUE")){SingleGenMuonTree->Write();}
  if(!strcmp(fIsPATInfo.c_str(),"TRUE")){SingleMuonTree->Write();}
  if(!strcmp(fIsPATWTInfo.c_str(),"TRUE")){SingleWTPATMuonTree->Write();}
  if(!strcmp(fIsRecoInfo.c_str(),"TRUE")){SingleRecoMuonTree->Write();}










  Centrality->Write(); 
  fOutputFile->Close();

}



bool DiMuonOnia2DPlots::matchPATMuon(const pat::Muon *pMuon) 
{
  return(
	 //to match with filter name
	 //(!pMuon->triggerObjectMatchesByFilter(fHLTFilterName).empty())  
	 //to match with trigger name
	 //!pMuon->triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen").empty()
	 !pMuon->triggerObjectMatchesByPath(fHLTFilterName).empty()
	 
	 );

}

void DiMuonOnia2DPlots::FillTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std;

  
  //reset EVENT information     
  JpsiNo=-9999.;
  eventNb= 0 ;
  runNb= 0 ;
  lumiBlock= 0 ;

  vertexTrk=-999;
  vertexX=-999;
  vertexY=-999;
  vertexZ=-999;
  vertexRho=-999;
  vertexNb=-999;
  isVtxFake=-999;



  Jpsi_eventTrigger1=-9999;
  Jpsi_eventTrigger2=-9999;
  Jpsi_eventTrigger3=-9999;
  Jpsi_eventTrigger4=-9999;
  Jpsi_eventTrigger5=-9999;
  Jpsi_eventTrigger6=-9999;

  rbin=0;


  Npix = -9999;
  NpixelTracks = -9999;
  Ntracks = -9999;
  SumET_HF = -9999;
  SumET_HFplus = -9999;
  SumET_HFminus = -9999;
  SumET_HFplusEta4 = -9999;
  SumET_HFminusEta4 = -9999;
  SumET_ZDC = -9999;
  SumET_ZDCplus = -9999;
  SumET_ZDCminus = -9999;
  SumET_EEplus = -9999;
  SumET_EEminus = -9999;
  SumET_EE = -9999;
  SumET_EB = -9999;
  SumET_ET = -9999;


  


 // Event related infos
  eventNb= iEvent.id().event();
  runNb=iEvent.id().run();
  lumiBlock= iEvent.luminosityBlock();

  

  // cout << " --------------------------Singe Mu Tree ---------------------------- " << endl; 
  
  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    
    centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup);
    rbin = centrality_->getBin();
  }
  else{
    rbin=-99999;
  }

  //========================= Reading Centrality distribution and filling event variables ======================//
  
  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
 
  
    edm::Handle<reco::Centrality> collCentrality;
    iEvent.getByLabel("pACentrality",collCentrality);
  
  
    Npix = collCentrality->multiplicityPixel();
    NpixelTracks = collCentrality->NpixelTracks();
    Ntracks = collCentrality->Ntracks();
  
    SumET_HF = collCentrality->EtHFtowerSum();
    SumET_HFplus = collCentrality->EtHFtowerSumPlus();
    SumET_HFminus = collCentrality->EtHFtowerSumMinus();
    
    SumET_HFplusEta4 = collCentrality->EtHFtruncatedPlus();
    SumET_HFminusEta4 = collCentrality->EtHFtruncatedMinus();
    
    SumET_ZDC = collCentrality->zdcSum();
    SumET_ZDCplus = collCentrality->zdcSumPlus();
    SumET_ZDCminus = collCentrality->zdcSumMinus();
    SumET_EEplus = collCentrality->EtEESumPlus();
    SumET_EEminus = collCentrality->EtEESumMinus();
    SumET_EE = collCentrality->EtEESum();
    SumET_EB = collCentrality->EtEBSum();
    SumET_ET = collCentrality->EtMidRapiditySum();
  }
  
  nPV = 0 ;
  // Primary Vertex
  Handle<reco::VertexCollection> privtxs;
  //iEvent.getByLabel("hiSelectedVertex", privtxs);
  iEvent.getByLabel("offlinePrimaryVertices", privtxs);
  VertexCollection::const_iterator privtx;
  nPV = privtxs->size();
  vertexNb=privtxs->size();
  //cout<<"  nPv  "<<nPV<<endl;
  //if(!nPV) return; 


  if(nPV>0) {
    if(privtxs->begin() != privtxs->end() ) {
      privtx=privtxs->begin();
      RefVtx = privtx->position();
      vertexTrk=privtx->tracksSize();
      vertexX=privtx->x();
      vertexY=privtx->y();
      vertexZ=privtx->z();
      vertexRho=privtx->position().rho();
      
      if(privtx->isFake()){isVtxFake=1;}else{isVtxFake=0;}
      //cout<<" vtx x: "<<vertexX<<" vtxy "<<vertexY<<" z "<<vertexZ<<endl;
      //cout<<" ref vtx: "<<RefVtx.x()  <<" vy "<<RefVtx.y()<<" z "<<RefVtx.z() <<endl;
      //cout<<"-------------------------------------------------------------------------------------"<<endl;
    } 
  }
  else {
    RefVtx.SetXYZ(0.,0.,0.);
  }
  


  //----------------------------------- Event Trigger ----------------------------------------------------------------//


 //======================================== Event Trigger =====================================//                                                                                
  int evTrigger[6]={0};

  Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel("TriggerResults", triggerResults);

  const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults);
  for (unsigned i=0; i<triggerNames.size(); i++) {
    std::string hltName = triggerNames.triggerName(i);
    unsigned int index = triggerNames.triggerIndex(hltName);
    
    if(!strcmp(hltName.c_str(),"HLT_PAL1DoubleMuOpen_v1")) {if(triggerResults->accept(index)) evTrigger[0]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAL1DoubleMu0_HighQ_v1")) { if(triggerResults->accept(index)) evTrigger[1]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAL2DoubleMu3_v1")) { if(triggerResults->accept(index)) evTrigger[2]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAMu3_v1")) { if(triggerResults->accept(index)) evTrigger[3]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAMu7_v1")) { if(triggerResults->accept(index)) evTrigger[4]=1;}
    
    //last muon trigger changed to min bias trigger
    //if(!strcmp(hltName.c_str(),"HLT_PAMu12_v1")) { if(triggerResults->accept(index)) evTrigger[5]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAZeroBiasPixel_SingleTrack_v1")) { if(triggerResults->accept(index)) evTrigger[5]=1;}
                                                                                                         
    //cout << "HLT: " << hltName << " fired? " << triggerResults->accept(index) << endl;                                                                                          
  }
  
  if(evTrigger[0]==1){Jpsi_eventTrigger1=1;}else{Jpsi_eventTrigger1=0;}
  if(evTrigger[1]==1){Jpsi_eventTrigger2=1;}else{Jpsi_eventTrigger2=0;}
  if(evTrigger[2]==1){Jpsi_eventTrigger3=1;}else{Jpsi_eventTrigger3=0;}
  if(evTrigger[3]==1){Jpsi_eventTrigger4=1;}else{Jpsi_eventTrigger4=0;}
  if(evTrigger[4]==1){Jpsi_eventTrigger5=1;}else{Jpsi_eventTrigger5=0;}
  if(evTrigger[5]==1){Jpsi_eventTrigger6=1;}else{Jpsi_eventTrigger6=0;}












  //--------------------------------------Reco DimuonGlobal --------------------------------------------------------//

  using namespace edm;
  using namespace std;
  using namespace pat;

  edm::Handle<edm::View<pat::CompositeCandidate> > diMuonsPATCand;
  //iEvent.getByLabel("onia2MuMuPatGlbGlb", diMuonsPATCand);
  iEvent.getByLabel("onia2MuMuPatTrkTrk", diMuonsPATCand);

  if(!(diMuonsPATCand.isValid())) return;
  edm::View<pat::CompositeCandidate>dimuonsPATColl= *diMuonsPATCand;
  JpsiNo=dimuonsPATColl.size();
  
  //cout<<" reco Jpsi size : "<<dimuonsPATColl.size()<<endl;
  
  for(size_t ii = 0; ii <dimuonsPATColl.size(); ++ ii) 
    {
      
      //reset J/psi RECO variables
      JpsiCharge=-9999.;
      JpsiMass=-9999.;
      JpsiPt=-9999.;
      JpsiRap=-9999.;
      JpsiVprob=-9999.;
      JpsiPx=-9999.;
      JpsiPy=-9999.;
      JpsiPz=-9999.;
      RecoCtau=-9999;
      RecoCtauErr=-9999;
      RecoCtauTrue=-9999;
      
      
      muPosPx=-9999.;
      muPosPy=-9999.;
      muPosPz=-9999.;
      muPosEta=-9999.;
      muPosPhi=-9999.;
      
      muNegPx=-9999.;
      muNegPy=-9999.;
      muNegPz=-9999.;
      muNegEta=-9999.;
      muNegPhi=-9999.;
      
      
      //1).Positive Muon                                                                                                                                                                                                                         
      muPos_nchi2In=-9999.;
      muPos_dxy=-9999.;
      muPos_dz=-9999.;
      muPos_nchi2Gl=-9999.;
      muPos_found=-9999;
      muPos_pixeLayers=-9999;
      muPos_nValidMuHits=-9999;
      muPos_arbitrated=-9999;
      muPos_stationTight=-9999;
      muPos_trackerLayers=-9999;
      muPos_matches=0;
      muPos_tracker=0;
      muPos_global=0;
      muPos_sta=0;
      


      
      
      muPos_Trigger1=-9999;
      muPos_Trigger2=-9999;
      muPos_Trigger3=-9999;
      muPos_Trigger4=-9999;
      muPos_Trigger5=-9999;
      muPos_Trigger6=-9999;
      muPos_Trigger7=-9999;
      muPos_Trigger8=-9999;
      muPos_Trigger9=-9999;
      muPos_Trigger10=-9999;
      
      muPos_Trigger11=-9999;
      muPos_Trigger12=-9999;
      muPos_Trigger13=-9999;
      
     //2).Negtive Muon                                                                                                                                                                                                                          
      muNeg_nchi2In=-9999.;
      muNeg_dxy=-9999.;
      muNeg_dz=-9999.;
      muNeg_nchi2Gl=-9999.;
      muNeg_found=-9999;
      muNeg_pixeLayers=-9999;
      muNeg_nValidMuHits=-9999;
      muNeg_arbitrated=-9999;
      muNeg_stationTight=-9999;
      muNeg_trackerLayers=-9999;
      muNeg_matches=0;
      muNeg_tracker=0;
      muNeg_global=0;
      muNeg_sta=0;
      
      muNeg_Trigger1=-9999;
      muNeg_Trigger2=-9999;
      muNeg_Trigger3=-9999;
      muNeg_Trigger4=-9999;
      muNeg_Trigger5=-9999;
      muNeg_Trigger6=-9999;
      muNeg_Trigger7=-9999;
      muNeg_Trigger8=-9999;
      muNeg_Trigger9=-9999;
      muNeg_Trigger10=-9999;
      muNeg_Trigger11=-9999;
      muNeg_Trigger12=-9999;
      muNeg_Trigger13=-9999;
      
      const pat::CompositeCandidate &p = (dimuonsPATColl)[ii];
      const reco::Candidate *dau0 = p.daughter(0);
      const pat::Muon *mu0 = dynamic_cast<const pat::Muon *>(dau0);
      const reco::Candidate *dau1 = p.daughter(1); 
      const pat::Muon *mu1 = dynamic_cast<const pat::Muon *>(dau1);
      const pat::Muon *muonPos = 0, *muonNeg = 0;
      
      if(mu0->charge() > 0){ muonPos = mu0; muonNeg = mu1;}
      else if(mu0->charge() < 0){ muonPos = mu1; muonNeg = mu0;}
      
      
      //---------------------------------------- Trigger Matches -----------------------------------------//
      //Trigger matches        
      //cout<<matchPATMuon(muonPos)<<" matches "<<matchPATMuon(muonNeg)<<endl; 
      
      //for pp 2010
      //if(!muonPos->triggerObjectMatchesByPath("HLT_L1DoubleMu0_v1").empty()) {muPos_matches=1;}
      //if(!muonNeg->triggerObjectMatchesByPath("HLT_L1DoubleMu0_v1").empty()) {muNeg_matches=1;}

      //for PbPb 2010
      if(!muonPos->triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen").empty()) {muPos_matches=1;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen").empty()) {muNeg_matches=1;}

      //for PPb13
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAL1DoubleMuOpen_v1").empty()){muPos_Trigger1=1;}else{muPos_Trigger1=0;}
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAL1DoubleMu0_HighQ_v1").empty()){muPos_Trigger2=1;}else{muPos_Trigger2=0;}
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAL2DoubleMu3_v1").empty()){muPos_Trigger3=1;}else{muPos_Trigger3=0;}
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAMu3_v1").empty()){muPos_Trigger4=1;}else{muPos_Trigger4=0;}
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAMu7_v1").empty()){muPos_Trigger5=1;}else{muPos_Trigger5=0;}
      if(!muonPos->triggerObjectMatchesByPath("HLT_PAMu12_v1").empty()){muPos_Trigger6=1;}else{muPos_Trigger6=0;}
      
      //match by filter !muonPos->triggerObjectMatchesByFilter("hltHIDoubleMuLevel1PathL1HighQFiltered").empty()
      if(!muonPos->triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMuOpenL1Filtered0").empty()){muPos_Trigger7=1;}else{muPos_Trigger7=0;}
      if(!muonPos->triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMu0HighQL1FilteredHighQ").empty()){muPos_Trigger8=1;}else{muPos_Trigger8=0;}
      if(!muonPos->triggerObjectMatchesByFilter("hltL2fL1sPAL2DoubleMu3L2Filtered3").empty()){muPos_Trigger9=1;}else{muPos_Trigger9=0;}
      if(!muonPos->triggerObjectMatchesByFilter("hltL3fL2sMu3L3Filtered3").empty()){muPos_Trigger10=1;}else{muPos_Trigger10=0;}
      if(!muonPos->triggerObjectMatchesByFilter("hltL3fL2sMu7L3Filtered7").empty()){muPos_Trigger11=1;}else{muPos_Trigger11=0;}
      if(!muonPos->triggerObjectMatchesByFilter("hltL3fL2sMu12L3Filtered12").empty()){muPos_Trigger12=1;}else{muPos_Trigger12=0;}
      
      if(!muonPos->triggerObjectMatchesByPath("HLT_HIL3DoubleMuOpen_v1").empty()){muPos_Trigger13=1;}else{muPos_Trigger13=0;}
      
      
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAL1DoubleMuOpen_v1").empty()){muNeg_Trigger1=1;}else{muNeg_Trigger1=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAL1DoubleMu0_HighQ_v1").empty()){muNeg_Trigger2=1;}else{muNeg_Trigger2=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAL2DoubleMu3_v1").empty()){muNeg_Trigger3=1;}else{muNeg_Trigger3=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAMu3_v1").empty()){muNeg_Trigger4=1;}else{muNeg_Trigger4=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAMu7_v1").empty()){muNeg_Trigger5=1;}else{muNeg_Trigger5=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_PAMu12_v1").empty()){muNeg_Trigger6=1;}else{muNeg_Trigger6=0;}
      


      if(!muonNeg->triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMuOpenL1Filtered0").empty()){muNeg_Trigger7=1;}else{muNeg_Trigger7=0;}
      if(!muonNeg->triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMu0HighQL1FilteredHighQ").empty()){muNeg_Trigger8=1;}else{muNeg_Trigger8=0;}
      if(!muonNeg->triggerObjectMatchesByFilter("hltL2fL1sPAL2DoubleMu3L2Filtered3").empty()){muNeg_Trigger9=1;}else{muNeg_Trigger9=0;}
      if(!muonNeg->triggerObjectMatchesByFilter("hltL3fL2sMu3L3Filtered3").empty()){muNeg_Trigger10=1;}else{muNeg_Trigger10=0;}
      if(!muonNeg->triggerObjectMatchesByFilter("hltL3fL2sMu7L3Filtered7").empty()){muNeg_Trigger11=1;}else{muNeg_Trigger11=0;}
      if(!muonNeg->triggerObjectMatchesByFilter("hltL3fL2sMu12L3Filtered12").empty()){muNeg_Trigger12=1;}else{muNeg_Trigger12=0;}
      if(!muonNeg->triggerObjectMatchesByPath("HLT_HIL3DoubleMuOpen_v1").empty()){muNeg_Trigger13=1;}else{muNeg_Trigger13=0;}
      
      if(muonPos->isTrackerMuon()){muPos_tracker=1;}else{muPos_tracker=0;}
      if(muonNeg->isTrackerMuon()){muNeg_tracker=1;}else{muNeg_tracker=0;}
      
      if(muonPos->isGlobalMuon()){muPos_global=1;}else{muPos_global=0;}
      if(muonNeg->isGlobalMuon()){muNeg_global=1;}else{muNeg_global=0;}

      if(muonPos->isStandAloneMuon()){muPos_sta=1;}else{muPos_sta=0;}
      if(muonNeg->isStandAloneMuon()){muNeg_sta=1;}else{muNeg_sta=0;}

      
      // if(!muonPos->triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()){cout<<" trigger match by path "<<endl;}
      //if(!muonNeg->triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()){cout<<" trigger match by path "<<endl;}

      //if(!muonPos->triggerObjectMatchesByFilter("hltHIDoubleMuLevel1PathL1HighQFiltered").empty()){cout<<" trigger match by filter "<<endl;}
      //if(!muonNeg->triggerObjectMatchesByFilter("hltHIDoubleMuLevel1PathL1HighQFiltered").empty()){cout<<" trigger match by filter "<<endl;}


      //!muonPos->triggerObjectMatchesByFilter("hltHIDoubleMuLevel1PathL1HighQFiltered"). empty()

     
      //cout<<"      Tracker            Global            Sta"<<endl;
      //cout<<muPos_tracker<<"    "<<muPos_global<<"   "<<muPos_sta<<endl;
      //cout<<muNeg_tracker<<"    "<<muNeg_global<<"   "<<muNeg_sta<<endl;
      
      //cout<<"muPos_matches "<<muPos_matches<<" muNeg_matches "<<muNeg_matches<<endl; 
      //cout<<"JpsiCharge " <<p.charge()<<" JpsiMass  "<<p.M()<<endl;
      
      
    // write out JPsi RECO information
    //cout<<" inside loop RecJPsiSize "<<RecJPsiSize<<endl;
      
    
      JpsiCharge  = p.charge(); //will changed for onia dimuon loop
    

    //JpsiCharge  = Charge;
    JpsiMass =p.mass();
    JpsiPt =p.pt();
    JpsiRap =p.rapidity();
    JpsiPx =p.px();
    JpsiPy =p.py();
    JpsiPz =p.pz();
    
    JpsiVprob =p.userFloat("vProb");
    RecoCtau=10.0*p.userFloat("ppdlPV");
    RecoCtauErr=10.0*p.userFloat("ppdlErrPV");
    RecoCtauTrue=10.*p.userFloat("ppdlTrue");

    //vector<TransientTrack> t_tks;
    //t_tks.push_back(theTTBuilder->build(*muonPos->track()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)                       
    //t_tks.push_back(theTTBuilder->build(*muonNeg->track())); // otherwise the vertex will have transient refs inside.                                         
    //TransientVertex myVertex = vtxFitter.vertex(t_tks);

    //if (myVertex.isValid()) {
    //float vChi2 = myVertex.totalChiSquared();
    //float vNDF  = myVertex.degreesOfFreedom();
    //float vProb(TMath::Prob(vChi2,(int)vNDF));

    //}
    
    //JpsiVprob=-9999;
    //RecoCtau=-9999;
    //RecoCtauErr=-9999;
    //RecoCtauTrue=-9999;
       
    // write out Muon RECO information                                                                                                                                                                                                     
    float f_muPosPx, f_muPosPy, f_muPosPz, f_muPosEta, f_muPosPhi;
    float f_muNegPx, f_muNegPy, f_muNegPz, f_muNegEta, f_muNegPhi;
    
    f_muPosPx = muonPos->px();
    f_muPosPy = muonPos->py();
    f_muPosPz = muonPos->pz();
    f_muPosEta = muonPos->eta();
    f_muPosPhi= muonPos->phi();

    f_muNegPx = muonNeg->px();
    f_muNegPy = muonNeg->py();
    f_muNegPz = muonNeg->pz();
    f_muNegEta = muonNeg->eta();
    f_muNegPhi = muonNeg->phi();
    
    muPosPx= f_muPosPx ;
    muPosPy= f_muPosPy ;
    muPosPz= f_muPosPz ;
    muPosEta= f_muPosEta;
    muPosPhi= f_muPosPhi;
    
    muNegPx= f_muNegPx ;
    muNegPy= f_muNegPy ;
    muNegPz= f_muNegPz ;
    muNegEta= f_muNegEta;
    muNegPhi= f_muNegPhi;
    
    //-----------------------------------------------------                                                                                                                                                                                
    //-----------additional Reco Muon Variables------------                                                                                                                                                                                
    //----------------------------------------------------- 
    //reco::TrackRef iTrack = aMuon->innerTrack();
    //const reco::HitPattern& p = iTrack->hitPattern();
    //return (isMuonInAccept(aMuon) &&
    //    iTrack->normalizedChi2() < 1.8 &&
    //    aMuon->muonID("TrackerMuonArbitrated") &&
    //    aMuon->muonID("TMOneStationTight") &&
    //    p.trackerLayersWithMeasurement() > 5 &&
    //    p.pixelLayersWithMeasurement() > 1 &&
    //    fabs(iTrack->dxy(RefVtx)) < 3.0 &&
    //    fabs(iTrack->dz(RefVtx)) < 30.0 );

    
    
    //1.Positive Muon                                                                                                                                                                                                                      
    //if(muonPos->isTrackerMuon() && muonPos->isGlobalMuon())    

    if(muonPos->isTrackerMuon())    
      {
	TrackRef iTrack =muonPos->innerTrack();
	const reco::HitPattern& p1=iTrack->hitPattern();

	muPos_found=iTrack->found();
	//muPos_nchi2In=iTrack->chi2()/iTrack->ndof();
	muPos_nchi2In=iTrack->normalizedChi2();
	muPos_arbitrated=muonPos->muonID("TrackerMuonArbitrated");
	muPos_stationTight=muonPos->muonID("TMOneStationTight");
	muPos_trackerLayers=p1.trackerLayersWithMeasurement();
	muPos_pixeLayers=p1.pixelLayersWithMeasurement();
	muPos_dxy=iTrack->dxy(RefVtx);
	muPos_dz=iTrack->dz(RefVtx);
	
	if(muonPos->isGlobalMuon())
	  {
	    TrackRef gTrack =muonPos->globalTrack();
	    const reco::HitPattern& q1=gTrack->hitPattern();
	    muPos_nValidMuHits=q1.numberOfValidMuonHits();
	    muPos_nchi2Gl=gTrack->chi2()/gTrack->ndof();
	  }
      }


  //2.Negative Muobn                                                                                                                                                                                                                     
    //if(muonNeg->isTrackerMuon() && muonNeg->isGlobalMuon())

    if(muonNeg->isTrackerMuon() )
      
      {
	TrackRef iTrack =muonNeg->innerTrack();
	const reco::HitPattern& p2=iTrack->hitPattern();
	muNeg_found=iTrack->found();
	muNeg_nchi2In=iTrack->normalizedChi2();
	muNeg_arbitrated=muonNeg->muonID("TrackerMuonArbitrated");
	muNeg_stationTight=muonNeg->muonID("TMOneStationTight");
	muNeg_trackerLayers=p2.trackerLayersWithMeasurement();
	muNeg_pixeLayers=p2.pixelLayersWithMeasurement();
	muNeg_dxy=iTrack->dxy(RefVtx);
	muNeg_dz=iTrack->dz(RefVtx);
      
	if(muonNeg->isGlobalMuon())
	  {
	    TrackRef gTrack =muonNeg->globalTrack();
	    const reco::HitPattern& q2=gTrack->hitPattern();
	    muNeg_nValidMuHits=q2.numberOfValidMuonHits();
	    muNeg_nchi2Gl=gTrack->chi2()/gTrack->ndof();
	  }
      }
    SingleMuonTree->Fill();
    
    //RecJPsiSize++;
    //cout<<"RecJPsiSiZe " <<RecJPsiSize<<endl;
    
    
    //loop should be changed when we put oniaDiMuon loop one loop will remain for onia
    //}//muon j loop
    
    }//Onia loop //muon i loop for PAT muons
  
  
  //cout<<" fill tree called "<<endl;

}

void DiMuonOnia2DPlots::FillGenTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  GenJpsiMassP=-9999.;
  GenJpsiPtP=-9999.;
  GenJpsiRapP=-9999.;
  GenJpsiPxP=-9999.;
  GenJpsiPyP=-9999.;
  GenJpsiPzP=-9999.;


  GenJpsiMass=-9999.;
  GenJpsiPt=-9999.;
  GenJpsiRap=-9999.;
  GenJpsiPx=-9999.;
  GenJpsiPy=-9999.;
  GenJpsiPz=-9999.;
  
  GenmuPosPx=-9999.;
  GenmuPosPy=-9999.;
  GenmuPosPz=-9999.;
  GenmuPosEta=-9999.;
  GenmuPosPhi=-9999.;
  
  GenmuNegPx=-9999.;
  GenmuNegPy=-9999.;
  GenmuNegPz=-9999.;
  GenmuNegEta=-9999.; 
  GenmuNegPhi=-9999.; 


  //reset EVENT information  
    GeventNb= 0 ;
    GrunNb= 0 ;
    GlumiBlock= 0 ;
    gbin=-999;
    GenvertexX=-9999;
    GenvertexY=-9999;
    GenvertexZ=-9999;



    Gen_Npix = -9999;
    Gen_NpixelTracks = -9999;
    Gen_Ntracks = -9999;
    Gen_SumET_HF = -9999;
    Gen_SumET_HFplus = -9999;
    Gen_SumET_HFminus = -9999;
    Gen_SumET_HFplusEta4 = -9999;
    Gen_SumET_HFminusEta4 = -9999;
    Gen_SumET_ZDC = -9999;
    Gen_SumET_ZDCplus = -9999;
    Gen_SumET_ZDCminus = -9999;
    Gen_SumET_EEplus = -9999;
    Gen_SumET_EEminus = -9999;
    Gen_SumET_EE = -9999;
    Gen_SumET_EB = -9999;
    Gen_SumET_ET = -9999;




    // Event related infos 
                                                                                                                    
    GeventNb= iEvent.id().event();
    GrunNb=iEvent.id().run();
    GlumiBlock= iEvent.luminosityBlock();
    
    //cout<<" Gen Level Tree "<<endl;
    
    //gbin = -9999;


    if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
      centrality_ = new CentralityProvider(iSetup);
      centrality_->newEvent(iEvent,iSetup);
      gbin = centrality_->getBin();
    }
    else{gbin=-99999;}

  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    edm::Handle<reco::Centrality> collCentrality;
    iEvent.getByLabel("pACentrality",collCentrality);
    

    Gen_Npix = collCentrality->multiplicityPixel();
    Gen_NpixelTracks = collCentrality->NpixelTracks();
    Gen_Ntracks = collCentrality->Ntracks();
    
    Gen_SumET_HF = collCentrality->EtHFtowerSum();
    Gen_SumET_HFplus = collCentrality->EtHFtowerSumPlus();
    Gen_SumET_HFminus = collCentrality->EtHFtowerSumMinus();
    Gen_SumET_HFplusEta4 = collCentrality->EtHFtruncatedPlus();
    Gen_SumET_HFminusEta4 = collCentrality->EtHFtruncatedMinus();
    Gen_SumET_ZDC = collCentrality->zdcSum();
    Gen_SumET_ZDCplus = collCentrality->zdcSumPlus();
    Gen_SumET_ZDCminus = collCentrality->zdcSumMinus();
    Gen_SumET_EEplus = collCentrality->EtEESumPlus();
    Gen_SumET_EEminus = collCentrality->EtEESumMinus();
    Gen_SumET_EE = collCentrality->EtEESum();
    Gen_SumET_EB = collCentrality->EtEBSum();
    Gen_SumET_ET = collCentrality->EtMidRapiditySum();
  }





  //-----------------------------------------------------hiGenParticle----------------------------------------------------------------------

  float mumass =0.105658389;
  using namespace edm;
  using namespace std;
  
  edm::Handle<edm::View<reco::GenParticle> >genPar;
  //iEvent.getByLabel("hiGenParticles",genPar) ;
  iEvent.getByLabel("genParticles",genPar) ;
  
  

  if(!(genPar.isValid())) return;


  edm::View<reco::GenParticle> genParticles = *genPar ;
  TLorentzVector  genvector1, genvector2;
  
  double px1[10000], py1[10000], pz1[10000], px2[10000], py2[10000], pz2[10000];
  double peta1[10000],pphi1[10000],peta2[10000],pphi2[10000];
  unsigned int nplus = 0, nminus =0;


  for(size_t i = 0; i < genParticles.size(); ++ i) {



    const reco::GenParticle& part = (*genPar)[i];
    const  Candidate *mom = part.mother();
   
    if (part.numberOfMothers()!=1) continue;
    int momId = mom->pdgId();

    char* MID=(char*)"AB";
    if(momId == 100443) MID=(char *)"PsiPrime";
    if(momId == 443) MID=(char *)"JPsi";
    if(momId == 553) MID=(char *)"Upsilon1s";
    if(momId == 100553)MID=(char *)"Upsilon2s";
    if(momId == 23)MID=(char *)"Z0";
    
    if ((abs(part.pdgId()) == 13) && (!strcmp(fMotherID.c_str(),MID)) ){
        
    //if (abs(part.pdgId()) == 13 &&  momId == 443){
   //Upsilon 1s
    // if (abs(part.pdgId()) == 13 &&  momId == 553){
   //Upsilon 2s
    //if (abs(part.pdgId()) == 13 &&  momId == 100553){

      GenvertexX = part.vx();
      GenvertexY = part.vy();
      GenvertexZ = part.vz();

      //cout<<"GenvertexX  "<<GenvertexX<<"GenvertexY"<<GenvertexY<<"GenvertexZ"<<GenvertexZ<<endl;

      double GenDiMuonYP=mom->rapidity();
      double GenDiMuonMinvP=mom->mass();
      double GenDiMuonPtP =mom->pt();
      double GenDiMuonPxP=mom->px();
      double GenDiMuonPyP=mom->py();
      double GenDiMuonPzP=mom->pz();



      GenJpsiMassP=GenDiMuonMinvP;
      GenJpsiPtP=GenDiMuonPtP;
      GenJpsiRapP=GenDiMuonYP;
      GenJpsiPxP=GenDiMuonPxP;
      GenJpsiPyP=GenDiMuonPyP;
      GenJpsiPzP=GenDiMuonPzP;
      
      //cout<<" mass P "<<GenDiMuonMinvP<<" pT P : "<<GenDiMuonPtP<<endl;

      if(part.pdgId() == -13 ){
        px1[nplus] = part.px();
	py1[nplus] = part.py();
        pz1[nplus] = part.pz();
	peta1[nplus] =part.eta();
	pphi1[nplus] =part.phi();
	nplus++;
	
	      
	//cout<<"motherID "<<MID<<endl;


      }
      
      if(part.pdgId()== 13) {
        px2[nminus] = part.px();
        py2[nminus] = part.py();
        pz2[nminus] = part.pz();
	peta2[nminus] =part.eta();
	pphi2[nminus] =part.phi();
	
      	nminus++;
      }
    }
  }
  
  //cout<<" nplus : "<<nplus<<"  "<<nminus<<endl;
  
  for(size_t i = 0; i < nplus; i++) {
    double en1 = sqrt(px1[i]*px1[i] + py1[i]*py1[i] + pz1[i]*pz1[i] + mumass*mumass);
        
    for(size_t j = 0; j< nminus; j++) {
      double en2 = sqrt(px2[j]*px2[j] + py2[j]*py2[j] + pz2[j]*pz2[j] + mumass*mumass);
      TLorentzVector  genvector1,genvector2,genvector3;
    
      GenmuPosPx=px1[i];
      GenmuPosPy=py1[i];
      GenmuPosPz=pz1[i];
      GenmuPosEta=peta1[i];
      GenmuPosPhi=pphi1[i];
            
      GenmuNegPx=px2[j];
      GenmuNegPy=py2[j];
      GenmuNegPz=pz2[j];
      GenmuNegEta=peta2[j];
      GenmuNegPhi=pphi2[j];
      
      genvector1.SetPxPyPzE(px1[i], py1[i], pz1[i], en1);
      genvector2.SetPxPyPzE(px2[j], py2[j], pz2[j], en2);
      
      genvector3=genvector1+genvector2;
      
      double GenDiMuonY=genvector3.Rapidity();
      double GenDiMuonMinv=genvector3.M();
      double GenDiMuonPt =genvector3.Pt();
      double GenDiMuonPx=genvector3.Px();
      double GenDiMuonPy=genvector3.Py();
      double GenDiMuonPz=genvector3.Pz();

      //cout<<" gen mass "<< GenDiMuonMinv   <<" pT "<< GenDiMuonPt<<endl; 
      GenJpsiMass=GenDiMuonMinv;
      GenJpsiPt=GenDiMuonPt;
      GenJpsiRap=GenDiMuonY;
      GenJpsiPx=GenDiMuonPx;
      GenJpsiPy=GenDiMuonPy;
      GenJpsiPz=GenDiMuonPz;
     
      SingleGenMuonTree->Fill();
      //cout<<"gen Tree Filled "<<endl;

    }
  }

}

//----------------------------------------- HepMC -----------------------------------------------------------------------------
/*
  using namespace HepMC;
  using namespace edm;
  edm::Handle<HepMCProduct> evt;
  iEvent.getByLabel("generator", evt);
  const HepMC::GenEvent *genEvent = evt->GetEvent();

  float pt1[100], eta1[100], phi1[100], ppx1[100], ppy1[100], ppz1[100],ener1[100];
  float pt2[100], eta2[100], phi2[100],ppx2[100], ppy2[100], ppz2[100],ener2[100];
  int imuplus=0, imuminus=0;
  int nmp=0;
  int nmn=0;
  const float muonmass = 0.105658369; //  GeV                                                                                                                                                    
  for (HepMC::GenEvent::particle_const_iterator p = genEvent->particles_begin();
  p != genEvent->particles_end(); ++p) {
  
  int ID     = (*p)->pdg_id();
  float eta = (*p)->momentum().eta();
  float phi = (*p)->momentum().phi();
  float pt = (*p)->momentum().perp();
  float px = (*p)->momentum().px();
  float py = (*p)->momentum().py();
  float pz = (*p)->momentum().pz();
  float ener=sqrt((px*px+py*py+pz*pz+muonmass*muonmass));
  
  if(ID==-13) {
  pt1[imuplus] = pt;
  eta1[imuplus] = eta;
  phi1[imuplus] = phi;
  ppx1[imuplus]=px;
  ppy1[imuplus]=py;
  ppz1[imuplus]=pz;
  ener1[imuplus]=ener;
  
  GenmuPosPx=px;
  GenmuPosPy=py;
  GenmuPosPz=pz;
  GenmuPosEta=eta;
  
  imuplus++;
  nmp++;
  }
  
  
  if(ID==13) {
  pt2[imuminus] = pt;
  eta2[imuminus] = eta;
  phi2[imuminus] = phi;
  ppx2[imuminus]=px;
  ppy2[imuminus]=py;
  ppz2[imuminus]=pz;
  ener2[imuminus]=ener;
  
  GenmuNegPx=px;
  GenmuNegPy=py;
  GenmuNegPz=pz;
  GenmuNegEta=eta;
  imuminus++;
  nmn++;
  }
  
  }//genrated particle loop 

  if(nmp>0 && nmn>0){
  // fill muon properties and do 2-mu inv.mass                                                                                                                                                 
  for (  int i=0; i< imuplus; i++ ) {
  for (  int j=0; j< imuminus; j++ ){
  double eptot=ener1[i]+ener2[j];
  double ppxtot=ppx1[i]+ppx2[j];
  double ppytot=ppy1[i]+ppy2[j];
  double ppztot=ppz1[i]+ppz2[j];
  double genupt=sqrt((ppxtot*ppxtot)+(ppytot*ppytot));
  double genuy=0.5*(log((eptot+ppztot)/(eptot-ppztot)));
  double minv = TMath::Sqrt(eptot*eptot - (ppxtot*ppxtot + ppytot*ppytot + ppztot*ppztot) );	
  
  GenJpsiMass=minv;
  GenJpsiPt=genupt;
  GenJpsiRap=genuy;
  GenJpsiPx=ppxtot;
  GenJpsiPy=ppytot;
  GenJpsiPz=ppztot;
  } // for j				
  } // for i				      
  
      SingleGenMuonTree->Fill();
      cout<<" Gen loop "<<endl;
      
  }// if      
  */

void DiMuonOnia2DPlots::FillEvTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int evTrigger[20]={0};
  
  using namespace edm;
  using namespace std;
  using namespace reco;
  
  eventNbEv= 0 ;
  runNbEv= 0 ;
  lumiBlockEv= 0 ;
  
  GenParNo=0;
  GenParSize=0;
  
  int NSG=0, NSG_no=0;
  
  //VrtxProb=-999;
  vertexNbEv=-999;
  vertexTrkEv=-999;
  isVtxFakeEv=-999;
  vertexXEv=-999;
  vertexYEv=-999;
  vertexZEv=-999;
  vertexRhoEv=-999;
  
  nPVEv = -999 ;
  
  GenvertexXEv=-9999;
  GenvertexYEv=-9999;
  GenvertexZEv=-9999;
  
  GlobalMuSize=0;
  SingleMuSize=0;
  GlobalMuNo=0;
  
  math::XYZPoint RefVtx1;
  // Event related infos			
    
    eventNbEv= iEvent.id().event();
    runNbEv=iEvent.id().run();
    lumiBlockEv= iEvent.luminosityBlock();
    
    if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
      centrality_ = new CentralityProvider(iSetup);
      centrality_->newEvent(iEvent,iSetup);
      rbinEv = centrality_->getBin();
    }
    else{rbinEv=-99999;}

    //rbinEv=-99999;

    //==================================== Gen Stuff ==================================//                                                                                                      
    //int genParId =0;                                                                         
    
    edm::Handle<edm::View<reco::GenParticle> >genPar;
    iEvent.getByLabel("hiGenParticles",genPar) ;
    if(!(genPar.isValid())) return;
    edm::View<reco::GenParticle> genParticles = *genPar ;
    
    GenParNo=genParticles.size();
  
    for(size_t i = 0; i < genParticles.size(); ++ i)
      {
	const reco::GenParticle& part = (*genPar)[i];
	const  Candidate *mom1 = part.mother();
	if (part.numberOfMothers()!=1) continue;
	int momId1 = mom1->pdgId();	


	//if(part.pdgId()== 443 || part.pdgId()== 553 || part.pdgId()== 100553 || part.pdgId()== 23 || TMath::Abs(part.pdgId())== 13)
	

	if( (momId1 == 443 && TMath::Abs(part.pdgId())== 13) || (part.pdgId()==443) ) 
	  {
	    
	    GenvertexXEv=part.vx();
	    GenvertexYEv=part.vy();
	    GenvertexZEv=part.vz();
	
	    //cout<<part.pdgId()<<" id : status "<<part.status()<<endl;
	    //cout<<GenvertexXEv<<" gen vertex  "<<GenvertexYEv<<"  "<<GenvertexZEv<<endl;                                                                                                    
	    GenParPx[GenParSize]=part.px();
	    GenParPy[GenParSize]=part.py();
	    GenParPz[GenParSize]=part.pz();
	    GenParPt[GenParSize]=part.pt();
	    
	    GenParEta[GenParSize]=part.eta();
	    GenParPhi[GenParSize]=part.phi();
	    GenParRap[GenParSize]=part.rapidity();
	    GenParCharge[GenParSize]=part.charge();
	    GenParId[GenParSize]=part.pdgId();
	    GenParStatus[GenParSize]=part.status();
	    
	    const  Candidate *mom = part.mother();
	    
	    //cout<<"  part.numberOfMothers()  "<<part.numberOfMothers()<<endl;     
	    
	    double momId ;double momstatus;
	    if (part.numberOfMothers()==1) {momId = mom->pdgId(); momstatus= mom->status();}
	    else{momId=-99999;momstatus=-99999;}
	    
	    GenParMomId[GenParSize]=momId ;
	    GenParMomStatus[GenParSize]=momstatus;
	    
	    //cout<<" id "<<part.pdgId()<<" status "<<part.status()<<endl;
	    //cout<<" mom id "<<momId<<" mom status "<<momstatus<<endl;
	  
	    GenParSize++;
	  }
      }

    //===================================== Reco Stuff ==================================//
    
    Handle<reco::VertexCollection>privtxsEv;
    iEvent.getByLabel("hiSelectedVertex",privtxsEv);
    VertexCollection::const_iterator privtxEv;
    nPVEv = privtxsEv->size();
    vertexNbEv = privtxsEv->size();
    
    edm::ESHandle<TransientTrackBuilder> theTTBuilder;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theTTBuilder);                                                                                                 
    KalmanVertexFitter vtxFitter;   
    //cout<<" nPVEv "<<nPVEv<<endl;                                                                                                                                                           
    if(nPVEv >0) {
      if (privtxsEv->begin() != privtxsEv->end() ) {
	privtxEv=privtxsEv->begin();
	vertexTrkEv=privtxEv->tracksSize();
	vertexXEv=privtxEv->x();
	vertexYEv=privtxEv->y();
	vertexZEv=privtxEv->z();
	vertexRhoEv=privtxEv->position().rho();
	RefVtx1 = privtxEv->position();
	if(privtxEv->isFake()){isVtxFakeEv=1;}else{isVtxFakeEv=0;}
	
	//cout<<"Ev vtx trk: "<<vertexTrkEv<<" is fake "<<privtxEv->isFake()<<" is ff "<<isVtxFakeEv<<endl;  
	//cout<<"Ev vtx x: "<<vertexXEv<<" vtxy "<<vertexYEv<<" z "<<vertexZEv<<endl;                                                                                                         
      }
    }
    else {RefVtx1.SetXYZ(0.,0.,0.);}
     
    //========================================== Trigger stuff==============================================================// \
      
      if(evTrigger[0]==1){eventTrigger0=1;}else{eventTrigger0=0;}
      if(evTrigger[1]==1){eventTrigger1=1;}else{eventTrigger1=0;}
      if(evTrigger[2]==1){eventTrigger2=1;}else{eventTrigger2=0;}
      if(evTrigger[3]==1){eventTrigger3=1;}else{eventTrigger3=0;}
      if(evTrigger[4]==1){eventTrigger4=1;}else{eventTrigger4=0;}
      if(evTrigger[5]==1){eventTrigger5=1;}else{eventTrigger5=0;}
      if(evTrigger[6]==1){eventTrigger6=1;}else{eventTrigger6=0;}
      if(evTrigger[7]==1){eventTrigger7=1;}else{eventTrigger7=0;}
      if(evTrigger[8]==1){eventTrigger8=1;}else{eventTrigger8=0;}
      if(evTrigger[9]==1){eventTrigger9=1;}else{eventTrigger9=0;}
      if(evTrigger[10]==1){eventTrigger10=1;}else{eventTrigger10=0;}
      //===============================  All Muons  =============================================//                                                                                             
      double massMu=0.105658;
      int nGlobalMuon=0;
     
      edm::Handle<edm::View<pat::Muon> > GMuons;
      iEvent.getByLabel("patMuonsWithTrigger", GMuons);
      
      if(GMuons.isValid()){
	
	edm::View<pat::Muon>GMuonColl= *GMuons;
	int GMuonsize =GMuonColl.size();
	
	GlobalMuNo=GMuonsize;
	nGlobalMuon=GMuonsize;
	
	if(nGlobalMuon < 1000)
	  {  
	    
	    for ( int i=0 ; i < GMuonsize ; ++i ){
	      const pat::Muon& gmuon1 = (*GMuons)[i];
	      
	      muonCounts++;
	      //cout<<"  GmuonSize "<<GMuonsize<<endl;
	      //cout<<"  muonCounts "<<muonCounts<<endl;
	      
	      //cout<<" GlobalMuSize  "<<GlobalMuSize<<endl;
	      
	      bool TrkSta=0,TrkGlb=0;
	      if(gmuon1.isTrackerMuon())
		{
		  
		  //cout<<"-----------------------------------------------"<<endl;
		  if(gmuon1.isStandAloneMuon()){TrkSta=1;}
		  //cout<<"  TrkSta "<<TrkSta<<endl;
		  
		  if(gmuon1.isGlobalMuon()){TrkGlb=1;}
		  //cout<<"  TrkGlb "<<TrkGlb<<endl;
		  
		  SingleMuPx[SingleMuSize][0]=gmuon1.innerTrack()->px();
		  SingleMuPy[SingleMuSize][0]=gmuon1.innerTrack()->py();
		  SingleMuPz[SingleMuSize][0]=gmuon1.innerTrack()->pz();
		  SingleMuPt[SingleMuSize][0]=gmuon1.innerTrack()->pt();
		  SingleMuEta[SingleMuSize][0]=gmuon1.innerTrack()->eta();
		  SingleMuPhi[SingleMuSize][0]=gmuon1.innerTrack()->phi();
		  SingleMuCharge[SingleMuSize][0]=gmuon1.charge();
		}
	      else{
		SingleMuPx[SingleMuSize][0]=-99999;
		SingleMuPy[SingleMuSize][0]=-99999;
		SingleMuPz[SingleMuSize][0]=-99999;
		SingleMuPt[SingleMuSize][0]=-99999;
		SingleMuEta[SingleMuSize][0]=-99999;
		SingleMuPhi[SingleMuSize][0]=-99999;
		SingleMuCharge[SingleMuSize][0]= -99999;
	      } 
	      
	      bool Strk=0,SGlb=0;
	      if(gmuon1.isTrackerMuon()){Strk=1;}
	      //cout<<"  STAtrk "<<Strk<<endl;
	      if(gmuon1.isGlobalMuon()){SGlb=1;}
	      //cout<<"  STAglb "<<SGlb<<endl;
	      if(gmuon1.isStandAloneMuon() && Strk==1 && SGlb==1){Sum_Y++;}
	      if(gmuon1.isStandAloneMuon() && Strk==1 && SGlb!=1){Sum_No++;}
	      
	      //cout<<"  Sum_Y  "<<Sum_Y<<"  Sum_No  "<<Sum_No<<endl;
	      
	      if(gmuon1.isStandAloneMuon())
		{
		  SingleMuPx[SingleMuSize][1]=gmuon1.outerTrack()->px();
		  SingleMuPy[SingleMuSize][1]=gmuon1.outerTrack()->py();
		  SingleMuPz[SingleMuSize][1]=gmuon1.outerTrack()->pz();
		  SingleMuPt[SingleMuSize][1]=gmuon1.outerTrack()->pt();
		  SingleMuEta[SingleMuSize][1]=gmuon1.outerTrack()->eta();
		  SingleMuPhi[SingleMuSize][1]=gmuon1.outerTrack()->phi();
		  SingleMuCharge[SingleMuSize][1]=gmuon1.charge();
		}
	      else{
		SingleMuPx[SingleMuSize][1]=-99999;
		SingleMuPy[SingleMuSize][1]=-99999;
		SingleMuPz[SingleMuSize][1]=-99999;
		SingleMuPt[SingleMuSize][1]=-99999;
		SingleMuEta[SingleMuSize][1]=-99999;
		SingleMuPhi[SingleMuSize][1]=-99999;
		SingleMuCharge[SingleMuSize][1]= -99999;
	      }
	      
	      bool Gtrk=0,Gsta=0;
	      if(gmuon1.isGlobalMuon())
		{
		  if(gmuon1.isTrackerMuon()){Gtrk=1;}
		  //cout<<"  Glbtrk "<<Gtrk<<endl;
		  
		  if(gmuon1.isStandAloneMuon()){Gsta=1;}
		  //cout<<"  Glbsta "<<Gsta<<endl;
		  
		  
		  SingleMuPx[SingleMuSize][2]=gmuon1.px();
		  SingleMuPy[SingleMuSize][2]=gmuon1.py();
		  SingleMuPz[SingleMuSize][2]=gmuon1.pz();
		  SingleMuPt[SingleMuSize][2]=gmuon1.pt();
		  SingleMuEta[SingleMuSize][2]=gmuon1.eta();
		  SingleMuPhi[SingleMuSize][2]=gmuon1.phi();
		  SingleMuCharge[SingleMuSize][2]=gmuon1.charge();
		}
	      
	      else{
		SingleMuPx[SingleMuSize][2]=-99999;
		SingleMuPy[SingleMuSize][2]=-99999;
		SingleMuPz[SingleMuSize][2]=-99999;
		SingleMuPt[SingleMuSize][2]=-99999;
		SingleMuEta[SingleMuSize][2]=-99999;
		SingleMuPhi[SingleMuSize][2]=-99999;
		SingleMuCharge[SingleMuSize][2]=-99999;
	      } 
	      
	      double globalmu_nchi2In=-9999;double globalmu_dxy=-9999;double globalmu_dz=-9999;double globalmu_found=-9999;
	      double globalmu_pixelLayers=-9999;double globalmu_arbitrated =-9999;double globalmu_nValidMuHits=-9999;
	      double globalmu_nchi2Gl=-9999;
	      int globalmu_global=-9999;int globalmu_tracker=-9999; int globalmu_sta=-9999,globalmu_sta_noglb=-9999;
	      
	      if(gmuon1.isGlobalMuon()){globalmu_global=1;}else{globalmu_global=0;}
	      if(gmuon1.isTrackerMuon()){globalmu_tracker=1;}else{globalmu_tracker=0;}
	      if(gmuon1.isStandAloneMuon()){globalmu_sta=1;}else{globalmu_sta=0;}
	      if((!gmuon1.isGlobalMuon()) && gmuon1.isStandAloneMuon()){globalmu_sta_noglb=1;}else{globalmu_sta_noglb=0;}
	      
	      SingleMu_global[SingleMuSize]=globalmu_global;
	      SingleMu_tracker[SingleMuSize]=globalmu_tracker;
	      SingleMu_sta[SingleMuSize]=globalmu_sta;
	      SingleMu_sta_noglb[SingleMuSize]=globalmu_sta_noglb;
	      
	      if(gmuon1.isTrackerMuon())
		{
		  TrackRef tTrack =gmuon1.innerTrack();
		  const reco::HitPattern& tp=tTrack->hitPattern();
		  globalmu_nchi2In  =tTrack->chi2()/tTrack->ndof();
		  //cout<<RefVtx1.x()<< " y "<<RefVtx1.y()<<" z "<<RefVtx1.z()<<endl;                                                                                                           
		  globalmu_dxy      =tTrack->dxy(RefVtx1);
		  globalmu_dz       =tTrack->dz(RefVtx1);
		  globalmu_found       =tTrack->found();
		  globalmu_pixelLayers =tp.pixelLayersWithMeasurement();
		  globalmu_arbitrated  =gmuon1.muonID("TrackerMuonArbitrated");
		  
		  SingleMu_nchi2In[SingleMuSize]=globalmu_nchi2In;
		  SingleMu_dxy[SingleMuSize]=globalmu_dxy;
		  SingleMu_dz[SingleMuSize]=globalmu_dz;
		  SingleMu_found[SingleMuSize]=globalmu_found;
		  SingleMu_pixeLayers[SingleMuSize]=globalmu_pixelLayers;
		  SingleMu_arbitrated[SingleMuSize]=globalmu_arbitrated;
		  
		}
	      else{
		SingleMu_nchi2In[SingleMuSize]=-9999;
		SingleMu_dxy[SingleMuSize]=-9999;
		SingleMu_dz[SingleMuSize]=-9999;
		SingleMu_found[SingleMuSize]=-9999;
		SingleMu_pixeLayers[SingleMuSize]=-9999;
		SingleMu_arbitrated[SingleMuSize]=-9999;
	      }
	      
	      if(gmuon1.isGlobalMuon())
		{
		  TrackRef gTrack =gmuon1.globalTrack();
		  const reco::HitPattern& q1=gTrack->hitPattern();
		  globalmu_nValidMuHits=q1.numberOfValidMuonHits();
		  globalmu_nchi2Gl=gTrack->chi2()/gTrack->ndof();
		  
		  SingleMu_nchi2Gl[SingleMuSize]=globalmu_nchi2Gl;
		  SingleMu_nValidMuHits[SingleMuSize]=globalmu_nValidMuHits;
		}
	      
	      else{
		SingleMu_nchi2Gl[SingleMuSize]=-9999;
		SingleMu_nValidMuHits[SingleMuSize]=-9999;
	      }
	      
	      
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen_v1").empty()){SingleMu_trigger1[SingleMuSize]=1;}else{SingleMu_trigger1[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()){SingleMu_trigger2[SingleMuSize]=1;}else{SingleMu_trigger2[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu3_v1").empty()){SingleMu_trigger3[SingleMuSize]=1;}else{SingleMu_trigger3[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu3_NHitQ_v1").empty()){SingleMu_trigger4[SingleMuSize]=1;}else{SingleMu_trigger4[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu7_v1").empty()){SingleMu_trigger5[SingleMuSize]=1;}else{SingleMu_trigger5[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu15_v1").empty()){SingleMu_trigger6[SingleMuSize]=1;}else{SingleMu_trigger6[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_v1").empty()){SingleMu_trigger7[SingleMuSize]=1;}else{SingleMu_trigger7[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_NHitQ_v1").empty()){SingleMu_trigger8[SingleMuSize]=1;}else{SingleMu_trigger8[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_L1HighQL2NHitQ_v1").empty()){SingleMu_trigger9[SingleMuSize]=1;}else{SingleMu_trigger9[SingleMuSize]=0;}
	      if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu3_v1").empty()){SingleMu_trigger10[SingleMuSize]=1;}else{SingleMu_trigger10[SingleMuSize]=0;}
	      
	      //cout<<" SingleMuSize  "<<SingleMuSize<<endl;
	      SingleMuSize++;
	      
	      for ( int j=i+1 ; j < GMuonsize ; ++j ){
		const pat::Muon& gmuon2 = (*GMuons)[j];
		
		//cout<<"  GMuonsize  "<<GMuonsize<<" j  "<<j<<endl;
		//cout<<" GlobalMuSize mu2 "<<GlobalMuSize<<endl;
		
		TLorentzVector dimu_out;
		

		if(gmuon1.isStandAloneMuon() && gmuon2.isStandAloneMuon()){
		  
		  TVector3 vmu1_out(gmuon1.outerTrack()->px(),gmuon1.outerTrack()->py(),gmuon1.outerTrack()->pz());
		  Double_t mu1_Eout =sqrt( (vmu1_out.Mag()*vmu1_out.Mag() ) + (massMu*massMu) );
		  TLorentzVector lvmu1_out(vmu1_out,mu1_Eout);
		  
		  TVector3 vmu2_out(gmuon2.outerTrack()->px(),gmuon2.outerTrack()->py(),gmuon2.outerTrack()->pz());
		  Double_t mu2_Eout =sqrt( (vmu2_out.Mag()*vmu2_out.Mag() ) + (massMu*massMu) );
		  TLorentzVector lvmu2_out(vmu2_out,mu2_Eout);
		  dimu_out = lvmu1_out+lvmu2_out;
		}
		
		
		TVector3 vmu1_in(gmuon1.px(),gmuon1.py(),gmuon1.pz());
		Double_t mu1_Ein =sqrt( (vmu1_in.Mag()*vmu1_in.Mag() ) + (massMu*massMu) );
		TLorentzVector lvmu1_in(vmu1_in,mu1_Ein);
		
		TVector3 vmu2_in(gmuon2.px(),gmuon2.py(),gmuon2.pz());
		Double_t mu2_Ein =sqrt( (vmu2_in.Mag()*vmu2_in.Mag() ) + (massMu*massMu) );
		TLorentzVector lvmu2_in(vmu2_in,mu2_Ein);
		
		TLorentzVector dimu_in = lvmu1_in + lvmu2_in;
		
		Int_t  DiMuChrage =gmuon1.charge() + gmuon2.charge();
		
		

		GlobaldiMuPx[GlobalMuSize][0]=dimu_in.Px();
		GlobaldiMuPy[GlobalMuSize][0]=dimu_in.Py();
		GlobaldiMuPz[GlobalMuSize][0]=dimu_in.Pz();
		GlobaldiMuPt[GlobalMuSize][0]=dimu_in.Pt();
		GlobaldiMuRap[GlobalMuSize][0]=dimu_in.Rapidity();
		GlobaldiMuPhi[GlobalMuSize][0]=dimu_in.Phi();
		GlobaldiMuCharge[GlobalMuSize][0]=DiMuChrage;
		GlobaldiMuMass[GlobalMuSize][0]=dimu_in.M();
		
		

		if(gmuon1.isStandAloneMuon() && gmuon2.isStandAloneMuon()){
		  GlobaldiMuPx[GlobalMuSize][1]=dimu_out.Px();
		  GlobaldiMuPy[GlobalMuSize][1]=dimu_out.Py();
		  GlobaldiMuPz[GlobalMuSize][1]=dimu_out.Pz();
		  GlobaldiMuPt[GlobalMuSize][1]=dimu_out.Pt();
		  GlobaldiMuRap[GlobalMuSize][1]=dimu_out.Rapidity();
		  GlobaldiMuPhi[GlobalMuSize][1]=dimu_out.Phi();
		  GlobaldiMuCharge[GlobalMuSize][1]=DiMuChrage;
		  GlobaldiMuMass[GlobalMuSize][1]=dimu_out.M();
		}		
		else{
		  GlobaldiMuPx[GlobalMuSize][1]=-99999;
		  GlobaldiMuPy[GlobalMuSize][1]=-99999;
		  GlobaldiMuPz[GlobalMuSize][1]=-99999;
		  GlobaldiMuPt[GlobalMuSize][1]=-99999;
		  GlobaldiMuRap[GlobalMuSize][1]=-99999;
		  GlobaldiMuPhi[GlobalMuSize][1]=-99999;
		  GlobaldiMuCharge[GlobalMuSize][1]=-99999;
		  GlobaldiMuMass[GlobalMuSize][1]=-99999;
		  
		}


		
		if(gmuon1.isStandAloneMuon()){
		  
		  GlobalMuPx[GlobalMuSize][0]=gmuon1.outerTrack()->px();
		  GlobalMuPy[GlobalMuSize][0]=gmuon1.outerTrack()->py();
		  GlobalMuPz[GlobalMuSize][0]=gmuon1.outerTrack()->pz();
		  GlobalMuPt[GlobalMuSize][0]=gmuon1.outerTrack()->pt();
		  GlobalMuEta[GlobalMuSize][0]=gmuon1.outerTrack()->eta();
		  GlobalMuPhi[GlobalMuSize][0]=gmuon1.outerTrack()->phi();
		  GlobalMuCharge[GlobalMuSize][0]=gmuon1.charge();
		}
		else{
		  GlobalMuPx[GlobalMuSize][0]=-99999;
		  GlobalMuPy[GlobalMuSize][0]=-99999;
		  GlobalMuPz[GlobalMuSize][0]=-99999;
		  GlobalMuPt[GlobalMuSize][0]=-99999;
		  GlobalMuEta[GlobalMuSize][0]=-99999;
		  GlobalMuPhi[GlobalMuSize][0]=-99999;
		  GlobalMuCharge[GlobalMuSize][0]=-99999;
		} 
		
		if(gmuon1.isGlobalMuon())
		  {
		    GlobalMuPx[GlobalMuSize][1]=gmuon1.px();
		    GlobalMuPy[GlobalMuSize][1]=gmuon1.py();
		    GlobalMuPz[GlobalMuSize][1]=gmuon1.pz();
		    GlobalMuPt[GlobalMuSize][1]=gmuon1.pt();
		    GlobalMuEta[GlobalMuSize][1]=gmuon1.eta();
		    GlobalMuPhi[GlobalMuSize][1]=gmuon1.phi();
		    GlobalMuCharge[GlobalMuSize][1]=gmuon1.charge();
		  }
		else{
		  GlobalMuPx[GlobalMuSize][1]=-99999;
		  GlobalMuPy[GlobalMuSize][1]=-99999;
		  GlobalMuPz[GlobalMuSize][1]=-99999;
		  GlobalMuPt[GlobalMuSize][1]=-99999;
		  GlobalMuEta[GlobalMuSize][1]=-99999;
		  GlobalMuPhi[GlobalMuSize][1]=-99999;
		  GlobalMuCharge[GlobalMuSize][1]=-99999;
		} 
		
		if(gmuon1.isTrackerMuon())
		  {
		    GlobalMuPx[GlobalMuSize][2]=gmuon1.innerTrack()->px();
		    GlobalMuPy[GlobalMuSize][2]=gmuon1.innerTrack()->py();
		    GlobalMuPz[GlobalMuSize][2]=gmuon1.innerTrack()->pz();
		    GlobalMuPt[GlobalMuSize][2]=gmuon1.innerTrack()->pt();
		    GlobalMuEta[GlobalMuSize][2]=gmuon1.innerTrack()->eta();
		    GlobalMuPhi[GlobalMuSize][2]=gmuon1.innerTrack()->phi();
		    GlobalMuCharge[GlobalMuSize][2]=gmuon1.charge();
		  }
		
		else{
		  GlobalMuPx[GlobalMuSize][2]=-99999;
		  GlobalMuPy[GlobalMuSize][2]=-99999;
		  GlobalMuPz[GlobalMuSize][2]=-99999;
		  GlobalMuPt[GlobalMuSize][2]=-99999;
		  GlobalMuEta[GlobalMuSize][2]=-99999;
		  GlobalMuPhi[GlobalMuSize][2]=-99999;
		  GlobalMuCharge[GlobalMuSize][2]=-99999;
		} 
		
		if(gmuon2.isStandAloneMuon()){
		  
		  GlobalMu2Px[GlobalMuSize][0]=gmuon2.outerTrack()->px();
		  GlobalMu2Py[GlobalMuSize][0]=gmuon2.outerTrack()->py();
		  GlobalMu2Pz[GlobalMuSize][0]=gmuon2.outerTrack()->pz();
		  GlobalMu2Pt[GlobalMuSize][0]=gmuon2.outerTrack()->pt();
		  GlobalMu2Eta[GlobalMuSize][0]=gmuon2.outerTrack()->eta();
		  GlobalMu2Phi[GlobalMuSize][0]=gmuon2.outerTrack()->phi();
		  GlobalMu2Charge[GlobalMuSize][0]=gmuon2.charge();
		  
		}
		else{
		  GlobalMu2Px[GlobalMuSize][0]=-99999;
		  GlobalMu2Py[GlobalMuSize][0]=-99999;
		  GlobalMu2Pz[GlobalMuSize][0]=-99999;
		  GlobalMu2Pt[GlobalMuSize][0]=-99999;
		  GlobalMu2Eta[GlobalMuSize][0]=-99999;
		  GlobalMu2Phi[GlobalMuSize][0]=-99999;
		  GlobalMu2Charge[GlobalMuSize][0]=-99999;
		} 
		
		if(gmuon2.isGlobalMuon())
		  {
		    GlobalMu2Px[GlobalMuSize][1]=gmuon2.px();
		    GlobalMu2Py[GlobalMuSize][1]=gmuon2.py();
		    GlobalMu2Pz[GlobalMuSize][1]=gmuon2.pz();
		    GlobalMu2Pt[GlobalMuSize][1]=gmuon2.pt();
		    GlobalMu2Eta[GlobalMuSize][1]=gmuon2.eta();
		    GlobalMu2Phi[GlobalMuSize][1]=gmuon2.phi();
		    GlobalMu2Charge[GlobalMuSize][1]=gmuon2.charge();
		  }
		else{
		  GlobalMu2Px[GlobalMuSize][1]=-99999;
		  GlobalMu2Py[GlobalMuSize][1]=-99999;
		  GlobalMu2Pz[GlobalMuSize][1]=-99999;
		  GlobalMu2Pt[GlobalMuSize][1]=-99999;
		  GlobalMu2Eta[GlobalMuSize][1]=-99999;
		  GlobalMu2Phi[GlobalMuSize][1]=-99999;
		  GlobalMu2Charge[GlobalMuSize][1]=-99999;
		} 
		
		if(gmuon2.isTrackerMuon())
		  {
		    GlobalMu2Px[GlobalMuSize][2]=gmuon2.innerTrack()->px();
		    GlobalMu2Py[GlobalMuSize][2]=gmuon2.innerTrack()->py();
		    GlobalMu2Pz[GlobalMuSize][2]=gmuon2.innerTrack()->pz();
		    GlobalMu2Pt[GlobalMuSize][2]=gmuon2.innerTrack()->pt();
		    GlobalMu2Eta[GlobalMuSize][2]=gmuon2.innerTrack()->eta();
		    GlobalMu2Phi[GlobalMuSize][2]=gmuon2.innerTrack()->phi();
		    GlobalMu2Charge[GlobalMuSize][2]=gmuon2.charge();
		  }
		
		else{
		  GlobalMu2Px[GlobalMuSize][2]=-99999;
		  GlobalMu2Py[GlobalMuSize][2]=-99999;
		  GlobalMu2Pz[GlobalMuSize][2]=-99999;
		  GlobalMu2Pt[GlobalMuSize][2]=-99999;
		  GlobalMu2Eta[GlobalMuSize][2]=-99999;
		  GlobalMu2Phi[GlobalMuSize][2]=-99999;
		  GlobalMu2Charge[GlobalMuSize][2]=-99999;
		} 
		
		if(gmuon1.isGlobalMuon() &&  gmuon2.isGlobalMuon() ){
		  
		  vector<TransientTrack> t_tks;
		  t_tks.push_back(theTTBuilder->build(gmuon1.track()));  // pass the reco::Track, not  the reco::TrackRef (which can be transient)  
		  t_tks.push_back(theTTBuilder->build(gmuon2.track())); // otherwise the vertex will have transient refs inside.
		  TransientVertex myVertex = vtxFitter.vertex(t_tks);
		  if(myVertex.isValid()) {
		    float vChi2 = myVertex.totalChiSquared();
		    float vNDF  = myVertex.degreesOfFreedom();
		    float vProb(TMath::Prob(vChi2,(int)vNDF));      
		    //cout<<"  vProb "<<vProb<<endl;
		    
		    VrtxProb[GlobalMuSize] = vProb;
		  }
		}
		else{VrtxProb[GlobalMuSize]=-99999;}
		
		//muons.innerTrack().pt()
		
		globalmu_nchi2In=-9999; globalmu_dxy=-9999; globalmu_dz=-9999; globalmu_found=-9999;
		globalmu_pixelLayers=-9999; globalmu_arbitrated =-9999; globalmu_nValidMuHits=-9999;
		globalmu_nchi2Gl=-9999;
		globalmu_global=-9999; globalmu_tracker=-9999;  globalmu_sta=-9999,globalmu_sta_noglb=-9999;
		
		
		if(gmuon1.isGlobalMuon()){globalmu_global=1;}else{globalmu_global=0;}
		

		if(gmuon1.isTrackerMuon()){globalmu_tracker=1;}else{globalmu_tracker=0;}
		if(gmuon1.isStandAloneMuon()){globalmu_sta=1;}else{globalmu_sta=0;}
		if((!gmuon1.isGlobalMuon()) && gmuon1.isStandAloneMuon()){globalmu_sta_noglb=1;}else{globalmu_sta_noglb=0;}
	      
		GlobalMu_global[GlobalMuSize][0]=globalmu_global;
		GlobalMu_tracker[GlobalMuSize][0]=globalmu_tracker;
		
		GlobalMu_sta[GlobalMuSize][0]=globalmu_sta;
		GlobalMu_sta_noglb[GlobalMuSize][0]=globalmu_sta_noglb;
		
		//if(gmuon1.isTrackerMuon() && gmuon1.isGlobalMuon())
		if(gmuon1.isTrackerMuon()){
		  TrackRef tTrack =gmuon1.innerTrack();
		  const reco::HitPattern& tp=tTrack->hitPattern();
		  globalmu_nchi2In  =tTrack->chi2()/tTrack->ndof();
		  //cout<<RefVtx1.x()<< " y "<<RefVtx1.y()<<" z "<<RefVtx1.z()<<endl;                                                                                                           
		  globalmu_dxy      =tTrack->dxy(RefVtx1);
		  globalmu_dz       =tTrack->dz(RefVtx1);
		  globalmu_found       =tTrack->found();
		  globalmu_pixelLayers =tp.pixelLayersWithMeasurement();
		  globalmu_arbitrated  =gmuon1.muonID("TrackerMuonArbitrated");
		  
		  GlobalMu_nchi2In[GlobalMuSize][0]=globalmu_nchi2In;
		  GlobalMu_dxy[GlobalMuSize][0]=globalmu_dxy;
		  GlobalMu_dz[GlobalMuSize][0]=globalmu_dz;
		  GlobalMu_found[GlobalMuSize][0]=globalmu_found;
		  GlobalMu_pixeLayers[GlobalMuSize][0]=globalmu_pixelLayers;
		  GlobalMu_arbitrated[GlobalMuSize][0]=globalmu_arbitrated;
		}
		else{
		  GlobalMu_nchi2In[GlobalMuSize][0]=-9999;
		  GlobalMu_dxy[GlobalMuSize][0]=-9999;
		  GlobalMu_dz[GlobalMuSize][0]=-9999;
		  GlobalMu_found[GlobalMuSize][0]=-9999;
		  GlobalMu_pixeLayers[GlobalMuSize][0]=-9999;
		  GlobalMu_arbitrated[GlobalMuSize][0]=-9999;
		}
		
		if(gmuon1.isGlobalMuon()){
		  TrackRef gTrack =gmuon1.globalTrack();
		  const reco::HitPattern& q1=gTrack->hitPattern();
		  globalmu_nValidMuHits=q1.numberOfValidMuonHits();
		  globalmu_nchi2Gl=gTrack->chi2()/gTrack->ndof();
		  
		  GlobalMu_nchi2Gl[GlobalMuSize][0]=globalmu_nchi2Gl;
		  GlobalMu_nValidMuHits[GlobalMuSize][0]=globalmu_nValidMuHits;
		}
		else{
		  GlobalMu_nchi2Gl[GlobalMuSize][0]=-9999;
		  GlobalMu_nValidMuHits[GlobalMuSize][0]=-9999;
		}
		
		// Muon2
		int globalmu2_global=-9999, globalmu2_tracker=-9999, globalmu2_sta=-9999,globalmu2_sta_noglb=-9999;
		
		if(gmuon2.isGlobalMuon()){globalmu2_global=1;}else{globalmu2_global=0;}

		if(gmuon2.isTrackerMuon()){globalmu2_tracker=1;}else{globalmu2_tracker=0;}
		if(gmuon2.isStandAloneMuon()){globalmu2_sta=1;}else{globalmu2_sta=0;}
		if((!gmuon2.isGlobalMuon()) && gmuon2.isStandAloneMuon()){globalmu2_sta_noglb=1;}else{globalmu2_sta_noglb=0;}
		
		GlobalMu_global[GlobalMuSize][1]=globalmu2_global;

		GlobalMu_tracker[GlobalMuSize][1]=globalmu2_tracker;
		GlobalMu_sta[GlobalMuSize][1]=globalmu2_sta;
		GlobalMu_sta_noglb[GlobalMuSize][1]=globalmu2_sta_noglb;
		
		if(gmuon2.isTrackerMuon()){
		  TrackRef tTrack =gmuon2.innerTrack();
		  const reco::HitPattern& tp=tTrack->hitPattern();
		  globalmu_nchi2In  =tTrack->chi2()/tTrack->ndof();
		  //cout<<RefVtx1.x()<< " y "<<RefVtx1.y()<<" z "<<RefVtx1.z()<<endl;                                                                                                           
		  globalmu_dxy      =tTrack->dxy(RefVtx1);
		  globalmu_dz       =tTrack->dz(RefVtx1);
		  globalmu_found       =tTrack->found();
		  globalmu_pixelLayers =tp.pixelLayersWithMeasurement();
		  globalmu_arbitrated  =gmuon2.muonID("TrackerMuonArbitrated");
		  
		  GlobalMu_nchi2In[GlobalMuSize][1]=globalmu_nchi2In;
		  GlobalMu_dxy[GlobalMuSize][1]=globalmu_dxy;
		  GlobalMu_dz[GlobalMuSize][1]=globalmu_dz;
		  GlobalMu_found[GlobalMuSize][1]=globalmu_found;
		  GlobalMu_pixeLayers[GlobalMuSize][1]=globalmu_pixelLayers;
		  GlobalMu_arbitrated[GlobalMuSize][1]=globalmu_arbitrated;
		}
		else{
		  GlobalMu_nchi2In[GlobalMuSize][1]=-9999;
		  GlobalMu_dxy[GlobalMuSize][1]=-9999;
		  GlobalMu_dz[GlobalMuSize][1]=-9999;
		  GlobalMu_found[GlobalMuSize][1]=-9999;
		  GlobalMu_pixeLayers[GlobalMuSize][1]=-9999;
		  GlobalMu_arbitrated[GlobalMuSize][1]=-9999;
		}
		
		if(gmuon2.isGlobalMuon()){
		  TrackRef gTrack =gmuon2.globalTrack();
		  const reco::HitPattern& q1=gTrack->hitPattern();
		  globalmu_nValidMuHits=q1.numberOfValidMuonHits();
		  globalmu_nchi2Gl=gTrack->chi2()/gTrack->ndof();
		  
		  GlobalMu_nchi2Gl[GlobalMuSize][1]=globalmu_nchi2Gl;
		  GlobalMu_nValidMuHits[GlobalMuSize][1]=globalmu_nValidMuHits;
		}
		else{
		  GlobalMu_nchi2Gl[GlobalMuSize][1]=-9999;
		  GlobalMu_nValidMuHits[GlobalMuSize][1]=-9999;
		}
		
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen_v1").empty()){GlobalMu_trigger1[GlobalMuSize][0]=1;}else{GlobalMu_trigger1[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()){GlobalMu_trigger2[GlobalMuSize][0]=1;}else{GlobalMu_trigger2[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu3_v1").empty()){GlobalMu_trigger3[GlobalMuSize][0]=1;}else{GlobalMu_trigger3[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu3_NHitQ_v1").empty()){GlobalMu_trigger4[GlobalMuSize][0]=1;}else{GlobalMu_trigger4[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu7_v1").empty()){GlobalMu_trigger5[GlobalMuSize][0]=1;}else{GlobalMu_trigger5[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2Mu15_v1").empty()){GlobalMu_trigger6[GlobalMuSize][0]=1;}else{GlobalMu_trigger6[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_v1").empty()){GlobalMu_trigger7[GlobalMuSize][0]=1;}else{GlobalMu_trigger7[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_NHitQ_v1").empty()){GlobalMu_trigger8[GlobalMuSize][0]=1;}else{GlobalMu_trigger8[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_L1HighQL2NHitQ_v1").empty()){GlobalMu_trigger9[GlobalMuSize][0]=1;}else{GlobalMu_trigger9[GlobalMuSize][0]=0;}
		if(!gmuon1.triggerObjectMatchesByPath("HLT_HIL2DoubleMu3_v1").empty()){GlobalMu_trigger10[GlobalMuSize][0]=1;}else{GlobalMu_trigger10[GlobalMuSize][0]=0;}
		
		
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL1DoubleMuOpen_v1").empty()){GlobalMu_trigger1[GlobalMuSize][1]=1;}else{GlobalMu_trigger1[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()){GlobalMu_trigger2[GlobalMuSize][1]=1;}else{GlobalMu_trigger2[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2Mu3_v1").empty()){GlobalMu_trigger3[GlobalMuSize][1]=1;}else{GlobalMu_trigger3[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2Mu3_NHitQ_v1").empty()){GlobalMu_trigger4[GlobalMuSize][1]=1;}else{GlobalMu_trigger4[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2Mu7_v1").empty()){GlobalMu_trigger5[GlobalMuSize][1]=1;}else{GlobalMu_trigger5[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2Mu15_v1").empty()){GlobalMu_trigger6[GlobalMuSize][1]=1;}else{GlobalMu_trigger6[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_v1").empty()){GlobalMu_trigger7[GlobalMuSize][1]=1;}else{GlobalMu_trigger7[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_NHitQ_v1").empty()){GlobalMu_trigger8[GlobalMuSize][1]=1;}else{GlobalMu_trigger8[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2DoubleMu0_L1HighQL2NHitQ_v1").empty()){GlobalMu_trigger9[GlobalMuSize][1]=1;}else{GlobalMu_trigger9[GlobalMuSize][1]=0;}
		if(!gmuon2.triggerObjectMatchesByPath("HLT_HIL2DoubleMu3_v1").empty()){GlobalMu_trigger10[GlobalMuSize][1]=1;}else{GlobalMu_trigger10[GlobalMuSize][1]=0;}
		
		GlobalMuSize++;
	      }
	    }
	  }//1000 muons 
      }
}


void DiMuonOnia2DPlots::FillPATTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;

  eventNbPAT= 0 ;
  runNbPAT= 0 ;
  lumiBlockPAT= 0 ;
  rbinPAT=-99999;
  


  PATMuSize=0;

  eventNbPAT= iEvent.id().event();
  runNbPAT=iEvent.id().run();
  lumiBlockPAT= iEvent.luminosityBlock();
  
  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup);
    rbinPAT=centrality_->getBin();
  }  
  else{
    rbinPAT=-9999;
  }
  
  edm::Handle<edm::View<pat::Muon> > GMuons;
  iEvent.getByLabel("patMuons", GMuons);
  
  if(GMuons.isValid()){
    edm::View<pat::Muon>GMuonColl= *GMuons;
    int GMuonsize =GMuonColl.size();
    
    int nGlobalMuon=GMuonColl.size();

    if(nGlobalMuon < 1000)
      {  
	for ( int i=0 ; i < GMuonsize ; ++i ){
	  const pat::Muon& gmuon1 = (*GMuons)[i];
	  
	  PATMuPx[PATMuSize]=gmuon1.px();
	  PATMuPy[PATMuSize]=gmuon1.py();
	  PATMuPz[PATMuSize]=gmuon1.pz();
	  PATMuPt[PATMuSize]=gmuon1.pt();
	  PATMuEta[PATMuSize]=gmuon1.eta();
	  PATMuPhi[PATMuSize]=gmuon1.phi();
	  PATMuCharge[PATMuSize]=gmuon1.charge();
		

	  int globalmu_global=-9999;int globalmu_tracker=-9999; int globalmu_sta=-9999,globalmu_sta_noglb=-9999;
	  if(gmuon1.isGlobalMuon()){globalmu_global=1;}else{globalmu_global=0;}
	  if(gmuon1.isTrackerMuon()){globalmu_tracker=1;}else{globalmu_tracker=0;}
	  if(gmuon1.isStandAloneMuon()){globalmu_sta=1;}else{globalmu_sta=0;}
	  if((!gmuon1.isGlobalMuon()) && gmuon1.isStandAloneMuon()){globalmu_sta_noglb=1;}else{globalmu_sta_noglb=0;}
	      
	  PATMu_global[PATMuSize]=globalmu_global;
	  PATMu_tracker[PATMuSize]=globalmu_tracker;
	  PATMu_sta[PATMuSize]=globalmu_sta;
	  PATMu_sta_noglb[PATMuSize]=globalmu_sta_noglb;
	  PATMuSize++;
	}//muon loop
      }//gmuon <1000
  }//is valid


}





void DiMuonOnia2DPlots::FillRecoTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  
  cout<<" reco tree "<<endl;
  using namespace edm;
  using namespace std;
  using namespace reco;

  eventNbReco= 0 ;
  runNbReco= 0 ;
  lumiBlockReco= 0 ;
  rbinReco=-99999;


  RecoMuSize=0;

  eventNbReco= iEvent.id().event();
  runNbReco=iEvent.id().run();
  lumiBlockReco= iEvent.luminosityBlock();
  
  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup);
    rbinReco=centrality_->getBin();
  }
  else{
    rbinReco=-9999;
  }
    //to see

  edm::Handle<edm::View<reco::Muon> > GMuons;
  iEvent.getByLabel("muons", GMuons);
  

  if(GMuons.isValid()){
    //cout<<" inside is valid "<<endl;

    edm::View<reco::Muon>GMuonColl= *GMuons;
    
    int GMuonsize =GMuonColl.size();
    int nGlobalMuon=GMuonColl.size();


    //cout<<"n Muon "<<nGlobalMuon<<endl;

    if(nGlobalMuon < 1000)
      {  
	for ( int i=0 ; i < GMuonsize ; ++i ){
	  const reco::Muon& gmuon1 = (*GMuons)[i];
	  
	  RecoMuPx[RecoMuSize]=gmuon1.px();
	  RecoMuPy[RecoMuSize]=gmuon1.py();
	  RecoMuPz[RecoMuSize]=gmuon1.pz();
	  RecoMuPt[RecoMuSize]=gmuon1.pt();
	  RecoMuEta[RecoMuSize]=gmuon1.eta();
	  RecoMuPhi[RecoMuSize]=gmuon1.phi();
	  RecoMuCharge[RecoMuSize]=gmuon1.charge();
		



	  int globalmu_global=-9999;int globalmu_tracker=-9999; int globalmu_sta=-9999,globalmu_sta_noglb=-9999;
	  if(gmuon1.isGlobalMuon()){globalmu_global=1;}else{globalmu_global=0;}
	  if(gmuon1.isTrackerMuon()){globalmu_tracker=1;}else{globalmu_tracker=0;}
	  if(gmuon1.isStandAloneMuon()){globalmu_sta=1;}else{globalmu_sta=0;}
	  if((!gmuon1.isGlobalMuon()) && gmuon1.isStandAloneMuon()){globalmu_sta_noglb=1;}else{globalmu_sta_noglb=0;}
	      
	  RecoMu_global[RecoMuSize]=globalmu_global;
	  RecoMu_tracker[RecoMuSize]=globalmu_tracker;
	  RecoMu_sta[RecoMuSize]=globalmu_sta;
	  RecoMu_sta_noglb[RecoMuSize]=globalmu_sta_noglb;
	  RecoMuSize++;
	}//muon loop
      }//gmuon <1000
    
  }//is valid


}





void DiMuonOnia2DPlots::FillWTPATTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  //cout<<"------------ With Trigger Muons tree ----------------------"<<endl;
  using namespace edm;
  using namespace std;
  using namespace reco;

  eventNbWTPAT= 0 ;
  runNbWTPAT= 0 ;
  lumiBlockWTPAT= 0 ;
  


  PAT_Npix = -9999;
  PAT_NpixelTracks = -9999;
  PAT_Ntracks = -9999;
  
  PAT_SumET_HF = -9999;
  PAT_SumET_HFplus = -9999;
  PAT_SumET_HFminus = -9999;
  PAT_SumET_HFplusEta4 = -9999;
  PAT_SumET_HFminusEta4 = -9999;
  PAT_SumET_ZDC = -9999;
  PAT_SumET_ZDCplus = -9999;
  PAT_SumET_ZDCminus = -9999;
  PAT_SumET_EEplus = -9999;
  PAT_SumET_EEminus = -9999;
  PAT_SumET_EE = -9999;
  PAT_SumET_EB = -9999;
  PAT_SumET_ET = -9999;




  PAT_vertexTrkEv=-9999;
  PAT_vertexXEv=-9999;
  PAT_vertexYEv=-9999;
  PAT_vertexZEv=-9999;
  PAT_vertexRhoEv=-9999;
  PAT_isVtxFakeEv=0;
  PAT_vertexNbEv=-99999;


  PAT_eventTrigger1=-9999;
  PAT_eventTrigger2=-9999;
  PAT_eventTrigger3=-9999;
  PAT_eventTrigger4=-9999;
  PAT_eventTrigger5=-9999;
  PAT_eventTrigger6=-9999;




  PAT_GenvertexXEv=-9999;
  PAT_GenvertexYEv=-9999;
  PAT_GenvertexZEv=-9999;

  rbinWTPAT=-99999;

  math::XYZPoint RefVtx2;  
  WTPATMuSize=0;

  eventNbWTPAT= iEvent.id().event();
  runNbWTPAT=iEvent.id().run();
  lumiBlockWTPAT= iEvent.luminosityBlock();
  
  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    centrality_ = new CentralityProvider(iSetup);
    centrality_->newEvent(iEvent,iSetup);
    rbinWTPAT=centrality_->getBin();
  }
  else{
    rbinWTPAT=-99999;
  }
  
 //========================= Reading Centrality distribution and filling event variables ======================//

  if(!strcmp(fIsCentInfo.c_str(),"TRUE")){
    edm::Handle<reco::Centrality> collCentrality;
    iEvent.getByLabel("pACentrality",collCentrality);
    

    PAT_Npix = collCentrality->multiplicityPixel();
    PAT_NpixelTracks = collCentrality->NpixelTracks();
    PAT_Ntracks = collCentrality->Ntracks();
    
    PAT_SumET_HF = collCentrality->EtHFtowerSum();
    PAT_SumET_HFplus = collCentrality->EtHFtowerSumPlus();
    PAT_SumET_HFminus = collCentrality->EtHFtowerSumMinus();
    PAT_SumET_HFplusEta4 = collCentrality->EtHFtruncatedPlus();
    PAT_SumET_HFminusEta4 = collCentrality->EtHFtruncatedMinus();
    PAT_SumET_ZDC = collCentrality->zdcSum();
    PAT_SumET_ZDCplus = collCentrality->zdcSumPlus();
    PAT_SumET_ZDCminus = collCentrality->zdcSumMinus();
    PAT_SumET_EEplus = collCentrality->EtEESumPlus();
    PAT_SumET_EEminus = collCentrality->EtEESumMinus();
    PAT_SumET_EE = collCentrality->EtEESum();
    PAT_SumET_EB = collCentrality->EtEBSum();
    PAT_SumET_ET = collCentrality->EtMidRapiditySum();
  }

  //==================================== Gen Stuff ==================================//
  PAT_GenParSize=0;

  if(!strcmp(fIsGenInfo.c_str(),"TRUE")){
    //int genParId =0;                                                                             
    edm::Handle<edm::View<reco::GenParticle> >genPar;
    iEvent.getByLabel("genParticles",genPar) ;
    if(!(genPar.isValid())) return;
    edm::View<reco::GenParticle> genParticles = *genPar ;
    
    PAT_GenParNo=genParticles.size();
    
    
    for(size_t i = 0; i < genParticles.size(); ++ i)
      {
	const reco::GenParticle& part = (*genPar)[i];
	const  Candidate *mom1 = part.mother();
	if (part.numberOfMothers()!=1) continue;
	int momId1 = mom1->pdgId();      
	//if(part.pdgId()== 443 || part.pdgId()== 553 || part.pdgId()== 100553 || part.pdgId()== 23 || TMath::Abs(part.pdgId())== 13)
	
	
	char* MID=(char*)"AB";
	if(momId1 == 100443) MID=(char *)"PsiPrime";
	if(momId1 == 443) MID=(char *)"JPsi";
	if(momId1 == 553) MID=(char *)"Upsilon1s";
	if(momId1 == 100553)MID=(char *)"Upsilon2s";
	if(momId1 == 23)MID=(char *)"Z0";
	
	char* MRID=(char*)"ABC";
	if( part.pdgId()== 100443) MRID=(char *)"PsiPrime";
	if( part.pdgId()== 443) MRID=(char *)"JPsi";
	if( part.pdgId()== 553) MRID=(char *)"Upsilon1s";
	if( part.pdgId()== 100553)MRID=(char *)"Upsilon2s";
	if( part.pdgId()== 23)MRID=(char *)"Z0";
	
	
	//cout<<"momId "<<momId<<endl;
	//JPsi
	//copy
	////if ((abs(part.pdgId()) == 13) && (!strcmp(fMotherID.c_str(),MID)) ){
	// if( (momId1 == 553 && TMath::Abs(part.pdgId())== 13) || (part.pdgId()==553) ) 
	
	if( (  (!strcmp(fMotherID.c_str(),MID)) && TMath::Abs(part.pdgId())== 13) || (  (!strcmp(fMotherID.c_str(),MRID))  ) )
	  
	  
	  {
	    
	    PAT_GenvertexXEv=part.vx();
	    PAT_GenvertexYEv=part.vy();
	    PAT_GenvertexZEv=part.vz();
	    
	    PAT_GenParPx[PAT_GenParSize]=part.px();
	    PAT_GenParPy[PAT_GenParSize]=part.py();
	    PAT_GenParPz[PAT_GenParSize]=part.pz();
	    PAT_GenParPt[PAT_GenParSize]=part.pt();
	    
	    PAT_GenParEta[PAT_GenParSize]=part.eta();
	    PAT_GenParPhi[PAT_GenParSize]=part.phi();
	    PAT_GenParRap[PAT_GenParSize]=part.rapidity();
	    PAT_GenParCharge[PAT_GenParSize]=part.charge();
	    PAT_GenParId[PAT_GenParSize]=part.pdgId();
	    PAT_GenParStatus[PAT_GenParSize]=part.status();
	    
	    const  Candidate *mom = part.mother();
	    
	    //cout<<"  part.numberOfMothers()  "<<part.numberOfMothers()<<endl;     
	    
	    double momId ;double momstatus;
	    if (part.numberOfMothers()==1) {momId = mom->pdgId(); momstatus= mom->status();}
	    else{momId=-99999;momstatus=-99999;}
	    
	    PAT_GenParMomId[PAT_GenParSize]=momId ;
	    PAT_GenParMomStatus[PAT_GenParSize]=momstatus;
	    PAT_GenParSize++;
	  }
      }
    
  }//is geninfo

  //===================================== Reco Stuff ==================================//
  int nPVEv;
  Handle<reco::VertexCollection>privtxsEv;
  iEvent.getByLabel("offlinePrimaryVertices",privtxsEv);
  VertexCollection::const_iterator privtxEv;
  
  nPVEv = privtxsEv->size();
  //vertexNbEv = privtxsEv->size();
  PAT_vertexNbEv=privtxsEv->size();

  if(nPVEv >0) {
    if (privtxsEv->begin() != privtxsEv->end() ) {
      privtxEv=privtxsEv->begin();
      RefVtx2 = privtxEv->position();
    
      PAT_vertexTrkEv=privtxEv->tracksSize();
      PAT_vertexXEv=privtxEv->x();
      PAT_vertexYEv=privtxEv->y();
      PAT_vertexZEv=privtxEv->z();
      PAT_vertexRhoEv=privtxEv->position().rho();
      if(privtxEv->isFake()){PAT_isVtxFakeEv=1;}else{PAT_isVtxFakeEv=0;}
      
    }
  }
  else {RefVtx2.SetXYZ(0.,0.,0.);}
  
    
  //======================================== Event Trigger =====================================//                                                                                
  int evTrigger[6]={0};

  Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel("TriggerResults", triggerResults);

  const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults);
  for (unsigned i=0; i<triggerNames.size(); i++) {
    std::string hltName = triggerNames.triggerName(i);
    unsigned int index = triggerNames.triggerIndex(hltName);
    
    if(!strcmp(hltName.c_str(),"HLT_PAL1DoubleMuOpen_v1")) {if(triggerResults->accept(index)) evTrigger[0]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAL1DoubleMu0_HighQ_v1")) { if(triggerResults->accept(index)) evTrigger[1]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAL2DoubleMu3_v1")) { if(triggerResults->accept(index)) evTrigger[2]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAMu3_v1")) { if(triggerResults->accept(index)) evTrigger[3]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAMu7_v1")) { if(triggerResults->accept(index)) evTrigger[4]=1;}
    
    //last muon trigger changed to min bias trigger
    //if(!strcmp(hltName.c_str(),"HLT_PAMu12_v1")) { if(triggerResults->accept(index)) evTrigger[5]=1;}
    if(!strcmp(hltName.c_str(),"HLT_PAZeroBiasPixel_SingleTrack_v1")) { if(triggerResults->accept(index)) evTrigger[5]=1;}
                                                                                                         
    //cout << "HLT: " << hltName << " fired? " << triggerResults->accept(index) << endl;    
                                                                                      
  }
  
  if(evTrigger[0]==1){PAT_eventTrigger1=1;}else{PAT_eventTrigger1=0;}
  if(evTrigger[1]==1){PAT_eventTrigger2=1;}else{PAT_eventTrigger2=0;}
  if(evTrigger[2]==1){PAT_eventTrigger3=1;}else{PAT_eventTrigger3=0;}
  if(evTrigger[3]==1){PAT_eventTrigger4=1;}else{PAT_eventTrigger4=0;}
  if(evTrigger[4]==1){PAT_eventTrigger5=1;}else{PAT_eventTrigger5=0;}
  if(evTrigger[5]==1){PAT_eventTrigger6=1;}else{PAT_eventTrigger6=0;}
  


  //======================== PAT Muons =============================//
  edm::Handle<edm::View<pat::Muon> > GMuons;
  iEvent.getByLabel("patMuonsWithTrigger", GMuons);
  
  if(GMuons.isValid()){
    edm::View<pat::Muon>GMuonColl= *GMuons;
    int GMuonsize =GMuonColl.size();
    
    int nGlobalMuon=GMuonColl.size();
    
    if( nGlobalMuon >0 && nGlobalMuon < 1000)
      {  
	for ( int i=0 ; i < GMuonsize ; ++i ){
	  const pat::Muon& gmuon1 = (*GMuons)[i];
	  
	  WTPATMuPx[WTPATMuSize]=gmuon1.px();
	  WTPATMuPy[WTPATMuSize]=gmuon1.py();
	  WTPATMuPz[WTPATMuSize]=gmuon1.pz();
	  WTPATMuPt[WTPATMuSize]=gmuon1.pt();
	  WTPATMuEta[WTPATMuSize]=gmuon1.eta();
	  WTPATMuPhi[WTPATMuSize]=gmuon1.phi();
	  WTPATMuCharge[WTPATMuSize]=gmuon1.charge();
	  

	  int globalmu_global=-9999;int globalmu_tracker=-9999; int globalmu_sta=-9999,globalmu_sta_noglb=-9999;
	  if(gmuon1.isGlobalMuon()){globalmu_global=1;}else{globalmu_global=0;}
	  if(gmuon1.isTrackerMuon()){globalmu_tracker=1;}else{globalmu_tracker=0;}
	  if(gmuon1.isStandAloneMuon()){globalmu_sta=1;}else{globalmu_sta=0;}
	  if((!gmuon1.isGlobalMuon()) && gmuon1.isStandAloneMuon()){globalmu_sta_noglb=1;}else{globalmu_sta_noglb=0;}

 
	  //Trigger Matching
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAL1DoubleMuOpen_v1").empty()){WTPATMu_Trigger1[WTPATMuSize]=1;}else{WTPATMu_Trigger1[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAL1DoubleMu0_HighQ_v1").empty()){WTPATMu_Trigger2[WTPATMuSize]=1;}else{WTPATMu_Trigger2[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAL2DoubleMu3_v1").empty()){WTPATMu_Trigger3[WTPATMuSize]=1;}else{WTPATMu_Trigger3[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAMu3_v1").empty()){WTPATMu_Trigger4[WTPATMuSize]=1;}else{WTPATMu_Trigger4[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAMu7_v1").empty()){WTPATMu_Trigger5[WTPATMuSize]=1;}else{WTPATMu_Trigger5[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByPath("HLT_PAMu12_v1").empty()){WTPATMu_Trigger6[WTPATMuSize]=1;}else{WTPATMu_Trigger6[WTPATMuSize]=0;}
	  

	  if(!gmuon1.triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMuOpenL1Filtered0").empty()){WTPATMu_Trigger7[WTPATMuSize]=1;}else{WTPATMu_Trigger7[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByFilter("hltL1fL1sPAL1DoubleMu0HighQL1FilteredHighQ").empty()){WTPATMu_Trigger8[WTPATMuSize]=1;}else{WTPATMu_Trigger8[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByFilter("hltL2fL1sPAL2DoubleMu3L2Filtered3").empty()){WTPATMu_Trigger9[WTPATMuSize]=1;}else{WTPATMu_Trigger9[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByFilter("hltL3fL2sMu3L3Filtered3").empty()){WTPATMu_Trigger10[WTPATMuSize]=1;}else{WTPATMu_Trigger10[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByFilter("hltL3fL2sMu7L3Filtered7").empty()){WTPATMu_Trigger11[WTPATMuSize]=1;}else{WTPATMu_Trigger11[WTPATMuSize]=0;}
	  if(!gmuon1.triggerObjectMatchesByFilter("hltL3fL2sMu12L3Filtered12").empty()){WTPATMu_Trigger12[WTPATMuSize]=1;}else{WTPATMu_Trigger12[WTPATMuSize]=0;}



	  if(gmuon1.isTrackerMuon() )
	    
	    {
	      TrackRef iTrack =gmuon1.innerTrack();
	      const reco::HitPattern& p2=iTrack->hitPattern();
	      WTPATMu_found[WTPATMuSize]=iTrack->found();
	      WTPATMu_nchi2In[WTPATMuSize]=iTrack->normalizedChi2();
	      WTPATMu_arbitrated[WTPATMuSize]=gmuon1.muonID("TrackerMuonArbitrated");
	      WTPATMu_stationTight[WTPATMuSize]=gmuon1.muonID("TMOneStationTight");
	      WTPATMu_trackerLayers[WTPATMuSize]=p2.trackerLayersWithMeasurement();
	      WTPATMu_pixeLayers[WTPATMuSize]=p2.pixelLayersWithMeasurement();
	      WTPATMu_dxy[WTPATMuSize]=iTrack->dxy(RefVtx2);
	      WTPATMu_dz[WTPATMuSize]=iTrack->dz(RefVtx2);
	      
	      if(gmuon1.isGlobalMuon())
		{
		  TrackRef gTrack =gmuon1.globalTrack();
		  const reco::HitPattern& q2=gTrack->hitPattern();
		  WTPATMu_nValidMuHits[WTPATMuSize]=q2.numberOfValidMuonHits();
		  WTPATMu_nchi2Gl[WTPATMuSize]=gTrack->chi2()/gTrack->ndof();
		}
	    }
	  
	  else{
	    WTPATMu_found[WTPATMuSize]=-9999;
	    WTPATMu_nchi2In[WTPATMuSize]=-9999;
	    WTPATMu_arbitrated[WTPATMuSize]=-9999;
	    WTPATMu_stationTight[WTPATMuSize]=-9999;
	    WTPATMu_trackerLayers[WTPATMuSize]=-9999;
	    WTPATMu_pixeLayers[WTPATMuSize]=-9999;
	    WTPATMu_dxy[WTPATMuSize]=-9999;
	    WTPATMu_dz[WTPATMuSize]=-9999;
	    WTPATMu_nValidMuHits[WTPATMuSize]=-9999;
	    WTPATMu_nchi2Gl[WTPATMuSize]=-9999;
	  }
	  
	  WTPATMu_global[WTPATMuSize]=globalmu_global;
	  WTPATMu_tracker[WTPATMuSize]=globalmu_tracker;
	  WTPATMu_sta[WTPATMuSize]=globalmu_sta;
	  WTPATMu_sta_noglb[WTPATMuSize]=globalmu_sta_noglb;
	  
	  WTPATMuSize++;
	}//muon loop
      }//gmuon <1000
  }//is valid
  
  
}


//define this as a plug-in
DEFINE_FWK_MODULE(DiMuonOnia2DPlots);



