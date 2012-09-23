#ifndef SignalCorrector_h
#define SignalCorrector_h
#include "TF1.h"

//---------------------------------------------------------------------
class Region
{
public:
   Region(TString n, TString v, TCut c, TString w, int nm) :
   name(n),var(v),cut(c),weight(w),normMode(nm) {}
   void Init(TTree * t, int nbins, float *bins, float frac, float area=1.) {
      fraction = frac;
      cut*=weight;
      h = new TH1D(name,"",nbins,bins);
      cout << "  " << h->GetName() << " with fraction: " << fraction << " area: " << area << endl;
      float nSel = t->Project(h->GetName(),var,cut);
      cout << "  draw: " << var << " cut: " << TString(cut) << ": " << nSel << endl;
      hNorm = (TH1D*)h->Clone(Form("%sNorm",h->GetName()));
      if (area>0&&normMode>0&&h->Integral()>0) hNorm->Scale(area/h->Integral());
      hScaled = (TH1D*)hNorm->Clone(Form("%sScaled",hNorm->GetName()));
      if (fraction>0&&fraction<1e8) {
         cout << "  " << hScaled->GetName() << " scale by: " << fraction << endl;
         hScaled->Scale(fraction);
      }
      //for (int i=1; i<=hScaled->GetNbinsX()+1 ; ++i) cout << hScaled->GetBinLowEdge(i) << " (" << hScaled->GetBinContent(i) << ") ";
      //cout << endl;
      // check
      //t->Draw("cBin>>"+name+"_cBin(40,0,40)",cut,"goff");
   }
   
   TH1D * h;
   TH1D * hNorm;
   TH1D * hScaled;
   TString name;
   TString var;
   TCut cut;
   TString weight;
   int normMode;
   float fraction;
};

//---------------------------------------------------------------------
class SignalCorrector
{
public:
   SignalCorrector(TTree * tree, TString n, TString var, TCut s, TString w="(1==1)", int nm=1) : 
   name(n),
   observable(var),
   sel(s),
   rSigAll(n+"SignalAll",var,s,w,nm),
   rBkgDPhi(n+"BkgDPhi",var,s,w,nm),
   rBkgSShape(n+"BkgSShape",var,s,w,nm),
   rBkgSShapeDPhi(n+"BkgSShapeDPhi",var,s,w,nm),
   weight(w),
   normMode(nm), // 0=area is signal region count, 1=unit normalization, 2=per photon normalization
   subDPhiSide(true),
   subSShapeSide(true),
   subSShapeSideDPhiSide(true),
   cutBkgDPhi("jetEt>30&&acos(cos(photonPhi-jetPhi))>0.7 && acos(cos(photonPhi-jetPhi))<3.14159/2. && sigmaIetaIeta<0.01"),
   cutSShape("jetEt>30&&acos(cos(photonPhi-jetPhi))>2.0944 && sigmaIetaIeta>0.011 && sigmaIetaIeta<0.017"),
   cutSShapeDPhi("jetEt>30&&acos(cos(photonPhi-jetPhi))>0.7 && acos(cos(photonPhi-jetPhi))<3.14159/2. && sigmaIetaIeta>0.011 && sigmaIetaIeta<0.017"),   
   nSelPhoton(0),nSigAll(0),fracDPhiBkg(0),photonPurity(0),fracPhotonBkg(0),fracPhotonBkgDPhiBkg(0),area(1) {
      t = tree;
      hFracPhotonBkg = (TH1D*)gDirectory->FindObjectAny("hFracPhotonBkg");
      if (!hFracPhotonBkg) hFracPhotonBkg = new TH1D("hFracPhotonBkg","",5,0,5);
   }
   
