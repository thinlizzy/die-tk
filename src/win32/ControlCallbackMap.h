#ifndef CONTROLCALLBACKMAP_H_DIE_TK_2014_01_27_25_54
#define	CONTROLCALLBACKMAP_H_DIE_TK_2014_01_27_25_54

#include <unordered_map>

namespace tk {

class NativeControlImpl;
    
template<typename T> using ControlCallbackMap = std::unordered_map<NativeControlImpl *, T>; 
    
}

#endif