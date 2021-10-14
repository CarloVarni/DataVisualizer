
#ifndef PROFILE_COMPARISON_PLOTTER_ALGORITHM_H
#define PROFILE_COMPARISON_PLOTTER_ALGORITHM_H 1

#include <ComparisonPlotterAlgorithm.hpp>
#include <Profile.hpp>

namespace Algorithm {

class ProfileComparisonPlotterAlgorithm
    : public ComparisonPlotterAlgorithm<
          EventDataModel::ProfileObjectCollection> {
public:
  ProfileComparisonPlotterAlgorithm() = delete;
  ProfileComparisonPlotterAlgorithm(
      const std::string &name,
      const ComparisonPlotterAlgorithm<
          EventDataModel::ProfileObjectCollection>::Config &cfg);
  ProfileComparisonPlotterAlgorithm(const ProfileComparisonPlotterAlgorithm &) =
      delete;
  ProfileComparisonPlotterAlgorithm &
  operator=(const ProfileComparisonPlotterAlgorithm &) = delete;
  virtual ~ProfileComparisonPlotterAlgorithm() = default;

protected:
  virtual void
  DrawCollection(TCanvas &canvas,
                 EventDataModel::ProfileObjectCollection &collection);
};

} // namespace Algorithm

#endif
