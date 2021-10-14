
#ifndef ROOTFILE_EFFICIENCY_MAKER_ALGORITHM_H
#define ROOTFILE_EFFICIENCY_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Efficiency.hpp>

#include <TFile.h>

namespace Algorithm {

class RootFileEfficiencyMakerAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string inputFile = "";
    std::string output_collection_1d = "";
    std::string output_collection_2d = "";

    std::function<std::pair<
        std::shared_ptr<EventDataModel::EfficiencyObjectCollection>,
        std::shared_ptr<EventDataModel::EfficiencyObjectCollection>>(TFile *)>
        extractionFunction;
  };

  RootFileEfficiencyMakerAlgorithm() = delete;
  RootFileEfficiencyMakerAlgorithm(const std::string &name, const Config &cfg);
  RootFileEfficiencyMakerAlgorithm(const RootFileEfficiencyMakerAlgorithm &) =
      delete;
  RootFileEfficiencyMakerAlgorithm &
  operator=(const RootFileEfficiencyMakerAlgorithm &) = delete;
  virtual ~RootFileEfficiencyMakerAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

private:
  const Config m_cfg;
};

} // namespace Algorithm

#endif
