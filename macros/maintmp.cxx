

#include <MultiData.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>
#include <type_traits>
#include <iostream>

#include "TH1.h"
#include "TH2.h"

#include <TCanvas.h>
#include <TApplication.h>

#include <EventContext.hpp>

#include <Scheduler.hpp>

TApplication Runner("gui",0,NULL);

int main() {

  std::vector<std::string> label{"uno", "due", "tre"};
  std::vector<
    std::variant< int >
    >
    data{2, 4, 123};
  
  EventDataModel::MultiData A(label, data, 1.2);
  std::cout << A << std::endl;

  EventDataModel::Histogram_2D<int, int, TH2D> prova("histogram", "due", "uno", 20, 0, 10, 10, 0, 10);
  prova.Fill(A);

  TCanvas c0("","");
  prova.Draw(c0, "COLZ");

  Runner.Run(true);
}
