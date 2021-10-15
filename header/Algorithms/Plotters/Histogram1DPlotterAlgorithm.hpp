
#ifndef HISTOGRAM_1D_PLOTTER_ALGORITHM_H
#define HISTOGRAM_1D_PLOTTER_ALGORITHM_H 1

#include <PlotterAlgorithm.hpp>
#include <Histogram_1D.hpp>

namespace Algorithm {

class Histogram1DPlotterAlgorithm :
    public PlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D> {
public:
  Histogram1DPlotterAlgorithm() = delete;
  Histogram1DPlotterAlgorithm(const std::string &name,
			      const Algorithm::PlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D>::Config &cfg);
  Histogram1DPlotterAlgorithm(const Histogram1DPlotterAlgorithm &) = delete;
  Histogram1DPlotterAlgorithm &operator=(const Histogram1DPlotterAlgorithm &) = delete;
  virtual ~Histogram1DPlotterAlgorithm() = default;

protected:
  virtual void DrawCollection(const EventDataModel::HistogramObjectCollection_1D *collection);
};

} // namespace Algorithm

#endif
