#!/bin/bash

if ! hash root-config; then
  echo "[ERROR]: Cannot find root-config, is ROOT set up?"
  exit 1
fi

if [ ! -e Prob3plusplus ]; then
    git clone https://github.com/rogerwendell/Prob3plusplus.git
fi

if [ ! -e Prob3plusplus/libThreeProb*.a ]; then
  cd Prob3plusplus
  CXXFLAGS=-fPIC CFLAGS=-fPIC make
  cd ../
fi

PROB3LIBNAME=$(ls Prob3plusplus/libThreeProb*.a)

CMD="g++ OscillationHelper.cxx -I Prob3plusplus ${PROB3LIBNAME} -fPIC -shared -o WrappedProb3plusplus.so $(root-config --cflags --libs)"

echo $CMD
${CMD}

echo "#pragma once" > Prob3ppWrapper.hxx
echo "#include \"$(pwd)/OscillationHelper.hxx\"" >> Prob3ppWrapper.hxx
echo "#pragma cling load(\"$(pwd)/WrappedProb3plusplus.so\")" >> Prob3ppWrapper.hxx

echo "cat Prob3ppWrapper.hxx"
cat Prob3ppWrapper.hxx
