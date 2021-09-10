
#include <Scheduler.hpp>

namespace Core {

  Scheduler::Scheduler()
    : m_eventContext(std::make_unique<Core::EventContext>())
  {}

  void
  Scheduler::run() {
    // Initialize everything
    for	(const auto& algo : m_algoSequence)
      m_store.at(algo)->initialize();

    // Execute Code
    for (const auto& algo : m_algoSequence)
      m_store.at(algo)->execute( *m_eventContext.get() );

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
