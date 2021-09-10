
#ifndef HISTOGRAM_1D_H
#define HISTOGRAM_1D_H

#include <type_traits>
#include <string>
#include <Data.hpp>
#include <MultiData.hpp>

#include <TH2.h>
#include <TCanvas.h>

namespace EventDataModel {
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable = std::enable_if_t<not std::is_base_of<TH2, histogram_t>::value>>
  class Histogram_1D {
  public:
    using data_type = data_t;
    using histogram_type = histogram_t;
    
    Histogram_1D() = delete;
    
    Histogram_1D(const std::string& title,
		 int nbins, double min, double max);
    
    Histogram_1D(const std::string& title,
		 const std::string& target_variable,
		 int nbins, double min, double max);
    ~Histogram_1D() = default;
    
    void Fill(const Data<data_t>& data);
    
    template<typename ... args>
    void Fill(const MultiData<args...>& data);
    
    void Draw(TCanvas& canvas,
	      const std::string& option);
    
  protected:
    template<typename fill_t>
    void Fill(const fill_t value,
	      const float weight);
    
  private:
    const std::string m_title;
    const std::string m_target_variable;
    histogram_t m_histogram;
  };

  /* ============================================================== */
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  Histogram_1D<data_t, histogram_t, Enable>::Histogram_1D(const std::string& title,
							  int nbins, double min, double max)
    : m_title(title),
      m_target_variable(""),
      m_histogram(title.c_str(), title.c_str(), nbins, min, max)
  {}
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  Histogram_1D<data_t, histogram_t, Enable>::Histogram_1D(const std::string& title,
							  const std::string& target_variable,
							  int nbins, double min, double max)
    : m_title(title),
      m_target_variable(target_variable),
      m_histogram(title.c_str(), title.c_str(), nbins, min, max)
  {}
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  void
  Histogram_1D<data_t, histogram_t, Enable>::Fill(const Data<data_t>& data)
  { Fill(data.value(), data.weight()); }
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  template<typename ... args>
  void
  Histogram_1D<data_t, histogram_t, Enable>::Fill(const MultiData<args...>& data)
  {
    if (not data.hasLabel(m_target_variable))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable + "` to fill the histogram with.");
    
    std::visit([&](const auto& arg){
		 Fill(arg, data.weight());
	       }, data.value(m_target_variable));
  }
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  void
  Histogram_1D<data_t, histogram_t, Enable>::Draw(TCanvas& canvas,
						  const std::string& option)
  {
    canvas.cd();
    m_histogram.Draw(option.c_str());
  }
  
  template<typename data_t,
	   typename histogram_t,
	   typename Enable>
  template<typename fill_t>
  void
  Histogram_1D<data_t, histogram_t, Enable>::Fill(const fill_t value,
						  const float weight )
  {
    if (not std::is_same<fill_t, data_t>::value)
      throw std::runtime_error("Trying to fill histogram ('" + m_title + "') with wrong variable type ('" + m_target_variable + "')");
    
    m_histogram.Fill(value, weight);
  }
  
}

#endif
