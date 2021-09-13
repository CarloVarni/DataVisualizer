
#include <PlotterAlgorithm.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>

namespace Algorithm {

  PlotterAlgorithm::PlotterAlgorithm(const std::string& name,
				     const Config& cfg)
    : Core::BaseAlgorithm(name),
      m_cfg(cfg)
  {}

  void
  PlotterAlgorithm::initialize()
  {
    MSG_INFO("Initializing " + name() + " ... ");

    if (m_cfg.inputCollection_1d.empty() and m_cfg.inputCollection_2d.empty())
      throw std::invalid_argument(name() + " requires an input collection to be defined!");

    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument(name() + " requires an output folder for the plots to be specified!");
  }

  void
  PlotterAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ... ");

    const EventDataModel::HistogramObjectCollection_1D* histograms_1D = nullptr;
    if (not m_cfg.inputCollection_1d.empty())
      histograms_1D = context.get<EventDataModel::HistogramObjectCollection_1D>( "h_1d_" + m_cfg.inputCollection_1d);
    
    const EventDataModel::HistogramObjectCollection_2D* histograms_2D = nullptr;
    if (not m_cfg.inputCollection_2d.empty())
      histograms_2D = context.get<EventDataModel::HistogramObjectCollection_2D>( "h_2d_" + m_cfg.inputCollection_2d);

    if (not histograms_1D and not histograms_2D)
      throw std::invalid_argument("Empty histograms... cannot be retrieved!");

    

    if (histograms_1D) {
      for (const auto& const_histo : *histograms_1D) {
	auto& histo = *const_cast<EventDataModel::HistogramObject_1D*>(&const_histo);
	const std::string saving_name = m_cfg.outputFolder + "/" + histo.title() + ".pdf";

	TCanvas canvas("canvas", "canvas");
	histo.Draw(canvas, "HIST");
	canvas.Draw();
	canvas.SaveAs( saving_name.c_str() );
	MSG_INFO("Created histogram: " + saving_name);
      }
    }

    if (histograms_2D) {
      for (const auto& const_histo : *histograms_2D) {
	auto& histo = *const_cast<EventDataModel::HistogramObject_2D*>(&const_histo);
	const std::string saving_name = m_cfg.outputFolder + "/" + histo.title() + ".pdf";

	TCanvas canvas("canvas", "canvas");
	histo.Draw(canvas, "COLZ");
	canvas.Draw();
	canvas.SaveAs( saving_name.c_str() );
	MSG_INFO("Created histogram: " + saving_name);
      }
    }
    
  }
  
}
