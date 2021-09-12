
#ifndef BASE_ALGORITHM_H
#define BASE_ALGORITHM_H 1

#include <EventContext.hpp>
#include <Messager.hpp>

namespace Core {

  class BaseAlgorithm {
  public:
    BaseAlgorithm() = delete;
    BaseAlgorithm(const std::string& name);
    virtual ~BaseAlgorithm() = default;

    virtual void initialize() = 0;
    virtual void execute(EventContext& context) = 0;
    virtual void finalize();

    const std::string& name() const;

  protected:
    const Messager& msg() const;
    
  protected:
    const std::string m_name;
    const Messager m_msg;
  };
  
}

#endif
