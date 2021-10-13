
#include <Scheduler.hpp>

#include <RootFileEfficiencyMakerAlgorithm.hpp>
#include <RootFileProfileMakerAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
#include <RootFileHistogramMakerAlgorithm.hpp>
#include <EfficiencyComparisonPlotterAlgorithm.hpp>
#include <ProfileComparisonPlotterAlgorithm.hpp>
#include <PlotComparisonAlgorithm.hpp>

void get_algo_sequence(Core::Scheduler& sequence,
		       const std::string& file_name,
		       const std::string& particle_type);

int main() {
  Core::Scheduler scheduler;
    // ================
  
  std::vector<std::string> particles{ "muon", "pion", "electron" };
  
  for (const std::string& particle : particles) {
    std::string fileName = "./data/"  + particle + "_efficiency_plots.root";
    get_algo_sequence(scheduler, fileName, particle);
  }

  // Compare Histograms
  Algorithm::PlotComparisonAlgorithm::Config PlotComparisonConfiguration;
  PlotComparisonConfiguration.outputFolder = "./plots/tracking_performance_plots";
  PlotComparisonConfiguration.variableNames =
    {
      "reco_particle_pt",
      "reco_particle_eta",
      "truth_particle_n_reco_times",
      "reco_particle_d0",
      "reco_particle_z0",
      "reco_particle_err_d0",
      "reco_particle_err_z0",
      "reco_particle_significance_d0",
      "reco_particle_significance_z0"
    };

  std::vector<std::string> input_collection_hist;
  input_collection_hist.reserve(particles.size());
  for (const std::string& particle : particles) {
    input_collection_hist.push_back( "h_collection_1d_" + particle );
  }
  PlotComparisonConfiguration.originalInputCollection = input_collection_hist;
  
  std::shared_ptr<Algorithm::PlotComparisonAlgorithm> plotComparisonAlgorithm =
    std::make_shared<Algorithm::PlotComparisonAlgorithm>("PlotComparisonAlgorithm",
							 PlotComparisonConfiguration);

  scheduler.addAlgorithm(plotComparisonAlgorithm);
  
      
  // Compare Prof
  Algorithm::ProfileComparisonPlotterAlgorithm::Config ProfileComparisonPlotterConfiguration;
  ProfileComparisonPlotterConfiguration.outputFolder = "./plots/tracking_performance_plots";
  ProfileComparisonPlotterConfiguration.variableNames =
    {
      "reco_particle_profile_d0_vs_eta",
      "reco_particle_profile_z0_vs_eta",
      "reco_particle_profile_err_d0_vs_eta",
      "reco_particle_profile_err_z0_vs_eta"
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
  // Histograms
  Algorithm::RootFileHistogramMakerAlgorithm::Config RootFileHistogramMakerConfiguration;
  RootFileHistogramMakerConfiguration.inputFile = file_name;
  RootFileHistogramMakerConfiguration.output_collection_1d = "h_collection_1d_" + particle_type;
  RootFileHistogramMakerConfiguration.output_collection_2d = "h_collection_2d_" + particle_type;
  RootFileHistogramMakerConfiguration.extractionFunction =
    [=] (TFile* file) -> std::pair<
      std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>,
      std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>
      >
    {
      std::shared_ptr<EventDataModel::HistogramObjectCollection_1D> output_1d =
      std::make_shared<EventDataModel::HistogramObjectCollection_1D>();

      std::shared_ptr<EventDataModel::HistogramObjectCollection_2D> output_2d =
      std::make_shared<EventDataModel::HistogramObjectCollection_2D>();

      TH1F *reco_particle_pt = (TH1F*) file->Get("reco_particle_pt");
      TH1F *reco_particle_eta = (TH1F*) file->Get("reco_particle_eta");
      
      TH1F *truth_particle_n_reco_times = (TH1F*) file->Get("truth_particle_n_reco_times");

      TH1F *reco_particle_d0 = (TH1F*) file->Get("reco_particle_d0");
      TH1F *reco_particle_z0 = (TH1F*) file->Get("reco_particle_z0");
      TH1F *reco_particle_err_d0 = (TH1F*) file->Get("reco_particle_err_d0");
      TH1F *reco_particle_err_z0 = (TH1F*) file->Get("reco_particle_err_z0");
      TH1F *reco_particle_significance_d0 = (TH1F*) file->Get("reco_particle_significance_d0");
      TH1F *reco_particle_significance_z0 = (TH1F*) file->Get("reco_particle_significance_z0");

      output_1d->emplace_back("reco_particle_pt",
			      *reco_particle_pt);
      output_1d->emplace_back("reco_particle_eta",
			      *reco_particle_eta);
      
      output_1d->emplace_back("truth_particle_n_reco_times",
			      *truth_particle_n_reco_times);

      output_1d->emplace_back("reco_particle_d0",
			      *reco_particle_d0);
      output_1d->emplace_back("reco_particle_z0",
			      *reco_particle_z0);
      output_1d->emplace_back("reco_particle_err_d0",
			      *reco_particle_err_d0);
      output_1d->emplace_back("reco_particle_err_z0",
			      *reco_particle_err_z0);
      output_1d->emplace_back("reco_particle_significance_d0",
			      *reco_particle_significance_d0);
      output_1d->emplace_back("reco_particle_significance_z0",
			      *reco_particle_significance_z0);
      
      return std::make_pair(output_1d, output_2d);
    };

  std::shared_ptr<Algorithm::RootFileHistogramMakerAlgorithm> rootFileHistogramMakerAlgorithm
    = std::make_shared<Algorithm::RootFileHistogramMakerAlgorithm>("RootFileHistogramMakerAlgorithm_" + particle_type,
								   RootFileHistogramMakerConfiguration);

  sequence.addAlgorithm(rootFileHistogramMakerAlgorithm);
  
  
  // TProfile  
  Algorithm::RootFileProfileMakerAlgorithm::Config RootFileProfileMakerConfiguration;
  RootFileProfileMakerConfiguration.inputFile = file_name;
  RootFileProfileMakerConfiguration.output_collection = "prof_collection_" + particle_type;
  RootFileProfileMakerConfiguration.extractionFunction =
    [=] (TFile* file) -> std::shared_ptr<EventDataModel::ProfileObjectCollection>
    {
      std::shared_ptr<EventDataModel::ProfileObjectCollection> output =
      std::make_shared<EventDataModel::ProfileObjectCollection>();
      
      TProfile *reco_particle_profile_d0_vs_eta = (TProfile*) file->Get("reco_particle_profile_d0_vs_eta");
      TProfile *reco_particle_profile_z0_vs_eta = (TProfile*) file->Get("reco_particle_profile_z0_vs_eta");

      TProfile *reco_particle_profile_err_d0_vs_eta = (TProfile*) file->Get("reco_particle_profile_err_d0_vs_eta");
      TProfile *reco_particle_profile_err_z0_vs_eta = (TProfile*) file->Get("reco_particle_profile_err_z0_vs_eta");

      output->emplace_back("reco_particle_profile_d0_vs_eta",
			   *reco_particle_profile_d0_vs_eta);
      output->emplace_back("reco_particle_profile_z0_vs_eta",
			   *reco_particle_profile_z0_vs_eta);
      
      output->emplace_back("reco_particle_profile_err_d0_vs_eta",
			   *reco_particle_profile_err_d0_vs_eta);
      output->emplace_back("reco_particle_profile_err_z0_vs_eta",
			   *reco_particle_profile_err_z0_vs_eta);
      
      return output;
    };
  
  std::shared_ptr<Algorithm::RootFileProfileMakerAlgorithm> rootFileProfileMakerAlgorithm
    = std::make_shared<Algorithm::RootFileProfileMakerAlgorithm>("RootFileProfileMakerAlgorithm_" + particle_type,
								 RootFileProfileMakerConfiguration);
  
  sequence.addAlgorithm(rootFileProfileMakerAlgorithm);

  
  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_hist_1d = RootFileHistogramMakerConfiguration.output_collection_1d;
  PlotterConfiguration.inputCollection_hist_2d = RootFileHistogramMakerConfiguration.output_collection_2d;
  PlotterConfiguration.inputCollection_prof = RootFileProfileMakerConfiguration.output_collection;
  PlotterConfiguration.outputFolder = "./plots/tracking_performance_plots";
  
  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm_"  + particle_type,
                                                  PlotterConfiguration);
  
  sequence.addAlgorithm(plotterAlgorithm);
}



