
#include <Scheduler.hpp>
#include <RootFileEfficiencyMakerAlgorithm.hpp>

void get_algo_sequence(Core::Scheduler& sequence,
		       const std::string& file_name,
		       const std::string& particle_type,
		       int nparticles);

int main() {
  Core::Scheduler scheduler;
  std::vector<std::string> particles{ "muon", "pion", "electron" };
  std::vector<int> nparticles{ 1, 10, 100};

  for (const auto& particle : particles) {
    for (int np : nparticles) {
      std::string fileName = "./data/performance_vs_nparticles/" + particle + "_efficiency_plots_n" + std::to_string(np) + ".root";
      get_algo_sequence(scheduler,
			fileName,
			particle,
			np);
    }
  }
  /*
				    "adhoc");
  seed_finding_timing_algo_sequence(scheduler,
				    "./data/xaod/extracted_timing_seed_finding_xaod.txt",
				    "xaod");


  // Compare Plots
  Algorithm::PlotComparisonAlgorithm::Config PlotComparisonConfiguration;
  PlotComparisonConfiguration.outputFolder = "./timing_comparison";
  PlotComparisonConfiguration.originalInputCollection = { "histograms_1d_adhoc",
                                                          "histograms_1d_xaod" };
  PlotComparisonConfiguration.variableNames = { "time" };

  std::shared_ptr<Algorithm::PlotComparisonAlgorithm> plotComparisonAlgorithm =
    std::make_shared<Algorithm::PlotComparisonAlgorithm>("PlotComparisonAlgorithm",
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
  
  */
  scheduler.run();
}



void
get_algo_sequence(Core::Scheduler& sequence,
		  const std::string& file_name,
		  const std::string& particle_type,
		  int nparticles)
{
  Algorithm::RootFileEfficiencyMakerAlgorithm::Config RootFileEfficiencyMakerConfiguration;
  RootFileEfficiencyMakerConfiguration.inputFile = file_name;
  RootFileEfficiencyMakerConfiguration.output_collection_1d = "profile_collection_1d_" + particle_type + "_n" + nparticles;
  RootFileEfficiencyMakerConfiguration.output_collection_2d = "profile_collection_2d_" + particle_type + "_n" + nparticles;
  RootFileEfficiencyMakerConfiguration.extractionFunction =
    [&] (TFile* file) -> std::pair<
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>,
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>
      >
    {
     std::shared_ptr<EventDataModel::EfficiencyObjectCollection> output_1d =
     std::make_shared<EventDataModel::EfficiencyObjectCollection>();

     output_1d->emplace_back("trackeff_vs_pT",
			     "trackeff_vs_pT",
			     * (TEfficiency*) file->Get("trackeff_vs_pT"));
     output_1d->emplace_back("trackeff_vs_eta",
			     "trackeff_vs_eta",
			     * (TEfficiency*) file->Get("trackeff_vs_eta"));

     
     std::shared_ptr<EventDataModel::EfficiencyObjectCollection> output_2d =
     std::make_shared<EventDataModel::EfficiencyObjectCollection>();
     
     return std::make_pair(output_1d, output_2d);
    };

  std::shared_ptr<Algorithm::RootFileEfficiencyMakerAlgorithm> rootFileEfficiencyMakerAlgorithm
    = std::make_shared<Algorithm::RootFileEfficiencyMakerAlgorithm>("RootFileEfficiencyMakerAlgorithm_" + particle_type + "_n" + std::to_string(nparticles),
								    RootFileEfficiencyMakerConfiguration);

  sequence.addAlgorithm(rootFileEfficiencyMakerAlgorithm);

  /*
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
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_hist_1d = MultiDataHistogramMakerConfiguration.outputCollection_1d;
  PlotterConfiguration.inputCollection_hist_2d = MultiDataHistogramMakerConfiguration.outputCollection_2d;
  PlotterConfiguration.inputCollection_gr = MultiDataGraphMakerConfiguration.outputCollection;
  PlotterConfiguration.outputFolder = "./timing_plots_" + edm_type;

  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_" + edm_type,
                                                  PlotterConfiguration);

  sequence.addAlgorithm(plotterAlgorithm);
  */
}



