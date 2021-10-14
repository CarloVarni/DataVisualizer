
#ifndef EFFICIENCY_COMPARISON_PLOTTER_ALGORITHM_H
#define EFFICIENCY_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <ComparisonPlotterAlgorithm.hpp>
#include <Efficiency.hpp>

namespace Algorithm {

class EfficiencyComparisonPlotterAlgorithm
    : public ComparisonPlotterAlgorithm<
          EventDataModel::EfficiencyObjectCollection> {
public:
  EfficiencyComparisonPlotterAlgorithm() = delete;
  EfficiencyComparisonPlotterAlgorithm(
      const std::string &name,
      const ComparisonPlotterAlgorithm<
          EventDataModel::EfficiencyObjectCollection>::Config &cfg);
  EfficiencyComparisonPlotterAlgorithm(
      const EfficiencyComparisonPlotterAlgorithm &) = delete;
  EfficiencyComparisonPlotterAlgorithm &
  operator=(const EfficiencyComparisonPlotterAlgorithm &) = delete;
  virtual ~EfficiencyComparisonPlotterAlgorithm() = default;

protected:
  virtual void
  DrawCollection(TCanvas &canvas,
                 EventDataModel::EfficiencyObjectCollection &collection);
};

} // namespace Algorithm

#endif
