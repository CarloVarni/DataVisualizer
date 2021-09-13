
#include <Scheduler.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <DataHistogramMakerAlgorithm.hpp> 
#include <FilteringAlgorithm.hpp>
#include <SeedingPerformanceRootReaderAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>

#include <RootFileHistogramMakerAlgorithm.hpp>

int main() {
  
  Core::Scheduler scheduler;

  // Read histograms
  Algorithm::RootFileHistogramMakerAlgorithm::Config RootFileHistogramMakerConfiguration;
  RootFileHistogramMakerConfiguration.inputFile = "./data/muon_efficiency_plots.root";
  RootFileHistogramMakerConfiguration.output_collection_1d = "muon_performance_1d";
  RootFileHistogramMakerConfiguration.output_collection_2d = "";
  RootFileHistogramMakerConfiguration.extractionFunction =
    [&] (TFile* file) -> std::pair<std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>,
					 std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>>
    {
     std::shared_ptr<EventDataModel::HistogramObjectCollection_1D> histo_collection_1d =
     std::make_shared<EventDataModel::HistogramObjectCollection_1D>();

     std::shared_ptr<EventDataModel::HistogramObjectCollection_2D> histo_collection_2d =
     std::make_shared<EventDataModel::HistogramObjectCollection_2D>();

     histo_collection_1d->emplace_back("muon_reco_pt",
				       "muon_reco_pt",
				       * (TH1F*) file->Get("muon_reco_pt")->Clone("muon_reco_pt"));
     histo_collection_1d->emplace_back("muon_reco_eta",
				       "muon_reco_eta",
				       * (TH1F*) file->Get("muon_reco_eta"));
     histo_collection_1d->emplace_back("muon_reco_phi",
				       "muon_reco_phi",
				       * (TH1F*) file->Get("muon_reco_phi"));

     histo_collection_1d->emplace_back("muon_truth_pt",
				       "muon_truth_pt",
				       * (TH1F*) file->Get("muon_truth_pt"));
     histo_collection_1d->emplace_back("muon_truth_eta",
				       "muon_truth_eta",
				       * (TH1F*) file->Get("muon_truth_eta"));
     histo_collection_1d->emplace_back("muon_truth_phi",
				       "muon_truth_phi",
				       * (TH1F*) file->Get("muon_truth_phi"));
     
     histo_collection_1d->emplace_back("n_reco_times",
				       "n_reco_times",
				       * (TH1F*) file->Get("n_reco_times"));

     return std::make_pair(histo_collection_1d, histo_collection_2d);
    };

  std::shared_ptr<Algorithm::RootFileHistogramMakerAlgorithm> rootFileHistogramMakerAlgorithm =
    std::make_shared<Algorithm::RootFileHistogramMakerAlgorithm>("RootFileHistogramMakerAlgorithm",
								 RootFileHistogramMakerConfiguration);

  scheduler.addAlgorithm(rootFileHistogramMakerAlgorithm);

  
  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_1d = RootFileHistogramMakerConfiguration.output_collection_1d;
  PlotterConfiguration.inputCollection_2d = RootFileHistogramMakerConfiguration.output_collection_2d;
  PlotterConfiguration.outputFolder = "./muon_performance_plots";
  
  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
    std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm",
						  PlotterConfiguration);
  
  scheduler.addAlgorithm(plotterAlgorithm);


  
  scheduler.run();
}
