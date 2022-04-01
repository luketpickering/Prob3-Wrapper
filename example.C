#include <iostream>

#include "Prob3ppWrapper.hxx"
#include "TFile.h"
#include "TGraph.h"
#include "TSystem.h"

// DUNE beam dip
double beam_dip_deg = 5.8;

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

std::pair<TGraph, TGraph> BiProb(double L_km, double E_GeV, int c) {
  TGraph bp_NH, bp_IH;

  OscillationHelper oh_nue_NH, oh_nueb_NH;
  oh_nue_NH.SetOscillationChannel(14, 12);
  oh_nueb_NH.SetOscillationChannel(-14, -12);

  OscillationHelper oh_nue_IH, oh_nueb_IH;
  oh_nue_IH.SetOscillationChannel(14, 12);
  oh_nueb_IH.SetOscillationChannel(-14, -12);

  for (int i = 0; i < 256; ++i) {
    double dcp = (double(i) / 256.0) * 2 * M_PI;

    double osc_params_NH[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm, dcp};
    double osc_params_IH[] = {s2th12, s2th13, s2th23, dm2_21, -dm2_atm, dcp};

    oh_nue_NH.Setup_baseline(osc_params_NH, L_km);
    oh_nueb_NH.Setup_baseline(osc_params_NH, L_km);

    oh_nue_IH.Setup_baseline(osc_params_IH, L_km);
    oh_nueb_IH.Setup_baseline(osc_params_IH, L_km);

    bp_NH.SetPoint(i, oh_nue_NH.GetWeight(E_GeV), oh_nueb_NH.GetWeight(E_GeV));
    bp_IH.SetPoint(i, oh_nue_IH.GetWeight(E_GeV), oh_nueb_IH.GetWeight(E_GeV));
  }
  bp_NH.SetLineColor(c);
  bp_IH.SetLineColor(c);
  bp_IH.SetLineStyle(2);
  return {bp_NH, bp_IH};
}

std::tuple<TGraph, TGraph, TGraph, TGraph> FixedE(double L_km_min,
                                                  double L_km_max, double E_GeV,
                                                  int c1, int c2) {
  TGraph bp_nue_NH, bp_nue_IH, bp_nueb_NH, bp_nueb_IH;

  OscillationHelper oh_nue_NH, oh_nueb_NH;
  oh_nue_NH.SetOscillationChannel(14, 12);
  oh_nueb_NH.SetOscillationChannel(-14, -12);

  OscillationHelper oh_nue_IH, oh_nueb_IH;
  oh_nue_IH.SetOscillationChannel(14, 12);
  oh_nueb_IH.SetOscillationChannel(-14, -12);

  for (int i = 0; i < 2560; ++i) {
    double dcp = 0;

    double L_km =
        pow(10, std::log10(L_km_min) +
                    double(i) * (std::log10(L_km_max) - std::log10(L_km_min)) /
                        2560.0);

    double osc_params_NH[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm, dcp};
    double osc_params_IH[] = {s2th12, s2th13, s2th23, dm2_21, -dm2_atm, dcp};

    oh_nue_NH.Setup_baseline(osc_params_NH, L_km);
    oh_nueb_NH.Setup_baseline(osc_params_NH, L_km);

    oh_nue_IH.Setup_baseline(osc_params_IH, L_km);
    oh_nueb_IH.Setup_baseline(osc_params_IH, L_km);

    bp_nue_NH.SetPoint(i, L_km, oh_nue_NH.GetWeight(E_GeV));
    bp_nue_IH.SetPoint(i, L_km, oh_nue_IH.GetWeight(E_GeV));
    bp_nueb_NH.SetPoint(i, L_km, oh_nueb_NH.GetWeight(E_GeV));
    bp_nueb_IH.SetPoint(i, L_km, oh_nueb_IH.GetWeight(E_GeV));
  }
  bp_nue_NH.SetLineColor(c1);
  bp_nue_IH.SetLineColor(c1);
  bp_nueb_NH.SetLineColor(c2);
  bp_nueb_IH.SetLineColor(c2);
  bp_nue_IH.SetLineStyle(2);
  bp_nueb_IH.SetLineStyle(2);
  return {bp_nue_NH, bp_nue_IH, bp_nueb_NH, bp_nueb_IH};
}

