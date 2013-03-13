#ifndef FILETREEVIEW_H_mc903jnvhd7234jdfa
#define	FILETREEVIEW_H_mc903jnvhd7234jdfa

#include "../../die-tk.h"
#include <string>

namespace tk {

// TODO this will be tested as a TreeView adapter. refactor in the future to allow user custom controls in the application handler
// do a thing like app.registerUserControl<FileTreeView,TreeView> that allows the creation of custom controls that delegate the callbacks

// TODO specify an enum mask to filter files or directories
    
class FileTreeView {
    std::shared_ptr<TreeView> treeView;
    std::string baseDir;
    std::string globMask;
public:
    class File {
    public:
        std::string filename;
        std::string fullPath;
        bool isDirectory;
    };
    
    enum ImageIndexes {
        imgFile,imgDirectory,
    };
    
    explicit FileTreeView(std::shared_ptr<TreeView> treeView);
    void setGlobMask(std::string const & globMask);
    void setBaseDir(std::string const & baseDir);
    // TODO void enableDirectories(boolean enabled);
    // TODO void enableFiles(boolean enabled);
    
    File getFile() const;
private:
    void build();
    bool directoryExpand(TreeView::Item item);
};

}

#endif	/* FILETREEVIEW_H */

