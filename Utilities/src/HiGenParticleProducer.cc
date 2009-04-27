#ifndef HiCandidateAlgos_HiGenParticleProducer_h
#define HiCandidateAlgos_HiGenParticleProducer_h
/* \class HiGenParticleProducer
 *
 * Combine generator-level particles from all signal and pileup
 * HepMCProducts in the CrossingFrame and produce a 
 * GenParticleCollection and a SubEventMap that maps candidates 
 * onto separate SubEvents.
 *
 * Based on PhysicsTools/HepMCCandAlgos/plugins/GenParticleProducer
 * 
 * $Date: 2009/01/12 10:46:40 $
 * $Revision: 1.1 $
 * \author Philip Allfrey, University of Auckland
 *
 */
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include <vector>
#include <map>
#include <set>

namespace edm { class ParameterSet; }
namespace HepMC { class GenParticle; class GenEvent; }

class HiGenParticleProducer : public edm::EDProducer {
 public:
  /// constructor
  HiGenParticleProducer( const edm::ParameterSet & );
  /// destructor
  ~HiGenParticleProducer();

 private:
  /// module init at begin of job
  void beginJob( const edm::EventSetup & );
  /// process one event
  void produce( edm::Event& e, const edm::EventSetup& );
  /// source collection name
  edm::InputTag src_;
  /// unknown code treatment flag
  bool abortOnUnknownPDGCode_;
  /// save bar-codes
  bool saveBarCodes_;
  /// charge indices
  std::vector<int> chargeP_, chargeM_;
  std::map<int, int> chargeMap_;
  int chargeTimesThree( int ) const;
};

#endif

#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/CrossingFrame/interface/CrossingFrame.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "SimDataFormats/CrossingFrame/interface/MixCollection.h"
#include "SimDataFormats/HiGenData/interface/SubEventMap.h"

#include <fstream>
#include <algorithm>
using namespace edm;
using namespace reco;
using namespace std;
using namespace HepMC;

static const int PDGCacheMax = 32768;
static const double mmToCm = 0.1;

HiGenParticleProducer::HiGenParticleProducer( const ParameterSet & cfg ) :
  src_( cfg.getUntrackedParameter<edm::InputTag>( "src",edm::InputTag("mix","source") ) ),
  abortOnUnknownPDGCode_( cfg.getUntrackedParameter<bool>( "abortOnUnknownPDGCode", true ) ),
  saveBarCodes_( cfg.getUntrackedParameter<bool>( "saveBarCodes", false ) ),
  chargeP_( PDGCacheMax, 0 ), chargeM_( PDGCacheMax, 0 ) {
  produces<GenParticleCollection>();
  produces<SubEventMap>();
  if( saveBarCodes_ ) {
    std::string alias( cfg.getParameter<std::string>( "@module_label" ) );
    //The following line copied from GenParticleProducer doesn't actually append BarCodes
    produces<vector<int> >().setBranchAlias( alias + "BarCodes" );
  }				  
}

HiGenParticleProducer::~HiGenParticleProducer() { 
}

int HiGenParticleProducer::chargeTimesThree( int id ) const {
  if( abs( id ) < PDGCacheMax ) 
    return id > 0 ? chargeP_[ id ] : chargeM_[ - id ];
  map<int, int>::const_iterator f = chargeMap_.find( id );
  if ( f == chargeMap_.end() ) 
    if ( abortOnUnknownPDGCode_ )
      throw edm::Exception( edm::errors::LogicError ) 
	<< "invalid PDG id: " << id << endl;
    else {
      return HepPDT::ParticleID(id).threeCharge();
    }
  return f->second;
}

void HiGenParticleProducer::beginJob( const EventSetup & es ) {
  ESHandle<HepPDT::ParticleDataTable> pdt;
  es.getData( pdt );
  for( HepPDT::ParticleDataTable::const_iterator p = pdt->begin(); p != pdt->end(); ++ p ) {
    const HepPDT::ParticleID & id = p->first;
    int pdgId = id.pid(), apdgId = abs( pdgId );
    int q3 = id.threeCharge();
    if ( apdgId < PDGCacheMax && pdgId > 0 ) {
      chargeP_[ apdgId ] = q3;
      chargeM_[ apdgId ] = -q3;
    } else if ( apdgId < PDGCacheMax ) {
      chargeP_[ apdgId ] = -q3;
      chargeM_[ apdgId ] = q3;
    } else {
      chargeMap_[ pdgId ] = q3; 
      chargeMap_[ -pdgId ] = -q3;
    } 
  }
}

