
#ifndef EFFICIENCY_1D_PLOTTER_ALGORITHM_H
#define EFFICIENCY_1D_PLOTTER_ALGORITHM_H 1

#include <Efficiency.hpp>
#include <PlotterAlgorithm.hpp>

namespace Algorithm {

class Efficiency1DPlotterAlgorithm
    : public PlotterAlgorithm<EventDataModel::EfficiencyObjectCollection> {
public:
  Efficiency1DPlotterAlgorithm() = delete;
  Efficiency1DPlotterAlgorithm(
      const std::string &name,
      const Algorithm::PlotterAlgorithm<
          EventDataModel::EfficiencyObjectCollection>::Config &cfg);
  Efficiency1DPlotterAlgorithm(const Efficiency1DPlotterAlgorithm &) = delete;
  Efficiency1DPlotterAlgorithm &
  operator=(const Efficiency1DPlotterAlgorithm &) = delete;
  virtual ~Efficiency1DPlotterAlgorithm() = default;

protected:
  virtual void
  DrawCollection(const EventDataModel::EfficiencyObjectCollection *collection);
};

} // namespace Algorithm

#endif
