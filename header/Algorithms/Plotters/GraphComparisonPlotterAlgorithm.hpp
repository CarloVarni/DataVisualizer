
#ifndef GRAPH_COMPARISON_PLOTTER_ALGORITHM_H
#define GRAPH_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Graph.hpp>

#include <TCanvas.h>

#include <string>
#include <vector>

namespace Algorithm {

  class GraphComparisonPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string outputFolder = "";
      std::vector<std::string> originalInputCollection = {};
      std::vector<std::string> variableNames = {};
    };

    GraphComparisonPlotterAlgorithm() = delete;
    GraphComparisonPlotterAlgorithm(const std::string& name,
				    const Config& cfg);
    GraphComparisonPlotterAlgorithm(const GraphComparisonPlotterAlgorithm&) = delete;
    GraphComparisonPlotterAlgorithm& operator=(const GraphComparisonPlotterAlgorithm&) = delete;
    virtual ~GraphComparisonPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
    
  protected:
    void DrawCollection(TCanvas& canvas,
			EventDataModel::GraphObjectCollection& collection);
       
  private:
    const Config m_cfg;
  };

}

#endif

