
#ifndef HISTOGRAM_1D_H
#define HISTOGRAM_1D_H

#include <Data.hpp>
#include <MultiData.hpp>
#include <type_traits>

#include <TCanvas.h>
#include <TH2.h>

namespace EventDataModel {

template <typename... histogram_supported_t> class Histogram_1D {
public:
  using data_type = DataObject::value_type;
  using histogram_type = std::variant<histogram_supported_t...>;

  static_assert(not std::disjunction<
                    std::is_base_of<TH2, histogram_supported_t>...>::value,
                "Histogram 1D Object does not support TH2-type histograms!");

  Histogram_1D() = delete;

  template <typename histogram_t,
            typename Enable = std::enable_if_t<std::disjunction<
                std::is_same<histogram_t, histogram_supported_t>...>::value>>
  Histogram_1D(const std::string &title, histogram_t histogram);

  template <typename histogram_t,
            typename Enable = std::enable_if_t<std::disjunction<
                std::is_same<histogram_t, histogram_supported_t>...>::value>>
  Histogram_1D(const std::string &title, const std::string &target_variable,
               histogram_t histogram);

  ~Histogram_1D() = default;

  void Fill(const DataObject &data);
  void Fill(const MultiDataObject &data);

  void SetLineColor(int color);
  void SetLineStyle(int style);
  void Draw(TCanvas &canvas, const std::string &option);

  void Scale(double scale_value);
  double Integral() const;

  const std::string &title() const;
  const std::string &target_variable() const;

protected:
  template <typename fill_t> void Fill(const fill_t value, const float weight);

private:
  const std::string m_title;
  const std::string m_target_variable;
  histogram_type m_histogram;
};

/* ============================================================== */

template <typename... histogram_supported_t>
template <typename histogram_t, typename Enable>
Histogram_1D<histogram_supported_t...>::Histogram_1D(const std::string &title,
                                                     histogram_t histogram)
    : m_title(title), m_target_variable(""), m_histogram(histogram) {}

template <typename... histogram_supported_t>
template <typename histogram_t, typename Enable>
Histogram_1D<histogram_supported_t...>::Histogram_1D(
    const std::string &title, const std::string &target_variable,
    histogram_t histogram)
    : m_title(title), m_target_variable(target_variable),
      m_histogram(histogram) {}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::Fill(const DataObject &data) {
  std::visit([&](const auto &arg) { Fill(arg, data.weight()); }, data.value());
}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::Fill(const MultiDataObject &data) {
  if (not data.hasLabel(m_target_variable))
    throw std::runtime_error("MultiData collection has not data with label `" +
                             m_target_variable +
                             "` to fill the histogram with.");

  std::visit([&](const auto &arg) { Fill(arg, data.weight()); },
             data.value(m_target_variable));
}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::SetLineColor(int color) {
  std::visit([&](auto &histo) { histo.SetLineColor(color); }, m_histogram);
}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::SetLineStyle(int style) {
  std::visit([&](auto &histo) { histo.SetLineStyle(style); }, m_histogram);
}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::Draw(TCanvas &canvas,
                                                  const std::string &option) {
  std::visit(
      [&](auto &histo) {
        canvas.cd();
        histo.Draw(option.c_str());
      },
      m_histogram);
}

template <typename... histogram_supported_t>
void Histogram_1D<histogram_supported_t...>::Scale(double scale_value) {
  std::visit([&](auto &histo) { histo.Scale(scale_value); }, m_histogram);
}

template <typename... histogram_supported_t>
double Histogram_1D<histogram_supported_t...>::Integral() const {
  return std::visit([&](auto &histo) -> double { return histo.Integral(); },
                    m_histogram);
}

template <typename... histogram_supported_t>
const std::string &Histogram_1D<histogram_supported_t...>::title() const {
  return m_title;
}

template <typename... histogram_supported_t>
const std::string &
Histogram_1D<histogram_supported_t...>::target_variable() const {
  return m_target_variable;
}

template <typename... histogram_supported_t>
template <typename fill_t>
void Histogram_1D<histogram_supported_t...>::Fill(const fill_t value,
                                                  const float weight) {
  std::visit([&](auto &histo) { histo.Fill(value, weight); }, m_histogram);
}

using HistogramObject_1D = Histogram_1D<TH1I, TH1D, TH1F>;
using HistogramObjectCollection_1D = std::vector<HistogramObject_1D>;
} // namespace EventDataModel

#endif
