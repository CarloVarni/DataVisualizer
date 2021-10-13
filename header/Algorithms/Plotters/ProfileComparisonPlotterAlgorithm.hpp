
#ifndef PROFILE_COMPARISON_PLOTTER_ALGORITHM_H
#define PROFILE_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Profile.hpp>

#include <TCanvas.h>

#include <string>
#include <vector>

namespace Algorithm {

  class ProfileComparisonPlotterAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string outputFolder = "";
      std::vector<std::string> originalInputCollection = {};
      std::vector<std::string> variableNames = {};
    };

    ProfileComparisonPlotterAlgorithm() = delete;
    ProfileComparisonPlotterAlgorithm(const std::string& name,
				      const Config& cfg);
    ProfileComparisonPlotterAlgorithm(const ProfileComparisonPlotterAlgorithm&) = delete;
    ProfileComparisonPlotterAlgorithm& operator=(const ProfileComparisonPlotterAlgorithm&) = delete;
    virtual ~ProfileComparisonPlotterAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext&) override;
    
  protected:
    void DrawCollection(TCanvas& canvas,
			EventDataModel::ProfileObjectCollection& collection);
       
  private:
    const Config m_cfg;
  };

}

#endif

