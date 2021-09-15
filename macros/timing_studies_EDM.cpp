

#include <Scheduler.hpp>
#include <TxtFileReaderAlgorithm.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
#include <sstream>

int main() {

  Core::Scheduler scheduler;

  // Reader
  Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm::Config TxtFileReaderConfiguration_full;
  TxtFileReaderConfiguration_full.inputFile = "./data/extracted_timing_full_adhoc_EDM.txt";
  TxtFileReaderConfiguration_full.outputCollection = "raw_timing_data_full";
  TxtFileReaderConfiguration_full.outputMask = "raw_timing_mask_full";
  TxtFileReaderConfiguration_full.extraction_function =
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

  std::shared_ptr<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm> timingFileReader_full =
    std::make_shared<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm>("TimingFileReader_full",
										   TxtFileReaderConfiguration_full);

  scheduler.addAlgorithm(timingFileReader_full);



  Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm::Config TxtFileReaderConfiguration_single;
  TxtFileReaderConfiguration_single.inputFile = "./data/extracted_timing_single_adhoc_EDM.txt";
  TxtFileReaderConfiguration_single.outputCollection = "raw_timing_data_single";
  TxtFileReaderConfiguration_single.outputMask = "raw_timing_mask_single";
  TxtFileReaderConfiguration_single.extraction_function =
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

        std::vector<EventDataModel::MultiDataObjectCollection::value_type::value_type> data_values( { std::stoi(s_n_points), std::stof(s_time) } );
        EventDataModel::MultiDataObjectCollection::value_type data( labels, data_values );
        data_collection->push_back( data );
      }
    };

  std::shared_ptr<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm> timingFileReader_single =
    std::make_shared<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm>("TimingFileReader_single",
                                                                                   TxtFileReaderConfiguration_single);

  scheduler.addAlgorithm(timingFileReader_single);



  
  // Histogram Maker
  Algorithm::MultiDataHistogramMakerAlgorithm::Config MultiDataHistogramMakerConfiguration_full;
  MultiDataHistogramMakerConfiguration_full.inputCollection = TxtFileReaderConfiguration_full.outputCollection;
  MultiDataHistogramMakerConfiguration_full.inputMaskName = TxtFileReaderConfiguration_full.outputMask;
  MultiDataHistogramMakerConfiguration_full.outputCollection_1d = "histograms_1d_full";
  MultiDataHistogramMakerConfiguration_full.outputCollection_2d = "histograms_2d_full";
  MultiDataHistogramMakerConfiguration_full.variableNames_1D = { "n_points", "time" };
  MultiDataHistogramMakerConfiguration_full.histogramDefs_1D =
    {
     TH1I("n_points_full", "N. Points;N. Points;Entries", 50, 0, 25000),
     TH1F("time_full", "Timing;Time [us];Entries", 100, 0, 10000)
    };
  MultiDataHistogramMakerConfiguration_full.variableNames_2D = { std::make_pair("n_points", "time") };;
  MultiDataHistogramMakerConfiguration_full.histogramDefs_2D =
    {
     TH2F("n_points_vs_time_full","Timing;N. Points;Time [us]", 50, 0, 25000, 100, 0, 10000)
    };
  
  std::shared_ptr<Algorithm::MultiDataHistogramMakerAlgorithm> multiDataHistogramMakerAlgorithm_full =
    std::make_shared<Algorithm::MultiDataHistogramMakerAlgorithm>("multiDataHistogramMakerAlgorithm_full",
								  MultiDataHistogramMakerConfiguration_full);
  
  scheduler.addAlgorithm(multiDataHistogramMakerAlgorithm_full);
  
  
  
  Algorithm::MultiDataHistogramMakerAlgorithm::Config MultiDataHistogramMakerConfiguration_single;
  MultiDataHistogramMakerConfiguration_single.inputCollection = TxtFileReaderConfiguration_single.outputCollection;
  MultiDataHistogramMakerConfiguration_single.inputMaskName = TxtFileReaderConfiguration_single.outputMask;
  MultiDataHistogramMakerConfiguration_single.outputCollection_1d = "histograms_1d_single";
  MultiDataHistogramMakerConfiguration_single.outputCollection_2d = "histograms_2d_single";
  MultiDataHistogramMakerConfiguration_single.variableNames_1D = {"n_points", "time"};
  MultiDataHistogramMakerConfiguration_single.variableNames_1D = {"time"};
  MultiDataHistogramMakerConfiguration_single.histogramDefs_1D =
    {
     TH1F("time_single", "Timing;Time [ns];Entries", 100, 0, 200)
    };
  MultiDataHistogramMakerConfiguration_single.variableNames_2D = {};
  MultiDataHistogramMakerConfiguration_single.histogramDefs_2D = {};
  

  std::shared_ptr<Algorithm::MultiDataHistogramMakerAlgorithm> multiDataHistogramMakerAlgorithm_single =
    std::make_shared<Algorithm::MultiDataHistogramMakerAlgorithm>("multiDataHistogramMakerAlgorithm_single",
                                                                  MultiDataHistogramMakerConfiguration_single);

  scheduler.addAlgorithm(multiDataHistogramMakerAlgorithm_single);

  
  
  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration_full;
  PlotterConfiguration_full.inputCollection_1d = MultiDataHistogramMakerConfiguration_full.outputCollection_1d;
  PlotterConfiguration_full.inputCollection_2d = MultiDataHistogramMakerConfiguration_full.outputCollection_2d;
  PlotterConfiguration_full.outputFolder = "./timing_plots_adhoc_EDM";

  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm_full =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_full",
						  PlotterConfiguration_full);

  scheduler.addAlgorithm(plotterAlgorithm_full);


  Algorithm::PlotterAlgorithm::Config PlotterConfiguration_single;
  PlotterConfiguration_single.inputCollection_1d = MultiDataHistogramMakerConfiguration_single.outputCollection_1d;
  PlotterConfiguration_single.inputCollection_2d = MultiDataHistogramMakerConfiguration_single.outputCollection_2d;
  PlotterConfiguration_single.outputFolder = "./timing_plots_adhoc_EDM";

  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm_single =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_single",
						  PlotterConfiguration_single);

  scheduler.addAlgorithm(plotterAlgorithm_single);

  
  
  scheduler.run();

}
