#ifndef FILETREEVIEW_H_mc903jnvhd7234jdfa
#define	FILETREEVIEW_H_mc903jnvhd7234jdfa

#include "../../die-tk.h"
#include <string>

namespace tk {

// TODO specify an enum mask to filter files or directories
    
class FileTreeView {
    TreeView treeView;
    std::string baseDir;
    std::string globMask;
    TreeView::AllowItemChange oldBeforeExpand;
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
    
    explicit FileTreeView(TreeView treeView);
    FileTreeView(FileTreeView const &) = delete;
    FileTreeView & operator=(FileTreeView const &) = delete;
    ~FileTreeView();
    
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

