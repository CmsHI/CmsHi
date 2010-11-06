import FWCore.ParameterSet.Config as cms

ecalHistProducer = cms.EDAnalyzer("EcalHistProducer",
                                  photonProducer = cms.InputTag("photons"),
                                  ebReducedRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEB"),
                                  eeReducedRecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE"),
                                  basicClusterBarrel        = cms.InputTag("islandBasicClusters","islandBarrelBasicClusters"),
                                  basicClusterEndcap        = cms.InputTag("islandBasicClusters","islandEndcapBasicClusters"),
                                  superClusterBarrel        = cms.InputTag("correctedIslandBarrelSuperClusters"),
                                  superClusterEndcap        = cms.InputTag("correctedIslandEndcapSuperClusters"),

                                  RHCent    = cms.untracked.bool(True),
                                  BCCent    = cms.untracked.bool(True),
                                  SCCent    = cms.untracked.bool(True),
                                  RHetaCent    = cms.untracked.bool(True),
                                  BCetaCent    = cms.untracked.bool(True),
                                  SCetaCent    = cms.untracked.bool(True),
                                  RHphiCent    = cms.untracked.bool(True),
                                  BCphiCent    = cms.untracked.bool(True),
                                  SCphiCent    = cms.untracked.bool(True),
                                  )

