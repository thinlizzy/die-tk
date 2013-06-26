/* 
 * File:   FileTreeView.cpp
 * Author: diego
 * 
 * Created on 4 de Dezembro de 2012, 23:01
 */

#include "FileTreeView.h"
#include <functional>
#include <vector>
#include <algorithm>

#include <fileutils.h>

namespace tk {
    
ItemProperties loadingItem{L"loading...",0};
    
FileTreeView::FileTreeView():
    treeView(0)
{}

FileTreeView::FileTreeView(TreeView & treeView):
    globMask("*"),
    treeView(&treeView),
    oldBeforeExpand(treeView.beforeExpand(std::bind(&FileTreeView::directoryExpand,this,std::placeholders::_1)))
{
}

// TODO change move operations to default and create move wrappers to treeView and oldBeforeExpand
FileTreeView::FileTreeView(FileTreeView && other):
    globMask(std::move(other.globMask)),
    baseDir(std::move(other.baseDir)),
    treeView(other.treeView),
    oldBeforeExpand(other.oldBeforeExpand)
{
    other.treeView = 0;
    other.oldBeforeExpand = nullptr;
}

FileTreeView & FileTreeView::operator=(FileTreeView && other)
{
    globMask = std::move(other.globMask);
    baseDir = std::move(other.baseDir);
    treeView = other.treeView;
    oldBeforeExpand = other.oldBeforeExpand;
    
    other.treeView = 0;
    other.oldBeforeExpand = nullptr;
    
    return *this;
}

FileTreeView::~FileTreeView()
{
    if( treeView ) {
        treeView->beforeExpand(oldBeforeExpand);
    }
}

FileTreeView & FileTreeView::setGlobMask(die::NativeString const & globMask)
{
    this->globMask = globMask;
    if( ! baseDir.empty() ) {
        build();
    }
    return *this;
}

FileTreeView & FileTreeView::setBaseDir(die::NativeString const & baseDir)
{
    this->baseDir = baseDir;
    build();
    return *this;
}

void populateFiles(fs::Path const & path, TreeView::Item baseItem) 
{
    std::vector<die::NativeString> directories;
    std::vector<die::NativeString> files;
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
        ItemProperties ip(directory,FileTreeView::imgDirectory);
        auto it = baseItem.addChild(ip);
        (*it).addChild(loadingItem);
    }
    
    for( auto const & filename : files ) {
        ItemProperties ip(filename,FileTreeView::imgFile);
        baseItem.addChild(ip);
    }
}

void FileTreeView::build()
{
    treeView->clear();
    auto path = fs::Path(baseDir).append(globMask);            
    populateFiles(path,treeView->root());    
}

fs::Path pathTo(TreeView const & treeView, TreeView::Item item)
{
    if( item == treeView.root() ) return fs::Path();
    
    return pathTo(treeView,treeView.getParent(item)).append(item.getProperties().text);
}

bool FileTreeView::directoryExpand(TreeView::Item item)
{
    if( oldBeforeExpand != nullptr && ! oldBeforeExpand(item) ) return false;
    
    auto it = item.begin();
    auto firstChild = *it;
    if( firstChild.getProperties().text == loadingItem.text ) {
        item.eraseChild(it);
        auto path = fs::Path(baseDir).append(pathTo(*treeView,item)).append(globMask);            
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
        result.fullPath = fs::Path(baseDir).append(pathTo(*treeView,item));
        result.isDirectory = item.getProperties().imageIndex == imgDirectory;
    }
    
    return result;
}

}
