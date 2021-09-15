
#include <MultiDataHistogramMakerAlgorithm.hpp>

namespace Algorithm {

  MultiDataHistogramMakerAlgorithm::MultiDataHistogramMakerAlgorithm(const std::string& name,
								     const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  MultiDataHistogramMakerAlgorithm::initialize()
  {
    if (m_cfg.inputCollection.empty())
      throw std::invalid_argument("Missing input collection for algorithm " + name());

    if (m_cfg.inputMaskName.empty())
      throw std::invalid_argument("Missing input mask for algorithm " + name());

    if (m_cfg.variableNames_1D.size() != m_cfg.histogramDefs_1D.size())
      throw std::invalid_argument("Mismatch between size of 1D variables and histograms for algorithm " + name());

    if (m_cfg.variableNames_2D.size() != m_cfg.histogramDefs_2D.size())
       throw std::invalid_argument("Mismatch between size of 2D variables and histograms for algorithm " + name());

    if (m_cfg.outputCollection_1d.empty() and m_cfg.outputCollection_2d.empty())
      throw std::invalid_argument("Both 1d and 2d output collections are missing for algorithm " + name());

    if (m_cfg.outputCollection_1d.empty() and m_cfg.variableNames_1D.size() != 0)
      throw std::invalid_argument("Output collection for 1D histograms cannot be empty if we are producing them");

    if (m_cfg.outputCollection_2d.empty() and m_cfg.variableNames_2D.size() != 0)
      throw std::invalid_argument("Output collection for 2D histograms cannot be empty if we are producing them");
  }

  void
  MultiDataHistogramMakerAlgorithm::execute(Core::EventContext& context)
  {
    
    MSG_INFO("Executing " + name() + " ... ");
    
    const auto dataCollection = context.get<EventDataModel::MultiDataObjectCollection>(m_cfg.inputCollection);
    MSG_INFO("Retrieved " + std::to_string(dataCollection->size()) + " objects");

    const std::vector<bool>* mask = context.get<std::vector<bool>>(m_cfg.inputMaskName);

    const auto nSize_1D = m_cfg.variableNames_1D.size();
    const auto nSize_2D = m_cfg.variableNames_2D.size();
    
    std::shared_ptr<EventDataModel::HistogramObjectCollection_1D> histograms_1D =
      std::make_shared<EventDataModel::HistogramObjectCollection_1D>();
    std::shared_ptr<EventDataModel::HistogramObjectCollection_2D> histograms_2D =
      std::make_shared<EventDataModel::HistogramObjectCollection_2D>();

    histograms_1D->reserve(nSize_1D);
    histograms_2D->reserve(nSize_2D);


    std::shared_ptr<std::unordered_map<std::string, std::size_t>> map_1D =
      std::make_shared<std::unordered_map<std::string, std::size_t>>();
    std::shared_ptr<std::unordered_map<std::string, std::size_t>> map_2D =
      std::make_shared<std::unordered_map<std::string, std::size_t>>();

    // 1D plots
    for (auto ihisto(0); ihisto<nSize_1D; ihisto++) {
      const std::string& var_name = m_cfg.variableNames_1D.at(ihisto);
      auto& histo = m_cfg.histogramDefs_1D.at(ihisto);
      
      std::visit([&] (auto& h_to_add) {
		   const std::string histoName(h_to_add.GetName());
		   EventDataModel::HistogramObject_1D toAdd( "h_" + histoName,
							     var_name ,
							     h_to_add);
		   map_1D->emplace(toAdd.title(), histograms_1D->size());
		   histograms_1D->push_back(toAdd);
		 }, histo);
    }

    // 2D plots
    for (auto ihisto(0); ihisto<nSize_2D; ihisto++) {
      const std::string& var_x_name = m_cfg.variableNames_2D.at(ihisto).first;
      const std::string& var_y_name = m_cfg.variableNames_2D.at(ihisto).second;
      auto& histo = m_cfg.histogramDefs_2D.at(ihisto);

      std::visit([&] (auto& h_to_add) {
		   const std::string histoName(h_to_add.GetName());
		   EventDataModel::HistogramObject_2D toAdd( "h_" + histoName,
							     var_x_name , var_y_name,
							     h_to_add);
		   map_2D->emplace(toAdd.title(), histograms_2D->size());
                   histograms_2D->push_back(toAdd);
		 }, histo);
    }
    
    
    // Fill histograms
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      if (not mask->at(ientry))
	continue;
      auto& data = dataCollection->at(ientry);
      
      // 1D plots
      for (auto& histo : *histograms_1D.get())
	histo.Fill(data);
      // 2D plots
      for (auto& histo : *histograms_2D.get())
	histo.Fill(data);
    }

    
    context.add( "h_1d_" + m_cfg.outputCollection_1d, std::move(histograms_1D) );
    context.add( "h_2d_" + m_cfg.outputCollection_2d, std::move(histograms_2D) );
    
    context.add( "index_1d_" + m_cfg.outputCollection_1d, std::move(map_1D));
    context.add( "index_2d_" + m_cfg.outputCollection_2d, std::move(map_2D));
  }
  
}
