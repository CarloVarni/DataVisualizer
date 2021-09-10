
#include <Scheduler.hpp>

namespace Core {

  Scheduler::Scheduler()
  {}

  Scheduler::Scheduler(const Core::Scheduler::Config& config)
    : m_cfg(config)
  {}
  
  void
  Scheduler::run() {
    // Creating Event Context
    m_eventContext.reserve(m_cfg.nEvents);
    for(auto ievt(0); ievt<m_cfg.nEvents; ievt++)
      m_eventContext.push_back(std::make_unique<Core::EventContext>());
    
    // Initialize everything
    for	(const auto& algo : m_algoSequence)
      m_store.at(algo)->initialize();

    // Execute Code
    for(auto ievt(0); ievt<m_cfg.nEvents; ievt++) {
      for (const auto& algo : m_algoSequence)
	m_store.at(algo)->execute( *m_eventContext.at(ievt).get() );
    }
    
    // Finalize everything
    for (const auto& algo : m_algoSequence)
      m_store.at(algo)->finalize();
  }

  void
  Scheduler::addAlgorithm(std::shared_ptr<Core::BaseAlgorithm> algorithm)
  {
    const std::string name = algorithm->name();
    if (m_store.find(name) != m_store.end())
      throw std::invalid_argument("Algorithm `" + name + "` is already in store. Cannot add another one to scheduler.");
    
    m_algoSequence.push_back(name);
    m_store.emplace(name, std::move(algorithm));
  }
  
}
