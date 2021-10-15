
#include <GraphPlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

GraphPlotterAlgorithm::GraphPlotterAlgorithm(
    const std::string &name,
    const Algorithm::PlotterAlgorithm<
        EventDataModel::GraphObjectCollection>::Config &cfg)
    : PlotterAlgorithm(name, cfg) {}

void GraphPlotterAlgorithm::DrawCollection(
    const EventDataModel::GraphObjectCollection *collection) {

  for (const auto &object : *collection) {
    auto &object_to_draw = *const_cast<EventDataModel::GraphObject *>(&object);
    const std::string saving_name =
        m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";
    object_to_draw.SetMarkerSize(0.7);
    object_to_draw.SetMarkerStyle(21);
    object_to_draw.SetMarkerColor(2);

    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "AP");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Graph Plot: " + saving_name);
  }
}

} // namespace Algorithm
