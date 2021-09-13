
#include <RootFileHistogramMakerAlgorithm.hpp>

namespace Algorithm {

  RootFileHistogramMakerAlgorithm::RootFileHistogramMakerAlgorithm(const std::string& name,
								   const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  RootFileHistogramMakerAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");
    
    if (m_cfg.inputFile.empty())
      throw std::invalid_argument("Missing input file for algorithm " + name());

    if (m_cfg.output_collection_1d.empty() and m_cfg.output_collection_2d.empty())
      throw std::invalid_argument("Missing output collection for algorithm " + name());
  }

  void RootFileHistogramMakerAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing  " + name() + " ...");

    std::shared_ptr<TFile> file = std::make_shared<TFile>(m_cfg.inputFile.c_str(), "READ");

    std::pair<std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>,
	      std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>> histo_collection =
      m_cfg.extractionFunction( *file.get() );
    
    std::shared_ptr<std::vector<std::string>> histo_names_1d =
      std::make_shared<std::vector<std::string>>();
    
    std::shared_ptr<std::vector<std::string>> histo_names_2d =
      std::make_shared<std::vector<std::string>>();
    
    histo_names_1d->reserve(histo_collection.first->size());
    histo_names_2d->reserve(histo_collection.second->size());

    if (not m_cfg.output_collection_1d.empty()) {
      context.add( "h_1d_" + m_cfg.output_collection_1d, std::move(histo_collection.first) );
      context.add( "index_1d_" + m_cfg.output_collection_1d, std::move(histo_names_1d) );
    }

    if (not m_cfg.output_collection_2d.empty()) {
      context.add( "h_2d_" + m_cfg.output_collection_2d, std::move(histo_collection.second) );
      context.add( "index_2d_" + m_cfg.output_collection_2d, std::move(histo_names_2d) );
    }

    file->Close();
  }
  
}
