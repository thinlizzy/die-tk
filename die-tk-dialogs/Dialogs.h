#ifndef DIALOGS_H_DIE_TK_2014_09_27_12_06
#define	DIALOGS_H_DIE_TK_2014_09_27_12_06

#include <vector>
#include "SelectFileParams.h"
#include "die-tk.h"

namespace tk {
    
class Window;
    
namespace dialog {

NativeString selectFile(Window & owner, SelectFileParams const & params = SelectFileParams());
std::vector<NativeString> selectFiles(Window & owner, SelectFileParams const & params = SelectFileParams());
NativeString selectFileForSave(Window & owner, SelectFileParams const & params = SelectFileParams());
void showMessage(Window & owner, NativeString const & message);

// TODO add messageBox routines (confirmation, warning, etc.) with return code

}
}

#endif
