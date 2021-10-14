
#include <Scheduler.hpp>
#include <TError.h>

namespace Core {

Scheduler::Scheduler() : m_name("Scheduler") { gErrorIgnoreLevel = kFatal; }

Scheduler::Scheduler(const Core::Scheduler::Config &config)
    : m_cfg(config), m_name("Scheduler") {}

void Scheduler::initialize() {
  MSG_INFO("===================================================");
  MSG_INFO("Sequence of algorithms:");

  for (const auto &algo : m_algoSequence) {
    MSG_INFO("   \\__ " + m_store.at(algo)->name());
  }
  MSG_INFO("===================================================");

  MSG_INFO("Initializing algorithms ... ");
  for (const auto &algo : m_algoSequence)
    m_store.at(algo)->initialize();
}

void Scheduler::finalize() {
  MSG_INFO("===================================================");
  MSG_INFO("finalizing ... ");

  for (const auto &algo : m_algoSequence)
    m_store.at(algo)->finalize();
}

void Scheduler::run() {
  MSG_INFO("Running Scheduler!");

  // Creating Event Context
  m_eventContext.reserve(m_cfg.nEvents);
  for (auto ievt(0); ievt < m_cfg.nEvents; ievt++)
    m_eventContext.push_back(std::make_unique<Core::EventContext>());

  // Initialize everything
  initialize();

  MSG_INFO("===================================================");
  // Execute Code
  for (auto ievt(0); ievt < m_cfg.nEvents; ievt++) {
    for (const auto &algo : m_algoSequence)
      m_store.at(algo)->execute(*m_eventContext.at(ievt).get());
  }

  // Finalize everything
  finalize();
}

void Scheduler::addAlgorithm(std::shared_ptr<Core::BaseAlgorithm> algorithm) {
  const std::string name = algorithm->name();
  if (m_store.find(name) != m_store.end()) {
    MSG_FATAL("Algorithm `" + name +
              "` is already in store. Cannot add another one to scheduler.");
  }

  m_algoSequence.push_back(name);
  m_store.emplace(name, std::move(algorithm));
}

const std::string &Scheduler::name() const { return m_name; }

} // namespace Core
