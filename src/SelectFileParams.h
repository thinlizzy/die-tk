#ifndef SELECT_FILE_PARAMS_H_j34328fg8hjjgo048gsdf
#define SELECT_FILE_PARAMS_H_j34328fg8hjjgo048gsdf

#include <NativeString.h>
#include <vector>
#include <utility>

enum class SelectFile { save,load, };

class SelectFileParams {
public:
    typedef std::pair<die::NativeString,die::NativeString> Filter;
    
    die::NativeString title_;
    die::NativeString path_;
    die::NativeString filename_;
    std::vector<Filter> filters_;
    bool multiSelect_;
    bool showHidden_;
    
    SelectFileParams():
        multiSelect_(false),
        showHidden_(false)
    {}
    
    SelectFileParams & title(die::NativeString const & value) { title_ = value; return *this; }    
    SelectFileParams & path(die::NativeString const & value) { path_ = value; return *this; }
    SelectFileParams & filename(die::NativeString const & value) { filename_ = value; return *this; }
    
    SelectFileParams & multiSelect(bool value = true) { multiSelect_ = value; return *this; }
    SelectFileParams & showHidden(bool value = true) { showHidden_ = value; return *this; }
    
    SelectFileParams & filter(die::NativeString const & mask, die::NativeString const & text = die::NativeString())
    { 
        filters_.push_back(Filter(mask,text));
        return *this; 
    }
};

#endif