
#ifndef MULTIDATA_PLOTTER_ALGORITHM_H
#define MULTIDATA_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>

#include <string>
#include <vector>
#include <variant>

namespace Algorithm {
  class MultiDataPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:

    struct Config {
      std::string inputCollection;
      std::string inputMaskName;
      std::string outputFolder;

      std::vector<std::string> variableNames_1D;
      std::vector<EventDataModel::HistogramObject_1D::histogram_type> histogramDefs_1D;

      std::vector<std::pair<std::string, std::string>> variableNames_2D;
      std::vector<EventDataModel::HistogramObject_2D::histogram_type> histogramDefs_2D;
    };
    
    MultiDataPlotterAlgorithm() = delete;
    MultiDataPlotterAlgorithm(const std::string&,
			      const Config& cfg);
    MultiDataPlotterAlgorithm& operator=(const MultiDataPlotterAlgorithm&) = delete;
    virtual ~MultiDataPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;
    
  private:
    const Config m_cfg;
  };

}

#endif
