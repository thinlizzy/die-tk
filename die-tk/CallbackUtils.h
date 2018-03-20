#ifndef CALLBACK_UTILS_H_jf435389fgshh45bdjz3428238fdsj
#define CALLBACK_UTILS_H_jf435389fgshh45bdjz3428238fdsj

#include <unordered_map>
#include <type_traits>
#include "util/optional.h"

namespace tk {

template<typename T, typename U> using CallbackMap = std::unordered_map<T, U>; 
    
template<typename T, typename CbType>
CbType setCallback(T * const impl, CallbackMap<T *,CbType> & map, CbType callback)
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

template<typename T, typename CbType>
CbType fetchCallback(T * const impl, CallbackMap<T *,CbType> & map)
{
    auto it = map.find(impl);
    if( it == map.end() ) return nullptr;
    
    return it->second;
}

template<typename T, typename CbType>
void removeFromCb(T * impl, CallbackMap<T *,CbType> & map)
{
    map.erase(impl);
}

// find and execute a callback with result type
// returns empty optional if callback was not found
template<typename T, typename CbType, typename... Args>
optional<std::result_of_t<CbType(Args...)>>
    findExec(T * const impl, CallbackMap<T *,CbType> & map, Args &&... args) 
{
    auto it = map.find(impl);
    if( it == map.end() ) return optional<typename std::result_of<CbType(Args...)>::type>();
    
    auto & callback = it->second;
    return callback(std::forward<Args>(args)...);
}


// find and execute a callback ignoring the result type
// returns false if callback was not found
// TODO it is the same function above, but with a return type void. see how to specialize
template<typename T, typename CbType, typename... Args>
bool executeCallback(T * const impl, CallbackMap<T *,CbType> & map, Args &&... args)
{
    auto it = map.find(impl);
    if( it == map.end() ) return false;
    
    auto & callback = it->second;
    callback(std::forward<Args>(args)...);
    return true;
}

}

#endif
