#!/bin/bash

if [ -z "${ROOTSYS}" ]; then
  echo "[ERROR]: \$ROOTSYS is not defined, did you set up your ROOT environment."
  exit 1
fi

PROB3PPVERSION="20121225"

if [ ! -e Prob3++.${PROB3PPVERSION}.tar.gz ] && [ ! -e Prob3++ ]; then
    wget http://webhome.phy.duke.edu/~raw22/public/Prob3++/Prob3++.${PROB3PPVERSION}.tar.gz
    tar -zxvf Prob3++.${PROB3PPVERSION}.tar.gz
    mv Prob3++.${PROB3PPVERSION} Prob3++
fi

if [ ! -e Prob3++/libThreeProb_2.10.a ]; then
  cd Prob3++
  CXXFLAGS=-fPIC CFLAGS=-fPIC make
  cd ../
fi

g++ OscillationHelper.cxx -I Prob3++ Prob3++/libThreeProb_2.10.a -fPIC -shared -o WrappedProb3++.${PROB3PPVERSION}.so $(root-config --cflags --libs)
