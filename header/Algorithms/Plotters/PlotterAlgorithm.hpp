
#ifndef PLOTTER_ALGORITHM_H
#define PLOTTER_ALGORITHM_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>

#include <vector>

namespace Algorithm {

template<typename object_collection_t>
class PlotterAlgorithm : public Core::BaseAlgorithm {
public:
  struct Config {
    std::string outputFolder = "";
    std::string prefix = "";
    std::string inputCollection = "";
  };
  
  PlotterAlgorithm() = delete;
  PlotterAlgorithm(const std::string &name, const Config &cfg);
  PlotterAlgorithm(const PlotterAlgorithm &) = delete;
  PlotterAlgorithm &operator=(const PlotterAlgorithm &) = delete;
  virtual ~PlotterAlgorithm() = default;

  virtual void initialize() override;
  virtual void execute(Core::EventContext &context) override;

protected:
  virtual void DrawCollection(const object_collection_t *collection) = 0;
  
protected:
  const Config m_cfg;
};

template<typename object_collection_t>
PlotterAlgorithm<object_collection_t>::PlotterAlgorithm(const std::string &name,
							const Config &cfg)
  : Core::BaseAlgorithm(name),
    m_cfg(cfg) {}

template<typename object_collection_t>
void
PlotterAlgorithm<object_collection_t>::initialize() {
  MSG_INFO("Initializing " + name() + " ... ");

  if (m_cfg.outputFolder.empty())
    throw std::invalid_argument("Missing output folder for algorithm " +
                                name());
  
  if (m_cfg.inputCollection.empty())
    throw std::invalid_argument("Missing input collection for algorithm " +
                                name());
}

template<typename object_collection_t>
void
PlotterAlgorithm<object_collection_t>::execute(Core::EventContext &context) {
  MSG_INFO("Executing " + name() + " ... ");
  
  const object_collection_t *object_collection =
    context.get<object_collection_t>(m_cfg.prefix + "_" + m_cfg.inputCollection);

  DrawCollection(object_collection);  
}

} // namespace Algorithm

#endif
