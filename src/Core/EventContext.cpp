
#include <EventContext.hpp>

namespace Core {

  EventContext::EventContext()
    : m_name("EventContext")
  {}
  
  const std::string&
  EventContext::name() const
  { return m_name; }

}
