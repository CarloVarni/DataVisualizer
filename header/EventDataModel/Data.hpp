
#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdexcept>
#include <type_traits>

template<typename value_t>
class Data {
public:
  using value_type = value_t;

  Data() = delete;
  
  template<typename = std::enable_if_t<not std::is_convertible<value_type, std::string>::value>>
  Data(const value_type& value,
       float weight = 1.);
  
  ~Data() = default;

  value_type value() const;
  float weight() const;
  
private:
  const value_type m_value;
  const float m_weight;
};

/* ============================================================== */

template<typename value_t>
std::ostream&
operator<<(std::ostream& os, const Data<value_t>& data) {
  os << data.value() << " [w = " << data.weight() << "]";
  return os;
}
  
/* ============================================================== */

template<typename value_t>
template<typename>
Data<value_t>::Data(const value_t& value,
		    float weight)
  : m_value(value),
    m_weight(weight < 0 ?
	     throw std::invalid_argument("Invalid weight for data [< 0].")
	     : weight) {}

template<typename value_t>
value_t
Data<value_t>::value() const
{ return m_value; }

template<typename value_t>
float
Data<value_t>::weight() const
{ return m_weight; }

#endif
