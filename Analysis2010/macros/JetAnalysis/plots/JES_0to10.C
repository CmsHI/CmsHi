{
//=========Macro generated from canvas: cJES/cJES
//=========  (Fri Dec 10 16:45:28 2010) by ROOT version5.22/00d
   TCanvas *cJES = new TCanvas("cJES", "cJES",0,0,500,550);
   gStyle->SetOptStat(0);
   cJES->Range(13.41772,0.3125,228.6076,1.5625);
   cJES->SetFillColor(0);
   cJES->SetBorderMode(0);
   cJES->SetBorderSize(0);
   cJES->SetTickx(1);
   cJES->SetTicky(1);
   cJES->SetLeftMargin(0.17);
   cJES->SetRightMargin(0.04);
   cJES->SetTopMargin(0.05);
   cJES->SetBottomMargin(0.15);
   cJES->SetFrameLineColor(0);
   cJES->SetFrameBorderMode(0);
   cJES->SetFrameLineColor(0);
   cJES->SetFrameBorderMode(0);
   Double_t xAxis1[18] = {18, 21, 24, 28, 32, 37, 43, 50, 56, 64, 74, 84, 97, 114, 133, 174, 220, 300}; 
   
   TH1 *JES_h_0 = new TH1D("JES_h_0","JES",17, xAxis1);
   JES_h_0->SetBinContent(3,-1.232831);
   JES_h_0->SetBinContent(4,1.593754);
   JES_h_0->SetBinContent(5,1.520031);
   JES_h_0->SetBinContent(6,1.158954);
   JES_h_0->SetBinContent(7,1.146892);
   JES_h_0->SetBinContent(8,1.10961);
   JES_h_0->SetBinContent(9,1.082835);
   JES_h_0->SetBinContent(10,1.035046);
   JES_h_0->SetBinContent(11,1.023638);
   JES_h_0->SetBinContent(12,1.004061);
   JES_h_0->SetBinContent(13,0.9770853);
   JES_h_0->SetBinContent(14,0.9763246);
   JES_h_0->SetBinContent(15,0.9660496);
   JES_h_0->SetBinContent(16,0.9951498);
   JES_h_0->SetBinContent(17,1.025303);
   JES_h_0->SetBinContent(18,0.9009221);
   JES_h_0->SetBinError(3,12.35085);
   JES_h_0->SetBinError(4,8.117965);
   JES_h_0->SetBinError(5,1.079949);
   JES_h_0->SetBinError(6,0.06251564);
   JES_h_0->SetBinError(7,0.01111706);
   JES_h_0->SetBinError(8,0.007458755);
   JES_h_0->SetBinError(9,0.00626806);
   JES_h_0->SetBinError(10,0.005388657);
   JES_h_0->SetBinError(11,0.004219033);
   JES_h_0->SetBinError(12,0.003422668);
   JES_h_0->SetBinError(13,0.003849762);
   JES_h_0->SetBinError(14,0.005922134);
   JES_h_0->SetBinError(15,0.007274589);
   JES_h_0->SetBinError(16,0.03285795);
   JES_h_0->SetBinError(17,0.341064);
   JES_h_0->SetBinError(18,6.76356);
   JES_h_0->SetMinimum(0.5);
   JES_h_0->SetMaximum(1.5);
   JES_h_0->SetEntries(16);
   JES_h_0->SetStats(0);
   JES_h_0->SetFillColor(1);
   JES_h_0->SetFillStyle(0);
   JES_h_0->SetLineWidth(2);
   JES_h_0->SetMarkerStyle(20);
   JES_h_0->GetXaxis()->SetTitle("p_{T}^{GenJet} (GeV/c)");
   JES_h_0->GetXaxis()->SetRange(8,16);
   JES_h_0->GetXaxis()->CenterTitle(true);
   JES_h_0->GetXaxis()->SetLabelFont(42);
   JES_h_0->GetXaxis()->SetLabelOffset(0.01);
   JES_h_0->GetXaxis()->SetLabelSize(0.045);
   JES_h_0->GetXaxis()->SetTitleSize(0.055);
   JES_h_0->GetXaxis()->SetTitleFont(42);
   JES_h_0->GetYaxis()->SetTitle("p_{T}^{CaloJet}/p_{T}^{GenJet}");
   JES_h_0->GetYaxis()->CenterTitle(true);
   JES_h_0->GetYaxis()->SetLabelFont(42);
   JES_h_0->GetYaxis()->SetLabelOffset(0.01);
   JES_h_0->GetYaxis()->SetLabelSize(0.045);
   JES_h_0->GetYaxis()->SetTitleSize(0.055);
   JES_h_0->GetYaxis()->SetTitleOffset(1.2);
   JES_h_0->GetYaxis()->SetTitleFont(42);
   JES_h_0->GetZaxis()->SetLabelFont(42);
   JES_h_0->GetZaxis()->SetLabelSize(0.045);
   JES_h_0->GetZaxis()->SetTitleFont(42);
   JES_h_0->Draw("E");
   Double_t xAxis2[18] = {18, 21, 24, 28, 32, 37, 43, 50, 56, 64, 74, 84, 97, 114, 133, 174, 220, 300}; 
   
   TH1 *JES_h_0 = new TH1D("JES_h_0","JES",17, xAxis2);
   JES_h_0->SetBinContent(3,-1.232831);
   JES_h_0->SetBinContent(4,1.593754);
   JES_h_0->SetBinContent(5,1.520031);
   JES_h_0->SetBinContent(6,1.158954);
   JES_h_0->SetBinContent(7,1.146892);
   JES_h_0->SetBinContent(8,1.10961);
   JES_h_0->SetBinContent(9,1.082835);
   JES_h_0->SetBinContent(10,1.035046);
   JES_h_0->SetBinContent(11,1.023638);
   JES_h_0->SetBinContent(12,1.004061);
   JES_h_0->SetBinContent(13,0.9770853);
   JES_h_0->SetBinContent(14,0.9763246);
   JES_h_0->SetBinContent(15,0.9660496);
   JES_h_0->SetBinContent(16,0.9951498);
   JES_h_0->SetBinContent(17,1.025303);
   JES_h_0->SetBinContent(18,0.9009221);
   JES_h_0->SetBinError(3,12.35085);
   JES_h_0->SetBinError(4,8.117965);
   JES_h_0->SetBinError(5,1.079949);
   JES_h_0->SetBinError(6,0.06251564);
   JES_h_0->SetBinError(7,0.01111706);
   JES_h_0->SetBinError(8,0.007458755);
   JES_h_0->SetBinError(9,0.00626806);
   JES_h_0->SetBinError(10,0.005388657);
   JES_h_0->SetBinError(11,0.004219033);
   JES_h_0->SetBinError(12,0.003422668);
   JES_h_0->SetBinError(13,0.003849762);
   JES_h_0->SetBinError(14,0.005922134);
   JES_h_0->SetBinError(15,0.007274589);
   JES_h_0->SetBinError(16,0.03285795);
   JES_h_0->SetBinError(17,0.341064);
   JES_h_0->SetBinError(18,6.76356);
   JES_h_0->SetMinimum(0.5);
   JES_h_0->SetMaximum(1.5);
   JES_h_0->SetEntries(16);
   JES_h_0->SetStats(0);
   JES_h_0->SetFillColor(1);
   JES_h_0->SetFillStyle(0);
   JES_h_0->SetLineWidth(2);
   JES_h_0->SetMarkerStyle(20);
   JES_h_0->GetXaxis()->SetTitle("p_{T}^{GenJet} (GeV/c)");
   JES_h_0->GetXaxis()->SetRange(8,16);
   JES_h_0->GetXaxis()->CenterTitle(true);
   JES_h_0->GetXaxis()->SetLabelFont(42);
   JES_h_0->GetXaxis()->SetLabelOffset(0.01);
   JES_h_0->GetXaxis()->SetLabelSize(0.045);
   JES_h_0->GetXaxis()->SetTitleSize(0.055);
   JES_h_0->GetXaxis()->SetTitleFont(42);
   JES_h_0->GetYaxis()->SetTitle("p_{T}^{CaloJet}/p_{T}^{GenJet}");
   JES_h_0->GetYaxis()->CenterTitle(true);
   JES_h_0->GetYaxis()->SetLabelFont(42);
   JES_h_0->GetYaxis()->SetLabelOffset(0.01);
   JES_h_0->GetYaxis()->SetLabelSize(0.045);
   JES_h_0->GetYaxis()->SetTitleSize(0.055);
   JES_h_0->GetYaxis()->SetTitleOffset(1.2);
   JES_h_0->GetYaxis()->SetTitleFont(42);
   JES_h_0->GetZaxis()->SetLabelFont(42);
   JES_h_0->GetZaxis()->SetLabelSize(0.045);
   JES_h_0->GetZaxis()->SetTitleFont(42);
   JES_h_0->Draw("sameE");
   Double_t xAxis3[18] = {18, 21, 24, 28, 32, 37, 43, 50, 56, 64, 74, 84, 97, 114, 133, 174, 220, 300}; 
   
   TH1 *JES_h_1 = new TH1D("JES_h_1","Fitted value of par[1]=Mean",17, xAxis3);
   JES_h_1->SetBinContent(0,2.232128);
   JES_h_1->SetBinContent(1,1.865189);
   JES_h_1->SetBinContent(2,1.639231);
   JES_h_1->SetBinContent(3,1.585832);
   JES_h_1->SetBinContent(4,1.425486);
   JES_h_1->SetBinContent(5,1.275686);
   JES_h_1->SetBinContent(6,1.19654);
   JES_h_1->SetBinContent(7,1.136028);
   JES_h_1->SetBinContent(8,1.081536);
   JES_h_1->SetBinContent(9,1.058903);
   JES_h_1->SetBinContent(10,1.025522);
   JES_h_1->SetBinContent(11,1.00812);
   JES_h_1->SetBinContent(12,0.9982166);
   JES_h_1->SetBinContent(13,0.9889869);
   JES_h_1->SetBinContent(14,0.989979);
   JES_h_1->SetBinContent(15,0.960124);
   JES_h_1->SetBinContent(16,1.020537);
   JES_h_1->SetBinContent(17,0.9800734);
   JES_h_1->SetBinError(0,19.77232);
   JES_h_1->SetBinError(1,12.7267);
   JES_h_1->SetBinError(2,2.135175);
   JES_h_1->SetBinError(3,0.3421838);
   JES_h_1->SetBinError(4,0.1112831);
   JES_h_1->SetBinError(5,0.02629758);
   JES_h_1->SetBinError(6,0.01165133);
   JES_h_1->SetBinError(7,0.006574946);
   JES_h_1->SetBinError(8,0.006961699);
   JES_h_1->SetBinError(9,0.005808403);
   JES_h_1->SetBinError(10,0.00504981);
   JES_h_1->SetBinError(11,0.004355339);
   JES_h_1->SetBinError(12,0.00464549);
   JES_h_1->SetBinError(13,0.006432337);
   JES_h_1->SetBinError(14,0.008891985);
   JES_h_1->SetBinError(15,0.01318816);
   JES_h_1->SetBinError(16,0.127703);
   JES_h_1->SetBinError(17,2.827644);
   JES_h_1->SetMinimum(0.5);
   JES_h_1->SetMaximum(1.5);
   JES_h_1->SetEntries(18);
   JES_h_1->SetStats(0);

   Int_t ci;   // for color index setting
   ci = TColor::GetColor("#000000");
   JES_h_1->SetFillColor(ci);
   JES_h_1->SetFillStyle(0);

   ci = TColor::GetColor("#000000");
   JES_h_1->SetLineColor(ci);
   JES_h_1->SetLineWidth(2);

   ci = TColor::GetColor("#000000");
   JES_h_1->SetMarkerColor(ci);
   JES_h_1->SetMarkerStyle(21);
   JES_h_1->GetXaxis()->SetRange(8,16);
   JES_h_1->GetXaxis()->SetLabelFont(42);
   JES_h_1->GetXaxis()->SetLabelOffset(0.01);
   JES_h_1->GetXaxis()->SetLabelSize(0.045);
   JES_h_1->GetXaxis()->SetTitleSize(0.055);
   JES_h_1->GetXaxis()->SetTitleFont(42);
   JES_h_1->GetYaxis()->SetLabelFont(42);
   JES_h_1->GetYaxis()->SetLabelOffset(0.01);
   JES_h_1->GetYaxis()->SetLabelSize(0.045);
   JES_h_1->GetYaxis()->SetTitleSize(0.055);
   JES_h_1->GetYaxis()->SetTitleOffset(1.2);
   JES_h_1->GetYaxis()->SetTitleFont(42);
   JES_h_1->GetZaxis()->SetLabelFont(42);
   JES_h_1->GetZaxis()->SetLabelSize(0.045);
   JES_h_1->GetZaxis()->SetTitleFont(42);
   JES_h_1->Draw("sameE");
   
   TLegend *leg = new TLegend(0.3,0.74,0.7,0.91,NULL,"brNDC");
   leg->SetBorderSize(0);
   leg->SetTextFont(62);
   leg->SetTextSize(0.03);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(19);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("NULL","Centrality 0 to 10","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry->SetTextFont(62);
   entry=leg->AddEntry("hJESNr2D_1","Leading Jet Reponse","PL");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(20);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("hJESAw2D_1","Away Jet Reponse","PL");

   ci = TColor::GetColor("#000000");
   entry->SetLineColor(ci);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);

   ci = TColor::GetColor("#000000");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   leg->Draw();
   TLine *line = new TLine(50,1,220,1);
   line->SetLineStyle(2);
   line->Draw();
   cJES->Modified();
   cJES->cd();
   cJES->SetSelected(cJES);
}
