#ifndef DIALOGS_H_DIE_TK_2014_09_27_12_06
#define	DIALOGS_H_DIE_TK_2014_09_27_12_06

#include "SelectFileParams.h"
#include <NativeString.h>
#include <vector>

namespace tk {
    
class Window;
    
namespace dialog {

die::NativeString selectFile(Window & owner, SelectFileParams const & params = SelectFileParams());
std::vector<die::NativeString> selectFiles(Window & owner, SelectFileParams const & params = SelectFileParams());
die::NativeString selectFileForSave(Window & owner, SelectFileParams const & params = SelectFileParams());
    
}
}

#endif
