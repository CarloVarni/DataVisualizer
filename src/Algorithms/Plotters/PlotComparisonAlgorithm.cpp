
#include <PlotComparisonAlgorithm.hpp>

namespace Algorithm {

  PlotComparisonAlgorithm::PlotComparisonAlgorithm(const std::string& name,
						   const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}
  
  void
  PlotComparisonAlgorithm::initialize()
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
  PlotComparisonAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");
    
    std::vector<const EventDataModel::HistogramObjectCollection_1D*> histo_collections;
    std::vector<const std::unordered_map<std::string, std::size_t>*> histo_maps;

    for (const std::string& input : m_cfg.originalInputCollection ) {
      histo_collections.push_back( context.get<EventDataModel::HistogramObjectCollection_1D>("h_1d_" + input) );
      histo_maps.push_back( context.get<std::unordered_map<std::string, std::size_t>>("index_1d_" + input) );
    }

    
    for (const std::string& var_name : m_cfg.variableNames) {
      EventDataModel::HistogramObjectCollection_1D histo_to_be_drawn;
      
      for (auto ientry(0); ientry < histo_maps.size(); ientry++) {
	const auto& map = histo_maps.at(ientry);
	if (map->find("h_" + var_name) == map->end())
	  throw std::runtime_error("Variable not available in collection: " + var_name);

	std::size_t index = map->at("h_" + var_name);
	histo_to_be_drawn.push_back( histo_collections.at(ientry)->at(index) );
      }

      TCanvas canvas("canvas", "canvas");
      DrawCollection(canvas,
		     histo_to_be_drawn);
      canvas.Draw();
      
      std::string save_name = m_cfg.outputFolder + "/comparison_" + var_name + ".pdf";
      canvas.SaveAs( save_name.c_str() );
      MSG_INFO( "Created histogram: " + save_name );
    }
    
  }

  void
  PlotComparisonAlgorithm::DrawCollection(TCanvas& canvas,
					  EventDataModel::HistogramObjectCollection_1D& collection)
  {
    canvas.cd();
    for (auto ihisto(0); ihisto<collection.size(); ihisto++) {
      auto& histo = collection.at(ihisto);
      histo.SetLineColor(ihisto + 1);
      histo.SetLineStyle(ihisto == 0 ? 1 : 2);
      histo.Draw(canvas, ihisto == 0 ? "HIST" : "HISTSAME" );
    }
  }
    
}


