
#ifndef GRAPH_COMPARISON_PLOTTER_ALGORITHM_H
#define GRAPH_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <ComparisonPlotterAlgorithm.hpp>
#include <Graph.hpp>

namespace Algorithm {

class GraphComparisonPlotterAlgorithm
    : public ComparisonPlotterAlgorithm<EventDataModel::GraphObjectCollection> {
public:
  GraphComparisonPlotterAlgorithm() = delete;
  GraphComparisonPlotterAlgorithm(
      const std::string &name,
      const ComparisonPlotterAlgorithm<
          EventDataModel::GraphObjectCollection>::Config &cfg);
  GraphComparisonPlotterAlgorithm(const GraphComparisonPlotterAlgorithm &) =
      delete;
  GraphComparisonPlotterAlgorithm &
  operator=(const GraphComparisonPlotterAlgorithm &) = delete;
  virtual ~GraphComparisonPlotterAlgorithm() = default;

protected:
  virtual void
  DrawCollection(TCanvas &canvas,
                 EventDataModel::GraphObjectCollection &collection);
};

} // namespace Algorithm

#endif
