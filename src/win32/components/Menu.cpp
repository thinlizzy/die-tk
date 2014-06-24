#include "../../components/Menu.h"
#include "MenuWin32.h"

namespace tk {
    
Menu::Menu():
    impl(std::make_shared<MenuImpl>())
{
}

Menu::Menu(std::shared_ptr<MenuImpl> impl):
    impl(impl)
{
}

Menu::~Menu()
{}

MenuItem Menu::root()
{
    return impl->root();
}

void Menu::attach(Window & window)
{
    impl->attach(window.getImplPtr());
}

void Menu::detach()
{
    impl->detach();
}

WindowRef Menu::getAttachedWindow()
{
    return impl->getAttachedWindow();
}

// MenuItem

MenuItem::MenuItem(MenuImpl & menuImpl):
    itemImpl(std::make_shared<MenuItemImplRoot>(menuImpl.hMenu))
{
}
    
MenuItem::MenuItem(std::shared_ptr<MenuItemImpl> itemImpl):
    itemImpl(itemImpl)
{
}

MenuItemProperties MenuItem::getProperties() const
{
    return itemImpl->getProperties();
}

void MenuItem::setText(die::NativeString const & text)
{
    itemImpl->setText(text);
}

void MenuItem::setImage(image::Ptr img)
{
    itemImpl->setImage(img);
}

void MenuItem::setEnabled(bool enabled)
{
    itemImpl->setEnabled(enabled);
}

void MenuItem::setChecked(bool checked)
{
    itemImpl->setChecked(checked);
}

MenuItemIterator MenuItem::begin()
{
    return itemImpl->begin();
}

MenuItemIterator MenuItem::end()
{
    return itemImpl->end();
}

MenuItemIterator const MenuItem::begin() const
{
    return itemImpl->begin();
}

MenuItemIterator const MenuItem::end() const
{
    return itemImpl->end();
}

bool MenuItem::empty() const
{
    return itemImpl->empty();
}

MenuItemIterator MenuItem::addItem(MenuItemProperties const & properties)
{
    return itemImpl->addItem(properties);
}

MenuItemIterator MenuItem::addItem(die::NativeString const & text)
{
    return addItem(MenuItemProperties(text));
}

void MenuItem::eraseItem(MenuItemIterator it)
{
    itemImpl->eraseItem(it);
}

HandleOperation MenuItem::onClick(HandleOperation callback)
{
    return itemImpl->onClick(callback);
}

// MenuItemIterator

MenuItemIterator::MenuItemIterator(MenuItemIteratorImpl * iteratorImpl):
    iteratorImpl(iteratorImpl)
{
}

MenuItemIterator::MenuItemIterator(MenuItemIterator const & it):
    iteratorImpl(it.iteratorImpl)
{
}

MenuItemIterator & MenuItemIterator::operator=(MenuItemIterator const & it)
{
    iteratorImpl = it.iteratorImpl;
    return *this;
}

MenuItemIterator::MenuItemIterator()
{
}

MenuItemIterator::~MenuItemIterator()
{
}

MenuItem & MenuItemIterator::operator*()
{
    return iteratorImpl->currentItem();
}

MenuItem const & MenuItemIterator::operator*() const
{
    return iteratorImpl->currentItem();
}

MenuItem * MenuItemIterator::operator->()
{
    return & operator*();
}

MenuItem const * MenuItemIterator::operator->() const
{
    return & operator*();
}

MenuItemIterator & MenuItemIterator::operator++()
{
    iteratorImpl->advance();
    return *this;
}

MenuItemIterator const & MenuItemIterator::operator++() const
{
    iteratorImpl->advance();
    return *this;
}

MenuItemIterator MenuItemIterator::operator++(int) const
{
    auto result = *this;
    operator++();
    return result;
}

bool MenuItemIterator::operator==(MenuItemIterator const & it) const
{
    if( ! iteratorImpl ) return ! it.iteratorImpl;
    if( ! it.iteratorImpl ) return false;

    return *iteratorImpl == *it.iteratorImpl;
}

bool MenuItemIterator::operator!=(MenuItemIterator const & it) const
{
    return ! operator==(it);
}

MenuItemIterator::operator bool() const
{
    return iteratorImpl.operator  bool();
}

}
