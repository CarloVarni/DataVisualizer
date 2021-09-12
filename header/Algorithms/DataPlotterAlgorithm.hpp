
#ifndef DATA_PLOTTER_ALGORITHM_H
#define DATA_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Histogram_1D.hpp>
#include <Data.hpp>

#include <vector>
#include <string>

namespace Algorithm {

  class DataPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string inputCollection;
      std::string inputMaskName;
      std::string outputFolder;
      
      std::vector<std::string> variableNames;
      std::vector<EventDataModel::HistogramObject_1D::histogram_type> histogramDefs;
    };

    DataPlotterAlgorithm() = delete;
    DataPlotterAlgorithm(const std::string&,
			 const Config& cfg);
    DataPlotterAlgorithm(const DataPlotterAlgorithm&) = delete;
    DataPlotterAlgorithm& operator=(const DataPlotterAlgorithm&) = delete;
    virtual ~DataPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
    
  private:
    const Config m_cfg;
  };

}
#endif
