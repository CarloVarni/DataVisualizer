

#include <Scheduler.hpp>
#include <TxtFileReaderAlgorithm.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
#include <PlotComparisonAlgorithm.hpp>

#include <sstream>

void getTimingAlgoSequence(Core::Scheduler& scheduler,
			   const std::string& file_name,
			   const std::string& edm_type,
			   const std::string& timing_type);

int main() {

  Core::Scheduler scheduler;

  getTimingAlgoSequence(scheduler,
			"./data/extracted_timing_full_adhoc_EDM.txt",
			"adhoc",
			"full");
  getTimingAlgoSequence(scheduler,
			"./data/extracted_timing_full_xaod_EDM.txt",
			"xaod",
			"full");

  // Compare Plots
  Algorithm::PlotComparisonAlgorithm::Config PlotComparisonConfiguration;
  PlotComparisonConfiguration.outputFolder = "./timing_comparison";
  PlotComparisonConfiguration.originalInputCollection = { "histograms_1d_adhoc_full",
							  "histograms_1d_xaod_full" };
  PlotComparisonConfiguration.variableNames = { "time" };

  std::shared_ptr<Algorithm::PlotComparisonAlgorithm> plotComparisonAlgorithm =
    std::make_shared<Algorithm::PlotComparisonAlgorithm>("PlotComparisonAlgorithm",
							 PlotComparisonConfiguration);

  scheduler.addAlgorithm(plotComparisonAlgorithm);

  // Run  
  scheduler.run();
}




void getTimingAlgoSequence(Core::Scheduler& sequence,
                           const std::string& file_name,
                           const std::string& edm_type,
                           const std::string& timing_type)
{
  // Reader  
  Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm::Config TxtFileReaderConfiguration;
  TxtFileReaderConfiguration.inputFile = file_name;
  TxtFileReaderConfiguration.outputCollection = "raw_timing_data_" + edm_type + "_" + timing_type;
  TxtFileReaderConfiguration.outputMask = "raw_timing_mask_" + edm_type + "_" + timing_type;
  TxtFileReaderConfiguration.extraction_function =
    [&] (std::ifstream& file,
         std::shared_ptr<EventDataModel::MultiDataObjectCollection>& data_collection) -> void
    {
      std::vector<std::string> labels( {"n_points", "time"} );

      std::string line = "";
      while ( std::getline(file, line) ) {
        std::istringstream iss( line );
        std::string s_n_points = "";
        std::string s_time = "";
        iss >> s_n_points >> s_time;

        std::vector<EventDataModel::MultiDataObjectCollection::value_type::value_type> data_values( { std::stoi(s_n_points), std::stof(s_time) * 1e-3} );
        EventDataModel::MultiDataObjectCollection::value_type data( labels, data_values );
        data_collection->push_back( data );
      }
    };

  std::shared_ptr<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm> timingFileReader =
    std::make_shared<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm>("TimingFileReader_" + edm_type + "_" + timing_type,
                                                                                   TxtFileReaderConfiguration);

  sequence.addAlgorithm(timingFileReader);




  // Histogram Maker
  Algorithm::MultiDataHistogramMakerAlgorithm::Config MultiDataHistogramMakerConfiguration;
  MultiDataHistogramMakerConfiguration.inputCollection = TxtFileReaderConfiguration.outputCollection;
  MultiDataHistogramMakerConfiguration.inputMaskName = TxtFileReaderConfiguration.outputMask;
  MultiDataHistogramMakerConfiguration.outputCollection_1d = "histograms_1d_" + edm_type + "_" + timing_type;
  MultiDataHistogramMakerConfiguration.outputCollection_2d = "histograms_2d_" + edm_type + "_" + timing_type;
  MultiDataHistogramMakerConfiguration.variableNames_1D = { "n_points", "time" };
  MultiDataHistogramMakerConfiguration.histogramDefs_1D =
    {
     TH1I(("n_points_" + edm_type + "_" + timing_type).c_str(), "N. Points;N. Points;Entries", 50, 0, 25000),
     TH1F(("time_" + edm_type + "_" + timing_type).c_str(), "Timing;Time [us];Entries", 100, 0, 200000)
    };
  MultiDataHistogramMakerConfiguration.variableNames_2D = { std::make_pair("n_points", "time") };;
  MultiDataHistogramMakerConfiguration.histogramDefs_2D =
    {
     TH2F(("n_points_vs_time_" + edm_type + "_" + timing_type).c_str(),"Timing;N. Points;Time [us]", 50, 0, 25000, 100, 0, 200000)
    };

  std::shared_ptr<Algorithm::MultiDataHistogramMakerAlgorithm> multiDataHistogramMakerAlgorithm =
    std::make_shared<Algorithm::MultiDataHistogramMakerAlgorithm>("multiDataHistogramMakerAlgorithm_" + edm_type + "_" + timing_type,
                                                                  MultiDataHistogramMakerConfiguration);

  sequence.addAlgorithm(multiDataHistogramMakerAlgorithm);

  
  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_1d = MultiDataHistogramMakerConfiguration.outputCollection_1d;
  PlotterConfiguration.inputCollection_2d = MultiDataHistogramMakerConfiguration.outputCollection_2d;
  PlotterConfiguration.outputFolder = "./timing_plots_" + edm_type;

  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_" + edm_type + "_" + timing_type,
                                                  PlotterConfiguration);

  sequence.addAlgorithm(plotterAlgorithm);
}
