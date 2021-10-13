
#include <Scheduler.hpp>

#include <RootFileEfficiencyMakerAlgorithm.hpp>
#include <RootFileProfileMakerAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
//#include <EfficiencyComparisonPlotterAlgorithm.hpp>
//#include <ProfileComparisonPlotterAlgorithm.hpp>
#include <ComparisonPlotterAlgorithm.hpp>

void get_algo_sequence(Core::Scheduler& sequence,
		       const std::string& file_name,
		       const std::string& particle_type);

int main() {
  Core::Scheduler scheduler;
    // ================
  
  std::vector<std::string> particles{ "muon", "pion", "electron" };
  
  for (const std::string& particle : particles) {
    std::string fileName = "./data/"  + particle + "_performance_ckf.root";
    get_algo_sequence(scheduler, fileName, particle);
  }

  // Compare Eff
  Algorithm::EfficiencyComparisonPlotterAlgorithm::Config EfficiencyComparisonPlotterConfiguration;
  EfficiencyComparisonPlotterConfiguration.prefix = "eff";
  EfficiencyComparisonPlotterConfiguration.outputFolder = "./plots";
  EfficiencyComparisonPlotterConfiguration.variableNames =
    {
      "trackeff_vs_pT",
      "trackeff_vs_eta" 
    };
  
  std::vector<std::string> input_collection_eff;
  input_collection_eff.reserve(particles.size());
  for (const std::string& particle : particles) {
    input_collection_eff.push_back( "eff_collection_1d_" + particle );
  }
  EfficiencyComparisonPlotterConfiguration.originalInputCollection = input_collection_eff;
  
  std::shared_ptr<Algorithm::EfficiencyComparisonPlotterAlgorithm> efficiencyComparisonPlotterAlgorithm =
    std::make_shared<Algorithm::EfficiencyComparisonPlotterAlgorithm>("EfficiencyComparisonPlotterAlgorithm",
								      EfficiencyComparisonPlotterConfiguration);
  
  scheduler.addAlgorithm(efficiencyComparisonPlotterAlgorithm);

			   
  // Compare Prof
  Algorithm::ProfileComparisonPlotterAlgorithm::Config ProfileComparisonPlotterConfiguration;
  ProfileComparisonPlotterConfiguration.prefix = "prof_1d"; 
  ProfileComparisonPlotterConfiguration.outputFolder = "./plots";
  ProfileComparisonPlotterConfiguration.variableNames =
    {
      "nStates_vs_eta",
      "nMeasurements_vs_eta",
      "nOutliers_vs_eta",
      "nStates_vs_pT",
      "nMeasurements_vs_pT",
      "nOutliers_vs_pT"
    };

  std::vector<std::string> input_collection_prof;
  input_collection_prof.reserve(particles.size());
  for (const std::string& particle : particles) {
    input_collection_prof.push_back( "prof_collection_" + particle );
  }
  ProfileComparisonPlotterConfiguration.originalInputCollection = input_collection_prof;

  std::shared_ptr<Algorithm::ProfileComparisonPlotterAlgorithm> profileComparisonPlotterAlgorithm =
    std::make_shared<Algorithm::ProfileComparisonPlotterAlgorithm>("ProfileComparisonPlotterAlgorithm",
								   ProfileComparisonPlotterConfiguration);

    scheduler.addAlgorithm(profileComparisonPlotterAlgorithm);
  
  // ================
  scheduler.run();
}

  
void
get_algo_sequence(Core::Scheduler& sequence,
		  const std::string& file_name,
		  const std::string& particle_type)
{
  // Efficiencies
  Algorithm::RootFileEfficiencyMakerAlgorithm::Config RootFileEfficiencyMakerConfiguration;
  RootFileEfficiencyMakerConfiguration.inputFile = file_name;
  RootFileEfficiencyMakerConfiguration.output_collection_1d = "eff_collection_1d_" + particle_type;
  RootFileEfficiencyMakerConfiguration.output_collection_2d = "eff_collection_2d_" + particle_type;
  RootFileEfficiencyMakerConfiguration.extractionFunction =
    [=] (TFile* file) -> std::pair<
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>,
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection>
      >
    {
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection> output_1d =
      std::make_shared<EventDataModel::EfficiencyObjectCollection>();
      
      std::shared_ptr<EventDataModel::EfficiencyObjectCollection> output_2d =
      std::make_shared<EventDataModel::EfficiencyObjectCollection>();

      TEfficiency *trackeff_vs_pT = (TEfficiency*) file->Get("trackeff_vs_pT");
      TEfficiency *trackeff_vs_eta = (TEfficiency*) file->Get("trackeff_vs_eta");

      output_1d->emplace_back("trackeff_vs_pT",
			      "trackeff_vs_pT",
			      *trackeff_vs_pT);
      output_1d->emplace_back("trackeff_vs_eta",
			      "trackeff_vs_eta",
			      *trackeff_vs_eta);

      return std::make_pair(output_1d, output_2d);
    };
  
  std::shared_ptr<Algorithm::RootFileEfficiencyMakerAlgorithm> rootFileEfficiencyMakerAlgorithm
    = std::make_shared<Algorithm::RootFileEfficiencyMakerAlgorithm>("RootFileEfficiencyMakerAlgorithm_" + particle_type,
								    RootFileEfficiencyMakerConfiguration);
  
  sequence.addAlgorithm(rootFileEfficiencyMakerAlgorithm);

  
  // TProfile  
  Algorithm::RootFileProfileMakerAlgorithm::Config RootFileProfileMakerConfiguration;
  RootFileProfileMakerConfiguration.inputFile = file_name;
  RootFileProfileMakerConfiguration.output_collection = "prof_collection_" + particle_type;
  RootFileProfileMakerConfiguration.extractionFunction =
    [=] (TFile* file) -> std::shared_ptr<EventDataModel::ProfileObjectCollection>
    {
      std::shared_ptr<EventDataModel::ProfileObjectCollection> output =
      std::make_shared<EventDataModel::ProfileObjectCollection>();

      output->emplace_back("nStates_vs_eta",
			   * (TProfile*) file->Get("nStates_vs_eta"));
      output->emplace_back("nMeasurements_vs_eta",
			   * (TProfile*) file->Get("nMeasurements_vs_eta"));
      output->emplace_back("nOutliers_vs_eta",
			   * (TProfile*) file->Get("nOutliers_vs_eta"));
      
      output->emplace_back("nStates_vs_pT",
			   * (TProfile*) file->Get("nStates_vs_pT"));
      output->emplace_back("nMeasurements_vs_pT",
			   * (TProfile*) file->Get("nMeasurements_vs_pT"));
      output->emplace_back("nOutliers_vs_pT",
			   * (TProfile*) file->Get("nOutliers_vs_pT"));
      
      return output;
    };

  std::shared_ptr<Algorithm::RootFileProfileMakerAlgorithm> rootFileProfileMakerAlgorithm
    = std::make_shared<Algorithm::RootFileProfileMakerAlgorithm>("RootFileProfileMakerAlgorithm_" + particle_type,
								 RootFileProfileMakerConfiguration);
  
  sequence.addAlgorithm(rootFileProfileMakerAlgorithm);

  
  // Plotter for eff
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_eff_1d = RootFileEfficiencyMakerConfiguration.output_collection_1d;
  PlotterConfiguration.inputCollection_eff_2d = RootFileEfficiencyMakerConfiguration.output_collection_2d;
  PlotterConfiguration.inputCollection_prof = RootFileProfileMakerConfiguration.output_collection;
  PlotterConfiguration.outputFolder = "./plots/ckf_performance_plots";
  
  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_"  + particle_type,
                                                  PlotterConfiguration);
  
  sequence.addAlgorithm(plotterAlgorithm);

  

}



