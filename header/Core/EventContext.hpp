
#ifndef EVENT_CONTEXT_H
#define EVENT_CONTEXT_H 1

#include <IHolder.hpp>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>

namespace Core {

class EventContext {
public:
  EventContext();
  EventContext(const EventContext &) = delete;
  EventContext &operator=(const EventContext &) = delete;
  ~EventContext() = default;

  template <typename T>
  void add(const std::string &name, std::shared_ptr<T> &&object);

  template <typename T> const T *get(const std::string &name) const;

protected:
  const std::string &name() const;

private:
  const std::string m_name;
  std::unordered_map<std::string, std::shared_ptr<IHolder>> m_store;
};

/* ============================================================== */

template <typename T>
inline void EventContext::add(const std::string &name,
                              std::shared_ptr<T> &&object) {
  if (name.empty())
    throw std::invalid_argument(
        "Object can not have an empty name in order to be stored in memory");

  if (m_store.find(name) != m_store.end())
    throw std::runtime_error("Object with name `" + name +
                             "` already stored in memory");

  m_store.emplace(name, std::make_shared<HolderT<T>>(
                            std::forward<std::shared_ptr<T>>(object)));
}

template <typename T>
inline const T *EventContext::get(const std::string &name) const {
  if (m_store.find(name) == m_store.end())
    throw std::invalid_argument(
        "Object with name `" + name +
        "` is not in memory. Cannot be retrieved: " + name);

  const IHolder *holder = m_store.at(name).get();
  if (typeid(T) != holder->type())
    throw std::out_of_range("Type mismatch for object '" + name + "'");

  return reinterpret_cast<const HolderT<T> *>(holder)->value().get();
}
} // namespace Core

#endif
