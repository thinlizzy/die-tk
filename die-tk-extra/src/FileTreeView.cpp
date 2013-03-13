/* 
 * File:   FileTreeView.cpp
 * Author: diego
 * 
 * Created on 4 de Dezembro de 2012, 23:01
 */

#include "FileTreeView.h"
#include "../../../magic/fileutils/src/fileutils.h"
#include <functional>
#include <vector>
#include <algorithm>

namespace tk {
    
TreeView::ItemProperties loadingItem{"loading...",0};
    
FileTreeView::FileTreeView(std::shared_ptr<TreeView> treeView):
    treeView(treeView),
    globMask("*")
{
    // TODO stack callbacks (make beforeExpand and onChange return the old function object
    treeView->beforeExpand(std::bind(&FileTreeView::directoryExpand,this,std::placeholders::_1));
}

void FileTreeView::setGlobMask(std::string const & globMask)
{
    this->globMask = globMask;
    if( ! baseDir.empty() ) {
        build();
    }
}

void FileTreeView::setBaseDir(std::string const & baseDir)
{
    this->baseDir = baseDir;
    build();
}

void populateFiles(fs::Path const & path, TreeView::Item baseItem) 
{
    std::vector<std::string> directories;
    std::vector<std::string> files;
    for( fs::GlobIterator git(path); git != fs::GlobIterator(); ++git ) {
        fs::GlobFile & file = *git;
        if( file.isSpecialDirectory() ) continue;

        if( file.isDirectory() ) {
            directories.push_back(file.filename());
        } else {
            files.push_back(file.filename());
        }
    }
    
    for( auto const & directory : directories ) {
        TreeView::ItemProperties ip(directory,FileTreeView::imgDirectory);
        auto it = baseItem.addChild(ip);
        (*it).addChild(loadingItem);
    }
    
    for( auto const & filename : files ) {
        TreeView::ItemProperties ip(filename,FileTreeView::imgFile);
        baseItem.addChild(ip);
    }
}

void FileTreeView::build()
{
    treeView->clear();
    auto path = fs::Path(baseDir).append(globMask);            
    populateFiles(path,treeView->root());    
}

fs::Path pathTo(std::shared_ptr<TreeView> const & treeView, TreeView::Item item)
{
    if( item == treeView->root() ) return fs::Path();
    
    return pathTo(treeView,treeView->getParent(item)).append(item.getProperties().text);
}

bool FileTreeView::directoryExpand(TreeView::Item item)
{
    auto it = item.begin();
    auto firstChild = *it;
    if( firstChild.getProperties().text == loadingItem.text ) {
        item.eraseChild(it);
        auto path = fs::Path(baseDir).append(pathTo(treeView,item)).append(globMask);            
        populateFiles(path,item);
    }
    
    return true;
}

FileTreeView::File FileTreeView::getFile() const
{
    File result;
    auto it = treeView->selected();
    if( it ) {
        auto item = *it;
        result.filename = item.getProperties().text;
        result.fullPath = fs::Path(baseDir).append(pathTo(treeView,item));
        result.isDirectory = item.getProperties().imageIndex == imgDirectory;
    }
    
    return result;
}

}
