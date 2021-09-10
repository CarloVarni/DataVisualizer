
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
  
}

#endif
