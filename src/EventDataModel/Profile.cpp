
#include <Profile.hpp>

namespace EventDataModel {

  Profile::Profile(const std::string& title,
		   TProfile profile)
    : m_title(title),
      m_target_variable_x(""),
      m_target_variable_y(""),
      m_profile(profile)
  {}
  
  Profile::Profile(const std::string& title,
		   const std::string& target_variable_x,
		   const std::string& target_variable_y,
		   TProfile profile)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_profile(profile)
  {}

  void
  Profile::Fill(const MultiDataObject& data)
  {
    if (not data.hasLabel(m_target_variable_x))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_x + "` to compute the profile with.");
    if (not data.hasLabel(m_target_variable_y))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_y + "` to compute the profile with.");

    std::visit([&] (const auto& arg_x) {
		 
		 std::visit([&] (const auto& arg_y) {
			      m_profile.Fill(arg_x, arg_y, data.weight());
			    }, data.value(m_target_variable_y));
		 
               }, data.value(m_target_variable_x));
    
  }

  void
  Profile::Draw(TCanvas& canvas,
		const std::string& option)
  {
    canvas.cd();
    m_profile.Draw(option.c_str());
  }
  
  const std::string&
  Profile::title() const
  { return m_title; }

  const std::string&
  Profile::target_variable_x() const
  { return m_target_variable_x; }

  const std::string&
  Profile::target_variable_y() const
  { return m_target_variable_y; }

  void
  Profile::SetLineColor(int color)
  { m_profile.SetLineColor(color); }

  void
  Profile::SetMarkerColor(int color)
  { m_profile.SetMarkerColor(color); }
  
}

