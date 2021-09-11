
#include <SeedingPerformanceRootReaderAlgorithm.hpp>
#include <MultiData.hpp>

namespace Algorithm {

  SeedingPerformanceRootReaderAlgorithm::SeedingPerformanceRootReaderAlgorithm(const std::string& name,
									       const Algorithm::SeedingPerformanceRootReaderAlgorithm::Config& cfg)
    : RootFileReaderAlgorithm(name, cfg)
  {}

  void
  SeedingPerformanceRootReaderAlgorithm::execute(Core::EventContext& context)
  {
    std::cout << "Executing " << name() << " ..." << std::endl;

    std::shared_ptr<EventDataModel::MultiDataObjectCollection> outputCollection
      = std::make_shared<EventDataModel::MultiDataObjectCollection>();


    std::vector<std::string> labels({"nhits", "ntracks"});
    std::vector<EventDataModel::MultiDataObjectCollection::value_type::value_type> values;
    values.reserve(labels.size());
    
    unsigned short nhits, ntracks;      

    m_tree->SetBranchAddress("nhits",&nhits);
    m_tree->SetBranchAddress("ntracks",&ntracks);

    for (auto ientry(0); ientry<m_tree->GetEntries(); ientry++ ) {
      m_tree->GetEntry(ientry);

      values.clear();
      values.push_back(nhits);
      values.push_back(ntracks);
      
      outputCollection->emplace_back( labels, values, 1. );
    }


    std::shared_ptr<std::vector<bool>> mask =
      std::make_shared<std::vector<bool>>(outputCollection->size(), true);
    
    context.add(m_cfg.dataCollectionName,
                std::move(outputCollection));    
    context.add(m_cfg.outputMaskName,
		std::move(mask));
  }

}