   void SetPhotonIsolation(int isolScheme, int cBin)
   {
      //
      // Centrality bins: 0=0-4, 1=4-8, 2=8-12, 3=12-20, 4=20-40
      // 
      if (isolScheme==0) { //sum isol
//         nameIsol="Sum(Isol.)<5";
//         cutIsol = "sumIsol/0.9<5";
//         if (cBin==0) photonPurity=0.50;
//         if (cBin>=1&&cBin<=2) photonPurity=0.64;
//         if (cBin>=3&&cBin<=4) photonPurity=0.57;
         nameIsol="Sum(Isol.)<1GeV";
         cutIsol = "(cc4+cr4+ct4PtCut20)/0.9 <1";
         if (cBin==0) photonPurity=0.73;
         if (cBin>=1&&cBin<=2) photonPurity=0.77;
         if (cBin==3) photonPurity=0.79;
         if (cBin==4) photonPurity=0.84;
         hFracPhotonBkg->SetBinContent(1,0.73);
         hFracPhotonBkg->SetBinContent(2,0.77);
         hFracPhotonBkg->SetBinContent(3,0.77);
         hFracPhotonBkg->SetBinContent(4,0.79);
         hFracPhotonBkg->SetBinContent(5,0.84);
      } else if (isolScheme==1) { // cut isol
         nameIsol="3DCutIsol.";
         cutIsol = "cc4 < 6.9 && ct4PtCut20 < 3.00 && cr4<5";
         if (cBin==0) photonPurity=0.46;
         if (cBin>=1&&cBin<=2) photonPurity=0.62;
         if (cBin>=3&&cBin<=4) photonPurity=0.54;
      } else if (isolScheme==2) { // fisher isol
         nameIsol="Fisher Isol. > 0.3";
         t->SetAlias("fisherIsol","(4.5536204845644690e-01 +cc5*-1.1621087258504197e-03 +cc4*-1.3139962130657250e-02 +cc3*9.8272534188056666e-03 +cc2*-7.9659880964355362e-02 +cc1*5.6661268034678275e-02 +cr5*-1.2763802967154852e-02 +cr4*-1.2594575465310987e-03 +cr3*-1.3333157740152167e-02 +cr2*-2.5518237583408113e-02 +cr1*-1.3706749407235775e-02 +ct4PtCut20*-7.9844325658248016e-03 +ct3PtCut20*-2.5276510400767658e-03 +ct2PtCut20*-2.0741636383420897e-02 +ct1PtCut20*7.1545293456054884e-04 +ct5PtCut20*7.8080659557798627e-03)");
//         cutIsol = "fisherIsol>0.2";
//         if (cBin==0) photonPurity=0.62;
//         if (cBin>=1&&cBin<=2) photonPurity=0.66;
//         if (cBin>=3&&cBin<=4) photonPurity=0.64;
//         hFracPhotonBkg->SetBinContent(1,0.62);
//         hFracPhotonBkg->SetBinContent(2,0.66);
//         hFracPhotonBkg->SetBinContent(3,0.66);
//         hFracPhotonBkg->SetBinContent(4,0.64);
//         hFracPhotonBkg->SetBinContent(5,0.64);         
         cutIsol = "fisherIsol>0.3";
         if (cBin==0) photonPurity=0.76;
         if (cBin>=1&&cBin<=2) photonPurity=0.77;
         if (cBin==3) photonPurity=0.74;
         if (cBin==4) photonPurity=0.78;
         hFracPhotonBkg->SetBinContent(1,0.76);
         hFracPhotonBkg->SetBinContent(2,0.77);
         hFracPhotonBkg->SetBinContent(3,0.77);
         hFracPhotonBkg->SetBinContent(4,0.74);
         hFracPhotonBkg->SetBinContent(5,0.78);         
      }
      fracPhotonBkg=1-photonPurity;
      // Isolation Cut
      sel = sel&&cutIsol;      
   }
   
   void MakeHistograms(TCut sigSel, int nbin, float xmin, float xmax) {
      float * bins = new float[nbin+1];
      float dx = (xmax-xmin)/nbin;
      for (int i=0; i<nbin+1; ++i) {
         bins[i] = xmin+i*dx;
      }
      MakeHistograms(sigSel,nbin,bins);
   }
   
