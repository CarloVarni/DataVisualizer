
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

}

namespace {
#define MSG_INFO( xmsg )				\
  msg().info_message( this->name(), xmsg )
  
#define MSG_DEBUG( xmsg )				\
  msg().debug_message( this->name(), xmsg )
  
#define MSG_ERROR( xmsg )				\
  this->msg().error_message( this->name(), xmsg )

#define MSG_FATAL( xmsg )				\
  this->msg().fatal_message( this->name(), xmsg )
}

#endif 