void HiGenParticleProducer::produce( Event& evt, const EventSetup& es ) {
  Handle< CrossingFrame<HepMCProduct> > mcp;
  evt.getByLabel( src_, mcp );

  //Create MixCollection of HepMCProduct, iterator runs over all signal and pileup events
  //in CrossingFrame
  auto_ptr<MixCollection<HepMCProduct> > cfhepmcprod(new MixCollection<HepMCProduct>(mcp.product()));
  MixCollection<HepMCProduct>::iterator cfhepmc_iter;

  //Get total number of particles
  size_t totalSize = 0;
  for (cfhepmc_iter=cfhepmcprod->begin(); cfhepmc_iter!=cfhepmcprod->end();cfhepmc_iter++) {
    totalSize += cfhepmc_iter->GetEvent()->particles_size();
  }

  //Initialise containers
  const size_t size = totalSize;
  vector<const HepMC::GenParticle *> particles( size );
  auto_ptr<GenParticleCollection> candsPtr( new GenParticleCollection( size ) );
  auto_ptr<SubEventMap> subsPtr( new SubEventMap() );

  auto_ptr<vector<int> > barCodeVector( new vector<int>( size ) );
  const GenParticleRefProd ref = evt.getRefBeforePut<GenParticleCollection>();
  GenParticleCollection & cands = * candsPtr;
  SubEventMap & subs = *subsPtr;
  size_t offset = 0;
  size_t suboffset = 0;

  //Loop over all GenEvents
  for (cfhepmc_iter=cfhepmcprod->begin(); cfhepmc_iter!=cfhepmcprod->end();cfhepmc_iter++) {
    const GenEvent * mc = cfhepmc_iter->GetEvent();
    size_t num_particles = mc->particles_size();
    map<int, size_t> barcodes;

    /// fill indices
    GenEvent::particle_const_iterator begin = mc->particles_begin(), end = mc->particles_end();

    size_t idx = offset;
    for( GenEvent::particle_const_iterator p = begin; p != end; ++ p ) {
      const HepMC::GenParticle * particle = * p;
      size_t barCode = particle->barcode() + offset;
      size_t barCode_this_event = particle->barcode();
      if( barcodes.find(barCode) != barcodes.end() )
	throw cms::Exception( "WrongReference" )
	  << "barcodes are duplicated! " << endl;
      particles[idx] = particle;
      (*barCodeVector)[idx] = barCode;
      barcodes.insert( make_pair(barCode_this_event, idx ++) );
    }

    // fill output collection and save association
    for( size_t i = offset; i < offset + num_particles; ++ i ) {
      const HepMC::GenParticle * part = particles[ i ];
      Candidate::LorentzVector p4( part->momentum() );
      int pdgId = part->pdg_id();
      reco::GenParticle & cand = cands[ i ];
      cand.setThreeCharge( chargeTimesThree( pdgId ) );
      cand.setPdgId( pdgId );
      cand.setStatus( part->status() );
      cand.setP4( p4 );
      const GenVertex * v = part->production_vertex();
      if ( v != 0 ) {
	ThreeVector vtx = v->point3d();
	Candidate::Point vertex( vtx.x() * mmToCm, vtx.y() * mmToCm, vtx.z() * mmToCm );
	cand.setVertex( vertex );
      } else {
	cand.setVertex( Candidate::Point( 0, 0, 0 ) );
      }
      cand.resetDaughters( ref.id() );
    }

    // fill references to daughters
    for( size_t d = offset; d < offset + num_particles; ++ d ) {
      const HepMC::GenParticle * part = particles[ d ];
      const GenVertex * productionVertex = part->production_vertex();
      if ( productionVertex != 0 ) {
	 cout<<"Production Vertex Id : "<<productionVertex->id()<<endl;
	 int subid = productionVertex->id() + suboffset;
	 GenParticleRef dref( ref, d );
	 subs.insert(dref,subid);
	size_t numberOfMothers = productionVertex->particles_in_size();
	if ( numberOfMothers > 0 ) {
	  GenVertex::particles_in_const_iterator motherIt = productionVertex->particles_in_const_begin();

	  for( ; motherIt != productionVertex->particles_in_const_end(); motherIt++) {
	    const HepMC::GenParticle * mother = * motherIt;
	    size_t m = barcodes.find( mother->barcode() )->second;
	    cands[ m ].addDaughter( GenParticleRef( ref, d ) );  
	    cands[ d ].addMother( GenParticleRef( ref, m ) );  
	  }
	}
      }
    }

    HepMC::HeavyIon * hi = mc->heavy_ion();
    if(hi) suboffset += hi->Ncoll_hard();
    else suboffset += 1;

    offset += num_particles;
  }

  evt.put( candsPtr );
  evt.put( subsPtr );

  if(saveBarCodes_) evt.put( barCodeVector );
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE( HiGenParticleProducer );
  
