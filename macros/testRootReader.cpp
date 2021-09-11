
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
  // 1D plots
  MultiDataPlotterConfiguration.variableNames_1D = {"nhits", "ntracks"};
  MultiDataPlotterConfiguration.histogramDefs_1D =
    {
     TH1I("nhits", "nhits", 20, 0 ,20),
     TH1I("ntracks", "ntracks", 20, 0, 20)
    };
  // 2D plots
  MultiDataPlotterConfiguration.variableNames_2D = { std::make_pair("nhits", "ntracks") };
  MultiDataPlotterConfiguration.histogramDefs_2D =
    {
     TH2I("nhits_ntracks", "nhits_ntracks", 20, 0, 20, 20, 0, 20)
    };
  
  std::shared_ptr<Algorithm::MultiDataPlotterAlgorithm> multiDataPlotterAlgorithm =
    std::make_shared<Algorithm::MultiDataPlotterAlgorithm>("MultiDataPlotterAlgorithm",
							   MultiDataPlotterConfiguration);
  
  scheduler.addAlgorithm(multiDataPlotterAlgorithm);

  
  scheduler.run();
}
