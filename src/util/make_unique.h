#ifndef MAKE_UNIQUE_H_kkkkkkk43248dsfh3432hgbgdf77hbcc
#define MAKE_UNIQUE_H_kkkkkkk43248dsfh3432hgbgdf77hbcc

#include <memory>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif