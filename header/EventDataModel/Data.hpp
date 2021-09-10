
#ifndef DATA_H
#define DATA_H

#include <string>
#include <stdexcept>
#include <type_traits>

namespace EventDataModel {

  template<typename value_t,
	   typename Enable = std::enable_if_t<not std::is_convertible<value_t, std::string>::value>>
  class Data {
  public:
    using value_type = value_t;
    
    Data() = delete;
    
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
  
  template<typename value_t,
	   typename Enable>
  std::ostream&
  operator<<(std::ostream& os, const Data<value_t, Enable>& data) {
    os << data.value() << " [w = " << data.weight() << "]";
    return os;
  }
  
  /* ============================================================== */
  
  template<typename value_t,
	   typename Enable>
  Data<value_t, Enable>::Data(const value_t& value,
			      float weight)
    : m_value(value),
      m_weight(weight < 0 ?
	       throw std::invalid_argument("Invalid weight for data [< 0].")
	       : weight) {}
  
  template<typename value_t,
	   typename Enable>
  value_t
  Data<value_t, Enable>::value() const
  { return m_value; }
  
  template<typename value_t,
	   typename Enable>
  float
  Data<value_t, Enable>::weight() const
  { return m_weight; }
  
}

#endif
