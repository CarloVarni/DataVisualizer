
#include <RootFileReaderAlgorithm.hpp>

namespace Algorithm {

  RootFileReaderAlgorithm::RootFileReaderAlgorithm(const std::string& name,
						   const Algorithm::RootFileReaderAlgorithm::Config& cfg)
    : BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  RootFileReaderAlgorithm::initialize() {}

  void
  RootFileReaderAlgorithm::execute(Core::EventContext& context)
  {}

  void
  RootFileReaderAlgorithm::finalize()
  {}
  
}