std::tuple<TGraph, TGraph> FixedE_tonueb(double L_km_min, double L_km_max,
                                        double E_GeV, int c1) {
  TGraph bp_nue_NH, bp_nue_IH;

  OscillationHelper oh_nue_NH, oh_nueb_NH;
  oh_nue_NH.SetOscillationChannel(14, 12);
  oh_nueb_NH.SetOscillationChannel(-14, -12);

  OscillationHelper oh_nue_IH, oh_nueb_IH;
  oh_nue_IH.SetOscillationChannel(14, 12);
  oh_nueb_IH.SetOscillationChannel(-14, -12);

  for (int i = 0; i < 2560; ++i) {
    double dcp = 0;

    double L_km =
        pow(10, std::log10(L_km_min) +
                    double(i) * (std::log10(L_km_max) - std::log10(L_km_min)) /
                        2560.0);

    double osc_params_NH[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm, dcp};
    double osc_params_IH[] = {s2th12, s2th13, s2th23, dm2_21, -dm2_atm, dcp};

    oh_nue_NH.Setup_baseline(osc_params_NH, L_km);
    oh_nueb_NH.Setup_baseline(osc_params_NH, L_km);

    oh_nue_IH.Setup_baseline(osc_params_IH, L_km);
    oh_nueb_IH.Setup_baseline(osc_params_IH, L_km);

    bp_nue_IH.SetPoint(
        i, L_km, oh_nue_IH.GetWeight(E_GeV) / oh_nueb_IH.GetWeight(E_GeV));

    bp_nue_NH.SetPoint(
        i, L_km, oh_nue_NH.GetWeight(E_GeV) / oh_nueb_NH.GetWeight(E_GeV));
  }
  bp_nue_NH.SetLineColor(c1);
  bp_nue_IH.SetLineColor(c1);
  bp_nue_IH.SetLineStyle(2);
  return {bp_nue_NH, bp_nue_IH};
}

std::tuple<TGraph, TGraph> FixedL_tonueb(double E_GeV_min, double E_GeV_max,
                                        double L_km, int c1) {
  TGraph bp_nue_NH, bp_nue_IH;

  OscillationHelper oh_nue_NH, oh_nueb_NH;
  oh_nue_NH.SetOscillationChannel(14, 12);
  oh_nueb_NH.SetOscillationChannel(-14, -12);

  OscillationHelper oh_nue_IH, oh_nueb_IH;
  oh_nue_IH.SetOscillationChannel(14, 12);
  oh_nueb_IH.SetOscillationChannel(-14, -12);

  double constant_Dm32_E = dm2_atm/5;

  for (int i = 0; i < 2560; ++i) {
    double dcp = 0;

    double E_GeV = E_GeV_min + double(i)*(E_GeV_max - E_GeV_min)/2560.0;

    double dm2_atm_local = constant_Dm32_E * E_GeV;

    double osc_params_NH[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm_local, dcp};
    double osc_params_IH[] = {s2th12, s2th13, s2th23, dm2_21, -dm2_atm_local, dcp};

    oh_nue_NH.Setup_baseline(osc_params_NH, L_km);
    oh_nueb_NH.Setup_baseline(osc_params_NH, L_km);

    oh_nue_IH.Setup_baseline(osc_params_IH, L_km);
    oh_nueb_IH.Setup_baseline(osc_params_IH, L_km);

    bp_nue_IH.SetPoint(
        i, E_GeV, oh_nue_IH.GetWeight(E_GeV) / oh_nueb_IH.GetWeight(E_GeV));

    bp_nue_NH.SetPoint(
        i, E_GeV, oh_nue_NH.GetWeight(E_GeV) / oh_nueb_NH.GetWeight(E_GeV));
  }
  bp_nue_NH.SetLineColor(c1);
  bp_nue_IH.SetLineColor(c1);
  bp_nue_IH.SetLineStyle(2);
  return {bp_nue_NH, bp_nue_IH};
}

