
#ifndef ROOTFILE_PROFILE_MAKER_ALGORITHM_H
#define ROOTFILE_PROFILE_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Profile.hpp>

#include <TFile.h>

namespace Algorithm {

class RootFileProfileMakerAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string inputFile = "";
    std::string output_collection = "";
    std::function<std::shared_ptr<EventDataModel::ProfileObjectCollection>(
        TFile *)>
        extractionFunction;
  };

  RootFileProfileMakerAlgorithm() = delete;
  RootFileProfileMakerAlgorithm(const std::string &name, const Config &cfg);
  RootFileProfileMakerAlgorithm(const RootFileProfileMakerAlgorithm &) = delete;
  RootFileProfileMakerAlgorithm &
  operator=(const RootFileProfileMakerAlgorithm &) = delete;
  virtual ~RootFileProfileMakerAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

private:
  const Config m_cfg;
};

} // namespace Algorithm

#endif
