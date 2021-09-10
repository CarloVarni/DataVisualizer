
#include <MultiDataPlotterAlgorithm.hpp>
#include <MultiData.hpp>
#include <Histogram_1D.hpp>

namespace Algorithm {

  MultiDataPlotterAlgorithm::MultiDataPlotterAlgorithm(const std::string& name,
						       const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  MultiDataPlotterAlgorithm::initialize()
  {
    std::cout <<"Initializing " << name() << " ... "<<std::endl;
  }
  
  void
  MultiDataPlotterAlgorithm::execute(Core::EventContext& context)
  {
    std::cout << "Executing " << name() << " ... " << std::endl;

    const auto dataCollection = context.get<EventDataModel::MultiDataCollection>(m_cfg.inputCollection);
    std::cout<<"Retrieved " << dataCollection->size() << " objects"<<std::endl;

    EventDataModel::HistogramObject_1D nhitsPlotter("nhits", "nhits",
						    TH1I("nhits", "nhits", 20, 0 ,20));
    EventDataModel::HistogramObject_1D ntracksPlotter("ntracks", "ntracks",
						      TH1I("ntracks", "ntracks", 20, 0, 20));
    
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      auto& data = dataCollection->at(ientry);
      nhitsPlotter.Fill(data);
      ntracksPlotter.Fill(data);
    }
    
    TCanvas c0("c0", "c0");
    nhitsPlotter.Draw(c0, "HIST");
    c0.Draw();
    c0.SaveAs( (m_cfg.outputFolder + "/nhits.pdf").c_str() );

    TCanvas c1("c1", "c1");
    ntracksPlotter.Draw(c1, "HIST");
    c1.Draw();
    c1.SaveAs( (m_cfg.outputFolder + "/ntracks.pdf").c_str() );

  }

  void
  MultiDataPlotterAlgorithm::finalize()
  {
    std::cout<<"Finalizing " << name() << " ..." << std::endl;
  }
}
