
#include <Efficiency.hpp>

namespace EventDataModel {

  Efficiency::Efficiency(const std::string& title,
			 TEfficiency efficiency)
    : m_title(title),
      m_target_variable_x(""),
      m_target_variable_y(""),
      m_efficiency(efficiency)
  {}

  Efficiency::Efficiency(const std::string& title,
			 const std::string& target_variable_x,
			 TEfficiency efficiency)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_target_variable_y(""),
      m_efficiency(efficiency)
  {}

  Efficiency::Efficiency(const std::string& title,
			 const std::string& target_variable_x,
			 const std::string& target_variable_y,
			 TEfficiency efficiency)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_target_variable_y(target_variable_y),
      m_efficiency(efficiency)
  {}

  void
  Efficiency::Fill(const DataObject& data, bool isGood)
  {
    std::visit([&] (const auto& arg) {
		 m_efficiency.Fill(isGood, arg);
	       }, data.value());
  }

  void
  Efficiency::Fill(const MultiDataObject& data, bool isGood)
  {
    if (not data.hasLabel(m_target_variable_x))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_x + "` to compute the efficiency with.");
    if (not m_target_variable_y.empty() and not data.hasLabel(m_target_variable_y))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_y + "` to compute the efficiency with.");
    
    std::visit([&] (const auto& arg_x) {

		 if (m_target_variable_y.empty()) {
		   m_efficiency.Fill(isGood, arg_x);
		 } else {
		   std::visit([&] (const auto& arg_y) {
				m_efficiency.Fill(isGood, arg_x, arg_y);
			      }, data.value(m_target_variable_y));
		 }

               }, data.value(m_target_variable_x));
  }

  void
  Efficiency::Draw(TCanvas& canvas,
		   const std::string& option)
  {
    canvas.cd();
    m_efficiency.Draw(option.c_str());
  }

  const std::string&
  Efficiency::title() const
  { return m_title; }

  const std::string&
  Efficiency::target_variable_x() const
  { return m_target_variable_x; }

  const std::string&
  Efficiency::target_variable_y() const
  { return m_target_variable_y; }

}
