
#ifndef ROOTFILE_HISTOGRAM_MAKER_ALGORITHM_H
#define ROOTFILE_HISTOGRAM_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Histogram_1D.hpp>
#include <Histogram_2D.hpp>

#include <TFile.h>

namespace Algorithm {

  class RootFileHistogramMakerAlgorithm
    : public Core::BaseAlgorithm {
  public:
    struct Config {
      std::string inputFile;
      std::string output_collection_1d;
      std::string output_collection_2d;
      std::function<
	std::pair<
	  std::shared_ptr<EventDataModel::HistogramObjectCollection_1D>,
	  std::shared_ptr<EventDataModel::HistogramObjectCollection_2D>
	  >(TFile*)
	> extractionFunction;
    };

    RootFileHistogramMakerAlgorithm() = delete;
    RootFileHistogramMakerAlgorithm(const std::string& name,
				    const Config& cfg);
    RootFileHistogramMakerAlgorithm(const RootFileHistogramMakerAlgorithm&) = delete;
    RootFileHistogramMakerAlgorithm& operator=(const RootFileHistogramMakerAlgorithm&) = delete;
    virtual ~RootFileHistogramMakerAlgorithm() = default;

    virtual void initialize() override;
    virtual void execute(Core::EventContext& context) override;
    
  private:
    const Config m_cfg;
  };

}

#endif