   void MakeHistograms(TCut sigSel, int nbin, float * bins, bool doScale=true) {
      // initialize vars
      fracDPhiBkg=0;
      if (!subSShapeSide) {
         fracPhotonBkg=0;
         fracPhotonBkgDPhiBkg=0;
      }
      // setup cuts
      cout << "Base Selection: " << sel << endl;
      rSigAll.cut = sel&&sigSel;
      rBkgDPhi.cut = sel&&cutBkgDPhi;
      rBkgSShape.cut = sel&&cutSShape;
      rBkgSShapeDPhi.cut = sel&&cutSShapeDPhi;
      
      // no scale mode (simply make the histograms and worry about the scale later)
      if (!doScale) {
         rSigAll.Init(t,nbin,bins,-1,-1);
         rBkgDPhi.Init(t,nbin,bins,-1,-1);
         rBkgSShape.Init(t,nbin,bins,-1,-1);
         rBkgSShapeDPhi.Init(t,nbin,bins,-1,-1);
         // done
         return;
      }
      
      // if doScale, then 1. find the scales 2. scale!
      // photon normalization
      nSelPhoton = t->GetEntries(sel&&"sigmaIetaIeta<0.01");
      // number of events in signal region
      nSigAll = t->GetEntries(rSigAll.cut);
      cout << " ** Number of selection photons: " << nSelPhoton << " gamma-jets: " << nSigAll << " ** " << endl;
      area=1.;
      if (normMode==0) area=nSigAll;
      if (normMode==2) area=nSigAll/nSelPhoton;
      rSigAll.Init(t,nbin,bins,1.,area);
      if (subDPhiSide) {
         float nDPhiSide = t->GetEntries(rBkgDPhi.cut);
         float nDPhiBkg = nDPhiSide * (3.14159-2.0944)/(3.14159/2.-0.7);
         fracDPhiBkg = nDPhiBkg/nSigAll;
         if (normMode>0) {
            rBkgDPhi.Init(t,nbin,bins,fracDPhiBkg,area);
         } else {
            rBkgDPhi.Init(t,nbin,bins,nDPhiBkg/nDPhiSide,-1);
         }
         cout << "  |dhpi| sig all = " << nSigAll << "|dphi| side = " << nDPhiSide << " bck contamination: " << nDPhiBkg << " = " << fracDPhiBkg << endl;
      }
      if (subSShapeSide) {
         cout << "  fracPhotonBkg: " << fracPhotonBkg << endl;
         if (normMode>0) {
            rBkgSShape.Init(t,nbin,bins,fracPhotonBkg,area);
         } else {
            float nSShapeSide = t->GetEntries(rBkgSShape.cut);
            rBkgSShape.Init(t,nbin,bins,fracPhotonBkg*nSigAll/nSShapeSide,-1);
         }
         if (subSShapeSideDPhiSide) {
            float nDPhiSide = t->GetEntries(rBkgSShapeDPhi.cut);
            float nDPhiBkg = nDPhiSide * (3.14159-2.0944)/(3.14159/2.-0.7);
            fracPhotonBkgDPhiBkg = nDPhiBkg/nSigAll;
            //fracPhotonBkgDPhiBkg = 0;
            if (normMode>0) {
               rBkgSShapeDPhi.Init(t,nbin,bins,fracPhotonBkgDPhiBkg,area);
            } else {
               rBkgSShapeDPhi.Init(t,nbin,bins,nDPhiBkg/nDPhiSide,-1);
            }
         }
      }
      // special case for dphi
      if (observable=="acos(cos(photonPhi-jetPhi))") {
         TF1 *p0 = new TF1("p0","pol0",0.7,3.14/2);
         if (subDPhiSide&&rBkgDPhi.hNorm->GetEntries()>10) {
            rBkgDPhi.hNorm->Fit("p0","0");
            for (int i=rBkgDPhi.hNorm->FindBin(0.7); i<rBkgDPhi.hNorm->GetNbinsX(); ++i) {
               rBkgDPhi.hNorm->SetBinContent(i,p0->GetParameter(0));
               rBkgDPhi.hNorm->SetBinError(i,p0->GetParError(0));
            }
            rBkgDPhi.hScaled->Fit("p0","0");
            for (int i=rBkgDPhi.hScaled->FindBin(0.7); i<rBkgDPhi.hScaled->GetNbinsX(); ++i) {
               rBkgDPhi.hScaled->SetBinContent(i,p0->GetParameter(0));
               rBkgDPhi.hScaled->SetBinError(i,p0->GetParError(0));
            }
         }
         if (subSShapeSideDPhiSide&&rBkgSShapeDPhi.hNorm->GetEntries()>10) {
            rBkgSShapeDPhi.hNorm->Fit("p0","0");
            for (int i=rBkgSShapeDPhi.hNorm->FindBin(0.7); i<rBkgSShapeDPhi.hNorm->GetNbinsX(); ++i) {
               rBkgSShapeDPhi.hNorm->SetBinContent(i,p0->GetParameter(0));
               rBkgSShapeDPhi.hNorm->SetBinError(i,p0->GetParError(0));
            }
            rBkgSShapeDPhi.hScaled->Fit("p0","0");
            for (int i=rBkgSShapeDPhi.hScaled->FindBin(0.7); i<rBkgSShapeDPhi.hScaled->GetNbinsX(); ++i) {
               rBkgSShapeDPhi.hScaled->SetBinContent(i,p0->GetParameter(0));
               rBkgSShapeDPhi.hScaled->SetBinError(i,p0->GetParError(0));
            }
         }
      }
   }

