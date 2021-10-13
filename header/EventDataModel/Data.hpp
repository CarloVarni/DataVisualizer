
#ifndef DATA_H
#define DATA_H

#include <stdexcept>
#include <type_traits>
#include <vector>
#include <variant>

namespace EventDataModel {

  template< typename ... supported_t >
  class Data {
  public:
    using value_type = std::variant<supported_t...>;

    static_assert(not std::disjunction<std::is_same<std::string, supported_t>...>::value,
		  "Data Object does not support std::string");
    
    Data() = delete;

    template<typename value_t,
	     typename Enable = std::enable_if_t<std::disjunction<std::is_same<value_t, supported_t>...>::value>>
    Data(value_t value,
	 float weight = 1.);
    
    ~Data() = default;
    
    value_type value() const;
    float weight() const;
    
  private:
    const value_type m_value;
    const float m_weight;
  };
  
  /* ============================================================== */
  
  template<typename ... supported_t>
  std::ostream&
  operator<<(std::ostream& os, const Data<supported_t...>& data) {
    std::visit( [&] (const auto& arg) {
		  os << arg << " [w = " << data.weight() << "]";
      }, data.value());
    return os;
  }
  
  /* ============================================================== */
  
  template<typename ... supported_t>
  template<typename value_t,
	   typename Enable>
  Data<supported_t...>::Data(value_t value,
			     float weight)
    : m_value(value),
      m_weight(weight < 0 ?
	       throw std::invalid_argument("Invalid weight for data [< 0].")
	       : weight) {}
  
  template<typename ... supported_t>
  std::variant<supported_t...>
  Data<supported_t...>::value() const
  { return m_value; }
  
  template<typename ... supported_t>
  float
  Data<supported_t...>::weight() const
  { return m_weight; }


  using DataObject = Data<double, int, float, long, unsigned int, short, bool, unsigned short>;
  using DataObjectCollection = std::vector<DataObject>;
}

#endif
