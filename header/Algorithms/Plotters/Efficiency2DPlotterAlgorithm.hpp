
#ifndef EFFICIENCY_2D_PLOTTER_ALGORITHM_H
#define EFFICIENCY_2D_PLOTTER_ALGORITHM_H 1

#include <PlotterAlgorithm.hpp>
#include <Efficiency.hpp>

namespace Algorithm {

class Efficiency2DPlotterAlgorithm :
    public PlotterAlgorithm<EventDataModel::EfficiencyObjectCollection> {
public:
  Efficiency2DPlotterAlgorithm() = delete;
  Efficiency2DPlotterAlgorithm(const std::string &name,
			      const Algorithm::PlotterAlgorithm<EventDataModel::EfficiencyObjectCollection>::Config &cfg);
  Efficiency2DPlotterAlgorithm(const Efficiency2DPlotterAlgorithm &) = delete;
  Efficiency2DPlotterAlgorithm &operator=(const Efficiency2DPlotterAlgorithm &) = delete;
  virtual ~Efficiency2DPlotterAlgorithm() = default;

protected:
  virtual void DrawCollection(const EventDataModel::EfficiencyObjectCollection *collection);
};

} // namespace Algorithm

#endif
