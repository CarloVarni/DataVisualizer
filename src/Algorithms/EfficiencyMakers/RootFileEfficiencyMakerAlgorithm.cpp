
#include <RootFileEfficiencyMakerAlgorithm.hpp>

namespace Algorithm {

  RootFileEfficiencyMakerAlgorithm::RootFileEfficiencyMakerAlgorithm(const std::string& name,
								     const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}
  
  void
  RootFileEfficiencyMakerAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");

    if (m_cfg.inputFile.empty())
      throw std::invalid_argument("Missing input file name for algorithm " + name());
    
    if (m_cfg.output_collection_1d.empty())
      throw std::invalid_argument("Missing 1D output collection name for algorithm " + name());

    if (m_cfg.output_collection_2d.empty())
      throw std::invalid_argument("Missing 2D output collection name for algorithm " + name());
  }
  
  void
  RootFileEfficiencyMakerAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");
    
    std::shared_ptr<TFile> file = std::make_shared<TFile>(m_cfg.inputFile.c_str(), "READ");

    std::pair<
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>,
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>
      > efficiency_collections = m_cfg.extractionFunction( file.get() );

    std::shared_ptr<
      std::unordered_map<std::string, std::size_t>
      > efficiency_names_1d =
      std::make_shared<
	std::unordered_map<std::string, std::size_t>
      >();
    
    std::shared_ptr<
      std::unordered_map<std::string, std::size_t>
      > efficiency_names_2d =
      std::make_shared<
	std::unordered_map<std::string, std::size_t>
      >();
    

    
    for (auto index(0); index<efficiency_collections.first->size(); index++) {
      const auto& object = efficiency_collections.first->at(index);
      (*efficiency_names_1d.get())["eff_1d_" + object.title()] = index;
    }

    for (auto index(0); index<efficiency_collections.second->size(); index++) {
      const auto& object = efficiency_collections.second->at(index);
      (*efficiency_names_2d.get())["eff_2d_" + object.title()] = index;
    }
    
    if (not m_cfg.output_collection_1d.empty()) {
      context.add( "eff_1d_" + m_cfg.output_collection_1d, std::move(efficiency_collections.first) );
      context.add( "index_eff_1d_" + m_cfg.output_collection_1d, std::move(efficiency_names_1d) );
    }

    if (not m_cfg.output_collection_2d.empty()) {
      context.add( "eff_2d_" + m_cfg.output_collection_2d, std::move(efficiency_collections.second) );
      context.add( "index_eff_2d_" + m_cfg.output_collection_2d, std::move(efficiency_names_2d) );
    }

    file->Close(); 
  }
  
}
