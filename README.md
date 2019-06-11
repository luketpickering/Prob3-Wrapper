#prob3++wrapper

A simple wrapper for prob3++ the neutrino oscillation
probability calculator for use with ROOT.

# build

With you root environment setup, run build.sh, if all goes well
you should end up with WrappedProb3++.20121225.so in the
current working directory.

# usage

See example.C as an exampled of how to use the interface.
The R__LOAD_LIBRARY is only applicable to ROOT6 and above,
use gSystem->Load if still using cint.

The beam dip angle is used to calculate the chord through
the earth and thus the baseline. DUNE is 5.8 degrees.

The 6 oscillation parameters are then given to OscillationHelper::Setup
to instantiate the oscillation calculation. Angle are set as
sin^2(mix_ang_rad), mass splittings are in eV, and dcp is cyclic from
-pi to pi (or 0 to 1pi).

An oscillation channel is then set with 
OscillationHelper::SetOscillationChannel where the neutrinos have
their standard PDG MC numbers (nue[bar] : 12/[-12], numu : 14,
nutau : 16), e.g. muon neutrino disappearance would set
SetOscillationChannel(14,14);

An oscillation probability is then obtained from 
OscillationHelper::GetWeight(energy), where the energy should 
be in GeV.
