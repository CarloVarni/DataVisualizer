
#include <RootFileReaderAlgorithm.hpp>
#include <MultiData.hpp>

#include <iostream>

namespace Algorithm {

  RootFileReaderAlgorithm::RootFileReaderAlgorithm(const std::string& name,
						   const Algorithm::RootFileReaderAlgorithm::Config& cfg)
    : BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  RootFileReaderAlgorithm::initialize()
  {
    std::cout << "Initializing " << name() << " ..." << std::endl;

    if (m_cfg.fileName.empty()) 
      throw std::invalid_argument("Input File Name has not been specified for algorithm " + name());

    if (m_cfg.treeName.empty())
      throw std::invalid_argument("Input Tree Name has not been specified for algorithm " + name());

    if (m_cfg.dataCollectionName.empty())
      throw std::invalid_argument("Output Collection Name has not been specified for algorithm " + name());

    m_file = std::make_unique<TFile>( m_cfg.fileName.c_str(), "READ" );
  }

  void
  RootFileReaderAlgorithm::execute(Core::EventContext& context)
  {
    std::cout << "Executing " << name() << " ..." << std::endl;

    using MultiDataCollection = std::vector<MultiData<double, int, float>>;
    std::shared_ptr<MultiDataCollection> outputCollection = std::make_shared<MultiDataCollection>();
    
    context.add(m_cfg.dataCollectionName,
    		std::move(outputCollection));
  }

  void
  RootFileReaderAlgorithm::finalize()
  {
    std::cout << "Finalizing " << name() << " ..." << std::endl;

    if (m_file)
      m_file->Close();
  }
  
}
