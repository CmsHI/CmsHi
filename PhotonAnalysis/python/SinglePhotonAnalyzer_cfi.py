#
# \version $Id: SinglePhotonAnalyzer_cfi.py,v 1.3 2010/10/19 14:34:40 yjlee Exp $
# 

import FWCore.ParameterSet.Config as cms

singlePhotonAnalyzer = cms.EDAnalyzer("SinglePhotonAnalyzer",
                                      verbose                   = cms.untracked.bool(False),
                                      FillMCNTuple              = cms.untracked.bool(True),
                                      isMC_      = cms.untracked.bool(False),
                                      DoL1Objects               = cms.untracked.bool(False),
                                      StorePhysVectors          = cms.untracked.bool(False),         
                                      OutputFile                = cms.string('test.root'),
                                      HepMCProducer             = cms.InputTag("generator"),
                                      GenParticleProducer       = cms.InputTag("hiGenParticles"),
                                      GenEventScale             = cms.InputTag("generator"),
                                      PhotonProducer            = cms.InputTag("selectedPatPhotons"),
                                      TrackProducer             = cms.InputTag("hiSelectedTracks"),
                                      JetProducer               = cms.InputTag("cleanLayer1Jets"),
    METProducer               = cms.InputTag("layer1METs"),
    VertexProducer            = cms.InputTag("offlinePrimaryVertices"),
    BeamSpotProducer          = cms.InputTag("offlineBeamSpot" ),
                                      ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
                                      eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
                                      basicClusterBarrel        = cms.InputTag("islandBasicClusters:islandBarrelBasicClusters"),
                                      basicClusterEndcap        = cms.InputTag("islandBasicClusters:islandEndcapBasicClusters"),
                                      compPhotonProducer   = cms.InputTag("compleSuperCluster"),
    hbhe                      = cms.InputTag("hbhereco"),
    hf                      = cms.InputTag("hfreco"),
    ho                      = cms.InputTag("horeco"),

    caloTowers                = cms.InputTag("towerMaker"),                                     
    HltTriggerResults         = cms.InputTag("TriggerResults::HLT"),
    L1gtReadout               = cms.InputTag("gtDigis"),
    L1IsolTag                 = cms.InputTag("l1extraParticles","Isolated"),
    L1NonIsolTag              = cms.InputTag("l1extraParticles","NonIsolated"),
    # The results of these trigger paths will get stored in the TTree by their name
    TriggerPathsToStore       = cms.vstring("HLT_MinBias", "HLT_MinBiasBSC", "HLT_MinBiasBSC_NoBPTX", "HLT_MinBiasBSC_OR",
                                            "HLT_L1SingleEG5",
                                            "HLT_L1SingleEG8",
                                            "HLT_Photon10_L1R",
                                            "HLT_Photon15_L1R",
                                            "HLT_Photon20_L1R"),
    GammaPtMin                = cms.untracked.double(15),
    GammaEtaMax               = cms.untracked.double(1.6),
    McPtMin                   = cms.untracked.double(5),
    McEtaMax                  = cms.untracked.double(1.6),
    EcalBarrelMaxEta          = cms.untracked.double(1.45),
    EcalEndcapMinEta          = cms.untracked.double(1.55),
    JetPtMin                  = cms.untracked.double(20),
    doStoreGeneral            = cms.untracked.bool(True),
    doStoreCentrality         = cms.untracked.bool(True),
    doStoreL1Trigger          = cms.untracked.bool(True),
    doStoreHLT                = cms.untracked.bool(True),
    doStoreHF                 = cms.untracked.bool(True),
    doStoreVertex             = cms.untracked.bool(True),
    doStoreMET                = cms.untracked.bool(True),
    doStoreJets               = cms.untracked.bool(True),
    doStoreCompCone           = cms.untracked.bool(True),





)


