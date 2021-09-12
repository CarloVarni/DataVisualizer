
#include <DataPlotterAlgorithm.hpp>

namespace Algorithm {

  DataPlotterAlgorithm::DataPlotterAlgorithm(const std::string& name,
					     const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  DataPlotterAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ... ");

    if (m_cfg.inputCollection.empty())
      throw std::invalid_argument("Input Collection has not been specified for algorithm " + name());

    if (m_cfg.inputMaskName.empty())
      throw std::invalid_argument("Input Mask has not been specified for algorithm " + name());

    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument("Output folder has not been specified for algorithm " + name());

    if (m_cfg.variableNames.size() != m_cfg.histogramDefs.size())
      throw std::invalid_argument("Mismatch between variable labels and histograms for algorithm " + name());
  }

  void
  DataPlotterAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ... ");

    const auto dataCollection = context.get<EventDataModel::DataObjectColletion>(m_cfg.inputCollection);
    MSG_INFO("Retrieved " + std::to_string(dataCollection->size()) + " objects");
    
    const std::vector<bool>* mask =
      m_cfg.inputMaskName.empty() ?
      new std::vector<bool>(dataCollection->size() ,true) :
      context.get<std::vector<bool>>(m_cfg.inputMaskName);

    const auto nSize = m_cfg.variableNames.size();
    EventDataModel::HistogramObjectCollection_1D histograms;
    histograms.reserve(nSize);

    for (auto ihisto(0); ihisto<nSize; ihisto++) {
      const std::string& var_name = m_cfg.variableNames.at(ihisto);
      auto& histo = m_cfg.histogramDefs.at(ihisto);

      std::visit([&] (auto& h_to_add) {
		   EventDataModel::HistogramObject_1D toAdd( "h_" + var_name, var_name , h_to_add);
		   histograms.push_back(toAdd);
		 }, histo);
    }

    // Fill histograms
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      if (not mask->at(ientry))
	continue;
      const auto& data = dataCollection->at(ientry);
      
      for (auto& histo : histograms)
	histo.Fill(data);
    }

    // Draw and save histograms
    for (auto& histo : histograms) {
      TCanvas canvas("canvas", "canvas");
      histo.Draw(canvas, "HIST");
      canvas.Draw();
      canvas.SaveAs( (m_cfg.outputFolder + "/" + histo.target_variable() + ".pdf").c_str() );
    }
    
  }

}
