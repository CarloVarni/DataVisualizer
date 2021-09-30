
#ifndef EFFICIENCY_COMPARISON_PLOTTER_ALGORITHM_H
#define EFFICIENCY_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Efficiency.hpp>

#include <TCanvas.h>

#include <string>
#include <vector>

namespace Algorithm {

  class EfficiencyComparisonPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string outputFolder = "";
      std::vector<std::string> originalInputCollection = {};
      std::vector<std::string> variableNames = {};
    };

    EfficiencyComparisonPlotterAlgorithm() = delete;
    EfficiencyComparisonPlotterAlgorithm(const std::string& name,
				    const Config& cfg);
    EfficiencyComparisonPlotterAlgorithm(const EfficiencyComparisonPlotterAlgorithm&) = delete;
    EfficiencyComparisonPlotterAlgorithm& operator=(const EfficiencyComparisonPlotterAlgorithm&) = delete;
    virtual ~EfficiencyComparisonPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
    
  protected:
    void DrawCollection(TCanvas& canvas,
			EventDataModel::EfficiencyObjectCollection& collection);
       
  private:
    const Config m_cfg;
  };

}

#endif

