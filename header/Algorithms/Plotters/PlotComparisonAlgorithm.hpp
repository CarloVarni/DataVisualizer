
#ifndef PLOT_COMPARISON_ALGORITHM_H
#define PLOT_COMPARISON_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Histogram_1D.hpp>

#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

#include <string>
#include <vector>

namespace Algorithm {

  class PlotComparisonAlgorithm
    : public Core::BaseAlgorithm {
  public:
    
    struct Config {
      std::string outputFolder;
      std::vector<std::string> originalInputCollection;
      std::vector<std::string> variableNames;
    };

    PlotComparisonAlgorithm() = delete;
    PlotComparisonAlgorithm(const std::string& name,
			    const Config& cfg);
    PlotComparisonAlgorithm(const PlotComparisonAlgorithm&) = delete;
    PlotComparisonAlgorithm& operator=(const PlotComparisonAlgorithm&) = delete;
    virtual ~PlotComparisonAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;

  protected:
    void DrawCollection(TCanvas& canvas,
			EventDataModel::HistogramObjectCollection_1D& collection);
      
  private:
    const Config m_cfg;
  };

}

#endif
