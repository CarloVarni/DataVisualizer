

#include <MultiData.hpp>
#include <Histogram_1D.hpp>
#include <type_traits>
#include <iostream>

#include "TH1.h"
#include "TH2.h"

#include <TCanvas.h>
#include <TApplication.h>

TApplication Runner("gui",0,NULL);

int main() {

  std::vector<std::string> label{"uno", "due", "tre"};
  std::vector<
    std::variant< int, double, float, bool, short, long, unsigned int>
    >
    data{2, 4.1, 123};
  
  MultiData A(label, data, 1.2);
  std::cout << A << std::endl;

  Histogram_1D<double, TH1D> prova("histogram", "due", 20, 0, 10);
  prova.Fill(A);

  TCanvas c0("","");
  prova.Draw(c0, "HIST");

  Runner.Run(true);
}