   void SubtractBkg(bool doSubtraction=true) {
      hSubtracted = (TH1D*)rSigAll.hScaled->Clone(name+"Subtracted");
      cout << "Raw area: " << hSubtracted->Integral() << endl;   // check rb
      float ajcut=0.12;
      float rb = hSubtracted->Integral(1,hSubtracted->FindBin(ajcut)-1);
      cout << "# Check raw RB " << hSubtracted->GetBinLowEdge(1) << " to " << hSubtracted->GetBinLowEdge(hSubtracted->FindBin(ajcut)) << ": " << rb << endl;
      if (!doSubtraction) return;
      
      if (subDPhiSide) {
         hSubtracted->Add(rBkgDPhi.hScaled,-1);
         cout << " - " << rBkgDPhi.hScaled->Integral();
      }
      if (subSShapeSide) {
         hSSSideDPhiSub = (TH1D*)rBkgSShape.hScaled->Clone(Form("%sDPhiSub",rBkgSShape.hScaled->GetName()));
         if (subSShapeSideDPhiSide) hSSSideDPhiSub->Add(rBkgSShapeDPhi.hScaled,-1);
         hSubtracted->Add(hSSSideDPhiSub,-1);
         cout << " - " << hSSSideDPhiSub->Integral();
      }
      cout << "=? " << hSubtracted->Integral() << endl;
      if (normMode>0) {
         rb = hSubtracted->Integral(1,hSubtracted->FindBin(ajcut)-1);
         //cout << "# Check RB before rescale " << hSubtracted->GetBinLowEdge(1) << " to " << hSubtracted->GetBinLowEdge(hSubtracted->FindBin(ajcut)) << ": " << rb << endl;
         // Rescale after subtraction
         //cout << "old area: " << area << ", to be scaled: " << fracDPhiBkg << " " << fracPhotonBkg << " " << fracPhotonBkgDPhiBkg << endl;
         if (normMode==2) area*=(1-fracDPhiBkg-(fracPhotonBkg-fracPhotonBkgDPhiBkg))/(1-fracPhotonBkg);
         //cout << "rescaled area: " << area << endl;
         rSigAll.hScaled->Scale(area/rSigAll.hScaled->Integral());
         //if (subSShapeSide) rBkgSShape.hScaled->Scale(area/rBkgSShape.hScaled->Integral());
         hSubtracted->Scale(area/hSubtracted->Integral());
         //cout << "After Subtraction area: " << hSubtracted->Integral() << endl;
      }
   }
   
