

#ifndef EFFICIENCY_H
#define EFFICIENCY_H 1

#include <Data.hpp>
#include <MultiData.hpp>

#include <TEfficiency.h>
#include <TCanvas.h>

namespace EventDataModel {

  class Efficiency {
  public:
    Efficiency() = delete;
    Efficiency(const std::string& title,
	       TEfficiency efficiency);
    Efficiency(const std::string& title,
	       const std::string& target_variable_x,
	       TEfficiency efficiency);
    Efficiency(const std::string& title,
	       const std::string& target_variable_x,
	       const std::string& target_variable_y,
	       TEfficiency efficiency);
    ~Efficiency() = default;

    
    void Fill(const DataObject& data,
	      bool isGood);
    void Fill(const MultiDataObject& data,
	      bool isGood); 
    
    void Draw(TCanvas& canvas,
	      const std::string& option);

    const std::string& title() const;
    const std::string& target_variable_x() const;
    const std::string& target_variable_y() const;

    void SetLineColor(int color);
    void SetLineStyle(int style);
    void SetMarkerColor(int color);
    void SetMarkerStyle(int style);
    void SetMarkerSize(double size);
    
  private:
    const std::string m_title;
    const std::string m_target_variable_x;
    const std::string m_target_variable_y;
    TEfficiency m_efficiency;
  };


  using EfficiencyObject = Efficiency;
  using EfficiencyObjectCollection = std::vector<Efficiency>;
}

#endif
