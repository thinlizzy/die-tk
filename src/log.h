#ifndef LOG_H_GUARD_dffd9fds9kkdmvfsf888888h5f857
#define LOG_H_GUARD_dffd9fds9kkdmvfsf888888h5f857

#include <iostream>
#include <string>

// TODO use a variadic macro that outputs line, function and forwards __VA_ARGS__ to logging functions

namespace tk {
namespace log {

std::string nativeErrorString();
    
template<typename T> 
void error(T argument)
{
    std::cerr << argument 
    	<< nativeErrorString()
		<< std::endl;
}
    
template<typename T, typename... V> 
void error(T argument, V... rest)
{
    std::cerr << argument;
    error(rest...);
}
    
template<typename T> 
void info(T argument)
{
    std::cerr << argument << std::endl;
}
    
template<typename T, typename... V> 
void info(T argument, V... rest)
{
    std::cerr << argument;
    info(rest...);
}

template<typename T, typename... V>
void debug(T argument, V... rest)
{
	// TODO use DEFINES for logging levels
}

}    
}

#endif
