#!/bin/sh

infile="/store/relval/CMSSW_3_3_0_pre6/RelValHydjetQ_MinBias_4TeV/GEN-SIM-RAW/MC_31X_V9-v1/0005/1CCBE44D-F1AF-DE11-A698-001D09F27003.root"
outfile="gammajet20_MIX_RECO.root"

cmsDriver.py Pyquen_GammaJet_pt20_4TeV_cfi.py -n 1 \
-s GEN:hiSignal,SIM,DIGI,L1,DIGI2RAW,RAW2DIGI,RECO  --scenario HeavyIons --himix \
--conditions FrontierConditions_GlobalTag,MC_31X_V9::All \
--datatier 'GEN-SIM-RAW-RECO' --eventcontent=FEVTDEBUG \
--filein=$infile --fileout=$outfile --processName 'HISIGNAL' \
--no_exec
