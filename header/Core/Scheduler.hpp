
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
    
    void addAlgorithm(std::shared_ptr<Core::BaseAlgorithm> algorithm);
    void run();
    
  private:
    std::unique_ptr<Core::EventContext> m_eventContext;
    std::vector<std::string> m_algoSequence;
    std::unordered_map<std::string,
		       std::shared_ptr<Core::BaseAlgorithm>> m_store;
  };
  
}

#endif
