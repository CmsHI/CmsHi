//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec  7 12:51:57 2011 by ROOT version 5.27/06b
// from TTree pfTree/dijet tree
// found on file: ../HiForestDijet_v2.root
//////////////////////////////////////////////////////////
#include "commonSetup.h"
#include <iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>

class PFs {
public :
   PFs(){};
   ~PFs(){};

   // Declaration of leaf types
   Int_t           nPFpart;
   Int_t           pfId[12728];   //[nPFpart]
   Float_t         pfPt[12728];   //[nPFpart]
   Float_t         pfEta[12728];   //[nPFpart]
   Float_t         pfPhi[12728];   //[nPFpart]

   // List of branches
   TBranch        *b_nPFpart;   //!
   TBranch        *b_pfId;   //!
   TBranch        *b_pfPt;   //!
   TBranch        *b_pfEta;   //!
   TBranch        *b_pfPhi;   //!

};


void setupPFTree(TTree *t,PFs &tPFs,bool doCheck = 0)
{
   // Set branch addresses and branch pointers
   t->SetBranchAddress("nPFpart", &tPFs.nPFpart, &tPFs.b_nPFpart);
   t->SetBranchAddress("pfId", tPFs.pfId, &tPFs.b_pfId);
   t->SetBranchAddress("pfPt", tPFs.pfPt, &tPFs.b_pfPt);
   t->SetBranchAddress("pfEta", tPFs.pfEta, &tPFs.b_pfEta);
   t->SetBranchAddress("pfPhi", tPFs.pfPhi, &tPFs.b_pfPhi);
   if (doCheck) {
      if (t->GetMaximum("nPFpart")>12728) cout <<"FATAL ERROR: Arrary size of nPFpart too small!!!  "<<t->GetMaximum("nPFpart")<<endl;
   }
}

