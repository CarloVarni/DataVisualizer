
#include <Scheduler.hpp>
#include <RootFileReaderAlgorithm.hpp>

int main() {

  Core::Scheduler scheduler;
  
  Algorithm::RootFileReaderAlgorithm::Config RootReaderConfiguration;
  RootReaderConfiguration.fileName = "testFile.root";
  RootReaderConfiguration.treeName = "testTree";
  RootReaderConfiguration.dataCollectionName = "raw_data";

  std::shared_ptr<Algorithm::RootFileReaderAlgorithm> rootFileReaderAlgorithm
    = std::make_shared<Algorithm::RootFileReaderAlgorithm>("RootFileReaderAlgorithm",
							   RootReaderConfiguration);
  
  scheduler.addAlgorithm(rootFileReaderAlgorithm);

  scheduler.run();
}
