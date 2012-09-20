#include <iostream>

#define TRACE(x) std::cerr << x << '\n'

#define TRACE_MSG(mvalue,x) if( message == mvalue ) TRACE(x)

#ifdef TRACE_ALL
#define TRACE_M(x) TRACE(x)
#else
#define TRACE_M(x) 
#endif
