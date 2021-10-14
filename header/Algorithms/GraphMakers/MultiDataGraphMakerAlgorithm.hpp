
#ifndef MUTIDATA_GRAPH_MAKER_ALGORITHM_H
#define MUTIDATA_GRAPH_MAKER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <Graph.hpp>

#include <vector>

namespace Algorithm {

class MultiDataGraphMakerAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string inputCollection = "";
    std::string inputMaskName = "";
    std::string outputCollection = "";

    std::vector<std::pair<std::string, std::string>> variableNames;
    std::vector<EventDataModel::GraphObject::graph_type> graphDefs;
  };

  MultiDataGraphMakerAlgorithm() = delete;
  MultiDataGraphMakerAlgorithm(const std::string &name, const Config &cfg);
  MultiDataGraphMakerAlgorithm(const MultiDataGraphMakerAlgorithm &) = delete;
  MultiDataGraphMakerAlgorithm &
  operator=(const MultiDataGraphMakerAlgorithm &) = delete;
  virtual ~MultiDataGraphMakerAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &) override;

private:
  const Config m_cfg;
};

} // namespace Algorithm

#endif