   void ScaleToPureSignal(TH1D * h, bool subDPhiSideInBin, bool subSShapeSideInBin, float dphiSigCut=2.0944, int histMode=1) {
      TH1D * hSigAllTmp = (TH1D*)rSigAll.hNorm->Clone("hSigAllTmp");
      TH1D * hDPhiSideTmp = (TH1D*)rSigAll.hNorm->Clone("hDPhiSideTmp");
      TH1D * hSShapeSideTmp = (TH1D*)rSigAll.hNorm->Clone("hSShapeSideTmp");
      TH1D * hSShapeSideDPhiTmp = (TH1D*)rSigAll.hNorm->Clone("hSShapeSideDPhiTmp");
      for (int i=1; i<=h->GetNbinsX(); ++i) {
         TCut varCut = Form("%s>=%.4f&&%s<%.4f",rSigAll.var.Data(),h->GetBinLowEdge(i),rSigAll.var.Data(),h->GetBinLowEdge(i+1));
         float fracDPhiBkgInBin=0, fracSShapeBkgInBin=0,fracSShapeBkgDPhiBkgInBin=0;
         // Count
         float nSigAllInBin = t->Project("hSigAllTmp",observable,rSigAll.cut&&varCut);
         float nDPhiSideInBin = t->Project("hDPhiSideTmp",observable,rBkgDPhi.cut&&varCut);
         float nSShapeSideInBin = t->Project("hSShapeSideTmp",observable,rBkgSShape.cut&&varCut);
         float nSShapeSideDPhiSideInBin = t->Project("hSShapeSideDPhiTmp",observable,rBkgSShapeDPhi.cut&&varCut);
         // Scales
         if (subDPhiSideInBin) {
            float nDPhiBkgInBin = nDPhiSideInBin * (3.14159-dphiSigCut)/(3.14159/2.-0.7);
            if (nSigAllInBin>0) fracDPhiBkgInBin = nDPhiBkgInBin/nSigAllInBin;
         }
         if (subSShapeSideInBin) {
            if (rSigAll.var=="cBin") {
               if (h->GetBinCenter(i)>=0&&h->GetBinCenter(i)<4) fracSShapeBkgInBin = 1-hFracPhotonBkg->GetBinContent(1);
               if (h->GetBinCenter(i)>=4&&h->GetBinCenter(i)<12) fracSShapeBkgInBin = 1-hFracPhotonBkg->GetBinContent(2);
               if (h->GetBinCenter(i)>=12&&h->GetBinCenter(i)<20) fracSShapeBkgInBin = 1-hFracPhotonBkg->GetBinContent(4);
               if (h->GetBinCenter(i)>=20&&h->GetBinCenter(i)<40) fracSShapeBkgInBin = 1-hFracPhotonBkg->GetBinContent(5);
               if (subDPhiSideInBin) {
                  float nSShapeSideDPhiBkgInBin = nSShapeSideDPhiSideInBin * (3.14159-dphiSigCut)/(3.14159/2.-0.7);
                  if (nSigAllInBin>0) fracSShapeBkgDPhiBkgInBin = nSShapeSideDPhiBkgInBin/nSigAllInBin;
               }                  
            }
         }
         cout << "varCut: " << varCut << ", bin" << i << ": " << h->GetBinContent(i) << ", fracDPhiBkgInBin: " << fracDPhiBkgInBin << " fracSShapeBkgInBin: " << fracSShapeBkgInBin << " fracSShapeBkgDPhiBkgInBin: " << fracSShapeBkgDPhiBkgInBin << endl;
         cout << " Count: nSigAll: " << nSigAllInBin << endl;
         cout << " Count: sideband dphi: " << nDPhiSideInBin << endl;
         cout << " Count: sideband ss: " << nSShapeSideInBin << " ssdphi: " << nSShapeSideDPhiSideInBin << endl;
         float dphisidescale = (3.14159-dphiSigCut)/(3.14159/2.-0.7);
         float sssidescale = 0;
         if (nSShapeSideInBin>0) sssidescale = nSigAllInBin*fracSShapeBkgInBin/nSShapeSideInBin;

         if (histMode==1) { // scale input histogram
            h->SetBinContent(i,h->GetBinContent(i)*(1-fracDPhiBkgInBin-(fracSShapeBkgInBin-fracSShapeBkgDPhiBkgInBin)));
            h->SetBinError(i,h->GetBinError(i)*(1-fracDPhiBkgInBin-(fracSShapeBkgInBin-fracSShapeBkgDPhiBkgInBin)));
         } else if (histMode==0) { // scale bck histograms
            cout << " Bkg scale dphi: " << dphisidescale << endl;
            cout << " Bkg scale ss: " << sssidescale << endl;
            cout << " Bkg scale ssdphi: " << dphisidescale << endl;
            //cout << " hSigAllTmp RMS: " << hSigAllTmp->GetRMS() << " / sqrt(n) = " << hSigAllTmp->GetRMS()/sqrt(nSigAllInBin) << endl;
            //rSigAll.hScaled->SetBinError(i,hSigAllTmp->GetRMS()/sqrt(nSigAllInBin));
            if (fracDPhiBkgInBin>0) {
               rBkgDPhi.hScaled->SetBinContent(i,rBkgDPhi.hNorm->GetBinContent(i)*dphisidescale);
               rBkgDPhi.hScaled->SetBinError(i,rBkgDPhi.hNorm->GetBinError(i));
               //cout << " hDPhiSideTmp RMS: " << hDPhiSideTmp->GetRMS() << " / sqrt(n) = "<< hDPhiSideTmp->GetRMS()/sqrt(nDPhiSideInBin) << endl;
               //rBkgDPhi.hScaled->SetBinError(i,hDPhiSideTmp->GetRMS()/sqrt(nDPhiSideInBin));
            }
            if (fracSShapeBkgInBin>0) {
               rBkgSShape.hScaled->SetBinContent(i,rBkgSShape.hNorm->GetBinContent(i)*sssidescale);
               rBkgSShape.hScaled->SetBinError(i,rBkgSShape.hNorm->GetBinError(i));
               //cout << " hSShapeSideTmp RMS: " << hSShapeSideTmp->GetRMS() << " / sqrt(n) = "<< hSShapeSideTmp->GetRMS()/sqrt(nSShapeSideInBin) << endl;
               //rBkgSShape.hScaled->SetBinError(i,hSShapeSideTmp->GetRMS()/sqrt(nSShapeSideInBin));
            }
            if (fracSShapeBkgDPhiBkgInBin>0) {
               rBkgSShapeDPhi.hScaled->SetBinContent(i,rBkgSShapeDPhi.hNorm->GetBinContent(i)*dphisidescale);
               rBkgSShapeDPhi.hScaled->SetBinError(i,rBkgSShapeDPhi.hNorm->GetBinError(i));
               //rBkgSShapeDPhi.hScaled->SetBinError(i,hSShapeSideDPhiTmp->GetRMS()/sqrt(nSShapeSideDPhiSideInBin));
               //cout << " hSShapeSideDPhiTmp RMS: " << hSShapeSideDPhiTmp->GetRMS() << " / sqrt(n) = "<< hSShapeSideDPhiTmp->GetRMS()/sqrt(nSShapeSideDPhiSideInBin) << endl;
            }
         }
      }
      delete hSigAllTmp;
      delete hDPhiSideTmp;
      delete hSShapeSideTmp;
      delete hSShapeSideDPhiTmp;
   }
   
   TTree * t;
   TString name,nameIsol,observable;
   TCut sel,cutIsol;
   Region rSigAll;
   Region rBkgDPhi;
   Region rBkgSShape;
   Region rBkgSShapeDPhi;
   TH1D * hSubtracted;
   TH1D * hSSSideDPhiSub;
   TString weight;
   int normMode;
   bool subDPhiSide;
   bool subSShapeSide;
   bool subSShapeSideDPhiSide;
   TCut cutBkgDPhi;
   TCut cutSShape;
   TCut cutSShapeDPhi;   
   float nSelPhoton;
   float nSigAll;
   float fracDPhiBkg;
   float photonPurity;
   float fracPhotonBkg;
   float fracPhotonBkgDPhiBkg;
   float area;
   TH1D * hFracPhotonBkg;
};

#endif