#ifndef FILETREEVIEW_H_mc903jnvhd7234jdfa
#define	FILETREEVIEW_H_mc903jnvhd7234jdfa

#include <die-tk-controls.h>
#include <NativeString.h>

namespace tk {

// TODO specify an enum mask to filter files or directories
    
class FileTreeView {
    die::NativeString globMask;
    die::NativeString baseDir;
    TreeView * treeView;
    TreeView::AllowItemChange oldBeforeExpand;
public:
    class File {
    public:
        die::NativeString filename;
        die::NativeString fullPath;
        bool isDirectory;
    };
    
    enum ImageIndexes {
        imgFile,imgDirectory,
    };
    
    FileTreeView();
    explicit FileTreeView(TreeView & treeView);
    FileTreeView(FileTreeView const &) = delete;
    FileTreeView & operator=(FileTreeView const &) = delete;
    FileTreeView(FileTreeView && other);
    FileTreeView & operator=(FileTreeView && other);
    ~FileTreeView();
    
    FileTreeView & setBaseDir(die::NativeString const & baseDir);
    FileTreeView & setGlobMask(die::NativeString const & globMask);
    // TODO void enableDirectories(boolean enabled);
    // TODO void enableFiles(boolean enabled);
    
    File getFile() const;
private:
    void build();
    bool directoryExpand(TreeView::Item item);
};

}

#endif	/* FILETREEVIEW_H */

