
#include <Efficiency1DPlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

Efficiency1DPlotterAlgorithm::Efficiency1DPlotterAlgorithm(
    const std::string &name,
    const Algorithm::PlotterAlgorithm<
        EventDataModel::EfficiencyObjectCollection>::Config &cfg)
    : PlotterAlgorithm(name, cfg) {}

void Efficiency1DPlotterAlgorithm::DrawCollection(
    const EventDataModel::EfficiencyObjectCollection *collection) {

  for (const auto &object : *collection) {
    auto &object_to_draw =
        *const_cast<EventDataModel::EfficiencyObject *>(&object);
    const std::string saving_name =
        m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";

    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "APL");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Profile Plot: " + saving_name);
  }
}

} // namespace Algorithm
