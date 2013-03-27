#ifndef CALLBACK_UTILS_H_jf435389fgshh45bdjz3428238fdsj
#define CALLBACK_UTILS_H_jf435389fgshh45bdjz3428238fdsj

#include <unordered_map>
#include "controls/NativeControlWin32.h"

namespace tk {

template<typename T> using ControlCallbackMap = std::unordered_map<NativeControlImpl *, T>; 
    
template<typename CbType>
CbType setCallback(NativeControlImpl * const impl, ControlCallbackMap<CbType> & map, CbType callback)
{
    auto it = map.find(impl);
    
    if( it == map.end() ) {
        if( callback != nullptr ) {
            map.insert(it,std::make_pair(impl,callback));
        }
        
        return nullptr;        
    } 

    // already have a callback. remove or replace
    CbType result = it->second;
    if( callback == nullptr ) {
        map.erase(it);            
    } else {
        it->second = callback;
    }
    
    return result;
}

template<typename CbType>
CbType fetchCallback(NativeControlImpl * const impl, ControlCallbackMap<CbType> & map)
{
    auto it = map.find(impl);
    if( it == map.end() ) return nullptr;
    
    return it->second;
}

template<typename CbType>
void removeFromCb(NativeControlImpl * impl, ControlCallbackMap<CbType> & map)
{
    auto it = map.find(impl);
    if( it != map.end() ) map.erase(it);    
}

// find and execute a callback ignoring the result type
// returns false if callback was not found
template<typename CbType, typename... Args>
bool findExec(NativeControlImpl * const impl, ControlCallbackMap<CbType> & map, Args &&... args)
{
    auto it = map.find(impl);
    if( it == map.end() ) return false;
    
    auto & callback = it->second;
    callback(std::forward<Args>(args)...);
    return true;
}

// find and execute a callback with result type
// returns empty optional if callback was not found
// TODO deduce R from CbType
template<typename R, typename CbType, typename... Args>
optional<R> findExec(NativeControlImpl * const impl, ControlCallbackMap<CbType> & map, Args &&... args)
{
    auto it = map.find(impl);
    if( it == map.end() ) return optional<R>();
    
    auto & callback = it->second;
    return callback(std::forward<Args>(args)...);
}

}

#endif
