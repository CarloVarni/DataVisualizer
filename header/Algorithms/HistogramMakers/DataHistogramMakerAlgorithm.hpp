
#ifndef DATA_HISTOGRAM_MAKER_ALGORITHM_H
#define DATA_HISTOGRAM_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Histogram_1D.hpp>

#include <vector>

namespace Algorithm {

class DataHistogramMakerAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string inputCollection;
    std::string inputMaskName;
    std::string outputCollection_1d;

    std::vector<std::string> variableNames_1D;
    std::vector<EventDataModel::HistogramObject_1D::histogram_type>
        histogramDefs_1D;
  };

  DataHistogramMakerAlgorithm() = delete;
  DataHistogramMakerAlgorithm(const std::string &name, const Config &cfg);
  DataHistogramMakerAlgorithm(const DataHistogramMakerAlgorithm &) = delete;
  DataHistogramMakerAlgorithm &
  operator=(const DataHistogramMakerAlgorithm &) = delete;
  virtual ~DataHistogramMakerAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

private:
  const Config m_cfg;
};

} // namespace Algorithm

#endif
