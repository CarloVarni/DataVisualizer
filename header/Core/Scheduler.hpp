
#ifndef SCHEDULER_H
#define SCHEDULER_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>

#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace Core {

  class Scheduler {
  public:
    Scheduler() = default;
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    ~Scheduler() = default;
    
    template <typename T>
    void addAlgorithm(const std::string& name,
		      T&& object);

    void run();
    
  private:
    std::unique_ptr<Core::EventContext> m_eventContext;
    std::vector<std::string> m_algoSequence;
    std::unordered_map<std::string,
		       std::unique_ptr<Core::BaseAlgorithm>> m_store;
  };

  /* ============================================================== */

  template <typename T>
  void
  Scheduler::addAlgorithm(const std::string& name,
			  T&& object)
  {
    if (m_store.find(name) != m_store.end())
      throw std::invalid_argument("Algorithm `" + name + "` is already in store. Cannot add another one");
    
    m_algoSequence.push_back(name);
    m_store,emplace(name,
		    std::make_unique<HolderT<T>>(std::forward<T>(object)));
  }

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

#endif
