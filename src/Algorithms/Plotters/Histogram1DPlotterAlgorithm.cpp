
#include <Histogram1DPlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

Histogram1DPlotterAlgorithm::Histogram1DPlotterAlgorithm(
    const std::string &name,
    const Algorithm::PlotterAlgorithm<
        EventDataModel::HistogramObjectCollection_1D>::Config &cfg)
    : PlotterAlgorithm(name, cfg) {}

void Histogram1DPlotterAlgorithm::DrawCollection(
    const EventDataModel::HistogramObjectCollection_1D *collection) {

  for (const auto &object : *collection) {
    auto &object_to_draw =
        *const_cast<EventDataModel::HistogramObject_1D *>(&object);
    const std::string saving_name =
        m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";

    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "HIST");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Graph Plot: " + saving_name);
  }
}

} // namespace Algorithm
