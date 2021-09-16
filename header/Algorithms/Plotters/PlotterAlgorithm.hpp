
#ifndef PLOTTER_ALGORITHM_H
#define PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>

#include <string>
#include <vector>

namespace Algorithm {

  class PlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string inputCollection_hist_1d = "";
      std::string inputCollection_hist_2d = "";
      std::string inputCollection_eff_1d = "";
      std::string inputCollection_eff_2d = "";
      std::string inputCollection_gr = "";
      std::string outputFolder = "";
    };

    PlotterAlgorithm() = delete;
    PlotterAlgorithm(const std::string& name,
		     const Config& cfg);
    PlotterAlgorithm(const PlotterAlgorithm&) = delete;
    PlotterAlgorithm& operator=(const PlotterAlgorithm&) = delete;
    virtual ~PlotterAlgorithm() = default;
    
    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;
    
  private:
    const Config m_cfg;
  };
  
}

#endif
