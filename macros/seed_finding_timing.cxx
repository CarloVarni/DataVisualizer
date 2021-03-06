
#include <Scheduler.hpp>
#include <TxtFileReaderAlgorithm.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <MultiDataGraphMakerAlgorithm.hpp>
#include <Histogram1DPlotterAlgorithm.hpp>
#include <Histogram2DPlotterAlgorithm.hpp>
#include <GraphPlotterAlgorithm.hpp>
#include <HistogramComparisonPlotterAlgorithm.hpp>
#include <GraphComparisonPlotterAlgorithm.hpp>

void seed_finding_timing_algo_sequence(Core::Scheduler& sequence,
				       const std::string& file_name,
				       const std::string& edm_type);

int main() {
  Core::Scheduler scheduler;

  seed_finding_timing_algo_sequence(scheduler,
				    "./data/adhoc/extracted_timing_seed_finding_adhoc.txt",
				    "adhoc");
  seed_finding_timing_algo_sequence(scheduler,
				    "./data/xaod/extracted_timing_seed_finding_xaod.txt",
				    "xaod");


  // Compare Plots
  Algorithm::HistogramComparisonPlotterAlgorithm::Config PlotComparisonConfiguration;
  PlotComparisonConfiguration.outputFolder = "./timing_comparison";
  PlotComparisonConfiguration.originalInputCollection = { "histograms_1d_adhoc",
                                                          "histograms_1d_xaod" };
  PlotComparisonConfiguration.variableNames = { "time" };

  std::shared_ptr<Algorithm::HistogramComparisonPlotterAlgorithm> plotComparisonAlgorithm =
    std::make_shared<Algorithm::HistogramComparisonPlotterAlgorithm>("HistogramComparisonPlotterAlgorithm",
								     PlotComparisonConfiguration);

  scheduler.addAlgorithm(plotComparisonAlgorithm);


  // Compare Graphs
  Algorithm::GraphComparisonPlotterAlgorithm::Config GraphComparisonPlotterConfiguration;
  GraphComparisonPlotterConfiguration.outputFolder = "./timing_comparison";
  GraphComparisonPlotterConfiguration.originalInputCollection = { "graphs_adhoc",
								  "graphs_xaod" };
  GraphComparisonPlotterConfiguration.variableNames = { "n_seeds_time",
							"n_points_time",
							"n_points_n_seeds"};

  std::shared_ptr<Algorithm::GraphComparisonPlotterAlgorithm> graphComparisonPlotterAlgorithm =
    std::make_shared<Algorithm::GraphComparisonPlotterAlgorithm>("GraphComparisonPlotterAlgorithm",
								 GraphComparisonPlotterConfiguration);

  scheduler.addAlgorithm(graphComparisonPlotterAlgorithm);
  
   
  scheduler.run();
}



