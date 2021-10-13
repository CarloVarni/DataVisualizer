
#include <EfficiencyComparisonPlotterAlgorithm.hpp>

namespace Algorithm {

  EfficiencyComparisonPlotterAlgorithm::EfficiencyComparisonPlotterAlgorithm(const std::string& name,
									     const ComparisonPlotterAlgorithm<EventDataModel::EfficiencyObjectCollection>::Config& cfg)
    : ComparisonPlotterAlgorithm(name, cfg)
  {}
  
  void
  EfficiencyComparisonPlotterAlgorithm::DrawCollection(TCanvas& canvas,
						       EventDataModel::EfficiencyObjectCollection& collection)
  {
    canvas.cd();
    for (auto igr(0); igr<collection.size(); igr++) {
      auto& gr = collection.at(igr);

      gr.SetLineColor(igr + 1);
      gr.SetLineStyle(igr == 0 ? 1 : 2);
      gr.SetMarkerStyle(20);
      gr.SetMarkerColor(igr + 1);
      
      gr.Draw(canvas, igr == 0 ? "APL" : "PLSAME" );
    }
  }
    
}


