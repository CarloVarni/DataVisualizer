
#include <Scheduler.hpp>
#include <SeedingPerformanceRootReaderAlgorithm.hpp>
#include <MultiDataPlotterAlgorithm.hpp>

int main() {

  Core::Scheduler scheduler;

  Algorithm::SeedingPerformanceRootReaderAlgorithm::Config SeedingPerformanceRootReaderConfiguration;
  SeedingPerformanceRootReaderConfiguration.fileName = "./data/performance_seeding_trees.root ";
  SeedingPerformanceRootReaderConfiguration.treeName = "track_finder_particles";
  SeedingPerformanceRootReaderConfiguration.dataCollectionName = "raw_data";

  std::shared_ptr<Algorithm::SeedingPerformanceRootReaderAlgorithm> seedingPerformanceRootReaderAlgorithm
    = std::make_shared<Algorithm::SeedingPerformanceRootReaderAlgorithm>("SeedingPerformanceRootReaderAlgorithm",
									 SeedingPerformanceRootReaderConfiguration);

  scheduler.addAlgorithm(seedingPerformanceRootReaderAlgorithm);




  Algorithm::MultiDataPlotterAlgorithm::Config MultiDataPlotterConfiguration;
  MultiDataPlotterConfiguration.inputCollection = SeedingPerformanceRootReaderConfiguration.dataCollectionName;
  MultiDataPlotterConfiguration.outputFolder = "./plots";
  MultiDataPlotterConfiguration.variableNames = {"nhits", "ntracks"};
  
  std::shared_ptr<Algorithm::MultiDataPlotterAlgorithm> multiDataPlotterAlgorithm =
    std::make_shared<Algorithm::MultiDataPlotterAlgorithm>("MultiDataPlotterAlgorithm",
							   MultiDataPlotterConfiguration);

  scheduler.addAlgorithm(multiDataPlotterAlgorithm);

  
  scheduler.run();
}
