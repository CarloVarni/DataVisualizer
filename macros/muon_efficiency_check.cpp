
#include <DataHistogramMakerAlgorithm.hpp>
#include <FilteringAlgorithm.hpp>
#include <MultiDataHistogramMakerAlgorithm.hpp>
#include <PlotterAlgorithm.hpp>
#include <Profile.hpp>
#include <RootFileHistogramMakerAlgorithm.hpp>
#include <Scheduler.hpp>
#include <SeedingPerformanceRootReaderAlgorithm.hpp>
#include <TxtFileReaderAlgorithm.hpp>

int main() {

  Core::Scheduler scheduler;

  // Read histograms
  Algorithm::RootFileHistogramMakerAlgorithm::Config
      RootFileHistogramMakerConfiguration;
  RootFileHistogramMakerConfiguration.inputFile =
      "./data/muon_performance/muon_efficiency_plots.root";
  RootFileHistogramMakerConfiguration.output_collection_1d =
      "muon_performance_1d";
  RootFileHistogramMakerConfiguration.output_collection_2d =
      "muon_performance_2d";
  RootFileHistogramMakerConfiguration.extractionFunction = [&](TFile *file)
      -> std::pair<
          std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>,
          std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>> {
    std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>
        histo_collection_1d =
            std::make_shared<EventDataModel::HistogramObjectCollection_1D>();

    std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>
        histo_collection_2d =
            std::make_shared<EventDataModel::HistogramObjectCollection_2D>();

    // 1D
    histo_collection_1d->emplace_back(
        "muon_reco_pt", "muon_reco_pt",
        *(TH1F *)file->Get("muon_reco_pt")->Clone("muon_reco_pt"));
    histo_collection_1d->emplace_back("muon_reco_eta", "muon_reco_eta",
                                      *(TH1F *)file->Get("muon_reco_eta"));
    histo_collection_1d->emplace_back("muon_reco_phi", "muon_reco_phi",
                                      *(TH1F *)file->Get("muon_reco_phi"));

    histo_collection_1d->emplace_back("muon_truth_pt", "muon_truth_pt",
                                      *(TH1F *)file->Get("muon_truth_pt"));
    histo_collection_1d->emplace_back("muon_truth_eta", "muon_truth_eta",
                                      *(TH1F *)file->Get("muon_truth_eta"));
    histo_collection_1d->emplace_back("muon_truth_phi", "muon_truth_phi",
                                      *(TH1F *)file->Get("muon_truth_phi"));

    histo_collection_1d->emplace_back("n_reco_times", "n_reco_times",
                                      *(TH1F *)file->Get("n_reco_times"));

    // 2D
    histo_collection_2d->emplace_back("muon_reco_pT_vs_eta", "muon_reco_pT",
                                      "muon_reco_eta",
                                      *(TH2F *)file->Get("muon_reco_pT_vs_eta")
                                           ->Clone("muon_reco_pT_vs_eta"));
    histo_collection_2d->emplace_back("muon_reco_pT_vs_phi", "muon_reco_pT",
                                      "muon_reco_phi",
                                      *(TH2F *)file->Get("muon_reco_pT_vs_phi")
                                           ->Clone("muon_reco_pT_vs_phi"));
    histo_collection_2d->emplace_back("muon_reco_eta_vs_phi", "muon_reco_eta",
                                      "muon_reco_phi",
                                      *(TH2F *)file->Get("muon_reco_eta_vs_phi")
                                           ->Clone("muon_reco_eta_vs_phi"));

    histo_collection_2d->emplace_back("muon_truth_pT_vs_eta", "muon_truth_pT",
                                      "muon_truth_eta",
                                      *(TH2F *)file->Get("muon_truth_pT_vs_eta")
                                           ->Clone("muon_truth_pT_vs_eta"));
    histo_collection_2d->emplace_back("muon_truth_pT_vs_phi", "muon_truth_pT",
                                      "muon_truth_phi",
                                      *(TH2F *)file->Get("muon_truth_pT_vs_phi")
                                           ->Clone("muon_truth_pT_vs_phi"));
    histo_collection_2d->emplace_back(
        "muon_truth_eta_vs_phi", "muon_truth_eta", "muon_truth_phi",
        *(TH2F *)file->Get("muon_truth_eta_vs_phi")
             ->Clone("muon_truth_eta_vs_phi"));

    return std::make_pair(histo_collection_1d, histo_collection_2d);
  };

  std::shared_ptr<Algorithm::RootFileHistogramMakerAlgorithm>
      rootFileHistogramMakerAlgorithm =
          std::make_shared<Algorithm::RootFileHistogramMakerAlgorithm>(
              "RootFileHistogramMakerAlgorithm",
              RootFileHistogramMakerConfiguration);

  scheduler.addAlgorithm(rootFileHistogramMakerAlgorithm);

  // Plotter
  Algorithm::PlotterAlgorithm::Config PlotterConfiguration;
  PlotterConfiguration.inputCollection_hist_1d =
      RootFileHistogramMakerConfiguration.output_collection_1d;
  PlotterConfiguration.inputCollection_hist_2d =
      RootFileHistogramMakerConfiguration.output_collection_2d;
  PlotterConfiguration.outputFolder = "./muon_performance_plots";

  std::shared_ptr<Algorithm::PlotterAlgorithm> plotterAlgorithm =
      std::make_shared<Algorithm::PlotterAlgorithm>("PlotterAlgorithm",
                                                    PlotterConfiguration);

  scheduler.addAlgorithm(plotterAlgorithm);

  scheduler.run();
}
