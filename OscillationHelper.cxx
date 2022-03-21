#include "OscillationHelper.hxx"

#include "BargerPropagator.h"

#include <algorithm>
#include <iostream>
#include <string>

static const double deg2rad = asin(1) / 90.0;

OscillationHelper::NuTypes OscillationHelper::GetNuType(int pdg) {
  switch (pdg) {
  case 16:
    return kNutauType;
  case 14:
    return kNumuType;
  case 12:
    return kNueType;
  case -16:
    return kNutaubarType;
  case -14:
    return kNumubarType;
  case -12:
    return kNuebarType;
  default: {
    std::cout << "[ERROR]: Attempting to convert \"neutrino pdg\": " << pdg
              << std::endl;
    exit(1);
  }
  }
}

OscillationHelper::OscillationHelper(OscillationHelper const &other) {
  DipAngle_degrees = other.DipAngle_degrees;
  std::copy_n(other.OscParams, 6, OscParams);
  LengthParam = other.LengthParam;
  IsSetUp = other.IsSetUp;
  FromPDG = other.FromPDG;
  ToPDG = other.ToPDG;
  FromType = other.FromType;
  ToType = other.ToType;
  bp = new BargerPropagator();
}

void OscillationHelper::Setup_dipangle(double OscParams[6],
                                       double DipAngle_degrees, bool loud) {
  std::copy_n(OscParams, 6, this->OscParams);
  this->DipAngle_degrees = DipAngle_degrees;

  if(loud){
    std::cout << "[INFO]: Using oscillation parameters: " << std::endl
              << "\tSin^2(Theta_12) = " << this->OscParams[0] << std::endl;
    std::cout << "\tSin^2(Theta_13) = " << this->OscParams[1] << std::endl;
    std::cout << "\tSin^2(Theta_23) = " << this->OscParams[2] << std::endl;

    std::cout << "\tDm^2_21 = " << this->OscParams[3] << " (GeV^2) " << std::endl;
    std::cout << "\t|Dm^2_Atm| = " << this->OscParams[4] << " (GeV^2) "
              << std::endl;

    std::cout << "\tdcp = " << this->OscParams[5] << std::endl;
    std::cout << "\tBeam dip angle = " << DipAngle_degrees << " degrees"
              << std::endl;
  }

  LengthParam = cos((90.0 + DipAngle_degrees) * deg2rad);
  IsSetUp = true;

  if (bp) {
    delete bp;
  }
  bp = new BargerPropagator();
}

void OscillationHelper::Setup_baseline(double OscParams[6],
                                       double baseline_km, bool loud) {

  const static double REarth_km = 6371.393;
  double DipAngle = asin(baseline_km / (2.0 * REarth_km)) / deg2rad;
  Setup_dipangle(OscParams, DipAngle, loud);
}

void OscillationHelper::SetOscillationChannel(int PDGFrom, int PDGTo) {
  FromPDG = PDGFrom;
  ToPDG = PDGTo;
  FromType = GetNuType(PDGFrom);
  ToType = GetNuType(PDGTo);
}

double OscillationHelper::GetWeight(double ENu_GeV) {
  if (!IsSetUp) {
    std::cout << "[ERROR]: Attempted to calculate oscillation weights without "
                 "setting up the OscillationHelper instance."
              << std::endl;
    throw;
  }
  bp->SetMNS(OscParams[0], OscParams[1], OscParams[2], OscParams[3],
             OscParams[4], OscParams[5], ENu_GeV, true, FromType);
  bp->DefinePath(LengthParam, 0);
  bp->propagate(ToType);

  return bp->GetProb(FromType, ToType);
}

void OscillationHelper::OscillateHistogram(TH1 *h) {
  for (Int_t bi_it = 1; bi_it < h->GetXaxis()->GetNbins() + 1; ++bi_it) {
    double E = h->GetXaxis()->GetBinCenter(bi_it);
    double weight = GetWeight(E);
    h->SetBinContent(bi_it, weight * h->GetBinContent(bi_it));
    h->SetBinError(bi_it, weight * h->GetBinError(bi_it));
  }
}
OscillationHelper::~OscillationHelper() { delete bp; }
