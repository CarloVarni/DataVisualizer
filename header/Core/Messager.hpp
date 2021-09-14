
#ifndef MESSAGER_H
#define MESSAGER_H 1

#include <iostream>
#include <string>
#include <iomanip>

namespace Core {
  enum MSG {INFO, DEBUG, ERROR, FATAL};
}

#define MSG( lvl, xmsg)							\
  std::cout << std::setw(65) << std::left << this->name() << "     ";	\
  std::cout << std::setw(8);						\
  if (lvl == Core::MSG::INFO) std::cout << "INFO" << "   ";	\
  else if (lvl == Core::MSG::DEBUG) std::cout << "DEBUG" << "   ";	\
  else if (lvl == Core::MSG::ERROR) std::cout << "ERROR" << "   ";	\
  else if (lvl == Core::MSG::FATAL) std::cout << "FATAL" << "   ";	\
  std::cout << xmsg << std::endl;					\
  if (lvl == Core::MSG::FATAL)				\
    throw std::runtime_error( xmsg )

      
#define MSG_INFO( xmsg ) MSG(Core::MSG::INFO, xmsg)	
#define MSG_DEBUG( xmsg ) MSG(Core::MSG::DEBUG, xmsg)	
#define MSG_ERROR( xmsg ) MSG(Core::MSG::ERROR, xmsg)
#define MSG_FATAL( xmsg ) MSG(Core::MSG::FATAL, xmsg)

#endif 
