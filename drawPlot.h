void drawPlot(TH1F* ha, TH1F* hb, string sinfo=""){

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  TCanvas * ca = new TCanvas("ca","ca",600,600);
  ca->SetLeftMargin(0.12);
  ca->SetBottomMargin(0.08);
  ca->SetRightMargin(0.05);
  ca->SetTopMargin(0.05);

  TLegend *leg = new TLegend(0.15, 0.78, 0.4, 0.92);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);

  ha->SetMaximum( ha->GetMaximum() < 0. ? ha->GetMaximum() * 0.9 : ha->GetMaximum() * 1.1);
  ha->SetMinimum( ha->GetMinimum() < 0. ? ha->GetMinimum() * 1.2 : ha->GetMinimum() * 0.8);

  //std::string xtitle = "time passed (s)";
  std::string xtitle = "time passed (minute)";
  ha->GetXaxis()->SetTitle( xtitle.c_str() );
  ha->GetYaxis()->SetTitle( "Thermal Couple Temperature (#circ C)" );
  ha->SetLineWidth(3);
  ha->SetLineColor(kBlue);
  ha->SetMarkerColor(kBlue);
  ha->SetMarkerStyle(20);
  ha->GetYaxis()->SetTitleOffset(1.35);
  ha -> Draw("HIST");
  hb->SetLineWidth(3);
  hb->SetLineColor(kRed);
  hb->SetMarkerColor(kRed);
  hb->SetMarkerStyle(21);
  hb->GetYaxis()->SetTitleOffset(1.35);
  hb -> Draw("HISTsame");
  if (!sinfo.empty()){
    TLatex Tl; Tl.SetTextSize(20); Tl.SetTextFont(43);
    double Xpos = 0.25 * ha->GetXaxis()->GetXmin() + 0.75 * ha->GetXaxis()->GetXmax();
    double Ypos = 0.25 * ha->GetYaxis()->GetXmin() + 0.75 * ha->GetYaxis()->GetXmax();
    Tl.DrawLatex(Xpos, Ypos, sinfo.c_str());
  }
  leg->AddEntry(ha, "Liquid IN", "lp");
  leg->AddEntry(hb, "Liquid OUT", "lp");
  leg->Draw();

  std::string sout= std::string("Tinout");
  ca->Print( (sout+".png").c_str());

  TH1F * hd = (TH1F*) ha -> Clone("hdiff");
  float Tdmin = 9999, Tdmax = -9999;
  for (int ib=1; ib<=ha->GetNbinsX(); ib++){
    float Tdif = ha->GetBinContent(ib) - hb->GetBinContent(ib);
    hd->SetBinContent(ib, Tdif);
    if (Tdmin > Tdif) Tdmin = Tdif;
    if (Tdmax < Tdif) Tdmax = Tdif;
  }
  hd->GetYaxis()->SetTitle( "Temperature IN - OUT (#circ C)" );
  hd->SetMaximum( 1.1 * Tdmax );
  hd->SetMinimum( 0.9 * Tdmin );

  hd->Draw("HIST");

  sout= std::string("Tinout_dif");
  ca->Print( (sout+".png").c_str());

  delete ca;

}
