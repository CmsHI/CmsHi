import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
import os 

process = cms.Process("SEEDVALIDATOR")
process.load("Configuration/StandardSequences/GeometryPilot2_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("FWCore.MessageService.MessageLogger_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V8::All'

###################

# message logging
process.MessageLogger.categories = ['TrackAssociator', 'TrackValidator']
process.MessageLogger.debugModules = ['*']
process.MessageLogger.cerr = cms.untracked.PSet(
    threshold = cms.untracked.string('DEBUG'),
    DEBUG = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
	INFO = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    TrackAssociator = cms.untracked.PSet(
        limit = cms.untracked.int32(0)
    ),
    TrackValidator = cms.untracked.PSet(
        limit = cms.untracked.int32(-1)
    )
)

# memory check
process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
                                        ignoreTotal=cms.untracked.int32(0),
                                        oncePerEventMode = cms.untracked.bool(False)
                                        )

# timing service
process.Timing = cms.Service("Timing")

###################

# setup 'standard'  options
options = VarParsing.VarParsing ('standard')

# register custom variables
options.register('skipEv', 0, options.multiplicity.singleton, options.varType.int, "Number of events to skip (default=0)")		

# setup any defaults you want
options.maxEvents = -1 
options.output = 'seedVal.root'
inDir = "/pnfs/cmsaf.mit.edu/hibat/cms/mc/hydjet_b4_310"
for seqNo in range(0,20):
	options.files.append( 'dcache:%s/HYDJET_MC31XV3_HighPtCleaner_adaptiveVtx_RECO_%d.root' % (inDir,seqNo) )

# get and parse the command line arguments
options.parseArguments()

# setup pool source
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(options.maxEvents)
)
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(options.files), 
	skipEvents = cms.untracked.uint32(options.skipEv)
)

###################

# quality cuts on tracking particles
process.load("Validation.RecoTrack.cuts_cff")
process.load("CmsHi.TrackAnalysis.findableSimTracks_cfi")
process.findableSimTracks.minHit = 3
process.findableSimTracks.signalOnly = True

# track associator settings
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.TrackAssociatorByHits.SimToRecoDenominator = 'reco' #quality = purity i.e. (shared/reco)
process.TrackAssociatorByHits.ThreeHitTracksAreSpecial = True #require all hits shared on 3-hit tracks

# setup track seed validator
process.load("Validation.RecoTrack.TrackerSeedValidator_cff")
process.trackerSeedValidator.associators = cms.vstring('TrackAssociatorByHits')
process.trackerSeedValidator.label = cms.VInputTag(cms.InputTag("hiPixelTrackSeeds"))
process.trackerSeedValidator.label_tp_fake = cms.InputTag("cutsTPFake")
process.trackerSeedValidator.label_tp_effic = cms.InputTag("findableSimTracks")
process.trackerSeedValidator.outputFile = options.output

###################

# paths
process.pcut  = cms.Path(process.findableSimTracks * process.cutsTPFake)
process.pval  = cms.Path(process.trackerSeedValidator)




