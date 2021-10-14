
#include <ProfileComparisonPlotterAlgorithm.hpp>

namespace Algorithm {

ProfileComparisonPlotterAlgorithm::ProfileComparisonPlotterAlgorithm(
    const std::string &name,
    const ComparisonPlotterAlgorithm<
        EventDataModel::ProfileObjectCollection>::Config &cfg)
    : ComparisonPlotterAlgorithm(name, cfg) {}

void ProfileComparisonPlotterAlgorithm::DrawCollection(
    TCanvas &canvas, EventDataModel::ProfileObjectCollection &collection) {
  canvas.cd();
  for (auto igr(0); igr < collection.size(); igr++) {
    auto &gr = collection.at(igr);

    gr.SetLineColor(igr + 1);
    gr.SetMarkerColor(igr + 1);

    gr.Draw(canvas, igr == 0 ? "" : "SAME");
  }
}

} // namespace Algorithm
