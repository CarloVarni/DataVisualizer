
#ifndef MULTIDATA_H
#define MULTIDATA_H

#include <utility>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <sstream>

namespace EventDataModel {
  
  template<typename ... supported_t>
  class MultiData {
  public:
    using value_type = std::variant<supported_t...>;

    static_assert(not std::disjunction<std::is_same<std::string, supported_t>...>::value,
		  "MultiData Object does not support std::string!");
    
    MultiData() = delete;
    
    MultiData(const std::vector<std::string>& labels,
	      const std::vector<value_type>& data,
	      float weight = 1.);
    
    ~MultiData() = default;
    
    std::size_t size() const;
    float weight() const;
    bool hasLabel(const std::string&) const; 

    std::vector<std::string> labels() const;
    value_type value(const std::string&) const;
    
  private:
    const float m_weight;
    std::unordered_map<std::string, int> m_indexMap;
    std::vector<value_type> m_data;
  };
  
  /* ============================================================== */
  
  template<typename ... data_type>
  std::ostream&
  operator<<(std::ostream& os, const MultiData<data_type...>& data) {
    os << "MultiData object with " << data.size() << " entries [w = " << data.weight() << "]\n";
    
    const auto& labels = data.labels();
    for (const auto& label : labels)
      std::visit([&] (const auto& arg) {
		   os << "    `" << label << "` : " << arg << std::endl;
		 }, data.value(label));
    
    return os;
  }
  
  /* ============================================================== */
  
  template<typename ... supported_t>
  MultiData<supported_t ...>::MultiData(const std::vector<std::string>& labels,
					const std::vector<std::variant<supported_t...>>& data,
					float weight)
    : m_weight(weight)
  {
    if (labels.size() != data.size())
      throw std::runtime_error("MultiData error: Size of labels and data do not match");
    
    m_data.reserve(labels.size());
    
    for (auto i(0); i<labels.size(); i++) {
      const std::string& label = labels.at(i);
      if (m_indexMap.find(label) != m_indexMap.end())
	throw std::runtime_error("Already store data with same label: `" + label + "`");
      
      m_indexMap[label] = m_data.size();
      std::visit([=] (auto arg) {
		   m_data.push_back(arg);
		 }, data.at(i));
    }
  }
  
  template<typename ... supported_t>
  std::size_t
  MultiData<supported_t ...>::size() const { return m_indexMap.size(); }
  
  template<typename ... supported_t>
  float
  MultiData<supported_t ...>::weight() const { return m_weight; }
  
  template<typename ... supported_t>
  bool
  MultiData<supported_t ...>::hasLabel(const std::string& label) const
  {
    return m_indexMap.find(label) != m_indexMap.end();
  }
  
  template<typename ... supported_t>
  std::vector<std::string>
  MultiData<supported_t ...>::labels() const
  {
    std::vector<std::string> output;
    output.reserve(m_indexMap.size());
    
    for (const auto& label : m_indexMap)
      output.push_back(label.first);
    
    return output;
  }
  
  template<typename ... supported_t>
  std::variant<supported_t...>
  MultiData<supported_t ...>::value(const std::string& label) const
  {
    if (not hasLabel(label))
      throw std::runtime_error("MultiData object has not label `" + label + "`");
    
    return m_data.at(m_indexMap.at(label));
  }
  

  using MultiDataObject = MultiData<double, int, float, long, unsigned int, short, bool, unsigned short>;
  using MultiDataObjectCollection = std::vector<MultiDataObject>;
  
}

#endif
