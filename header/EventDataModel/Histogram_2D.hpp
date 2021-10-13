
#ifndef HISTOGRAM_2D_H
#define HISTOGRAM_2D_H

#include <type_traits>
#include <MultiData.hpp>

#include <TH2.h>
#include <TCanvas.h>

namespace EventDataModel {

  template<typename ... histogram_supported_t>
  class Histogram_2D {
  public:
    using data_x_type = MultiDataObject::value_type;
    using data_y_type = MultiDataObject::value_type;
    using histogram_type = std::variant<histogram_supported_t...>;

    static_assert(std::conjunction<std::is_base_of<TH2, histogram_supported_t>...>::value,
		  "Histogram 2D Object does not support TH1-type histograms!");
    
    Histogram_2D() = delete;

    template<typename histogram_t,
	     typename Enable = std::enable_if_t<std::disjunction<std::is_same<histogram_t, histogram_supported_t>...>::value>>
    Histogram_2D(const std::string& title,
		 histogram_t histogram);

    template<typename histogram_t,
	     typename Enable = std::enable_if_t<std::disjunction<std::is_same<histogram_t, histogram_supported_t>...>::value>>
    Histogram_2D(const std::string& title,
		 const std::string& target_variable_x,
		 const std::string& target_variable_y,
		 histogram_t histogram);
    
    ~Histogram_2D() = default;
    
    void Fill(const MultiDataObject& data);

    void SetLineColor(int color);
    void SetLineStyle(int style);
    void Draw(TCanvas& canvas,
	      const std::string& option);

    void Scale(double scale_value);
    double Integral() const;
    
    const std::string& title() const;
    const std::string& target_variable_x() const;
    const std::string& target_variable_y() const;
    
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
    histogram_type m_histogram;
  };
  
  /* ============================================================== */
  
  template<typename ... histogram_supported_t>
  template<typename histogram_t,
	   typename Enable>
  Histogram_2D<histogram_supported_t...>::Histogram_2D(const std::string& title,
						       histogram_t histogram)
  : m_title(title),
      m_target_variable_x(""),
      m_target_variable_y(""),
      m_histogram(histogram)
  {}
  
  template<typename ... histogram_supported_t>
  template<typename histogram_t,
           typename Enable>
  Histogram_2D<histogram_supported_t...>::Histogram_2D(const std::string& title,
						       const std::string& target_variable_x,
						       const std::string& target_variable_y,
						       histogram_t histogram)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_target_variable_y(target_variable_y),
      m_histogram(histogram)
  {}
  
  template<typename ... histogram_supported_t>
  void
  Histogram_2D<histogram_supported_t...>::Fill(const MultiDataObject& data)
  {
    if (not data.hasLabel(m_target_variable_x) or not data.hasLabel(m_target_variable_y))
      throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_x + "` or `" + m_target_variable_y + "` to fill the histogram with.");

    std::visit([&] (const auto& argx) {
		 std::visit([&] (const auto& argy) {
			      Fill(argx, argy, data.weight());
			    }, data.value(m_target_variable_y));
	       }, data.value(m_target_variable_x));
  }

  template<typename ... histogram_supported_t>
  void
  Histogram_2D<histogram_supported_t...>::SetLineColor(int color)
  {
    std::visit( [&] (auto& histo) {
                  histo.SetLineColor(color);
                }, m_histogram);
  }

  template<typename ... histogram_supported_t>
  void
  Histogram_2D<histogram_supported_t...>::SetLineStyle(int style)
  {
    std::visit( [&] (auto& histo) {
                  histo.SetLineStyle(style);
                }, m_histogram);
  }
  
  template<typename ... histogram_supported_t>
  void
  Histogram_2D<histogram_supported_t...>::Draw(TCanvas& canvas,
					       const std::string& option)
  {
    std::visit([&] (auto& histo) {
		 canvas.cd();
		 histo.Draw(option.c_str());
	       }, m_histogram);
  }

  template<typename ... histogram_supported_t>
  void
  Histogram_2D<histogram_supported_t...>::Scale(double scale_value)
  {
    std::visit( [&] (auto& histo) {
                  histo.Scale(scale_value);
                }, m_histogram);
  }

  template<typename ... histogram_supported_t>
  double
  Histogram_2D<histogram_supported_t...>::Integral() const
  {
    return std::visit( [&] (auto& histo) -> double
                       {
                         return	histo.Integral();
                       }, m_histogram);
  }
  
  template<typename ... histogram_supported_t>
  const std::string&
  Histogram_2D<histogram_supported_t...>::title() const
  { return m_title; }

  template<typename ... histogram_supported_t>
  const std::string&
  Histogram_2D<histogram_supported_t...>::target_variable_x() const
  { return m_target_variable_x; }

  template<typename ... histogram_supported_t>
  const std::string&
  Histogram_2D<histogram_supported_t...>::target_variable_y() const
  { return m_target_variable_y; }
  
  
  template<typename ... histogram_supported_t>
  template<typename fill_x_t,
	   typename fill_y_t>
  void
  Histogram_2D<histogram_supported_t...>::Fill(const fill_x_t value_x,
					       const fill_y_t value_y,
					       const float weight )
  {
    std::visit([&] (auto& histo) {
		 histo.Fill(value_x, value_y, weight);
	       }, m_histogram);
  }

  using HistogramObject_2D = Histogram_2D<TH2I, TH2D, TH2F>;
  using HistogramObjectCollection_2D = std::vector<HistogramObject_2D>;
}

#endif
