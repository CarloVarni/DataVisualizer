
#ifndef HISTOGRAM_2D_PLOTTER_ALGORITHM_H
#define HISTOGRAM_2D_PLOTTER_ALGORITHM_H 1

#include <Histogram_2D.hpp>
#include <PlotterAlgorithm.hpp>

namespace Algorithm {

class Histogram2DPlotterAlgorithm
    : public PlotterAlgorithm<EventDataModel::HistogramObjectCollection_2D> {
public:
  Histogram2DPlotterAlgorithm() = delete;
  Histogram2DPlotterAlgorithm(
      const std::string &name,
      const Algorithm::PlotterAlgorithm<
          EventDataModel::HistogramObjectCollection_2D>::Config &cfg);
  Histogram2DPlotterAlgorithm(const Histogram2DPlotterAlgorithm &) = delete;
  Histogram2DPlotterAlgorithm &
  operator=(const Histogram2DPlotterAlgorithm &) = delete;
  virtual ~Histogram2DPlotterAlgorithm() = default;

protected:
  virtual void DrawCollection(
      const EventDataModel::HistogramObjectCollection_2D *collection);
};

} // namespace Algorithm

#endif