void
seed_finding_timing_algo_sequence(Core::Scheduler& sequence,
				  const std::string& file_name,
				  const std::string& edm_type)
{
  // Reader
  Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm::Config TxtFileReaderConfiguration;
  TxtFileReaderConfiguration.inputFile = file_name;
  TxtFileReaderConfiguration.outputCollection = "raw_timing_data_" + edm_type;
  TxtFileReaderConfiguration.outputMask = "raw_timing_mask_" + edm_type;
  TxtFileReaderConfiguration.extraction_function =
    [&] (std::ifstream& file,
         std::shared_ptr<EventDataModel::MultiDataObjectCollection>& data_collection) -> void
    {
      std::vector<std::string> labels( {"n_points", "n_seeds", "time"} );

      std::string line = "";
      while ( std::getline(file, line) ) {
        std::istringstream iss( line );
        std::string s_n_points = "";
	std::string s_n_seeds = "";
        std::string s_time = "";
        iss >> s_n_points >> s_n_seeds >> s_time;

        std::vector<EventDataModel::MultiDataObjectCollection::value_type::value_type> data_values( { std::stoi(s_n_points),
												      std::stoi(s_n_seeds),
												      std::stof(s_time) * 1e-3} );
        EventDataModel::MultiDataObjectCollection::value_type data( labels, data_values );
        data_collection->push_back( data );
      }
    };
  
  std::shared_ptr<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm> timingFileReader =
    std::make_shared<Algorithm::TXTFileReaderToMultiDataObjectCollectionAlgorithm>("TimingFileReader_" + edm_type,
                                                                                   TxtFileReaderConfiguration);
  
  sequence.addAlgorithm(timingFileReader);
  


  // Histogram Maker
  Algorithm::MultiDataHistogramMakerAlgorithm::Config MultiDataHistogramMakerConfiguration;
  MultiDataHistogramMakerConfiguration.inputCollection = TxtFileReaderConfiguration.outputCollection;
  MultiDataHistogramMakerConfiguration.inputMaskName = TxtFileReaderConfiguration.outputMask;
  MultiDataHistogramMakerConfiguration.outputCollection_1d = "histograms_1d_" + edm_type;
  MultiDataHistogramMakerConfiguration.outputCollection_2d = "histograms_2d_" + edm_type;
  MultiDataHistogramMakerConfiguration.variableNames_1D = { "n_points", "n_seeds", "time" };
  MultiDataHistogramMakerConfiguration.histogramDefs_1D =
    {
     TH1I(("points_" + edm_type).c_str(), "Timing;N. Points;Entries", 100, 0, 30000),
     TH1I(("seeds_" + edm_type).c_str(), "Timing;N. Seeds;Entries", 50, 0, 10000),
     TH1F(("time_" + edm_type).c_str(), "Timing;Time [us];Entries", 50, 0, 100000)
    };
  MultiDataHistogramMakerConfiguration.variableNames_2D =
    {
     std::make_pair("n_seeds", "time"),
     std::make_pair("n_points", "time"),
     std::make_pair("n_points", "n_seeds")
  };
  MultiDataHistogramMakerConfiguration.histogramDefs_2D =
    {
     TH2F(("n_seeds_vs_time_" + edm_type).c_str(),
	  "Timing;N. Seeds;Time [us]",
	  50, 0, 10000,
	  50, 0, 100000),
     TH2F(("n_points_vs_time_" + edm_type).c_str(),
          "Timing;N. Points;Time [us]",
          100, 0, 30000,
          50 , 0, 100000),
     TH2F(("n_points_vs_seeds_" + edm_type).c_str(),
          "Timing;N. Points;N. Seeds",
          100, 0, 30000,
          50 , 0, 10000)
    };

  std::shared_ptr<Algorithm::MultiDataHistogramMakerAlgorithm> multiDataHistogramMakerAlgorithm =
    std::make_shared<Algorithm::MultiDataHistogramMakerAlgorithm>("multiDataHistogramMakerAlgorithm_" + edm_type,
                                                                  MultiDataHistogramMakerConfiguration);
  
  sequence.addAlgorithm(multiDataHistogramMakerAlgorithm);
  

  // Graph Maker  
  Algorithm::MultiDataGraphMakerAlgorithm::Config MultiDataGraphMakerConfiguration;
  MultiDataGraphMakerConfiguration.inputCollection = TxtFileReaderConfiguration.outputCollection;;
  MultiDataGraphMakerConfiguration.inputMaskName = TxtFileReaderConfiguration.outputMask;;
  MultiDataGraphMakerConfiguration.outputCollection = "graphs_" + edm_type;
  MultiDataGraphMakerConfiguration.variableNames =
    {
     std::make_pair("n_seeds", "time"),
     std::make_pair("n_points", "time"),
     std::make_pair("n_points", "n_seeds")
    };
  MultiDataGraphMakerConfiguration.graphDefs =
    {
     TGraph(),
     TGraph(),
     TGraph()
    };

	
  std::shared_ptr<Algorithm::MultiDataGraphMakerAlgorithm> multiDataGraphMakerAlgorithm =
    std::make_shared<Algorithm::MultiDataGraphMakerAlgorithm>("MultiDataGraphMakerAlgorithm_" + edm_type,
							      MultiDataGraphMakerConfiguration);

  sequence.addAlgorithm(multiDataGraphMakerAlgorithm);
  
  // Plotter
  Algorithm::Histogram1DPlotterAlgorithm::Config Histogram1DPlotterConfiguration;
  Histogram1DPlotterConfiguration.prefix = "h_1d";
  Histogram1DPlotterConfiguration.outputFolder = "./timing_plots_" + edm_type;
  Histogram1DPlotterConfiguration.inputCollection = MultiDataHistogramMakerConfiguration.outputCollection_1d;

  std::shared_ptr<Algorithm::Histogram1DPlotterAlgorithm> histogram1DPlotterAlgorithm =
    std::make_shared<Algorithm::Histogram1DPlotterAlgorithm>("Histogram1DPlotterAlgorithm_" + edm_type,
							     Histogram1DPlotterConfiguration);

  sequence.addAlgorithm(histogram1DPlotterAlgorithm);

  
  Algorithm::Histogram2DPlotterAlgorithm::Config Histogram2DPlotterConfiguration;
  Histogram2DPlotterConfiguration.prefix = "h_2d";
  Histogram2DPlotterConfiguration.outputFolder = Histogram1DPlotterConfiguration.outputFolder;
  Histogram2DPlotterConfiguration.inputCollection = MultiDataHistogramMakerConfiguration.outputCollection_2d;

  std::shared_ptr<Algorithm::Histogram2DPlotterAlgorithm> histogram2DPlotterAlgorithm =
    std::make_shared<Algorithm::Histogram2DPlotterAlgorithm>("Histogram2DPlotterAlgorithm_" + edm_type,
							     Histogram2DPlotterConfiguration);

  sequence.addAlgorithm(histogram2DPlotterAlgorithm);


  Algorithm::GraphPlotterAlgorithm::Config GraphPlotterConfiguration;
  GraphPlotterConfiguration.prefix = "gr";
  GraphPlotterConfiguration.outputFolder = Histogram1DPlotterConfiguration.outputFolder;
  GraphPlotterConfiguration.inputCollection = MultiDataGraphMakerConfiguration.outputCollection;

  std::shared_ptr<Algorithm::GraphPlotterAlgorithm> graphPlotterAlgorithm =
    std::make_shared<Algorithm::GraphPlotterAlgorithm>("GraphPlotterAlgorithm_" + edm_type,
						       GraphPlotterConfiguration);

  sequence.addAlgorithm(graphPlotterAlgorithm);
}



