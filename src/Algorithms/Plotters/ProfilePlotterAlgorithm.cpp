
#include <ProfilePlotterAlgorithm.hpp>
#include <TCanvas.h>

namespace Algorithm {

ProfilePlotterAlgorithm::ProfilePlotterAlgorithm(const std::string &name,
						 const Algorithm::PlotterAlgorithm<EventDataModel::ProfileObjectCollection>::Config &cfg)
  : PlotterAlgorithm(name, cfg) {}

void
ProfilePlotterAlgorithm::DrawCollection(const EventDataModel::ProfileObjectCollection *collection) {

  for (const auto &object : *collection) {
    auto &object_to_draw = *const_cast<EventDataModel::ProfileObject *>(&object);
    const std::string saving_name =
      m_cfg.outputFolder + "/" + object_to_draw.title() + ".pdf";
    object_to_draw.SetMarkerColor(2);
    object_to_draw.SetLineColor(2);
    
    TCanvas canvas("canvas", "canvas");
    object_to_draw.Draw(canvas, "");
    canvas.Draw();
    canvas.SaveAs(saving_name.c_str());
    MSG_INFO("Created Profile Plot: " + saving_name);
  }
}

} // namespace Algorithm
