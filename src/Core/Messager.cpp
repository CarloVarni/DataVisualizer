
#include <Messager.hpp>

namespace Core {

  Messager::Messager(const std::string& algo_name)
    : m_algoName(algo_name)
  {
    std::cout << "Creating Messager for algorithm " << algo_name << std::endl;
  }

  void
  Messager::info_message(const std::string& algoName,
			 const std::string& message) const
  { write_message("INFO", algoName, message); }
  
  void
  Messager::debug_message(const std::string& algoName,
			  const std::string& message) const
  { write_message("DEBUG", algoName, message); }
  
  void
  Messager::error_message(const std::string& algoName,
			  const std::string& message) const
  { write_message("ERROR", algoName, message); }
  
  void
  Messager::fatal_message(const std::string& algoName,
			  const std::string& message) const
  { write_message("FATAL", algoName, message); }
  
  void
  Messager::write_message(const std::string& level,
			  const std::string& algoName,
			  const std::string& message) const
  {
    std::cout << std::setw(65) << std::left << algoName << "     ";
    std::cout << std::setw(8) << level << "   ";
    std::cout << message << std::endl;
  }
  
}
