# Lxplus Batch Job Script

set CMSSW_PROJECT_SRC="CMS/CMSSW_rel/CMSSW_9_3_2/src"
set X509_USER_PROXY=/afs/cern.ch/user/v/vpalladi/CMS/voms/x509up_u`id -u`
gfal-copy srm://gfe02.grid.hep.ph.ic.ac.uk/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/vpalladi/TPG/CMSSW_9_3_7PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root /tmp/vpalladi

cp /afs/cern.ch/user/v/vpalladi/CMS/HGC-CMSSW-ANLYSIS/bin/normForest.exe .
cp /afs/cern.ch/user/v/vpalladi/CMS/HGC-CMSSW-ANLYSIS/Dict* .
./normForest.exe -i /tmp/vpalladi/PhaseIITDRFall17DR__SinglePiPt100Eta1p6_2p8__GEN-SIM-RECO__PU200FEVT_93X_upgrade2023_realistic_v2-v1__0694AE79-EEB0-E711-A16F-A4BF0112DFA0_1.root -o fileout.root -s 1 -e -v 3 -r 0.03
gfal-copy fileout.root srm://gfe02.grid.hep.ph.ic.ac.uk/pnfs/hep.ph.ic.ac.uk/data/cms/store/user/vpalladi
