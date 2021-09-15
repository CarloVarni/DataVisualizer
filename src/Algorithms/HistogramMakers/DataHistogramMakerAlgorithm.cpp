
#include <DataHistogramMakerAlgorithm.hpp>
#include <Data.hpp>

namespace Algorithm {

  DataHistogramMakerAlgorithm::DataHistogramMakerAlgorithm(const std::string& name,
							   const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  DataHistogramMakerAlgorithm::initialize()
  {
    if (m_cfg.inputCollection.empty())
      throw std::invalid_argument("Missing input collection for algorithm " + name());

    if (m_cfg.inputMaskName.empty())
      throw std::invalid_argument("Missing input mask for algorithm " + name());

    if (m_cfg.variableNames_1D.size() != m_cfg.histogramDefs_1D.size())
      throw std::invalid_argument("Mismatch between size of 1D variables and histograms for algorithm " + name());

    if (m_cfg.outputCollection_1d.empty())
      throw std::invalid_argument("Missing output collection name for algorithm " + name());
  }

  void
  DataHistogramMakerAlgorithm::execute(Core::EventContext& context)
  {
    
    MSG_INFO("Executing " + name() + " ... ");
    
    const auto dataCollection = context.get<EventDataModel::DataObjectCollection>(m_cfg.inputCollection);
    MSG_INFO("Retrieved " + std::to_string(dataCollection->size()) + " objects");
    if (dataCollection->size() == 0) {
      MSG_FATAL("No input data have been found. Cannot create histogram from that!!!");
    }
    
    const std::vector<bool>* mask = context.get<std::vector<bool>>(m_cfg.inputMaskName);

    const auto nSize_1D = m_cfg.variableNames_1D.size();
    
    std::shared_ptr<EventDataModel::HistogramObjectCollection_1D> histograms_1D =
      std::make_shared<EventDataModel::HistogramObjectCollection_1D>();

    histograms_1D->reserve(nSize_1D);

    std::shared_ptr<std::unordered_map<std::string, std::size_t>> map_1D =
      std::make_shared<std::unordered_map<std::string, std::size_t>>();

    for (auto ihisto(0); ihisto<nSize_1D; ihisto++) {
      const std::string& var_name = m_cfg.variableNames_1D.at(ihisto);
      auto& histo = m_cfg.histogramDefs_1D.at(ihisto);
      
      std::visit([&] (auto& h_to_add) {
		   EventDataModel::HistogramObject_1D toAdd( "h_" + var_name, var_name , h_to_add);
		   map_1D->emplace(toAdd.title(), histograms_1D->size());
		   histograms_1D->push_back(toAdd);
		 }, histo);
    }
    
    // Fill histograms
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      if (not mask->at(ientry))
	continue;
      auto& data = dataCollection->at(ientry);
      
      for (auto& histo : *histograms_1D.get())
	histo.Fill(data);
    }

    
    context.add( "h_1d_" + m_cfg.outputCollection_1d, std::move(histograms_1D) );
    context.add( "index_1d_" + m_cfg.outputCollection_1d, std::move(map_1D));
  }
  
}
