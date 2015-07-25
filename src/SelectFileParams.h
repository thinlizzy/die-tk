#ifndef SELECT_FILE_PARAMS_H_j34328fg8hjjgo048gsdf
#define SELECT_FILE_PARAMS_H_j34328fg8hjjgo048gsdf

#include "NativeString.h"
#include <vector>
#include <utility>

namespace tk {

class SelectFileParams {
public:
    typedef std::pair<NativeString,NativeString> Filter;
    
    NativeString title_;
    NativeString path_;
    NativeString filename_;
    std::vector<Filter> filters_;
    bool showHidden_;
    
    SelectFileParams():
        showHidden_(false)
    {}
    
    SelectFileParams & title(NativeString const & value) { title_ = value; return *this; }    
    SelectFileParams & path(NativeString const & value) { path_ = value; return *this; }
    SelectFileParams & filename(NativeString const & value) { filename_ = value; return *this; }
    
    SelectFileParams & showHidden(bool value = true) { showHidden_ = value; return *this; }
    
    SelectFileParams & filter(NativeString const & mask, NativeString const & text = NativeString())
    { 
        filters_.push_back(Filter(mask,text));
        return *this; 
    }
};

}

#endif
