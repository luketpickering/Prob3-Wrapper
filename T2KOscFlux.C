#include "Prob3ppWrapper.hxx"

#include "TCanvas.h"
#include "TFile.h"
#include "TLegend.h"

#include "TColor.h"
#include "TLatex.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TSystem.h"

#include <array>
#include <sstream>

void defc(int &c, std::array<int, 3> cs, std::string const &n) {
  static int currC = 51;
  TColor *col = gROOT->GetColor(currC);
  if (!col) {
    col = new TColor(currC, Float_t(cs[0]) / 255.0, Float_t(cs[1]) / 255.0,
                     Float_t(cs[2]) / 255.0, n.c_str());
  } else {
    col->SetRGB(Float_t(cs[0]) / 255.0, Float_t(cs[1]) / 255.0,
                Float_t(cs[2]) / 255.0);
  }
  c = currC;
  currC++;
}

int main(int argc, char const *argv[]) {

  gStyle->SetOptStat(false);

  int kT2KBlue;
  int kT2KGreen;
  int kT2KRed;

  defc(kT2KBlue, {0, 0, 128}, "kT2KBlue");
  defc(kT2KGreen, {0, 128, 0}, "kT2KGreen");
  defc(kT2KRed, {128, 0, 0}, "kT2KRed");

  TFile fin(argv[1]);
  TH1 *SK_numu_FHC_unosc = nullptr;
  fin.GetObject("enu_sk_numu", SK_numu_FHC_unosc);

  TH1 *SK_numu_FHC_disp = (TH1 *)SK_numu_FHC_unosc->Clone("SK_numu_FHC_disp");
  TH1 *SK_numu_FHC_app = (TH1 *)SK_numu_FHC_unosc->Clone("SK_numu_FHC_app");

  OscillationHelper oh;

  double baseline_km = 295;

  // Sin^2(Theta_12)
  double s2th12 = 0.297;
  // Sin^2(Theta_13)
  double s2th13 = 0.0214;
  // Sin^2(Theta_23)
  double s2th23 = 0.526;
  // Dm^2_21
  double dm2_21 = 7.37E-5;
  //|Dm^2_Atm|
  double dm2_atm = 2.463E-3;

  double dcp = 0;

  double params[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm, dcp};

  oh.Setup_baseline(params, baseline_km);
  oh.SetOscillationChannel(14, 14);
  oh.OscillateHistogram(SK_numu_FHC_disp);
  oh.SetOscillationChannel(14, 12);
  oh.OscillateHistogram(SK_numu_FHC_app);

  SK_numu_FHC_unosc->SetLineColorAlpha(0, 0);
  SK_numu_FHC_disp->SetLineColorAlpha(0, 0);
  SK_numu_FHC_app->SetLineColorAlpha(0, 0);

  SK_numu_FHC_unosc->SetFillColorAlpha(kT2KRed, 0.5);
  SK_numu_FHC_disp->SetFillColorAlpha(kT2KGreen, 0.5);
  SK_numu_FHC_app->SetFillColorAlpha(kT2KBlue, 0.5);

  SK_numu_FHC_unosc->SetFillStyle(1001);
  SK_numu_FHC_disp->SetFillStyle(1001);
  SK_numu_FHC_app->SetFillStyle(1001);

  TCanvas c1("c1", "", 900, 900);
  c1.SetLeftMargin(0.175);
  c1.SetRightMargin(0.125);
  c1.SetBottomMargin(0.175);
  c1.SetTopMargin(0.125);

  SK_numu_FHC_unosc->GetXaxis()->SetRangeUser(0, 2);
  SK_numu_FHC_unosc->GetYaxis()->SetTitleOffset(1.5);
  SK_numu_FHC_unosc->GetYaxis()->SetNdivisions(505);
  SK_numu_FHC_unosc->GetXaxis()->SetNdivisions(505);
  SK_numu_FHC_unosc->SetTitle("");
  SK_numu_FHC_unosc->Draw("CHIST");
  SK_numu_FHC_disp->Draw("CHIST SAME");
  SK_numu_FHC_app->Draw("CHIST SAME");

  TLegend leg(0.45, 0.5, 0.85, 0.8);
  leg.SetBorderSize(0);
  leg.AddEntry(SK_numu_FHC_unosc, "No Oscillations", "f");
  leg.AddEntry(SK_numu_FHC_disp, "Surviving #nu_{#mu}", "f");
  leg.AddEntry(SK_numu_FHC_app, "Appearing #nu_{e}", "f");
  leg.Draw();

  TLatex ltx;
  ltx.SetTextAlign(12);
  ltx.SetTextSize(0.0325);
  std::stringstream ss;
  ss << "sin^{2}(#theta{}_{23}) = " << (float(int(s2th23 * 1000)) / 1000.0);
  ltx.DrawLatexNDC(0.3, 0.9, ss.str().c_str());

  ss.str("");
  ss << "|#Deltam^{2}_{32}| = " << (float(int(dm2_atm * 100000)) / 100.0)
     << "x10^{-3} eV";
  ltx.DrawLatexNDC(0.55, 0.9, ss.str().c_str());

  ss.str("");
  ss << "sin^{2}(#theta{}_{13}) = " << (float(int(s2th13 * 1000)) / 1000.0);
  ltx.DrawLatexNDC(0.3, 0.95, ss.str().c_str());

  ss.str("");
  ss << "#delta_{CP}/#pi = " << (float(int((dcp / M_PI) * 1000)) / 1000.0);
  ltx.DrawLatexNDC(0.55, 0.95, ss.str().c_str());

  ss.str("");
  ss << "Super-K, #nu_{#mu} Flux";
  ltx.SetTextSize(0.04);
  ltx.DrawLatexNDC(0.475, 0.825, ss.str().c_str());


  c1.Print("T2KOsc.pdf");
}