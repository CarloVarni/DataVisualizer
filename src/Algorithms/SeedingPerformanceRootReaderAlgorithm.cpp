
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

    std::shared_ptr<EventDataModel::MultiDataCollection> outputCollection
      = std::make_shared<EventDataModel::MultiDataCollection>();


    std::vector<std::string> labels({"nhits", "ntracks"});
    std::vector<EventDataModel::MultiDataCollection::value_type::data_type> values;
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

    context.add(m_cfg.dataCollectionName,
                std::move(outputCollection));    
  }

}
