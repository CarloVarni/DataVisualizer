
#include <PlotterAlgorithm.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>
#include <Efficiency.hpp>
#include <Graph.hpp>

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

    if (m_cfg.inputCollection_hist_1d.empty() and
	m_cfg.inputCollection_hist_2d.empty() and
	m_cfg.inputCollection_eff_1d.empty() and
	m_cfg.inputCollection_eff_2d.empty() and
	m_cfg.inputCollection_gr.empty() )
      throw std::invalid_argument(name() + " requires an input collection to be defined!");

    if (m_cfg.outputFolder.empty())
      throw std::invalid_argument(name() + " requires an output folder for the plots to be specified!");
  }

  void
  PlotterAlgorithm::execute(Core::EventContext& context)
  {
    MSG_INFO("Executing " + name() + " ... ");

    const EventDataModel::HistogramObjectCollection_1D* histograms_1D = nullptr;
    if (not m_cfg.inputCollection_hist_1d.empty())
      histograms_1D = context.get<EventDataModel::HistogramObjectCollection_1D>( "h_1d_" + m_cfg.inputCollection_hist_1d);
    
    const EventDataModel::HistogramObjectCollection_2D* histograms_2D = nullptr;
    if (not m_cfg.inputCollection_hist_2d.empty())
      histograms_2D = context.get<EventDataModel::HistogramObjectCollection_2D>( "h_2d_" + m_cfg.inputCollection_hist_2d);

    const EventDataModel::EfficiencyObjectCollection* efficiency_1D = nullptr;
    if (not m_cfg.inputCollection_eff_1d.empty())
      efficiency_1D = context.get<EventDataModel::EfficiencyObjectCollection>( "eff_1d_" + m_cfg.inputCollection_eff_1d );

    const EventDataModel::EfficiencyObjectCollection* efficiency_2D = nullptr;
    if (not m_cfg.inputCollection_eff_2d.empty())
      efficiency_2D = context.get<EventDataModel::EfficiencyObjectCollection>( "eff_2d_" + m_cfg.inputCollection_eff_2d );

    const EventDataModel::GraphObjectCollection* graphs = nullptr;
    if (not m_cfg.inputCollection_gr.empty())
      graphs = context.get<EventDataModel::GraphObjectCollection>( "gr_" + m_cfg.inputCollection_gr );
    
    // Histograms 1D
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

    // Histograms 2D
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


    // Efficiency 1D
    if (efficiency_1D) {
      for (const auto& const_eff : *efficiency_1D) {
        auto& eff = *const_cast<EventDataModel::EfficiencyObject*>(&const_eff);
        const std::string saving_name = m_cfg.outputFolder + "/" + eff.title() + ".pdf";

        TCanvas canvas("canvas", "canvas");
        eff.Draw(canvas, "APL");
        canvas.Draw();
        canvas.SaveAs( saving_name.c_str() );
        MSG_INFO("Created Efficiency: " + saving_name);
      }
    }

    // Efficiency 2D
    if (efficiency_2D) {
      for (const auto& const_eff : *efficiency_2D) {
        auto& eff = *const_cast<EventDataModel::EfficiencyObject*>(&const_eff);
        const std::string saving_name = m_cfg.outputFolder + "/" + eff.title() + ".pdf";

        TCanvas canvas("canvas", "canvas");
        eff.Draw(canvas, "COLZ");
        canvas.Draw();
        canvas.SaveAs( saving_name.c_str() );
        MSG_INFO("Created Efficiency: " + saving_name);
      }
    }

    // Graphs
    if (graphs) {
      for (const auto& const_gr : *graphs) {
        auto& gr = *const_cast<EventDataModel::GraphObject*>(&const_gr);
        const std::string saving_name = m_cfg.outputFolder + "/" + gr.title() + ".pdf";
	gr.SetMarkerSize(0.7);
	gr.SetMarkerStyle(21);
	gr.SetMarkerColor(2);
	
        TCanvas canvas("canvas", "canvas");
        gr.Draw(canvas, "AP");
        canvas.Draw();
        canvas.SaveAs( saving_name.c_str() );
        MSG_INFO("Created Graph: " + saving_name);
      }
    }
    
  }
  
}
