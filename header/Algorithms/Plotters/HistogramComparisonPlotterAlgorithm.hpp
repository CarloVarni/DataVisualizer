
#ifndef HISTOGRAM_COMPARISON_PLOTTER_ALGORITHM_H
#define HISTOGRAM_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <ComparisonPlotterAlgorithm.hpp>
#include <Histogram_1D.hpp>

namespace Algorithm {
  
  class HistogramComparisonPlotterAlgorithm
    : public ComparisonPlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D> {
  public:
    HistogramComparisonPlotterAlgorithm() = delete;
    HistogramComparisonPlotterAlgorithm(const std::string& name,
					const ComparisonPlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D>::Config& cfg);
    HistogramComparisonPlotterAlgorithm(const HistogramComparisonPlotterAlgorithm&) = delete;
    HistogramComparisonPlotterAlgorithm& operator=(const HistogramComparisonPlotterAlgorithm&) = delete;
    virtual ~HistogramComparisonPlotterAlgorithm() = default;

  protected:
    virtual void DrawCollection(TCanvas& canvas,
				EventDataModel::HistogramObjectCollection_1D& collection);
  };

}

#endif
