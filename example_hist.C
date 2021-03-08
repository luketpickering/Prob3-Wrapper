#include "Prob3ppWrapper.hxx"

#include "TFile.h"
#include "TH1.h"

void example_hist(char const *filename, char const *histname) {

  TFile fin(filename,"UPDATE");
  if (!fin.IsOpen()) {
    std::cout << "[ERROR]: Failed to open " << filename << " for reading."
              << std::endl;
    return 1;
  }

  TH1 *h = nullptr;
  fin.GetObject(histname, h);

  if (!h) {
    std::cout << "[ERROR]: Failed to read " << histname
              << " from file: " << filename << std::endl;
    return 1;
  }

  h->SetName((std::string(histname) + "_osc").c_str());
  h->SetDirectory(&fin);

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

  oh_numu.Setup_dipangle(osc_params, beam_dip_deg);

  int pdg_numu = 14;
  int pdg_nue = 12;
  int pdg_nutau = 16;

  oh_numu.SetOscillationChannel(pdg_numu, pdg_numu);


  oh_numu.OscillateHistogram(h);

  fin.Write();
  fin.Close();
}