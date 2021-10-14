
#ifndef IHOLDER_H
#define IHOLDER_H

#include "iostream"
#include <memory>
#include <typeinfo>
namespace Core {

class IHolder {
public:
  virtual ~IHolder() = default;
  virtual const std::type_info &type() const = 0;
};

template <typename T,
          typename Enable =
              std::enable_if_t<std::is_nothrow_move_constructible<T>::value>>
class HolderT : public IHolder {
public:
  HolderT(std::shared_ptr<T> v);

  const std::type_info &type() const;

  const std::shared_ptr<T> &value() const;

private:
  std::shared_ptr<T> m_value;
};

/* ============================================================== */

template <typename T, typename Enable>
HolderT<T, Enable>::HolderT(std::shared_ptr<T> v) : m_value(std::move(v)) {}

template <typename T, typename Enable>
const std::type_info &HolderT<T, Enable>::type() const {
  return typeid(T);
}

template <typename T, typename Enable>
const std::shared_ptr<T> &HolderT<T, Enable>::value() const {
  return m_value;
}

} // namespace Core

#endif
