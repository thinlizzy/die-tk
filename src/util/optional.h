#ifndef OPTIONAL_H_jjj54444409vgfdgkj234
#define OPTIONAL_H_jjj54444409vgfdgkj234

// this header picks the available std::optional implementation and wraps it into the tk namespace

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L

#include <optional>

namespace tk {

template<typename T>
using optional = std::optional<T>;
    
}

// TODO: instead test a macro to make sure whether experimental/optional is available
#elif __cplusplus >= 201402L

#include <experimental/optional>

namespace tk {

template<typename T>
using optional = std::experimental::optional<T>;
    
}

#else

namespace tk {

// includes Krzemienski's standalone optional class
namespace private {
#include "optional.hpp"
}

template<typename T>
using optional = private::optional<T>;
    
}

#endif

#endif
