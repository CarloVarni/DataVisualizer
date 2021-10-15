
#include <Scheduler.hpp>

#include <RootFileEfficiencyMakerAlgorithm.hpp>
#include <Efficiency1DPlotterAlgorithm.hpp>
#include <Efficiency2DPlotterAlgorithm.hpp>
#include <EfficiencyComparisonPlotterAlgorithm.hpp>

void get_algo_sequence(Core::Scheduler& sequence,
		       const std::string& file_name,
		       const std::string& particle_type,
		       const std::string& nparticles);

int main() {
  Core::Scheduler scheduler;
  std::vector<std::string> particles{ "muon", "pion", "electron" };
  std::vector<int> nparticles{1};

  for (const auto& particle : particles) {
    for (int np : nparticles) {
      std::string fileName = "./data/performance_vs_nparticles/" + particle + "_efficiency_plots_n" + std::to_string(np) + ".root";
      std::string snp = std::to_string(np);
      get_algo_sequence(scheduler,
			fileName,
			particle,
			snp);
    }
  }


  for (const auto& particle : particles) {
    // Comparison Plots
    Algorithm::EfficiencyComparisonPlotterAlgorithm::Config EfficiencyComparisonPlotterConfiguration;
    EfficiencyComparisonPlotterConfiguration.outputFolder = "./efficiency_plots_vs_nparticles/" + particle;
    if (particle != "muon") {
      EfficiencyComparisonPlotterConfiguration.originalInputCollection =
	{
	 "profile_collection_1d_" + particle + "_n1",
	 "profile_collection_1d_" + particle + "_n10",
	 "profile_collection_1d_" + particle + "_n100",
	};
    } else {
      EfficiencyComparisonPlotterConfiguration.originalInputCollection =
        {
         "profile_collection_1d_" + particle + "_n1",
         "profile_collection_1d_" + particle + "_n10",
         "profile_collection_1d_" + particle + "_n100",
	 "profile_collection_1d_" + particle + "_n1000",
        };
    }
    EfficiencyComparisonPlotterConfiguration.variableNames =
      {
       "trackeff_vs_pT",
       "trackeff_vs_eta"
      };
    
    std::shared_ptr<Algorithm::EfficiencyComparisonPlotterAlgorithm> efficiencyComparisonPlotterAlgorithm =
      std::make_shared<Algorithm::EfficiencyComparisonPlotterAlgorithm>("EfficiencyComparisonPlotterAlgorithm_" + particle,
									EfficiencyComparisonPlotterConfiguration);
    
    scheduler.addAlgorithm(efficiencyComparisonPlotterAlgorithm);
  }

  for (int np : nparticles) {  
    if (np == 1000) continue; 
    Algorithm::EfficiencyComparisonPlotterAlgorithm::Config EfficiencyComparisonPlotterConfiguration;
    EfficiencyComparisonPlotterConfiguration.outputFolder = "./efficiency_plots_vs_nparticles/n" + std::to_string(np);
    EfficiencyComparisonPlotterConfiguration.originalInputCollection =
      {
       "profile_collection_1d_muon_n" + std::to_string(np),
       "profile_collection_1d_pion_n" + std::to_string(np),
       "profile_collection_1d_electron_n" + std::to_string(np),
    };
    EfficiencyComparisonPlotterConfiguration.variableNames =
      {
       "trackeff_vs_pT",
       "trackeff_vs_eta"
      };

    std::shared_ptr<Algorithm::EfficiencyComparisonPlotterAlgorithm> efficiencyComparisonPlotterAlgorithm =
      std::make_shared<Algorithm::EfficiencyComparisonPlotterAlgorithm>("EfficiencyComparisonPlotterAlgorithm_n" + std::to_string(np),
                                                                        EfficiencyComparisonPlotterConfiguration);

    scheduler.addAlgorithm(efficiencyComparisonPlotterAlgorithm);
  }
  
  scheduler.run();
}



void
get_algo_sequence(Core::Scheduler& sequence,
		  const std::string& file_name,
		  const std::string& particle_type,
		  const std::string& nparticles)
{

  Algorithm::RootFileEfficiencyMakerAlgorithm::Config RootFileEfficiencyMakerConfiguration;
  RootFileEfficiencyMakerConfiguration.inputFile = file_name;
  RootFileEfficiencyMakerConfiguration.output_collection_1d = "profile_collection_1d_" + particle_type + "_n" + nparticles;
  RootFileEfficiencyMakerConfiguration.output_collection_2d = "profile_collection_2d_" + particle_type + "_n" + nparticles;
  RootFileEfficiencyMakerConfiguration.extractionFunction =
    [=] (TFile* file) -> std::pair<
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
    = std::make_shared<Algorithm::RootFileEfficiencyMakerAlgorithm>("RootFileEfficiencyMakerAlgorithm_" + particle_type + "_n" + nparticles,
								    RootFileEfficiencyMakerConfiguration);

  sequence.addAlgorithm(rootFileEfficiencyMakerAlgorithm);

  // Plotter
  Algorithm::Efficiency1DPlotterAlgorithm::Config Efficiency1DPlotterConfiguration;
  Efficiency1DPlotterConfiguration.prefix = "eff_1d";
  Efficiency1DPlotterConfiguration.inputCollection = RootFileEfficiencyMakerConfiguration.output_collection_1d;
  Efficiency1DPlotterConfiguration.outputFolder = "./efficiency_plots_vs_nparticles/" + particle_type + "_n" + nparticles;

  std::shared_ptr<Algorithm::Efficiency1DPlotterAlgorithm> efficiency1DPlotterAlgorithm =
    std::make_shared<Algorithm::Efficiency1DPlotterAlgorithm>("Efficiency1DPlotterAlgorithm_" + particle_type + "_n" + nparticles,
							      Efficiency1DPlotterConfiguration);

  sequence.addAlgorithm(efficiency1DPlotterAlgorithm);
  
  
  Algorithm::Efficiency2DPlotterAlgorithm::Config Efficiency2DPlotterConfiguration;
  Efficiency2DPlotterConfiguration.prefix = "eff_2d";
  Efficiency2DPlotterConfiguration.inputCollection = RootFileEfficiencyMakerConfiguration.output_collection_2d;
  Efficiency2DPlotterConfiguration.outputFolder = "./efficiency_plots_vs_nparticles/" + particle_type + "_n" + nparticles;

  std::shared_ptr<Algorithm::Efficiency2DPlotterAlgorithm> efficiency2DPlotterAlgorithm =
    std::make_shared<Algorithm::Efficiency2DPlotterAlgorithm>("Efficiency2DPlotterAlgorithm_" + particle_type + "_n" + nparticles,
							      Efficiency2DPlotterConfiguration);

  sequence.addAlgorithm(efficiency2DPlotterAlgorithm);
}



