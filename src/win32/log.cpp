#include "../log.h"
#include <windows.h>

namespace tk {
namespace log {

std::string nativeErrorString()
{
    DWORD errorCode = GetLastError();
    std::string result;
    if( errorCode != NO_ERROR ) {
        result.resize(1024);
        int num = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,0,errorCode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),&result[0],result.size(),0);
        result.resize(num);
        result = " native error was " + result; // + std::tostring(errorCode) 
    }
    return result;
}

}
}