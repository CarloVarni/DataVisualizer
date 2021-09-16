
#include <Scheduler.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <DataHistogramMakerAlgorithm.hpp> 
#include <FilteringAlgorithm.hpp>
#include <SeedingPerformanceRootReaderAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
#include <TxtFileReaderAlgorithm.hpp>

int main() {
  
  Core::Scheduler scheduler;

  // Reader
  Algorithm::SeedingPerformanceRootReaderAlgorithm::Config SeedingPerformanceRootReaderConfiguration;
  SeedingPerformanceRootReaderConfiguration.fileName = "./data/performance_seeding_trees.root ";
  SeedingPerformanceRootReaderConfiguration.treeName = "track_finder_particles";
  SeedingPerformanceRootReaderConfiguration.dataCollectionName = "raw_data";
  SeedingPerformanceRootReaderConfiguration.outputMaskName = "raw_mask";

  std::shared_ptr<Algorithm::SeedingPerformanceRootReaderAlgorithm> seedingPerformanceRootReaderAlgorithm
    = std::make_shared<Algorithm::SeedingPerformanceRootReaderAlgorithm>("SeedingPerformanceRootReaderAlgorithm",
									 SeedingPerformanceRootReaderConfiguration);

  scheduler.addAlgorithm(seedingPerformanceRootReaderAlgorithm);


  
  // Filtering
  Algorithm::MultiDataFilteringAlgorithm::Config multiDataFilteringConfiguration;
  multiDataFilteringConfiguration.inputCollection = SeedingPerformanceRootReaderConfiguration.dataCollectionName;
  multiDataFilteringConfiguration.inputMaskName = SeedingPerformanceRootReaderConfiguration.outputMaskName;
  multiDataFilteringConfiguration.outputMaskName = "filter_mask";
  multiDataFilteringConfiguration.filterFunction =
    [=] (const EventDataModel::MultiDataObject& obg) -> bool {
      return true;
    };
  
  std::shared_ptr<Algorithm::MultiDataFilteringAlgorithm> multiDataFilteringAlgorithm
    = std::make_shared<Algorithm::MultiDataFilteringAlgorithm>("MultiDataFilteringAlgorithm",
							       multiDataFilteringConfiguration);
  
  scheduler.addAlgorithm(multiDataFilteringAlgorithm);


  // Histogram Maker
  Algorithm::MultiDataHistogramMakerAlgorithm::Config MultiDataHistogramMakerConfiguration;
  MultiDataHistogramMakerConfiguration.inputCollection = SeedingPerformanceRootReaderConfiguration.dataCollectionName;
  MultiDataHistogramMakerConfiguration.inputMaskName = multiDataFilteringConfiguration.outputMaskName;
  MultiDataHistogramMakerConfiguration.variableNames_1D = {"nhits", "ntracks"};
  // 1D Plots
  MultiDataHistogramMakerConfiguration.outputCollection_1d = "collection_1d";
  MultiDataHistogramMakerConfiguration.histogramDefs_1D =
    {
     TH1I("nhits", "nhits", 20, 0 ,20),
     TH1I("ntracks", "ntracks", 20, 0, 20)
    };
  // 2D Plots
  MultiDataHistogramMakerConfiguration.outputCollection_2d = "collection_2d";
  MultiDataHistogramMakerConfiguration.variableNames_2D = { std::make_pair("nhits", "ntracks") };
  MultiDataHistogramMakerConfiguration.histogramDefs_2D =
    {
     TH2I("nhits_ntracks", "nhits_ntracks", 20, 0, 20, 20, 0, 20)
    };

  std::shared_ptr<Algorithm::MultiDataHistogramMakerAlgorithm> multiDataHistogramMakerAlgorithm =
    std::make_shared<Algorithm::MultiDataHistogramMakerAlgorithm>("MultiDataHistogramMakerAlgorithm",
								  MultiDataHistogramMakerConfiguration);

  scheduler.addAlgorithm(multiDataHistogramMakerAlgorithm);

  
  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_hist_1d = MultiDataHistogramMakerConfiguration.outputCollection_1d;
  PlotterConfiguration.inputCollection_hist_2d = MultiDataHistogramMakerConfiguration.outputCollection_2d;
  PlotterConfiguration.outputFolder = "./plots";
  
  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm",
						  PlotterConfiguration);
  
  scheduler.addAlgorithm(plotterAlgorithm);




  Algorithm::TxtFileReaderAlgorithm<EventDataModel::DataObjectCollection>::Config TxtFileReaderConfiguration;
  TxtFileReaderConfiguration.inputFile = "";
  TxtFileReaderConfiguration.outputCollection = "raw_data";
  TxtFileReaderConfiguration.extraction_function =
    [=] (const std::ifstream& file,
	 std::shared_ptr<EventDataModel::DataObjectCollection>& data_collection) -> void {
    };

  std::shared_ptr<Algorithm::TxtFileReaderToDataObjectCollectionAlgorithm> txtFileReaderToDataObjectCollectionAlgorithm =
    std::make_shared<Algorithm::TxtFileReaderToDataObjectCollectionAlgorithm>("TxtFileReaderToDataObjectCollectionAlgorithm",
									      TxtFileReaderConfiguration);
  
  scheduler.addAlgorithm(txtFileReaderToDataObjectCollectionAlgorithm);

    
  scheduler.run();
}
