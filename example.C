#include "OscillationHelper.hxx"
R__LOAD_LIBRARY(WrappedProb3++.20121225.so)

#include "TFile.h"
#include "TGraph.h"
#include "TSystem.h"

#include <iostream>

void example() {

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
  // dcp
  double dcp = 0;
  double osc_params[] = {s2th12, s2th13, s2th23, dm2_21, dm2_atm, dcp};

  OscillationHelper oh_numu, oh_nue, oh_nutau;

  oh_numu.Setup(osc_params, beam_dip_deg);
  oh_nue.Setup(osc_params, beam_dip_deg);
  oh_nutau.Setup(osc_params, beam_dip_deg);

  int pdg_numu = 14;
  int pdg_nue = 12;
  int pdg_nutau = 16;

  oh_numu.SetOscillationChannel(pdg_numu, pdg_numu);
  oh_nue.SetOscillationChannel(pdg_numu, pdg_nue);
  oh_nutau.SetOscillationChannel(pdg_numu, pdg_nutau);

  TGraph POsc_numudisp;
  TGraph POsc_nueapp;
  TGraph POsc_nutauapp;

  double emin = 0.1; // GeV
  double emax = 10;  // GeV
  size_t nsteps = 1000;
  double step = (emax - emin) / double(nsteps);

  for (size_t i = 0; i < nsteps; ++i) {
    double e_gev = emin + i * step;

    double pnumu = oh_numu.GetWeight(e_gev);
    double pnue = oh_nue.GetWeight(e_gev);
    double pnutau = oh_nutau.GetWeight(e_gev);

    std::cout << "[INFO]: E = " << e_gev << " , PDisp = " << pnumu << std::endl;

    POsc_numudisp.SetPoint(i, e_gev, pnumu);
    POsc_nueapp.SetPoint(i, e_gev, pnue);
    POsc_nutauapp.SetPoint(i, e_gev, pnutau);
  }

  TFile *f = new TFile("example_osc.root", "RECREATE");
  POsc_numudisp.Write("POsc_numudisp");
  POsc_nueapp.Write("POsc_nueapp");
  POsc_nutauapp.Write("POsc_nutauapp");
  f->Write();
  f->Close();
}
