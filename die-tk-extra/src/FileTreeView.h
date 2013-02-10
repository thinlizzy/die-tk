#ifndef FILETREEVIEW_H_mc903jnvhd7234jdfa
#define	FILETREEVIEW_H_mc903jnvhd7234jdfa

#include "../../die-tk.h"

namespace tk {

// TODO this will be tested as a TreeView adapter. refactor in the future to allow user custom controls in the application handler
// do a thing like app.registerUserControl<FileTreeView,TreeView> that allows the creation of custom controls that delegate the callbacks
    
class FileTreeView {
    std::shared_ptr<TreeView> treeView;
public:
    explicit FileTreeView(std::shared_ptr<TreeView> treeView, std::string const & baseDir = std::string());
private:

};

}

#endif	/* FILETREEVIEW_H */

