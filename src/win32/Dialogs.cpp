#include "../Dialogs.h"
#include "WindowImplWin32.h"

namespace tk {
namespace dialog {

std::wstring ofnFilters(std::vector<SelectFileParams::Filter> const & filters)
{
    std::wstring result;
    for( auto const & filter : filters ) {
        result += filter.second.empty() ? filter.first.wstr : filter.second.wstr;
        result.push_back(0);
        result += filter.first.wstr;
        result.push_back(0);
    }
    
    result.push_back(0);
    return result;
}

wchar_t * ofnString(die::NativeString const & ns)
{
    return ns.empty() ? NULL : const_cast<wchar_t *>(ns.wstr.c_str());
}

DWORD ofnFlags(SelectFileParams const & params)
{
    DWORD result = 0;
    if( params.showHidden_ ) result |= 0x10000000;  // couldn't find it on mingw    
    return result;
}

WORD ofnLast(SelectFileParams const & params, wchar_t needle)
{
    auto p = params.filename_.wstr.rfind(needle);
    if( p == std::wstring::npos ) return 0;
    
    return p+1;
}

std::vector<die::NativeString> ofnFiles(wchar_t * files)
{
    std::vector<die::NativeString> result;
    wchar_t * end = files;
    while( *end ) { 
       wchar_t * beg = end;
       while( *end ) ++end;
       result.push_back(std::wstring(beg,end));
       ++end;
    }
    
    return result;
}

typedef WINBOOL WINAPI OpenSaveFunc(LPOPENFILENAMEW);

std::vector<die::NativeString> selectFiles(Window & owner, SelectFileParams const & params, OpenSaveFunc * operation, DWORD flags)
{
    OPENFILENAMEW ofn;
    ofn.lStructSize = sizeof(ofn);    
    ofn.hwndOwner = owner.getImpl().hWnd;
    std::wstring strFilters;
    if( params.filters_.empty() ) {
        ofn.lpstrFilter = NULL;
    } else {
        strFilters = ofnFilters(params.filters_);
        ofn.lpstrFilter = &strFilters[0];
    }
    ofn.lpstrCustomFilter = NULL;
    ofn.nFilterIndex = 0;
    std::wstring buffer = params.filename_.wstr;
    buffer.resize(8192);
    ofn.lpstrFile = &buffer[0];
    ofn.nMaxFile = buffer.size()-1;
    ofn.lpstrFileTitle = NULL;
    ofn.lpstrInitialDir = ofnString(params.path_);
    ofn.lpstrTitle = ofnString(params.title_);
    ofn.Flags = ofnFlags(params) | flags;
    ofn.nFileOffset = ofnLast(params,L'\\');
    ofn.nFileExtension = ofnLast(params,L'.');
    ofn.lpstrDefExt = NULL;

    BOOL ok = operation(&ofn);
    if( ok == 0 ) return std::vector<die::NativeString>();
    
    return ofnFiles(ofn.lpstrFile);
}
    

die::NativeString selectFile(Window & owner, SelectFileParams const & params)
{
    auto files = selectFiles(owner,params,&GetOpenFileNameW,0);
    if( files.empty() ) return die::NativeString();
    
    return files[0];
}

die::NativeString selectFileForSave(Window & owner, SelectFileParams const & params)
{
    auto files = selectFiles(owner,params,&GetSaveFileNameW,0);
    if( files.empty() ) return die::NativeString();
    
    return files[0];
}

std::vector<die::NativeString> selectFiles(Window & owner, SelectFileParams const & params)
{
    return selectFiles(owner,params,&GetOpenFileNameW,OFN_ALLOWMULTISELECT | OFN_EXPLORER);
}

}
}
    
