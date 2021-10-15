
#ifndef PROFILE_PLOTTER_ALGORITHM_H
#define PROFILE_PLOTTER_ALGORITHM_H 1

#include <PlotterAlgorithm.hpp>
#include <Profile.hpp>

namespace Algorithm {

class ProfilePlotterAlgorithm :
    public PlotterAlgorithm<EventDataModel::ProfileObjectCollection> {
public:
  ProfilePlotterAlgorithm() = delete;
  ProfilePlotterAlgorithm(const std::string &name,
			  const Algorithm::PlotterAlgorithm<EventDataModel::ProfileObjectCollection>::Config &cfg);
  ProfilePlotterAlgorithm(const ProfilePlotterAlgorithm &) = delete;
  ProfilePlotterAlgorithm &operator=(const ProfilePlotterAlgorithm &) = delete;
  virtual ~ProfilePlotterAlgorithm() = default;

protected:
  virtual void DrawCollection(const EventDataModel::ProfileObjectCollection *collection);
};

} // namespace Algorithm

#endif
