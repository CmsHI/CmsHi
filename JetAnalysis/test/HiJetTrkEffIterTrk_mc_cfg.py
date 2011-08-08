import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process('JetAna')

# setup runtime options
options = VarParsing.VarParsing ('standard')
options.register('sampleType', 0, # by default
	VarParsing.VarParsing.multiplicity.singleton,
	VarParsing.VarParsing.varType.int,
	"0: non-embedded, 1: embedded")
options.parseArguments()

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

# Input source
process.source = cms.Source("PoolSource",
	fileNames = cms.untracked.vstring(
	# mb
	'/store/himc/Fall10/Hydjet_Bass_MinBias_2760GeV/GEN-SIM-RECODEBUG/START39_V7HI-v1/0002/DCA084E3-9DFA-DF11-B873-001E68862A77.root'
	# embedded
	#'/store/user/davidlw/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_Pt170_GEN-SIM-RECO_393_set1/fae6fe9048513d9ac8f476dd10ba6ba7/hiReco_RAW2DIGI_RECO_99_1_Zza.root'
	))

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(5))

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
# pair-seeded step
process.load("RecoHI.HiTracking.secondStep_cff")                            # pair-seeded step

#process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.GlobalTag.globaltag = 'START39_V7::All'

# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)
process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFhits"),
	nonDefaultGlauberModel = cms.string("Hydjet_Bass"),
	centralitySrc = cms.InputTag("hiCentrality")
	)

# tree output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string("JetTrkTree.root"))

# Define Jet energy correction
process.jec = cms.ESSource("PoolDBESSource",
	DBParameters = cms.PSet(messageLevel = cms.untracked.int32(0)),
	timetype = cms.string('runnumber'),
	toGet = cms.VPSet(
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_hiGoodTightTracks_D6T_399_IC5Calo"),
		label = cms.untracked.string("IC5Calo")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK3PF"),
		label = cms.untracked.string("AK3PF")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK4PF"),
		label = cms.untracked.string("AK4PF")),
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
		tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_399_AK5PF"),
		label = cms.untracked.string("AK5PF")
		),
	),
	connect = cms.string("sqlite_file:JEC_HI_PFTowers_hiGoodTightTracks_D6T_399.db"),
)
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

# Define Analysis sequencues
process.load('CmsHi.JetAnalysis.ExtraGenReco_cff')
process.load('CmsHi.JetAnalysis.ExtraTrackReco_cff')
process.load('CmsHi.JetAnalysis.ExtraPfReco_cff')
process.load('CmsHi.JetAnalysis.ExtraJetReco_cff')
process.load('CmsHi.JetAnalysis.PatAna_cff')
process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')
process.load('CmsHi.JetAnalysis.TrkAnalyzers_cff')
process.load('MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff')
process.hiPixelAdaptiveVertex.useBeamConstraint = cms.bool(False) # better data vs mc comparison

# matt's iterative tracking
process.load("MNguyen.iterTracking.TrackSelections_cff")
process.hiGoodTightTracks.keepAllTracks = False
process.load("MNguyen.iterTracking.secondStep_triplet_cff")
process.load("MNguyen.iterTracking.thirdStep_pairs_cff")
process.load("MNguyen.iterTracking.MergeTrackCollectionsHI_cff")
process.hiNewClusters.Strip.maxSize = cms.uint32(2)

process.iterGlobalPrimTracks = process.iterTracks.clone(
    TrackProducer1 = 'hiScndGlobalPrimTracks',
    TrackProducer2 = 'hiThrdGlobalPrimTracks',
    )


process.hiGeneralGlobalPrimTracks = process.hiGeneralTracks.clone(
    TrackProducer1 = 'hiGlobalPrimTracks',
    TrackProducer2 = 'iterGlobalPrimTracks',
    )

process.globalPrimTrackCollectionMerging = cms.Sequence(
    process.iterGlobalPrimTracks*
    process.hiGeneralGlobalPrimTracks
    )

process.iterTracking_seq = cms.Sequence(
    process.hiTrackReReco *
    process.hiGoodTightTracks *
    process.secondStep *
    process.thirdStep *
    process.globalPrimTrackCollectionMerging *
    process.trackCollectionMerging
    )

# pf
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGeneralGlobalPrimTracks")
process.trackerDrivenElectronSeeds.UseQuality = cms.bool(False)

# analyzer
process.anaTrack.useCentrality = cms.untracked.bool(True)
process.anaTrack.trackPtMin = cms.untracked.double(4)
process.anaTrack.simTrackPtMin = cms.untracked.double(4)
process.anaTrack.doSimTrack = cms.untracked.bool(True)
process.anaTrack.trackSrc = cms.InputTag('hiGeneralGlobalPrimTracks')
process.anaTrack.tpFakeSrc = cms.untracked.InputTag('cutsTPForFak')
process.anaTrack.tpEffSrc = cms.untracked.InputTag('cutsTPForEff')
process.anaTrack.doPFMatching = cms.untracked.bool(True)

process.anaTrack_hgt = process.anaTrack.clone(trackSrc = 'hiGoodTightTracks')
process.anaTrack_hgt.trackPtMin = cms.untracked.double(20)


# fill tree
process.hitrkEffAnalyzer_akpu3pf.fillNtuples = cms.bool(False)
process.hitrkEffAnalyzer_akpu3pf.trkPtMin = cms.double(-1)
process.hitrkEffAnalyzer_akpu3pf.tracks = cms.untracked.InputTag("hiGeneralGlobalPrimTracks")
# use calo jet
process.hitrkEffAnalyzer_akpu3pf.jets = cms.untracked.InputTag("icPu5patJets")

process.hitrkEffAnalyzer_generaltrk = process.hitrkEffAnalyzer_akpu3pf.clone(tracks = cms.untracked.InputTag("hiGeneralTracks"))
process.hitrkEffAna_akpu3pf *= process.hitrkEffAnalyzer_generaltrk

process.reco_extra = cms.Path( process.hiGen * process.iterTracking_seq * process.HiParticleFlowRecoNoJets)
process.reco_extra_jet = cms.Path( process.iterativeConePu5CaloJets )
process.pat_step = cms.Path(process.icPu5patSequence)
process.ana_step = cms.Path(process.hitrkEffAna_akpu3pf * process.icPu5JetAnalyzer * process.anaTrack * process.anaTrack_hgt)

# Customization
from CmsHi.JetAnalysis.customise_cfi import *
useSampleType(process,options.sampleType)
#enableDataPat(process)
#enableDataAnalyzers(process)
#enableOpenHlt(process,process.ana_step)

# =============== Final Schedule =====================
process.schedule = cms.Schedule(process.reco_extra,process.reco_extra_jet,process.pat_step,process.ana_step)

