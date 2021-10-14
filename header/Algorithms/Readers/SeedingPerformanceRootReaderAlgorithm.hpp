
#ifndef SEEDING_PERFORMANCE_ROOTFILE_READER_ALGORITHM_H
#define SEEDING_PERFORMANCE_ROOTFILE_READER_ALGORITHM_H 1

#include <RootFileReaderAlgorithm.hpp>

namespace Algorithm {

class SeedingPerformanceRootReaderAlgorithm : public RootFileReaderAlgorithm {

public:
  struct Config : public Algorithm::RootFileReaderAlgorithm::Config {};

  SeedingPerformanceRootReaderAlgorithm(const std::string &name,
                                        const Config &cfg);
  SeedingPerformanceRootReaderAlgorithm(
      const SeedingPerformanceRootReaderAlgorithm &) = delete;
  SeedingPerformanceRootReaderAlgorithm &
  operator=(const SeedingPerformanceRootReaderAlgorithm &) = delete;
  virtual ~SeedingPerformanceRootReaderAlgorithm() = default;

  virtual void execute(Core::EventContext &context) override;
};

} // namespace Algorithm

#endif
