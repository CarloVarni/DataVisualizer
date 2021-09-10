
#ifndef EVENT_CONTEXT_H
#define EVENT_CONTEXT_H 1

#include <IHolder.hpp>
#include <stdexcept>
#include <string>
#include <memory>
#include <utility>
#include <unordered_map>

namespace Core {

  class EventContext {
  public:
    EventContext() = default;
    EventContext(const EventContext&) = delete;
    EventContext& operator=(const EventContext&) = delete;
    ~EventContext() = default;

    template <typename T>
    void add(const std::string& name, T&& object);
    
    template <typename T>
    const T& get(const std::string& name) const;
    
  private:
    std::unordered_map<std::string,
		       std::unique_ptr<IHolder>> m_store;
  };

  /* ============================================================== */
  
  template<typename T>
  inline
  void
  EventContext::add(const std::string& name,
		    T&& object)
  {
    if (name.empty()) 
      throw std::invalid_argument("Object can not have an empty name in order to be stored in memory");
    
    if (m_store.find(name) != m_store.end())
      throw std::runtime_error("Object with name `" + name + "` already stored in memory");

    m_store.emplace(name,
		    std::make_unique<HolderT<T>>(std::forward<T>(object)));
  }


  template<typename T>
  inline
  const T&
  EventContext::get(const std::string& name) const
  {
    if (m_store.find(name) == m_store.end())
      throw std::invalid_argument("Object with name `" + name + "` is not in memory. Cannot be retrieved.");

    const IHolder* holder = m_store.at(name).get();
    if (typeid(T) != holder->type()) 
      throw std::out_of_range("Type mismatch for object '" + name + "'");

    return reinterpret_cast<const HolderT<T>*>(holder)->value;
  }
}

#endif 