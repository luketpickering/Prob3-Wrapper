#include "OscillationHelper.hxx"

#undef R__LOAD_LIBRARY
#define R__LOAD_LIBRARY(a)

#include "example.C"

int main(int argc, char const *argv[]) {
  example();
}
