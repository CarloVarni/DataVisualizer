
#include <MultiDataGraphMakerAlgorithm.hpp>

namespace Algorithm {

  MultiDataGraphMakerAlgorithm::MultiDataGraphMakerAlgorithm(const std::string& name,
							     const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  MultiDataGraphMakerAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");

    if (m_cfg.inputCollection.empty())
      throw std::invalid_argument("Missing input collection for algorithm " + name());

    if (m_cfg.inputMaskName.empty())
      throw std::invalid_argument("Missing input mask name for algorithm " + name());

    if (m_cfg.outputCollection.empty())
      throw std::invalid_argument("Missing output collection for algorithm " + name());

    if (m_cfg.variableNames.size() != m_cfg.graphDefs.size())
      throw std::invalid_argument("Mismatch between variables and graph definitions for algorithm " + name());
  }

  void
  MultiDataGraphMakerAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");

    const EventDataModel::MultiDataObjectCollection* dataCollection =
      context.get<EventDataModel::MultiDataObjectCollection>(m_cfg.inputCollection);

    const std::vector<bool>* mask = context.get<std::vector<bool>>(m_cfg.inputMaskName);
    
    const auto nSize = m_cfg.variableNames.size();
    
    std::shared_ptr<EventDataModel::GraphObjectCollection> graph_collection =
      std::make_shared<EventDataModel::GraphObjectCollection>();

    graph_collection->reserve(nSize);
    
    std::shared_ptr<std::unordered_map<std::string, std::size_t>> map =
      std::make_shared<std::unordered_map<std::string, std::size_t>>();



    // 1D plots
    for (auto igr(0); igr<nSize; igr++) {
      const std::pair<
	std::string,
	std::string
	>& var_names = m_cfg.variableNames.at(igr);
      auto& gr = m_cfg.graphDefs.at(igr);
      
      std::visit([&] (auto& h_to_add) {
		   const std::string grName(var_names.first + "_" + var_names.second);
		   EventDataModel::GraphObject toAdd( "gr_" + grName,
						      var_names.first,
						      var_names.second,
						      h_to_add);
		   map->emplace(toAdd.title(), graph_collection->size());
		   graph_collection->push_back(toAdd);
		 }, gr);
    }

    // Fill
    for (auto ientry(0); ientry<dataCollection->size(); ientry++) {
      if (not mask->at(ientry))
	continue;
      auto& data = dataCollection->at(ientry);
      
      // graphs
      for (auto& gr : *graph_collection.get())
	gr.Fill(data);
    }

    
    context.add( "gr_" + m_cfg.outputCollection, std::move(graph_collection));
    context.add( "index_gr_" + m_cfg.outputCollection, std::move(map));
  }
  
}

