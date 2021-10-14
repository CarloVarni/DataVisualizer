
#ifndef MULTIDATA_HISTOGRAM_MAKER_ALGORITHM_H
#define MULTIDATA_HISTOGRAM_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>

#include <vector>

namespace Algorithm {

class MultiDataHistogramMakerAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string inputCollection;
    std::string inputMaskName;
    std::string outputCollection_1d;
    std::string outputCollection_2d;

    std::vector<std::string> variableNames_1D;
    std::vector<EventDataModel::HistogramObject_1D::histogram_type>
        histogramDefs_1D;

    std::vector<std::pair<std::string, std::string>> variableNames_2D;
    std::vector<EventDataModel::HistogramObject_2D::histogram_type>
        histogramDefs_2D;
  };

  MultiDataHistogramMakerAlgorithm() = delete;
  MultiDataHistogramMakerAlgorithm(const std::string &name, const Config &cfg);
  MultiDataHistogramMakerAlgorithm(const MultiDataHistogramMakerAlgorithm &) =
      delete;
  MultiDataHistogramMakerAlgorithm &
  operator=(const MultiDataHistogramMakerAlgorithm &) = delete;
  virtual ~MultiDataHistogramMakerAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

private:
  const Config m_cfg;
};

} // namespace Algorithm

#endif
