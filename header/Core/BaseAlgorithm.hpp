
#ifndef BASE_ALGORITHM_H
#define BASE_ALGORITHM_H 1

#include <EventContext.hpp>
#include <string>

namespace Core {

  class BaseAlgorithm {
  public:
    BaseAlgorithm() = delete;
    BaseAlgorithm(const std::string& name);
    ~BaseAlgorithm() = default;

    virtual void initialize() = 0;
    virtual void execute(EventContext& context) = 0;
    virtual void finalize() = 0;

    const std::string& name() const;
    
  protected:
    const std::string m_name;
  };

  /* ============================================================== */
  
  BaseAlgorithm::BaseAlgorithm(const std::string& name)
    : m_name(name)
  {}

  const std::string&
  BaseAlgorithm::name() const
  { return m_name; }
  
}

#endif
