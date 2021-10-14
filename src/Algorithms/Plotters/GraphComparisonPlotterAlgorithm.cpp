
#include <GraphComparisonPlotterAlgorithm.hpp>

namespace Algorithm {

GraphComparisonPlotterAlgorithm::GraphComparisonPlotterAlgorithm(
    const std::string &name,
    const ComparisonPlotterAlgorithm<
        EventDataModel::GraphObjectCollection>::Config &cfg)
    : ComparisonPlotterAlgorithm(name, cfg) {}

void GraphComparisonPlotterAlgorithm::DrawCollection(
    TCanvas &canvas, EventDataModel::GraphObjectCollection &collection) {
  canvas.cd();
  for (auto igr(0); igr < collection.size(); igr++) {
    auto &gr = collection.at(igr);

    gr.SetLineColor(igr + 1);
    gr.SetLineStyle(igr == 0 ? 1 : 2);
    gr.SetMarkerStyle(20);
    gr.SetMarkerColor(igr + 1);

    gr.Draw(canvas, igr == 0 ? "AP" : "PSAME");
  }
}

} // namespace Algorithm
