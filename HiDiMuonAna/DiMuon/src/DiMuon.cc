// -*- C++ -*-
//
// Package:    DiMuon
// Class:      DiMuon
// 
/**\class DiMuon DiMuon.cc DiMuonAna/DiMuon/src/DiMuon.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Vineet Kumar,,,
//         Created:  Wed May 12 13:45:14 CEST 2010
// $Id: DiMuon.cc,v 1.1 2010/05/14 10:44:32 kumarv Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//
// class declaration
//

class DiMuon : public edm::EDAnalyzer {
   public:
      explicit DiMuon(const edm::ParameterSet&);
      ~DiMuon();


   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------
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
DiMuon::DiMuon(const edm::ParameterSet& iConfig)

{
   //now do what ever initialization is needed

}


DiMuon::~DiMuon()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
DiMuon::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;



#ifdef THIS_IS_AN_EVENT_EXAMPLE
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);
#endif
   
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
#endif
}


// ------------ method called once each job just before starting event loop  ------------
void 
DiMuon::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DiMuon::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(DiMuon);