void example() {
  gStyle->SetOptStat(false);
  std::pair<TGraph, TGraph> bp_23_GeV_1300km = BiProb(1300, 2.3, kRed);
  std::pair<TGraph, TGraph> bp_4_GeV_1300km = BiProb(1300, 2.3 * 2, kBlue);
  std::pair<TGraph, TGraph> bp_1_GeV_1300km = BiProb(1300, 2.3 * 0.5, kBlack);

  std::pair<TGraph, TGraph> bp_23_GeV_1300km_double =
      BiProb(1300 * 2, 2.3 * 2, kGreen);
  std::pair<TGraph, TGraph> bp_23_GeV_1300km_half =
      BiProb(1300 * 0.5, 2.3 * 0.5, kMagenta);

  TCanvas c1("c1", "");

  c1.SetTopMargin(0.2);
  c1.Print("biprob.pdf[");

  {
    TH2D dummy(
        "dummy",
        ";P(#nu_{#mu}#rightarrow#nu_{e});P(#bar{#nu}_{#mu}#rightarrow#bar{"
        "#nu}_{e});",
        1, 0, 0.125, 1, 0, 0.125);

    dummy.Draw();

    bp_23_GeV_1300km.first.Draw("L");
    bp_23_GeV_1300km.second.Draw("L");

    bp_4_GeV_1300km.first.Draw("L");
    bp_4_GeV_1300km.second.Draw("L");

    bp_1_GeV_1300km.first.Draw("L");
    bp_1_GeV_1300km.second.Draw("L");

    bp_23_GeV_1300km_double.first.Draw("L");
    bp_23_GeV_1300km_double.second.Draw("L");

    bp_23_GeV_1300km_half.first.Draw("L");
    bp_23_GeV_1300km_half.second.Draw("L");

    TLegend leg(0.1, 0.82, 1, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(3);

    leg.AddEntry(&bp_23_GeV_1300km.first, "2.3 GeV, 1300km, NH", "l");
    leg.AddEntry(&bp_23_GeV_1300km.second, "2.3 GeV, 1300km, IH", "l");

    leg.AddEntry(&bp_4_GeV_1300km.first, "2.3*2 GeV, 1300km, NH", "l");
    leg.AddEntry(&bp_1_GeV_1300km.first, "2.3*0.5 GeV, 1300km, NH", "l");

    leg.AddEntry(&bp_23_GeV_1300km_double.first, "2.3*2 GeV, 1300*2km, NH",
                 "l");
    leg.AddEntry(&bp_23_GeV_1300km_half.first, "2.3*0.5 GeV, 1300*0.5km, NH",
                 "l");

    leg.Draw();
    c1.Print("biprob.pdf");
  }

  double lmin = 50;
  double lmax = 1500;

  {
    auto fe = FixedE(lmin, lmax, 2.3, kBlue, kBlack);

    c1.SetLogx();

    TH2D dummy("dummy", ";L (km);P(#nu_{#mu}#rightarrow#nu_{e});", 1, lmin,
               lmax, 1, 0, 0.15);
    dummy.Draw();

    std::get<0>(fe).Draw("L");
    std::get<1>(fe).Draw("L");
    std::get<2>(fe).Draw("L");
    std::get<3>(fe).Draw("L");

    TLegend leg(0, 0.82, 0.9, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(2);

    leg.AddEntry(&std::get<0>(fe),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), NH", "l");
    leg.AddEntry(&std::get<1>(fe),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), IH", "l");

    leg.AddEntry(&std::get<2>(fe),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), NH",
                 "l");
    leg.AddEntry(&std::get<3>(fe),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), IH",
                 "l");

    leg.Draw();

    TLatex ltx;
    ltx.DrawLatexNDC(0.3, 0.7, "E = 2.3 GeV");

    c1.Print("biprob.pdf");
  }

  {
    auto fe = FixedE(lmin, lmax, 0.6, kBlue, kBlack);

    c1.SetLogx();

    TH2D dummy("dummy", ";L (km);P(#nu_{#mu}#rightarrow#nu_{e});", 1, lmin,
               lmax, 1, 0, 0.15);
    dummy.Draw();

    std::get<0>(fe).Draw("L");
    std::get<1>(fe).Draw("L");
    std::get<2>(fe).Draw("L");
    std::get<3>(fe).Draw("L");

    TLegend leg(0, 0.82, 0.9, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(2);

    leg.AddEntry(&std::get<0>(fe),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), NH", "l");
    leg.AddEntry(&std::get<1>(fe),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), IH", "l");

    leg.AddEntry(&std::get<2>(fe),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), NH",
                 "l");
    leg.AddEntry(&std::get<3>(fe),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), IH",
                 "l");

    leg.Draw();

    TLatex ltx;
    ltx.DrawLatexNDC(0.3, 0.7, "E = 0.6 GeV");

    c1.Print("biprob.pdf");
  }

  {
    auto fe_06 = FixedE(lmin, lmax, 0.6, kBlue, kBlack);
    auto fe_23 = FixedE(lmin, lmax, 2.3, kRed, kMagenta);

    c1.SetLogx();

    TH2D dummy("dummy", ";L (km);P(#nu_{#mu}#rightarrow#nu_{e});", 1, lmin,
               lmax, 1, 0, 0.15);
    dummy.Draw();

    std::get<0>(fe_06).Draw("L");
    std::get<1>(fe_06).Draw("L");
    std::get<2>(fe_06).Draw("L");
    std::get<3>(fe_06).Draw("L");

    std::get<0>(fe_23).Draw("L");
    std::get<1>(fe_23).Draw("L");
    std::get<2>(fe_23).Draw("L");
    std::get<3>(fe_23).Draw("L");

    TLegend leg(0, 0.82, 0.9, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(2);

    leg.AddEntry(&std::get<0>(fe_06),
                 "0.6 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), NH", "l");
    leg.AddEntry(&std::get<1>(fe_06),
                 "0.6 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), IH", "l");

    leg.AddEntry(&std::get<2>(fe_06),
                 "0.6 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), NH",
                 "l");
    leg.AddEntry(&std::get<3>(fe_06),
                 "0.6 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), IH",
                 "l");

    leg.AddEntry(&std::get<0>(fe_23),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), NH", "l");
    leg.AddEntry(&std::get<1>(fe_23),
                 "2.3 GeV, P(#nu_{#mu}#rightarrow#nu_{e}), IH", "l");

    leg.AddEntry(&std::get<2>(fe_23),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), NH",
                 "l");
    leg.AddEntry(&std::get<3>(fe_23),
                 "2.3 GeV, P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e}), IH",
                 "l");

    leg.Draw();

    c1.Print("biprob.pdf");
  }

  {
    auto fe_06 = FixedE_tonueb(lmin, lmax, 0.6, kBlue);
    auto fe_15 = FixedE_tonueb(lmin, lmax, 1.5, kRed);
    auto fe_23 = FixedE_tonueb(lmin, lmax, 2.3, kBlack);

    c1.SetLogx();

    TH2D dummy("dummy",
               ";L "
               "(km);P(#nu_{#mu}#rightarrow#nu_{e})/"
               "P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e});",
               1, lmin, lmax, 1, 0, 5);
    dummy.Draw();

    std::get<0>(fe_06).Draw("L");
    std::get<1>(fe_06).Draw("L");

    std::get<0>(fe_15).Draw("L");
    std::get<1>(fe_15).Draw("L");

    std::get<0>(fe_23).Draw("L");
    std::get<1>(fe_23).Draw("L");

    TLegend leg(0, 0.82, 0.9, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(3);

    leg.AddEntry(&std::get<0>(fe_06), "0.6 GeV, NH", "l");
    leg.AddEntry(&std::get<0>(fe_15), "1.5 GeV, NH", "l");
    leg.AddEntry(&std::get<0>(fe_23), "2.3 GeV, NH", "l");

    leg.AddEntry(&std::get<1>(fe_06), "0.6 GeV, IH", "l");
    leg.AddEntry(&std::get<1>(fe_15), "1.5 GeV, IH", "l");
    leg.AddEntry(&std::get<1>(fe_23), "2.3 GeV, IH", "l");

    leg.Draw();

    c1.Print("biprob.pdf");
  }

  {
    auto fe_295 = FixedL_tonueb(0.2, 10, 295, kBlue);
    auto fe_800 = FixedL_tonueb(0.2, 10, 800, kRed);
    auto fe_1300 = FixedL_tonueb(0.2, 10, 1300, kBlack);

    c1.SetLogx(false);

    TH2D dummy("dummy",
               ";E (GeV);P(#nu_{#mu}#rightarrow#nu_{e})/"
               "P(#bar{#nu}_{#mu}#rightarrow#bar{#nu}_{e});",
               1, 0.2, 10, 1, 0, 5);
    dummy.Draw();

    std::get<0>(fe_295).Draw("L");
    std::get<1>(fe_295).Draw("L");

    std::get<0>(fe_800).Draw("L");
    std::get<1>(fe_800).Draw("L");

    std::get<0>(fe_1300).Draw("L");
    std::get<1>(fe_1300).Draw("L");

    TLegend leg(0, 0.82, 0.9, 1);

    leg.SetBorderSize(0);
    leg.SetNColumns(3);

    leg.AddEntry(&std::get<0>(fe_295), "295 km, NH", "l");
    leg.AddEntry(&std::get<0>(fe_800), "800 km, NH", "l");
    leg.AddEntry(&std::get<0>(fe_1300), "1300 km, NH", "l");

    leg.AddEntry(&std::get<1>(fe_295), "295 km, IH", "l");
    leg.AddEntry(&std::get<1>(fe_800), "800 km, IH", "l");
    leg.AddEntry(&std::get<1>(fe_1300), "1300 km, IH", "l");

    leg.Draw();

    c1.Print("biprob.pdf");
  }

  c1.Print("biprob.pdf]");
}
