
#include <GraphComparisonPlotterAlgorithm.hpp>

namespace Algorithm {

  GraphComparisonPlotterAlgorithm::GraphComparisonPlotterAlgorithm(const std::string& name,
								   const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}
  
  void
  GraphComparisonPlotterAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");

    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument("Missing output folder for algorithm " + name());

    if (m_cfg.originalInputCollection.size() == 0)
      throw std::invalid_argument("Missing input collection for algorithm " + name());

    if (m_cfg.variableNames.size() == 0)
      throw std::invalid_argument("Missing variable name list for algorithm " + name());

  }

  void
  GraphComparisonPlotterAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");

    std::vector<const EventDataModel::GraphObjectCollection*> graph_collections;
    std::vector<const std::unordered_map<std::string, std::size_t>*> graph_maps;

    for (const std::string& input : m_cfg.originalInputCollection ) {
      graph_collections.push_back( context.get<EventDataModel::GraphObjectCollection>("gr_" + input) );
      graph_maps.push_back( context.get<std::unordered_map<std::string, std::size_t>>("index_gr_" + input) );
    }

    for (const std::string& var_name : m_cfg.variableNames) {
      EventDataModel::GraphObjectCollection graph_to_be_drawn;
      
      for (auto ientry(0); ientry < graph_maps.size(); ientry++) {
	const auto& map = graph_maps.at(ientry);
	if (map->find("gr_" + var_name) == map->end())
	  throw std::runtime_error("Variable not available in collection: " + var_name);

	std::size_t index = map->at("gr_" + var_name);
	graph_to_be_drawn.push_back( graph_collections.at(ientry)->at(index) );
      }

      TCanvas canvas("canvas", "canvas");
      DrawCollection(canvas,
		     graph_to_be_drawn);
      canvas.Draw();
      
      std::string save_name = m_cfg.outputFolder + "/comparison_gr_" + var_name + ".pdf";
      canvas.SaveAs( save_name.c_str() );
      MSG_INFO( "Created histogram: " + save_name );
    }

  }

  void
  GraphComparisonPlotterAlgorithm::DrawCollection(TCanvas& canvas,
						  EventDataModel::GraphObjectCollection& collection)
  {
    canvas.cd();
    for (auto igr(0); igr<collection.size(); igr++) {
      auto& gr = collection.at(igr);

      gr.SetLineColor(igr + 1);
      gr.SetLineStyle(igr == 0 ? 1 : 2);
      gr.SetMarkerStyle(20);
      gr.SetMarkerColor(igr + 1);
      
      gr.Draw(canvas, igr == 0 ? "AP" : "PSAME" );
    }
  }
    
}


