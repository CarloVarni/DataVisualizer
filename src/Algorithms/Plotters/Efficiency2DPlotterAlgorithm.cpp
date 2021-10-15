
#include <Efficiency2DPlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

Efficiency2DPlotterAlgorithm::Efficiency2DPlotterAlgorithm(const std::string &name,
							   const Algorithm::PlotterAlgorithm<EventDataModel::EfficiencyObjectCollection>::Config &cfg)
  : PlotterAlgorithm(name, cfg) {}
  
void
Efficiency2DPlotterAlgorithm::DrawCollection(const EventDataModel::EfficiencyObjectCollection *collection) {
  
  for (const auto &object : *collection) {
    auto &object_to_draw = *const_cast<EventDataModel::EfficiencyObject *>(&object);
    const std::string saving_name =
      m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";
    
    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "COLZ");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Profile Plot: " + saving_name);
  }
}

} // namespace Algorithm
