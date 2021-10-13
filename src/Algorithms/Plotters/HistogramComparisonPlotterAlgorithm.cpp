
#include <HistogramComparisonPlotterAlgorithm.hpp>

namespace Algorithm {

  HistogramComparisonPlotterAlgorithm::HistogramComparisonPlotterAlgorithm(const std::string& name,
									   const ComparisonPlotterAlgorithm<EventDataModel::HistogramObjectCollection_1D>::Config& cfg)
    : ComparisonPlotterAlgorithm(name, cfg)
  {}
  
  void
  HistogramComparisonPlotterAlgorithm::DrawCollection(TCanvas& canvas,
						      EventDataModel::HistogramObjectCollection_1D& collection)
  {
    canvas.cd();
    for (auto ihisto(0); ihisto<collection.size(); ihisto++) {
      auto& histo = collection.at(ihisto);
      histo.Scale( 1./histo.Integral() );
      histo.SetLineColor(ihisto + 1);
      histo.SetLineStyle(ihisto == 0 ? 1 : 2);

      histo.Draw(canvas, ihisto == 0 ? "HIST" : "HISTSAME" );
    }
  }
    
}


