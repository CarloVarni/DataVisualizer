
#include <Histogram2DPlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

Histogram2DPlotterAlgorithm::Histogram2DPlotterAlgorithm(
    const std::string &name,
    const Algorithm::PlotterAlgorithm<
        EventDataModel::HistogramObjectCollection_2D>::Config &cfg)
    : PlotterAlgorithm(name, cfg) {}

void Histogram2DPlotterAlgorithm::DrawCollection(
    const EventDataModel::HistogramObjectCollection_2D *collection) {

  for (const auto &object : *collection) {
    auto &object_to_draw =
        *const_cast<EventDataModel::HistogramObject_2D *>(&object);
    const std::string saving_name =
        m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";

    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "COLZ");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Graph Plot: " + saving_name);
  }
}

} // namespace Algorithm
