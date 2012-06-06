import FWCore.ParameterSet.Config as cms

from MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff import *

anaTrack.trackPtMin = 0.5
anaTrack.useQuality = False
anaTrack.doPFMatching = True
anaTrack.pfCandSrc = cms.InputTag("particleFlowTmp")
anaTrack.trackSrc = cms.InputTag("hiGeneralCaloMatchedTracks")

pixelTrack = anaTrack.clone(trackSrc = cms.InputTag("hiConformalPixelTracks"))
pixelTrack.useQuality = False
pixelTrack.trackPtMin = 0.5

mergedTrack = pixelTrack.clone(trackSrc = cms.InputTag("hiMergedTracks"))


