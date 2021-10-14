
#ifndef PROFILE_H
#define PROFILE_H 1

#include <Data.hpp>
#include <MultiData.hpp>

#include <TCanvas.h>
#include <TProfile.h>

namespace EventDataModel {

class Profile {
public:
  Profile() = delete;
  Profile(const std::string &title, TProfile profile);
  Profile(const std::string &title, const std::string &target_variable_x,
          const std::string &target_variable_y, TProfile profile);
  ~Profile() = default;

  void Fill(const MultiDataObject &data);
  void Draw(TCanvas &canvas, const std::string &option);

  const std::string &title() const;
  const std::string &target_variable_x() const;
  const std::string &target_variable_y() const;

  void SetLineColor(int color);
  void SetMarkerColor(int color);

private:
  const std::string m_title;
  const std::string m_target_variable_x;
  const std::string m_target_variable_y;
  TProfile m_profile;
};

using ProfileObject = Profile;
using ProfileObjectCollection = std::vector<Profile>;

} // namespace EventDataModel

#endif
