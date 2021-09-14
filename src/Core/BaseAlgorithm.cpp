
#include <BaseAlgorithm.hpp>

namespace Core {

  BaseAlgorithm::BaseAlgorithm(const std::string& name)
    : m_name(name)
  {}

  void
  BaseAlgorithm::finalize()
  {
    MSG_INFO("Finalizing " + name() + " ...");
  }
  
  const std::string&
  BaseAlgorithm::name() const
  { return m_name; }

}

