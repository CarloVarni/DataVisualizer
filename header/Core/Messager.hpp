
#ifndef MESSAGER_H
#define MESSAGER_H 1

#include <iostream>
#include <string>
#include <iomanip>

namespace Core {

  class Messager {
  public:
    enum LEVEL {INFO, DEBUG, ERROR, FATAL};

    Messager() = delete;
    Messager(const std::string& algo_name);
    Messager(const Messager&) = delete;
    Messager& operator=(const Messager&) = delete;
    ~Messager() = default;

    void info_message(const std::string& algoName,
		      const std::string& message) const;
    void debug_message(const std::string& algoName,
		       const std::string& message) const;
    void error_message(const std::string& algoName,
		       const std::string& message) const;
    void fatal_message(const std::string& algoName,
		       const std::string& message) const;

  private:
    void write_message(const std::string& level,
		       const std::string& algoName,
		       const std::string& message) const;
    
  private:
    const std::string m_algoName;
  };
  
#define MSG( lvl, xmsg)							\
  std::cout << std::setw(65) << std::left << this->name() << "     ";	\
  std::cout << std::setw(8);						\
  if (lvl == Core::Messager::LEVEL::INFO) std::cout << "INFO" << "   ";	\
  else if (lvl == Core::Messager::LEVEL::DEBUG) std::cout << "DEBUG" << "   "; \
  else if (lvl == Core::Messager::LEVEL::ERROR) std::cout << "ERROR" << "   "; \
  else if (lvl == Core::Messager::LEVEL::FATAL) std::cout << "FATAL" << "   "; \
  std::cout << xmsg << std::endl;					\
  if (lvl == Core::Messager::LEVEL::FATAL)				\
    throw std::runtime_error( xmsg )
  
      
#define MSG_INFO( xmsg ) MSG(Core::Messager::LEVEL::INFO, xmsg)	
#define MSG_DEBUG( xmsg ) MSG(Core::Messager::LEVEL::DEBUG, xmsg)	
#define MSG_ERROR( xmsg ) MSG(Core::Messager::LEVEL::ERROR, xmsg)
#define MSG_FATAL( xmsg ) MSG(Core::Messager::LEVEL::FATAL, xmsg)
      
      }

#endif 
