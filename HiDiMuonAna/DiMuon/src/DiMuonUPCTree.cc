// -*- C++ -*-
//
// Package:    DiMuonUPCTree
// Class:      DiMuonUPCTree
// 
/**\class DiMuonUPCTree DiMuonUPCTree.cc DiMuonAna/DiMuonUPCTree/src/DiMuonUPCTree.cc
   
 Description: [one line class summary]
 
 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Dilep PING, Vineet Kumar, Prashant Shukla
//         Created:  Wed May 12 13:45:14 CEST 2010
// $Id: DiMuonUPCTree.cc,v 1.3 2011/09/12 11:45:11 kumarv Exp $
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
#include "FWCore/Common/interface/TriggerNames.h"
# include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"



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

class DiMuonUPCTree : public edm::EDAnalyzer {
   public:
  explicit DiMuonUPCTree(const edm::ParameterSet&);
  ~DiMuonUPCTree();

  //const CentralityBins *cbins_;
  CentralityProvider *centrality_;    
 
 
  bool fisCuts;  
  std::string fOutputFileName;
  std::string fIsGenInfo;
  std::string fIsPATInfo;
  std::string fHLTFilterName;  
  std::string fMotherID;  

  //TFile *In;

  TH1F *Centrality;
  TFile *fOutputFile ;
  TTree *EventTree;

  int bin, rbin;

private:
  
  virtual void beginJob() ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void FillTree(const edm::Event&, const edm::EventSetup&);
  bool selPATMuon(const pat::Muon &aMuon); 
  virtual void endJob() ;
  
  math::XYZPoint RefVtx;
  float nPV;
  int nDiMuon;


  
  //Tree variables defined here  
  int eventNb,runNb,lumiBlock, trackNb,glbMuNb,vertexNb;
  double vertexTrk,vertexX,vertexY,vertexZ,vertexRho;
  bool eventTrigger0,eventTrigger1,eventTrigger2,eventTrigger3,eventTrigger4,eventTrigger5;
  bool eventTrigger6,eventTrigger7,eventTrigger8,eventTrigger9,eventTrigger10;   
  bool isVtxFake;
  
  //int ZDCCh;
  
  double ZDCEn[18];
  
  int TrackSize;
  double TrackPx[1000], TrackPy[1000], TrackPz[1000], TrackPhi[1000], TrackEta[1000], TrackCharge[1000];

  int TrackMuSize;
  
  
  double TrackMuPx[1000], TrackMuPy[1000], TrackMuPz[1000], TrackMuPhi[1000], TrackMuEta[1000], TrackMuCharge[1000];
  double TrackMu_nchi2In[1000], TrackMu_dxy[1000],TrackMu_dz[1000];
  int TrackMu_found[1000], TrackMu_pixeLayers[1000], TrackMu_arbitrated[1000],TrackMu_global[1000];


  int DiMuSize;
  
  double DiMuCharge[1000],DiMuNo[1000], DiMuMass[1000] , DiMuPt[1000], DiMuRap[1000];
  double DiMuPx[1000], DiMuPy[1000], DiMuPz[1000];

  double Mu1Charge[1000],Mu1No[1000], Mu1Mass[1000] , Mu1Pt[1000], Mu1Eta[1000];
  double Mu1Px[1000], Mu1Py[1000], Mu1Pz[1000], Mu1Phi[1000];
  double Mu1_nchi2In[1000], Mu1_dxy[1000], Mu1_dz[1000], Mu1_nchi2Gl[1000];
  int Mu1_found[1000], Mu1_pixeLayers[1000], Mu1_nValidMuHits[1000], Mu1_arbitrated[1000];
  int Mu1_tracker[1000], Mu1_global[1000], Mu1_standalone[1000];
  int Mu1_trigger1[1000],Mu1_trigger2[1000],Mu1_trigger3[1000];

  double Mu2Charge[1000],Mu2No[1000], Mu2Mass[1000] , Mu2Pt[1000], Mu2Eta[1000];
  double Mu2Px[1000], Mu2Py[1000], Mu2Pz[1000], Mu2Phi[1000];
  double Mu2_nchi2In[1000], Mu2_dxy[1000], Mu2_dz[1000], Mu2_nchi2Gl[1000];
  int Mu2_found[1000], Mu2_pixeLayers[1000], Mu2_nValidMuHits[1000],Mu2_arbitrated[1000];
  int Mu2_tracker[1000], Mu2_global[1000], Mu2_standalone[1000];

  int Mu2_trigger1[1000],Mu2_trigger2[1000],Mu2_trigger3[1000];

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


DiMuonUPCTree::DiMuonUPCTree(const edm::ParameterSet& iConfig):

  centrality_(0),
  //cbins_(0),
  fisCuts(iConfig.getUntrackedParameter<bool>("IsCuts")),
  fOutputFileName(iConfig.getUntrackedParameter<string>("OutputFileName")),
  fIsGenInfo(iConfig.getUntrackedParameter<string>("IsGenInfo")), 
  fIsPATInfo(iConfig.getUntrackedParameter<string>("IsPATInfo")), 
  fHLTFilterName(iConfig.getUntrackedParameter<string>("HLTFilterName")),
  fMotherID(iConfig.getUntrackedParameter<string>("MotherID"))
  

{ 

//now do what ever initialization is needed

}


DiMuonUPCTree::~DiMuonUPCTree()
{
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}


//
// member functions
//

// ------------ method called to for each event  ------------
void
DiMuonUPCTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //cout << " ------------------------------------------------------ " << endl;
  using namespace edm;
  using namespace std;
  
  nPV = 0 ;
  
  centrality_ = new CentralityProvider(iSetup);
  centrality_->newEvent(iEvent,iSetup);
  bin = centrality_->getBin();
  Centrality->Fill(bin);

  //int NoOfTracks =0;
  
  //Handle<TrackCollection> trackCollectionHandle;
  //iEvent.getByLabel("hiGlobalPrimTracks", trackCollectionHandle);
  //iEvent.getByLabel("hiSelectedTracks", trackCollectionHandle);

  //if( !trackCollectionHandle.isValid() ){
  //cout << "Error! Can't get selectTracks!" << endl;
  //return ;
  //}

  //NoOfTracks =trackCollectionHandle->size();
    
  //cout<<" no of tracks : "<<NoOfTracks<<endl;

  //if( NoOfTracks > 1 && NoOfTracks < 100 )
  //{
      FillTree(iEvent,iSetup);
      EventTree->Fill();
      //}
  
 
  
}

// ------------ method called once each job just before starting event loop  ------------
void 
DiMuonUPCTree::beginJob()
{



  edm::Service<TFileService> fs;
  fOutputFile   = new TFile( fOutputFileName.c_str(), "RECREATE" );
  cout<<"begin job"<<endl;
  
  EventTree = new TTree("EventTree","EventTree");
  // Event variables
  EventTree->Branch("eventNb",             &eventNb,             "eventNb/I");
  EventTree->Branch("runNb",               &runNb,               "runNb/I");
  EventTree->Branch("lumiBlock",           &lumiBlock,           "lumiBlock/I");
  EventTree->Branch("rbin",                 &rbin,                  "rbin/I");
 
  EventTree->Branch("vertexNb",            &vertexNb,            "vertexNb/I");
  EventTree->Branch("vertexTrk",           &vertexTrk,            "vertexTrk/D");
  EventTree->Branch("isVtxFake",          &isVtxFake,            "isVtxFake/O");
  EventTree->Branch("vertexX",            &vertexX,            "vertexX/D");
  EventTree->Branch("vertexY",            &vertexY,            "vertexY/D");
  EventTree->Branch("vertexZ",             &vertexZ,            "vertexZ/D");
  EventTree->Branch("vertexRho",             &vertexRho,            "vertexRho/D");
  
  EventTree->Branch("ZDCEn",               ZDCEn,            "ZDCEn[18]/D");
  EventTree->Branch("glbMuNb",            &glbMuNb,            "glbMuNb/I");

  EventTree->Branch("trackNb",             &trackNb,             "trackNb/I");

  EventTree->Branch("TrackSize",    &TrackSize,           "TrackSize/I");
  EventTree->Branch("TrackPx",      TrackPx,        "TrackPx[TrackSize]/D");
  EventTree->Branch("TrackPy",      TrackPy,        "TrackPy[TrackSize]/D");
  EventTree->Branch("TrackPz",      TrackPz,        "TrackPz[TrackSize]/D");
  EventTree->Branch("TrackEta",     TrackEta,       "TrackEta[TrackSize]/D");
  EventTree->Branch("TrackPhi",     TrackPhi,       "TrackPhi[TrackSize]/D");
  EventTree->Branch("TrackCharge",  TrackCharge,    "TrackCharge[TrackSize]/D");


  EventTree->Branch("TrackMuSize",    &TrackMuSize,           "TrackMuSize/I");
  EventTree->Branch("TrackMuPx",      TrackMuPx,        "TrackMuPx[TrackMuSize]/D");
  EventTree->Branch("TrackMuPy",      TrackMuPy,        "TrackMuPy[TrackMuSize]/D");
  EventTree->Branch("TrackMuPz",      TrackMuPz,        "TrackMuPz[TrackMuSize]/D");
  EventTree->Branch("TrackMuEta",     TrackMuEta,       "TrackMuEta[TrackMuSize]/D");
  EventTree->Branch("TrackMuPhi",     TrackMuPhi,       "TrackMuPhi[TrackMuSize]/D");
  EventTree->Branch("TrackMuCharge",  TrackMuCharge,    "TrackMuCharge[TrackMuSize]/D");
  //Tracker Muon cut variables
  EventTree->Branch("TrackMu_nchi2In",  TrackMu_nchi2In, "TrackMu_nchi2In[TrackMuSize]/D");
  EventTree->Branch("TrackMu_dxy",  TrackMu_dxy, "TrackMu_dxy[TrackMuSize]/D");
  EventTree->Branch("TrackMu_dz",  TrackMu_dz, "TrackMu_dz[TrackMuSize]/D");
  EventTree->Branch("TrackMu_found",  TrackMu_found, "TrackMu_found[TrackMuSize]/I");
  EventTree->Branch("TrackMu_pixeLayers",  TrackMu_pixeLayers, "TrackMu_pixeLayers[TrackMuSize]/I");
  EventTree->Branch("TrackMu_arbitrated",  TrackMu_arbitrated, "TrackMu_arbitrated[TrackMuSize]/I");
  EventTree->Branch("TrackMu_global",  TrackMu_global, "TrackMu_global[TrackMuSize]/I");


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
 
  //DiMuon variables
  EventTree->Branch("DiMuSize",   &DiMuSize,  "DiMuSize/I");  
  EventTree->Branch("DiMuCharge", DiMuCharge,  "DiMuCharge[DiMuSize]/D");
  EventTree->Branch("DiMuMass",   DiMuMass,  "DiMuMass[DiMuSize]/D");
  EventTree->Branch("DiMuPt",     DiMuPt,    "DiMuPt[DiMuSize]/D");
  EventTree->Branch("DiMuRap",    DiMuRap,   "DiMuRap[DiMuSize]/D");
  EventTree->Branch("DiMuPx",     DiMuPx,    "DiMuPx[DiMuSize]/D");
  EventTree->Branch("DiMuPy",     DiMuPy,    "DiMuPy[DiMuSize]/D");
  EventTree->Branch("DiMuPz",     DiMuPz,    "DiMuPz[DiMuSize]/D");
  
  //Muon1 variables
  EventTree->Branch("Mu1Charge", Mu1Charge,  "Mu1Charge[DiMuSize]/D");
  EventTree->Branch("Mu1Mass",   Mu1Mass,  "Mu1Mass[DiMuSize]/D");
  EventTree->Branch("Mu1Pt",     Mu1Pt,    "Mu1Pt[DiMuSize]/D");
  EventTree->Branch("Mu1Eta",    Mu1Eta,   "Mu1Eta[DiMuSize]/D");
  EventTree->Branch("Mu1Px",     Mu1Px,    "Mu1Px[DiMuSize]/D");
  EventTree->Branch("Mu1Py",     Mu1Py,    "Mu1Py[DiMuSize]/D");
  EventTree->Branch("Mu1Pz",     Mu1Pz,    "Mu1Pz[DiMuSize]/D");
  EventTree->Branch("Mu1Phi",     Mu1Phi,    "Mu1Phi[DiMuSize]/D");
  //Muon1 cut variables
  EventTree->Branch("Mu1_nchi2In",  Mu1_nchi2In, "Mu1_nchi2In[DiMuSize]/D");
  EventTree->Branch("Mu1_dxy",  Mu1_dxy, "Mu1_dxy[DiMuSize]/D");
  EventTree->Branch("Mu1_dz",  Mu1_dz, "Mu1_dz[DiMuSize]/D");
  EventTree->Branch("Mu1_nchi2Gl",  Mu1_nchi2Gl, "Mu1_nchi2Gl[DiMuSize]/D");
  EventTree->Branch("Mu1_found",  Mu1_found, "Mu1_found[DiMuSize]/I");
  EventTree->Branch("Mu1_pixeLayers",  Mu1_pixeLayers, "Mu1_pixeLayers[DiMuSize]/I");
  EventTree->Branch("Mu1_nValidMuHits",  Mu1_nValidMuHits, "Mu1_nValidMuHits[DiMuSize]/I");
  EventTree->Branch("Mu1_arbitrated",  Mu1_arbitrated, "Mu1_arbitrated[DiMuSize]/I");

  EventTree->Branch("Mu1_tracker",  Mu1_tracker, "Mu1_tracker[DiMuSize]/I");
  EventTree->Branch("Mu1_global",  Mu1_global, "Mu1_global[DiMuSize]/I");
  EventTree->Branch("Mu1_standalone",  Mu1_standalone, "Mu1_standalone[DiMuSize]/I");

  EventTree->Branch("Mu1_trigger1",  Mu1_trigger1, "Mu1_trigger1[DiMuSize]/I");
  EventTree->Branch("Mu1_trigger2",  Mu1_trigger2, "Mu1_trigger2[DiMuSize]/I");
  EventTree->Branch("Mu1_trigger3",  Mu1_trigger3, "Mu1_trigger3[DiMuSize]/I");
  

 //Muon2 variables
  EventTree->Branch("Mu2Charge", Mu2Charge,  "Mu2Charge[DiMuSize]/D");
  EventTree->Branch("Mu2Mass",   Mu2Mass,  "Mu2Mass[DiMuSize]/D");
  EventTree->Branch("Mu2Pt",     Mu2Pt,    "Mu2Pt[DiMuSize]/D");
  EventTree->Branch("Mu2Eta",    Mu2Eta,   "Mu2Eta[DiMuSize]/D");
  EventTree->Branch("Mu2Px",     Mu2Px,    "Mu2Px[DiMuSize]/D");
  EventTree->Branch("Mu2Py",     Mu2Py,    "Mu2Py[DiMuSize]/D");
  EventTree->Branch("Mu2Pz",     Mu2Pz,    "Mu2Pz[DiMuSize]/D");
  EventTree->Branch("Mu2Phi",     Mu2Phi,    "Mu2Phi[DiMuSize]/D");
  //Muon2 cut variables                                                                                                                                                                                         
  EventTree->Branch("Mu2_nchi2In",  Mu2_nchi2In, "Mu2_nchi2In[DiMuSize]/D");
  EventTree->Branch("Mu2_dxy",  Mu2_dxy, "Mu2_dxy[DiMuSize]/D");
  EventTree->Branch("Mu2_dz",  Mu2_dz, "Mu2_dz[DiMuSize]/D");
  EventTree->Branch("Mu2_nchi2Gl",  Mu2_nchi2Gl, "Mu2_nchi2Gl[DiMuSize]/D");
  EventTree->Branch("Mu2_found",  Mu2_found, "Mu2_found[DiMuSize]/I");
  EventTree->Branch("Mu2_pixeLayers",  Mu2_pixeLayers, "Mu2_pixeLayers[DiMuSize]/I");
  EventTree->Branch("Mu2_nValidMuHits",  Mu2_nValidMuHits, "Mu2_nValidMuHits[DiMuSize]/I");
  EventTree->Branch("Mu2_arbitrated",  Mu2_arbitrated, "Mu2_arbitrated[DiMuSize]/I");
  EventTree->Branch("Mu2_tracker",  Mu2_tracker, "Mu2_tracker[DiMuSize]/I");
  EventTree->Branch("Mu2_global",  Mu2_global, "Mu2_global[DiMuSize]/I");
  EventTree->Branch("Mu2_standalone",  Mu2_standalone, "Mu2_standalone[DiMuSize]/I");


  EventTree->Branch("Mu2_trigger1",  Mu2_trigger1, "Mu2_trigger1[DiMuSize]/I");
  EventTree->Branch("Mu2_trigger2",  Mu2_trigger2, "Mu2_trigger2[DiMuSize]/I");
  EventTree->Branch("Mu2_trigger3",  Mu2_trigger3, "Mu2_trigger3[DiMuSize]/I");


  
  cout<<"Tree booked "<<endl;
  
  //Histograms       
  Centrality = new TH1F("Centrality","Centrality", 60,-10,50);
   


  // Write comments in a file
}



// ------------ method called once each job just after ending the event loop  ------------
void DiMuonUPCTree::endJob() 
{
  //In->Close();
  cout<<"End Job"<<endl;
  fOutputFile->cd();
  EventTree->Write();
  Centrality->Write(); 
  fOutputFile->Close();
}




bool DiMuonUPCTree::selPATMuon(const pat::Muon &aMuon) {
  
if(!(aMuon.isTrackerMuon()))
    return false;
  
  TrackRef iTrack = aMuon.innerTrack();
  if(!iTrack.isAvailable()) return false;
  const reco::HitPattern& p = iTrack->hitPattern();
  
  return (
	  p.numberOfValidHits() > 10 &&
	  p.pixelLayersWithMeasurement()> 0 &&
	  iTrack->chi2()/iTrack->ndof() <= 4.0 &&
	  fabs(iTrack->dxy(RefVtx)) <= 2.0 &&
          fabs(iTrack->dz(RefVtx)) <= 2.0 
	  );
}




void DiMuonUPCTree::FillTree(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  // cout<<" starting fill tree "<<endl;

  using namespace edm;
  using namespace std;
  using namespace reco;


  using reco::TrackCollection;
  using reco::MuonCollection;
  
  float massMu=0.105658;
  
  //int evTrigger[20]={0};

  //reset EVENT information
  eventNb= 0 ;
  runNb= 0 ;
  lumiBlock= 0 ;
  rbin=0;
  trackNb=0;

  vertexNb=-999;
  vertexTrk=-999;
  isVtxFake=-999;
  vertexX=-999;
  vertexY=-999;  
  vertexZ=-999;
  vertexRho=-999;
  
  glbMuNb=-999;
  
  DiMuSize=0;
  TrackSize=0;
  TrackMuSize = 0;


  //cout<<" 0 TrackMuSize "<<TrackMuSize<<endl;

  // Event related infos
  eventNb = iEvent.id().event();
  runNb=iEvent.id().run();
  lumiBlock= iEvent.luminosityBlock();
  
  centrality_ = new CentralityProvider(iSetup);
  centrality_->newEvent(iEvent,iSetup);
  rbin = centrality_->getBin();

  //=================================== Primary Vertex==========================================// 
  Handle<reco::VertexCollection> privtxs;
  iEvent.getByLabel("hiSelectedVertex", privtxs);
  VertexCollection::const_iterator privtx;
  nPV = privtxs->size();
  vertexNb = privtxs->size();
  if(!nPV) return;
  if ( privtxs->begin() != privtxs->end() ) {
    privtx=privtxs->begin();
    vertexTrk=privtx->tracksSize();
    vertexX=privtx->x();
    vertexY=privtx->y();
    vertexZ=privtx->z();
    vertexRho=privtx->position().rho();
    RefVtx = privtx->position();
    if(privtx->isFake()){isVtxFake=1;}else{isVtxFake=0;}
    //cout<<" vtx trk: "<<vertexTrk<<" is fake "<<privtx->isFake()<<" is ff "<<isVtxFake<<endl;
    //cout<<" vtx x: "<<vertexX<<" vtxy "<<vertexY<<" z "<<vertexZ<<endl;
  } 
  else {RefVtx.SetXYZ(0.,0.,0.);}  
  

  //cout<<" 1 TrackMuSize "<<TrackMuSize <<endl; 
  //========================================== Trigger stuff==============================================================//
  /*
  Handle<edm::TriggerResults> triggerResults;
  iEvent.getByLabel("TriggerResults", triggerResults);
  const edm::TriggerNames triggerNames = iEvent.triggerNames(*triggerResults);
  
  for (unsigned i=0; i<triggerNames.size(); i++) {
    std::string hltName = triggerNames.triggerName(i);
    unsigned int index = triggerNames.triggerIndex(hltName);
    if(!strcmp(hltName.c_str(),"HLT_HIL1DoubleMuOpen"))                     { if(triggerResults->accept(index)) evTrigger[0]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIL1SingleMu3"))                        { if(triggerResults->accept(index)) evTrigger[1]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIUpcMu"))                              { if(triggerResults->accept(index)) evTrigger[2]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIZeroBiasPixel_SingleTrack"))          { if(triggerResults->accept(index)) evTrigger[3]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIMinBiasZDC_Calo"))                    { if(triggerResults->accept(index)) evTrigger[4]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIMinBiasZDC_Calo_PlusOrMinus"))        { if(triggerResults->accept(index)) evTrigger[5]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIMinBiasZDC_Scint"))                   { if(triggerResults->accept(index)) evTrigger[6]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIMinBiasZDCPixel_SingleTrack"))        { if(triggerResults->accept(index)) evTrigger[7]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIL1ETT30"))                            { if(triggerResults->accept(index)) evTrigger[8]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIL1ETT60"))                            { if(triggerResults->accept(index)) evTrigger[9]=1;} 
    if(!strcmp(hltName.c_str(),"HLT_HIActivityHF_Coincidence3"))            { if(triggerResults->accept(index)) evTrigger[10]=1;} 
    //if(!strcmp(hltName.c_str(),"HLT_HIActivityHF_Coincidence3"))    cout << "HLT: " << hltName << " fired? " << triggerResults->accept(index) << endl;
  }

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
  */

  //========================================== Track stuff ======================================================================//
  
  
  //Get a handle ("pointer") to the TrackCollection within the event                                                                                                           
  /*
  Handle<TrackCollection> trackCollectionHandle;
  iEvent.getByLabel("hiGlobalPrimTracks", trackCollectionHandle);
  //iEvent.getByLabel("hiSelectedTracks", trackCollectionHandle);
  //Check if there was a problem accessing the TrackCollection
  if( !trackCollectionHandle.isValid() ){
    cout << "Error! Can't get selectTracks!" << endl;
    return ;
  }
  
  trackNb=trackCollectionHandle->size();
  cout<<" no of reconstrcuted tracks in event "<<trackNb<<endl;
  
  for(TrackCollection::const_iterator track = trackCollectionHandle->begin(); track != trackCollectionHandle->end(); track++){
    
    //cout<<" in track loop "<<endl;

    int track_charge=track->charge();
    double track_eta = track->eta();
    double track_phi = track->phi();
    double track_px  = track->px();
    double track_py  = track->py();
    double track_pz  = track->pz();
    
    //cout<< " trk size "<< TrackSize << "    "<<track_eta <<" eta " <<endl;
 
    TrackPx[TrackSize]=track_px;
    TrackPy[TrackSize]=track_py;
    TrackPz[TrackSize]=track_pz;
    TrackEta[TrackSize]=track_eta;
    TrackPhi[TrackSize]=track_phi;
    TrackCharge[TrackSize]=track_charge;
    
    TrackSize++;
  }
  

  cout<<" out of track loop "<<endl;
  cout<<" 2 TrackMuSize "<<TrackMuSize <<endl; 
  */

  //===================================== ZDC ====================================================//
  
  /*  
    double ZDCEnergy=0;  
    cout<<" 1 "<<endl;
    Handle<ZDCRecHitCollection> zdc_recHits_h;
    cout<<" 2 "<<endl;
    iEvent.getByLabel("zdcreco", zdc_recHits_h);
    cout<<" 3 "<<endl;
    const ZDCRecHitCollection *zdc_recHits = zdc_recHits_h.failedToGet()? 0 : &*zdc_recHits_h;
    
    cout<< " ZDC "<<endl;
    
  
    for (ZDCRecHitCollection::const_iterator zhit=zdc_recHits->begin();zhit!=zdc_recHits->end();zhit++){
    cout<<" 4 "<<endl;
    int iSide      = (zhit->id()).zside();
    cout<<" 5 "<<endl;
    int iSection   = (zhit->id()).section();
    cout<<" 6 "<<endl;
    int iChannel   = (zhit->id()).channel();
    cout<<" 7 "<<endl;
    int chid = (iSection-1)*5+(iSide+1)/2*9+(iChannel-1);
    cout<<" 8 "<<endl;
    ZDCEnergy+=zhit->energy();
    cout<<" 9 "<<endl;
    ZDCEn[chid]=zhit->energy(); 
    cout<<" chid "<<chid<<" zdcen "<<ZDCEn[chid]<<endl;  
    }
  */
  
 
  
  //===============================  Tracker Muons  =====================================================//
  
    /*
    int nTrackMuon=0;
  
  cout<<" 3 TrackMuSize "<<TrackMuSize <<endl; 
  
  edm::Handle<edm::View<pat::Muon> >TMuons;
  iEvent.getByLabel("patMuonsWithTrigger",TMuons);
  edm::View<pat::Muon>TMuonColl=*TMuons;
  
  int TMuonSize =TMuonColl.size();
  
  cout<<" no of tracker muons "<<TMuonSize <<endl;
  
  //TrackMuSize=0;
  
  for ( int i=0 ; i < TMuonSize ; ++i ){
    const pat::Muon& tmuon = (*TMuons)[i];
    
    if(!(selPATMuon(tmuon))) continue;
    if(selPATMuon(tmuon))nTrackMuon++;
    cout<<"nTrackMuon : "<<nTrackMuon<<endl;  
    
    
    if(nTrackMuon<100)
      {
	cout<<" inside nTrackMuon "<<endl;
	
	int trackmu_charge=tmuon.charge();
	double trackmu_eta = tmuon.eta();
	double trackmu_phi = tmuon.phi();
	double trackmu_px  = tmuon.px();
	double trackmu_py  = tmuon.py();
	double trackmu_pz  = tmuon.pz();
	
	TrackRef tTrack =tmuon.innerTrack();
	if(!tTrack.isAvailable()) continue;
	const reco::HitPattern& tp=tTrack->hitPattern();
	
	double trackmu_nchi2In  =tTrack->chi2()/tTrack->ndof();
	double trackmu_dxy      =tTrack->dxy(RefVtx);
	double trackmu_dz       =tTrack->dz(RefVtx);
	int trackmu_found       =tTrack->found();
	int trackmu_pixelLayers =tp.pixelLayersWithMeasurement();
	int trackmu_arbitrated  =tmuon.muonID("TrackerMuonArbitrated");
	int trackmu_global=0;  if(tmuon.isGlobalMuon())trackmu_global=1;
	
	cout<<" track mu charge "<< trackmu_charge <<endl;
	cout<<" track mu px "<<trackmu_px<<endl;
	
	cout<<" 4 TrackMuSize "<<TrackMuSize<<endl;
	
	TrackMuPx[TrackMuSize]=trackmu_px;
	
	cout<<" track mu px "<<trackmu_px<<endl;
	
	TrackMuPy[TrackMuSize]=trackmu_py;
	TrackMuPz[TrackMuSize]=trackmu_pz;
	TrackMuEta[TrackMuSize]=trackmu_eta;
	TrackMuPhi[TrackMuSize]=trackmu_phi;
	TrackMuCharge[TrackMuSize]=trackmu_charge;
	

	
	TrackMu_nchi2In[TrackMuSize]=trackmu_nchi2In;
	TrackMu_dxy[TrackMuSize]=trackmu_dxy; 
	TrackMu_dz[TrackMuSize]=trackmu_dz;
	TrackMu_found[TrackMuSize]=trackmu_found;
	TrackMu_pixeLayers[TrackMuSize]=trackmu_pixelLayers;
	TrackMu_arbitrated[TrackMuSize]=trackmu_arbitrated;
	TrackMu_global[TrackMuSize]=trackmu_global;
	
	TrackMuSize++;
	
	cout<<" track mu size "<<TrackMuSize<<endl;
      
      }
    cout<< " out of track muon loop1 "<<endl;
  }
  cout<< " out of track muon loop2 "<<endl;
    */

  
  //===============================  Muons  =====================================================//
  // get muon collection                                                                                                                                       
  edm::Handle<edm::View<pat::Muon> >Muons;
  //iEvent.getByLabel("patMuons",Muons);
  iEvent.getByLabel("patMuonsWithTrigger",Muons);

  edm::View<pat::Muon>MuonColl=*Muons;
  int MuonSize =MuonColl.size();
  
  cout<<" no of pat muons : "<<MuonSize<<endl;  
  
  int GlobalMuNb=0;
  
  for ( int i=0 ; i < MuonSize ; ++i ){
    const pat::Muon& aMuon = (*Muons)[i];
    
    if(aMuon.isGlobalMuon())GlobalMuNb++;
    //cout<<" sel pat muon "<< selPATMuon(aMuon) <<endl;
    
    if(!aMuon.isGlobalMuon()) continue;
    
    //if(!(selPATMuon(aMuon))) continue;

    TVector3 vmuon1(aMuon.px(),aMuon.py(),aMuon.pz());
    float muon1_e =sqrt((vmuon1.Mag()*vmuon1.Mag())+(massMu*massMu));
    TLorentzVector lmuon1(vmuon1,muon1_e);
    
    for ( int j=i+1 ; j < MuonSize ; ++j){
      const pat::Muon& bMuon = (*Muons)[j];
      
      //if(!(selPATMuon(bMuon))) continue;
      
       if(!bMuon.isGlobalMuon()) continue;
       
       TVector3 vmuon2(bMuon.px(),bMuon.py(),bMuon.pz());
       float muon2_e =sqrt((vmuon2.Mag()*vmuon2.Mag())+(massMu*massMu));
       TLorentzVector lmuon2(vmuon2,muon2_e);
       TLorentzVector dimuon = lmuon1+lmuon2;
       int Charge=aMuon.charge()+bMuon.charge();
       DiMuMass[DiMuSize]= dimuon.M();
       DiMuPt[DiMuSize]=dimuon.Pt();     
       DiMuRap[DiMuSize]=dimuon.Rapidity();
       DiMuPx[DiMuSize]=dimuon.Px();
       DiMuPy[DiMuSize]=dimuon.Py();
       DiMuPz[DiMuSize]=dimuon.Pz();
       DiMuCharge[DiMuSize]=Charge;
       
       Mu1Mass[DiMuSize]=massMu;
       Mu1Pt[DiMuSize]=aMuon.pt();
       Mu1Eta[DiMuSize]=aMuon.eta();
       Mu1Px[DiMuSize]=aMuon.px();
       Mu1Py[DiMuSize]=aMuon.py();
       Mu1Pz[DiMuSize]=aMuon.pz();
       Mu1Phi[DiMuSize]=aMuon.phi();
       Mu1Charge[DiMuSize]=aMuon.charge();
       
       if(aMuon.isGlobalMuon()){Mu1_global[DiMuSize]=1;} else {Mu1_global[DiMuSize]=0;}
       if(aMuon.isTrackerMuon()){Mu1_tracker[DiMuSize]=1;} else {Mu1_tracker[DiMuSize]=0;}
       if(aMuon.isStandAloneMuon()){Mu1_standalone[DiMuSize]=1;} else{Mu1_standalone[DiMuSize]=0;} 
       

       
       if(!aMuon.triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()) {Mu1_trigger1[DiMuSize]=1;} else {Mu1_trigger1[DiMuSize]=0;}
       if(!aMuon.triggerObjectMatchesByPath("HLT_HIL2DoubleMu3_v1").empty()) {Mu1_trigger2[DiMuSize]=1;} else {Mu1_trigger2[DiMuSize]=0;}
       if(!aMuon.triggerObjectMatchesByPath("HLT_HIL3Mu3_v1").empty()) {Mu1_trigger3[DiMuSize]=1;} else {Mu1_trigger3[DiMuSize]=0;}
       
       if( aMuon.isGlobalMuon() &&  aMuon.isTrackerMuon())
	 {
	   TrackRef iTrack =aMuon.innerTrack();
	   const reco::HitPattern& p1=iTrack->hitPattern();
	   Mu1_arbitrated[DiMuSize]=aMuon.muonID("TrackerMuonArbitrated");
	   Mu1_found[DiMuSize]=iTrack->found();
	   Mu1_nchi2In[DiMuSize]=iTrack->chi2()/iTrack->ndof();
	   Mu1_pixeLayers[DiMuSize]=p1.pixelLayersWithMeasurement();
	   Mu1_dxy[DiMuSize]=iTrack->dxy(RefVtx);
	   Mu1_dz[DiMuSize]=iTrack->dz(RefVtx);
	  if(aMuon.isGlobalMuon())
	    {
	      TrackRef gTrack =aMuon.globalTrack();
	      const reco::HitPattern& q1=gTrack->hitPattern();
	      Mu1_nValidMuHits[DiMuSize]=q1.numberOfValidMuonHits();
	      Mu1_nchi2Gl[DiMuSize]=gTrack->chi2()/gTrack->ndof();
	    }
	  
	  else {Mu1_nValidMuHits[DiMuSize]=-999;Mu1_nchi2Gl[DiMuSize]=-999;}
	  
	}
       else{Mu1_arbitrated[DiMuSize]=-999;Mu1_found[DiMuSize]=-999;Mu1_nchi2In[DiMuSize]=-999;Mu1_pixeLayers[DiMuSize]=-999;Mu1_dxy[DiMuSize]=-999;Mu1_dz[DiMuSize]=-999;}
       
       Mu2Mass[DiMuSize]=massMu;
       Mu2Pt[DiMuSize]=bMuon.pt();
       Mu2Eta[DiMuSize]=bMuon.eta();
       Mu2Px[DiMuSize]=bMuon.px();
       Mu2Py[DiMuSize]=bMuon.py();
       Mu2Pz[DiMuSize]=bMuon.pz();
       Mu2Phi[DiMuSize]=bMuon.phi();
       Mu2Charge[DiMuSize]=bMuon.charge();
       
      
       if(bMuon.isGlobalMuon()){Mu2_global[DiMuSize]=1;} else {Mu2_global[DiMuSize]=0;}
       if(bMuon.isTrackerMuon()){Mu2_tracker[DiMuSize]=1;} else {Mu2_tracker[DiMuSize]=0;}
       if(bMuon.isStandAloneMuon()){Mu2_standalone[DiMuSize]=1;} else{Mu2_standalone[DiMuSize]=0;}
       
       
      
       if(!bMuon.triggerObjectMatchesByPath("HLT_HIL1DoubleMu0_HighQ_v1").empty()) {Mu2_trigger1[DiMuSize]=1;} else {Mu2_trigger1[DiMuSize]=0;}
       if(!bMuon.triggerObjectMatchesByPath("HLT_HIL2DoubleMu3_v1").empty()) {Mu2_trigger2[DiMuSize]=1;} else {Mu2_trigger2[DiMuSize]=0;}
       if(!bMuon.triggerObjectMatchesByPath("HLT_HIL3Mu3_v1").empty()) {Mu2_trigger3[DiMuSize]=1;} else {Mu2_trigger3[DiMuSize]=0;}
       
      
      
       //cout<<"mu2 : global "<<Mu2_global[DiMuSize]<<" tracker "<<Mu2_tracker[DiMuSize]<<" sa "<<Mu2_standalone[DiMuSize]<<endl; 
      
       if( bMuon.isGlobalMuon() &&  bMuon.isTrackerMuon())
	 {
	   TrackRef iTrack =bMuon.innerTrack();
	   const reco::HitPattern& p2=iTrack->hitPattern();
	   Mu2_arbitrated[DiMuSize]=bMuon.muonID("TrackerMuonArbitrated");
	   Mu2_found[DiMuSize]=iTrack->found();
	   Mu2_nchi2In[DiMuSize]=iTrack->chi2()/iTrack->ndof();
	   Mu2_pixeLayers[DiMuSize]=p2.pixelLayersWithMeasurement();
	   Mu2_dxy[DiMuSize]=iTrack->dxy(RefVtx);
	   Mu2_dz[DiMuSize]=iTrack->dz(RefVtx);
	   if(bMuon.isGlobalMuon())
	     {
	       TrackRef gTrack =bMuon.globalTrack();
	       const reco::HitPattern& q2=gTrack->hitPattern();
	       Mu2_nValidMuHits[DiMuSize]=q2.numberOfValidMuonHits();
	       Mu2_nchi2Gl[DiMuSize]=gTrack->chi2()/gTrack->ndof();
	     }
	   else {Mu2_nValidMuHits[DiMuSize]=-999;Mu2_nchi2Gl[DiMuSize]=-999;}
	   
	}
       else{Mu2_arbitrated[DiMuSize]=-999;Mu2_found[DiMuSize]=-999;Mu2_nchi2In[DiMuSize]=-999;Mu2_pixeLayers[DiMuSize]=-999;Mu2_dxy[DiMuSize]=-999;Mu2_dz[DiMuSize]=-999;}
       //cout<<"Mu2_found[DiMuSize] "<<Mu2_found[DiMuSize]<<endl;
       //cout<<" DiMuMass "<<DiMuMass[DiMuSize]<< " DiMuCharge  "<< DiMuCharge[DiMuSize]<<endl;
       DiMuSize++;
    }//j loop
  }//i loop
  
  glbMuNb=GlobalMuNb;
  nDiMuon=DiMuSize;
  cout<<" di mu size "<<nDiMuon<<" glb Mu "<<GlobalMuNb<<endl;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiMuonUPCTree);



