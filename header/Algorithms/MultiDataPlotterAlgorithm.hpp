
#ifndef MULTIDATA_PLOTTER_ALGORITHM_H
#define MULTIDATA_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>

#include <string>
#include <vector>

namespace Algorithm {
  class MultiDataPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:

    struct Config {
      std::string inputCollection;
      std::string outputFolder;
      std::vector<std::string> variableNames;
    };

    MultiDataPlotterAlgorithm() = delete;
    MultiDataPlotterAlgorithm(const std::string&,
			      const Config& cfg);
    MultiDataPlotterAlgorithm& operator=(const MultiDataPlotterAlgorithm&) = delete;
    virtual ~MultiDataPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;
    virtual void finalize() override;
    
  private:
    const Config m_cfg;
  };

}

#endif
