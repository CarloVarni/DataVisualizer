
#ifndef COMPARISON_PLOTTER_ALGORITHM_H
#define COMPARISON_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Histogram_1D.hpp>
#include <Graph.hpp>
#include <Profile.hpp>
#include <Efficiency.hpp>

#include <TCanvas.h>

#include <string>
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
    template<typename Enable = std::enable_if_t<std::is_same<object_collection_t, EventDataModel::HistogramObjectCollection_1D>::value>>
    void DrawCollection(TCanvas& canvas,
			EventDataModel::HistogramObjectCollection_1D& collection);

    template<typename Enable = std::enable_if_t<std::is_same<object_collection_t, EventDataModel::GraphObjectCollection>::value>>
    void DrawCollection(TCanvas& canvas,
			EventDataModel::GraphObjectCollection& collection);
    
    template<typename Enable = std::enable_if_t<std::is_same<object_collection_t, EventDataModel::EfficiencyObjectCollection>::value>>
    void DrawCollection(TCanvas& canvas,
			EventDataModel::EfficiencyObjectCollection& collection);

    template<typename Enable = std::enable_if_t<std::is_same<object_collection_t, EventDataModel::ProfileObjectCollection>::value>>
    void DrawCollection(TCanvas& canvas,
			EventDataModel::ProfileObjectCollection& collection);
    
  private:
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

    for (const std::string& input : m_cfg.originalInputCollection ) {
      object_collections.push_back( context.get<object_collection_t>( m_cfg.prefix + "_" + input) );
      object_maps.push_back( context.get<std::unordered_map<std::string, std::size_t>>("index_" + m_cfg.prefix + "_" + input) );
    }

    for (const std::string& var_name : m_cfg.variableNames) {
      object_collection_t object_to_be_drawn;
      
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
      MSG_INFO( "Created histogram: " + save_name );
    }
  }

  template<typename object_collection_t>
  template<typename Enable>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::DrawCollection(TCanvas& canvas,
								  EventDataModel::HistogramObjectCollection_1D& collection)
  {
    canvas.cd();
    for (auto ihisto(0); ihisto<collection.size(); ihisto++) {
      auto& histo = collection.at(ihisto);
      histo.Scale( 1./histo.Integral() );
      histo.SetLineColor(ihisto + 1);
      histo.SetLineStyle(ihisto == 0 ? 1 : 2);

      histo.Draw(canvas, ihisto == 0 ? "HIST" : "HISTSAME" );
    }
  }

  template<typename object_collection_t>
  template<typename Enable>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::DrawCollection(TCanvas& canvas,
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
  
  
  template<typename object_collection_t>
  template<typename Enable>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::DrawCollection(TCanvas& canvas,
								  EventDataModel::EfficiencyObjectCollection& collection)
  {
    canvas.cd();
    for (auto igr(0); igr<collection.size(); igr++) {
      auto& gr = collection.at(igr);
      
      gr.SetLineColor(igr + 1);
      gr.SetLineStyle(igr == 0 ? 1 : 2);
      gr.SetMarkerStyle(20);
      gr.SetMarkerColor(igr + 1);
      
      gr.Draw(canvas, igr == 0 ? "APL" : "PLSAME" );
    }
  }
  
  template<typename object_collection_t>
  template<typename Enable>
  void
  ComparisonPlotterAlgorithm<object_collection_t>::DrawCollection(TCanvas& canvas,
								  EventDataModel::ProfileObjectCollection& collection)
  {
    canvas.cd();
    for (auto igr(0); igr<collection.size(); igr++) {
      auto& gr = collection.at(igr);

      gr.SetLineColor(igr + 1);
      gr.SetMarkerColor(igr + 1);
      
      gr.Draw(canvas, igr == 0 ? "" : "SAME" );
    }
  }


  using HistogramComparisonPlotterAlgorithm = ComparisonPlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D>;
  using GraphComparisonPlotterAlgorithm = ComparisonPlotterAlgorithm<EventDataModel::GraphObjectCollection>;
  using EfficiencyComparisonPlotterAlgorithm = ComparisonPlotterAlgorithm<EventDataModel::EfficiencyObjectCollection>;
  using ProfileComparisonPlotterAlgorithm = ComparisonPlotterAlgorithm<EventDataModel::ProfileObjectCollection>;
}

#endif

