// -*- C++ -*-
//
// Package:    HiEcalRecHitSpikeFilter
// Class:      HiEcalRecHitSpikeFilter
// 
/**\class HiEcalRecHitSpikeFilter HiEcalRecHitSpikeFilter.cc RecoHI/HiEgammaAlgos/plugins/HiEcalRecHitSpikeFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Yong Kim,32 4-A08,+41227673039,
//         Created:  Wed Oct 27 23:56:49 CEST 2010
// $Id: HiEcalRecHitSpikeFilter.cc,v 1.1 2010/10/28 16:02:06 kimy Exp $
//
//




// system include files
/*
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"

#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatusCode.h"




//
// class declaration
//

using namespace edm;
using namespace std;
using namespace reco;


class HiEcalRecHitSpikeFilter : public edm::EDFilter {
   public:
      explicit HiEcalRecHitSpikeFilter(const edm::ParameterSet&);
      ~HiEcalRecHitSpikeFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
   edm::InputTag ebReducedRecHitCollection_;
   double minEt_;
   double swissThreshold_;

   
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
HiEcalRecHitSpikeFilter::HiEcalRecHitSpikeFilter(const edm::ParameterSet& iConfig)
{
   //now do what ever initialization is needed
   ebReducedRecHitCollection_       = iConfig.getParameter<edm::InputTag>("ebReducedRecHitCollection"); //,"reducedEcalRecHitsEB");
    minEt_                          = iConfig.getParameter<double>("minEt");
    swissThreshold_                 = iConfig.getParameter<double>("swissThreshold");
}


HiEcalRecHitSpikeFilter::~HiEcalRecHitSpikeFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
HiEcalRecHitSpikeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   //using namespace edm;
   //using namespace std;
   //using namespace reco;

   //grab rechits
   edm::Handle<EcalRecHitCollection> EBReducedRecHits;
   iEvent.getByLabel(ebReducedRecHitCollection_, EBReducedRecHits);
   const EcalRecHitCollection *rechits = EBReducedRecHits.product();

   if(rechits) {
       for(EcalRecHitCollection::const_iterator it=rechits->begin(); it!=rechits->end(); it++) {
           if(it->energy() > minEt_ && EcalSeverityLevelAlgo::swissCross(it->id(), *rechits,0,true) > swissThreshold_)
               return false;
       }
   }
   return true;
}

// ------------ method called once each job just before starting event loop  ------------
void 
HiEcalRecHitSpikeFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
HiEcalRecHitSpikeFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(HiEcalRecHitSpikeFilter);
