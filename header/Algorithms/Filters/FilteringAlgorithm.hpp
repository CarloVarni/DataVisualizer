
#ifndef FILTERING_ALGORITHM_H
#define FILTERING_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Data.hpp>
#include <MultiData.hpp>

#include <functional>

namespace Algorithm {

template <typename data_t>
class FilteringAlgorithm : public Core::BaseAlgorithm {

public:
  struct Config {
    std::string inputCollection;
    std::string inputMaskName;
    std::string outputMaskName;
    std::function<bool(const typename data_t::value_type &)> filterFunction;
  };

  static_assert(
      std::is_same<data_t, EventDataModel::DataObjectCollection>::value or
          std::is_same<data_t,
                       EventDataModel::MultiDataObjectCollection>::value,
      "Filtering algorithm requires EventDataModel::DataObjectCollection or "
      "EventDataModel::MultiDataObjectCollection as inputs.");

  FilteringAlgorithm() = delete;
  FilteringAlgorithm(const std::string &name, const Config &cfg);
  FilteringAlgorithm(const FilteringAlgorithm &) = delete;
  FilteringAlgorithm &operator=(const FilteringAlgorithm &) = delete;
  virtual ~FilteringAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

private:
  const Config m_cfg;
};

/* ============================================================== */

template <typename data_t>
FilteringAlgorithm<data_t>::FilteringAlgorithm(const std::string &name,
                                               const Config &cfg)
    : Core::BaseAlgorithm(name), m_cfg(cfg) {}

template <typename data_t> void FilteringAlgorithm<data_t>::initialize() {
  MSG_INFO("Inizializing " + name() + " ...");

  if (m_cfg.inputCollection.empty())
    throw std::invalid_argument(
        "Input Data Collection Name has not beed specified for algorithm " +
        name());

  if (m_cfg.inputMaskName.empty())
    throw std::invalid_argument(
        "Input Mask Name has not beed specified for algorithm " + name());

  if (m_cfg.outputMaskName.empty())
    throw std::invalid_argument(
        "Output Mask Name has not beed specified for algorithm " + name());

  if (not m_cfg.filterFunction)
    throw std::invalid_argument(
        "Filter Function has not been specified for algorithm " + name());
}

template <typename data_t>
void FilteringAlgorithm<data_t>::execute(Core::EventContext &context) {
  MSG_INFO("Executing " + name() + " ...");

  const data_t *dataCollection = context.get<data_t>(m_cfg.inputCollection);

  const std::vector<bool> *inputMask =
      context.get<std::vector<bool>>(m_cfg.inputMaskName);

  std::shared_ptr<std::vector<bool>> outputMask =
      std::make_shared<std::vector<bool>>(inputMask->begin(), inputMask->end());

  for (auto idata(0); idata < dataCollection->size(); idata++) {
    if (not inputMask->at(idata))
      continue;
    outputMask->at(idata) = m_cfg.filterFunction(dataCollection->at(idata));
  }

  context.add(m_cfg.outputMaskName, std::move(outputMask));
}

using DataFilteringAlgorithm =
    FilteringAlgorithm<EventDataModel::DataObjectCollection>;
using MultiDataFilteringAlgorithm =
    FilteringAlgorithm<EventDataModel::MultiDataObjectCollection>;

} // namespace Algorithm

#endif
