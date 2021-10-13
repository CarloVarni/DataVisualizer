
#ifndef COMPARISON_PLOTTER_ALGORITHM_H
#define COMPARISON_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>

#include <TCanvas.h>

#include <vector>

namespace Algorithm {

  template<typename object_collection_t>
  class ComparisonPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string outputFolder = "";
      std::string prefix = "";
      std::vector<std::string> originalInputCollection = {};
      std::vector<std::string> variableNames = {};
    };

    using object_collection_type = object_collection_t;
    using object_type = typename object_collection_type::value_type;
    
    ComparisonPlotterAlgorithm() = delete;
    ComparisonPlotterAlgorithm(const std::string& name,
			       const Config& cfg);
    ComparisonPlotterAlgorithm(const ComparisonPlotterAlgorithm&) = delete;
    ComparisonPlotterAlgorithm& operator=(const ComparisonPlotterAlgorithm&) = delete;
    virtual ~ComparisonPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
    
  protected:
    virtual void DrawCollection(TCanvas& canvas,
				object_collection_t& collection) = 0;
    
  protected:
    const Config m_cfg;
  };

  /* ============================================================== */

  template<typename object_collection_t>
  ComparisonPlotterAlgorithm<object_collection_t>::ComparisonPlotterAlgorithm(const std::string& name,
									      const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  template<typename object_collection_t>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::initialize()
  {
    MSG_INFO("Initializing " + name() + " ...");
    
    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument("Missing output folder for algorithm " + name());
    
    if (m_cfg.originalInputCollection.size() == 0)
      throw std::invalid_argument("Missing input collection for algorithm " + name());
    
    if (m_cfg.variableNames.size() == 0)
      throw std::invalid_argument("Missing variable name list for algorithm " + name());
  }

  template<typename object_collection_t>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ...");

    std::vector<const object_collection_t*> object_collections;
    std::vector<const std::unordered_map<std::string, std::size_t>*> object_maps;
    object_collections.reserve(m_cfg.originalInputCollection.size());
    object_maps.reserve(m_cfg.originalInputCollection.size());
    
    for (const std::string& input : m_cfg.originalInputCollection ) {
      object_collections.push_back( context.get<object_collection_t>( m_cfg.prefix + "_" + input) );
      object_maps.push_back( context.get<std::unordered_map<std::string, std::size_t>>("index_" + m_cfg.prefix + "_" + input) );
    }

    for (const std::string& var_name : m_cfg.variableNames) {
      object_collection_t object_to_be_drawn;
      object_to_be_drawn.reserve(object_maps.size());
      
      for (auto ientry(0); ientry < object_maps.size(); ientry++) {
	const auto& map = object_maps.at(ientry);
	if (map->find( m_cfg.prefix + "_" + var_name) == map->end())
	  throw std::runtime_error("Variable not available in collection: " + var_name);
	
	std::size_t index = map->at( m_cfg.prefix + "_" + var_name);
	object_to_be_drawn.push_back( object_collections.at(ientry)->at(index) );
      }
      
      TCanvas canvas("canvas", "canvas");
      DrawCollection(canvas,
		     object_to_be_drawn);
      canvas.Draw();
      
      std::string save_name = m_cfg.outputFolder + "/comparison_" + m_cfg.prefix + "_" + var_name + ".pdf";
      canvas.SaveAs( save_name.c_str() );
      MSG_INFO( "Created plot: " + save_name );
    }
  }

}

#endif

