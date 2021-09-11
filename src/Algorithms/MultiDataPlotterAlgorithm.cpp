
#include <MultiDataPlotterAlgorithm.hpp>
#include <MultiData.hpp>

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

    if (m_cfg.inputCollection.empty())
      throw std::invalid_argument(name() + " requires an input collection to be defined!");

    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument(name() + " requires an output folder for the plots to be specified!");

    if (m_cfg.variableNames_1D.size() != m_cfg.histogramDefs_1D.size())
      throw std::invalid_argument(name() + " mismatch between size of 1D variables and plots!");

    if (m_cfg.variableNames_2D.size() != m_cfg.histogramDefs_2D.size())
      throw std::invalid_argument(name() + " mismatch between size of 2D variables and plots!");
  }
  
  void
  MultiDataPlotterAlgorithm::execute(Core::EventContext& context)
  {
    std::cout << "Executing " << name() << " ... " << std::endl;

    const auto dataCollection = context.get<EventDataModel::MultiDataCollection>(m_cfg.inputCollection);
    std::cout<<"Retrieved " << dataCollection->size() << " objects"<<std::endl;

    const auto nSize_1D = m_cfg.variableNames_1D.size();
    const auto nSize_2D = m_cfg.variableNames_2D.size();
    
    EventDataModel::HistogramObjectCollection_1D histograms_1D;
    EventDataModel::HistogramObjectCollection_2D histograms_2D;

    histograms_1D.reserve(nSize_1D);
    histograms_2D.reserve(nSize_2D);

    // 1D plots
    for (auto ihisto(0); ihisto<nSize_1D; ihisto++) {
      const std::string& var_name = m_cfg.variableNames_1D.at(ihisto);
      auto& histo = m_cfg.histogramDefs_1D.at(ihisto);

      std::visit([&] (auto& h_to_add) {
		   EventDataModel::HistogramObject_1D toAdd( "h_" + var_name, var_name , h_to_add);
		   histograms_1D.push_back(toAdd);
		 }, histo);
    }

    // 2D plots
    for (auto ihisto(0); ihisto<nSize_2D; ihisto++) {
      const std::string& var_x_name = m_cfg.variableNames_2D.at(ihisto).first;
      const std::string& var_y_name = m_cfg.variableNames_2D.at(ihisto).second;
      auto& histo = m_cfg.histogramDefs_2D.at(ihisto);

      std::visit([&] (auto& h_to_add) {
		   EventDataModel::HistogramObject_2D toAdd( "h_" + var_x_name + "_" + var_y_name,
							     var_x_name , var_y_name,
							     h_to_add);
                   histograms_2D.push_back(toAdd);
	}, histo);
    }

    // Fill histograms
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      auto& data = dataCollection->at(ientry);

      // 1D plots
      for (auto& histo : histograms_1D)
	histo.Fill(data);
      // 2D plots
      for (auto& histo : histograms_2D)
	histo.Fill(data);
    }

    // Draw and save histograms
    for (auto& histo : histograms_1D) {
      TCanvas canvas("canvas", "canvas");
      histo.Draw(canvas, "HIST");
      canvas.Draw();
      canvas.SaveAs( (m_cfg.outputFolder + "/" + histo.target_variable() + ".pdf").c_str() );
    }

    // Draw and save histograms
    for (auto& histo : histograms_2D) {
      TCanvas canvas("canvas", "canvas");
      histo.Draw(canvas, "COLZ");
      canvas.Draw();
      canvas.SaveAs( (m_cfg.outputFolder + "/" + histo.target_variable_x() + "_" + histo.target_variable_y() + ".pdf").c_str() );
    }

  }

  void
  MultiDataPlotterAlgorithm::finalize()
  {
    std::cout<<"Finalizing " << name() << " ..." << std::endl;
  }
}


