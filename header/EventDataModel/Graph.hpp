
#ifndef GRAPH_H
#define GRAPH_H 1

#include <Data.hpp>
#include <MultiData.hpp>

#include <string>
#include <variant>
#include <vector>

#include <TGraph.h>
#include <TGraphErrors.h>
#include <TCanvas.h>

namespace EventDataModel {

  template<typename ... supported_graph_t>
  class Graph {
  public:
    using graph_type = std::variant<supported_graph_t...>;

    static_assert(std::conjunction< std::is_base_of<TGraph, supported_graph_t>... >::value,
		  "Graph Object only supports TGraph and TGraphError objects");

    Graph() = delete;

    template<typename graph_t,
	     typename Enable = std::enable_if_t<std::disjunction<std::is_same<graph_t, supported_graph_t>...>::value>
	     >
    Graph(const std::string& title,
	  graph_t graph);
    
    template<typename graph_t,
	     typename Enable = std::enable_if_t<std::disjunction<std::is_same<graph_t, supported_graph_t>...>::value>
	     >
    Graph(const std::string& title,
	  const std::string& target_variable_x,
	  const std::string& target_variable_y,
	  graph_t graph);

    ~Graph() = default;

    void Fill(const DataObject& data);
    void Fill(const MultiDataObject& data);

    void Draw(TCanvas& canvas,
	      const std::string& option);
    
    int GetN() const;
    
  private:
    const std::string m_title;
    const std::string m_target_variable_x;
    const std::string m_target_variable_y;
    graph_type m_graph;
  };

  /* ============================================================== */

  template<typename ... supported_graph_t>
  template<typename graph_t,
	   typename Enable>
  Graph<supported_graph_t...>::Graph(const std::string& title,
				     graph_t graph)
    : m_title(title),
      m_target_variable_x(""),
      m_target_variable_y(""),
      m_graph(graph)
  {}

  template<typename ... supported_graph_t>
  template<typename graph_t,
	   typename Enable>
  Graph<supported_graph_t...>::Graph(const std::string& title,
				     const std::string& target_variable_x,
				     const std::string& target_variable_y,
				     graph_t graph)
    : m_title(title),
      m_target_variable_x(target_variable_x),
      m_target_variable_y(target_variable_y),
      m_graph(graph)
  {}

  template<typename ... supported_graph_t>
  void
  Graph<supported_graph_t...>::Fill(const DataObject& data)
  {
    std::visit([&] (auto& gr) {
		 int n_points = gr.GetN();
		 std::visit( [&] (const auto& value) {
			       gr.SetPoint(n_points,
					   n_points,
					   value);
			     }, data.value() );
	       }, m_graph);
  }

  template<typename ... supported_graph_t>
  void
  Graph<supported_graph_t...>::Fill(const MultiDataObject& data)
  {
    if (not data.hasLabel(m_target_variable_x) or not data.hasLabel(m_target_variable_y))
    throw std::runtime_error("MultiData collection has not data with label `" + m_target_variable_x + "` or `" + m_target_variable_y + "` to fill the graph with.");

    
    std::visit([&] (auto& gr) {
		 int n_points = gr.GetN();
		 std::visit([&] (const auto& arg_x) {
			      std::visit([&] (const auto& arg_y) {
					   gr.SetPoint(n_points,
						       arg_x,
						       arg_y);
					 }, data.value(m_target_variable_y));
			    }, data.value(m_target_variable_x));
	       }, m_graph);
  }


  template<typename ... supported_graph_t>
  void
  Graph<supported_graph_t...>::Draw(TCanvas& canvas,
				    const std::string& option)
  {
    std::visit([&] (auto& gr) {
		 canvas.cd();
		 gr.Draw(option.c_str());
	       }, m_graph);
  }
  
  template<typename ... supported_graph_t>
  int
  Graph<supported_graph_t...>::GetN() const
  {
    return std::visit([=] (const auto& gr) -> int {
			return gr.GetN();
		      }, m_graph);
  }
  
  /* ============================================================== */

  using GraphObject = Graph<TGraph>; // TGraphError to be added here
  using GraphObjectCollection = std::vector<GraphObject>;
}

#endif
