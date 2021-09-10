
#include <Scheduler.hpp>

namespace Core {

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

}
