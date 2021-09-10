
#ifndef HISTOGRAM_2D_H
#define HISTOGRAM_2D_H

#include <type_traits>
#include <string>
#include <MultiData.hpp>

#include <TH2.h>
#include <TCanvas.h>

namespace EventDataModel {

  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable = std::enable_if_t<std::is_base_of<TH2, histogram_t>::value>>
  class Histogram_2D {
  public:
    using data_x_type = data_x_t;
    using data_y_type = data_y_t;
    using histogram_type = histogram_t;
    
    Histogram_2D() = delete;

    Histogram_2D(const std::string& title,
		 int nbins_x, double min_x, double max_x,
		 int nbins_y, double min_y, double max_y);

    Histogram_2D(const std::string& title,
		 const std::string& target_variable_x,
		 const std::string& target_variable_y,
		 int nbins_x, double min_x, double max_x,
		 int nbins_y, double min_y, double max_y);
    
    ~Histogram_2D() = default;
    
    template<typename ... args>
    void Fill(const MultiData<args...>& data);
    
    void Draw(TCanvas& canvas,
	      const std::string& option);
    
  protected:
    template<typename fill_x_t,
	     typename fill_y_t>
    void Fill(const fill_x_t value_x,
	      const fill_y_t value_y,
	      const float weight);
    
  private:
    const std::string m_title;
    const std::string m_target_variable_x;
    const std::string m_target_variable_y;
    histogram_t m_histogram;
  };
  
  /* ============================================================== */
  
  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable>
  Histogram_2D<data_x_t, data_y_t, histogram_t, Enable>::Histogram_2D(const std::string& title,
								      int nbins_x, double min_x, double max_x,
								      int nbins_y, double min_y, double max_y)
    : m_title(title),
      m_target_variable_x(""),
      m_target_variable_y(""),
      m_histogram(title.c_str(), title.c_str(),
		  nbins_x, min_x, max_x,
		  nbins_y, min_y, max_y)
  {}
  
  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable>
  Histogram_2D<data_x_t, data_y_t, histogram_t, Enable>::Histogram_2D(const std::string& title,
								      const std::string& target_variable_x,
								      const std::string& target_variable_y,
								      int nbins_x, double min_x, double max_x,
								      int nbins_y, double min_y, double max_y)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_target_variable_y(target_variable_y),
      m_histogram(title.c_str(), title.c_str(),
		  nbins_x, min_x, max_x,
		  nbins_y, min_y, max_y)
  {}
  
  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable>
  template<typename ... args>
  void
  Histogram_2D<data_x_t, data_y_t, histogram_t, Enable>::Fill(const MultiData<args...>& data)
  {
    if (not data.hasLabel(m_target_variable_x) or not data.hasLabel(m_target_variable_y))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_x + "` or `" + m_target_variable_y + "` to fill the histogram with.");
    
    data_x_t value_x = std::visit([&](const auto& arg) -> data_x_t {
				    return arg;
				  }, data.value(m_target_variable_x));
    data_y_t value_y = std::visit([&](const auto& arg) -> data_y_t {
				    return arg;
				  }, data.value(m_target_variable_y));
    Fill(value_x, value_y, data.weight());
  }
  
  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable>
  void
  Histogram_2D<data_x_t, data_y_t, histogram_t, Enable>::Draw(TCanvas& canvas,
							      const std::string& option)
  {
    canvas.cd();
    m_histogram.Draw(option.c_str());
  }
  
  template<typename data_x_t,
	   typename data_y_t,
	   typename histogram_t,
	   typename Enable>
  template<typename fill_x_t,
	   typename fill_y_t>
  void
  Histogram_2D<data_x_t, data_y_t, histogram_t, Enable>::Fill(const fill_x_t value_x,
							      const fill_y_t value_y,
							      const float weight )
  {
    if (not std::is_same<fill_x_t, data_x_t>::value or not std::is_same<fill_y_t, data_y_t>::value)
      throw std::runtime_error("Trying to fill histogram ('" + m_title + "') with wrong variable type ('" + m_target_variable_x + "' and `" + m_target_variable_y + "`)");
    
    m_histogram.Fill(value_x, value_y, weight);
  }

}

#endif
