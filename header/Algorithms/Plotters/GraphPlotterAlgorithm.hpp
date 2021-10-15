
#ifndef GRAPH_PLOTTER_ALGORITHM_H
#define GRAPH_PLOTTER_ALGORITHM_H 1

#include <Graph.hpp>
#include <PlotterAlgorithm.hpp>

namespace Algorithm {

class GraphPlotterAlgorithm
    : public PlotterAlgorithm<EventDataModel::GraphObjectCollection> {
public:
  GraphPlotterAlgorithm() = delete;
  GraphPlotterAlgorithm(
      const std::string &name,
      const Algorithm::PlotterAlgorithm<
          EventDataModel::GraphObjectCollection>::Config &cfg);
  GraphPlotterAlgorithm(const GraphPlotterAlgorithm &) = delete;
  GraphPlotterAlgorithm &operator=(const GraphPlotterAlgorithm &) = delete;
  virtual ~GraphPlotterAlgorithm() = default;

protected:
  virtual void
  DrawCollection(const EventDataModel::GraphObjectCollection *collection);
};

} // namespace Algorithm

#endif
