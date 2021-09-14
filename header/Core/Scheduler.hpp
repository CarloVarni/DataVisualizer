
#ifndef SCHEDULER_H
#define SCHEDULER_H 1

#include <BaseAlgorithm.hpp>
#include <EventContext.hpp>
#include <Messager.hpp>

#include <stdexcept>
#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

namespace Core {

  class Scheduler {
  public:
    struct Config {
      long int nEvents = 1;
    };

    Scheduler();
    Scheduler(const Config&);
    Scheduler(const Scheduler&) = delete;
    Scheduler& operator=(const Scheduler&) = delete;
    ~Scheduler() = default;
    
    void addAlgorithm(std::shared_ptr<Core::BaseAlgorithm> algorithm);
    void run();

  protected:
    const std::string& name() const;
    
  private:
    const Config m_cfg;
    const std::string m_name;
    std::vector<std::unique_ptr<Core::EventContext>> m_eventContext;
    std::vector<std::string> m_algoSequence;
    std::unordered_map<std::string,
		       std::shared_ptr<Core::BaseAlgorithm>> m_store;
  };
  
}

#endif
