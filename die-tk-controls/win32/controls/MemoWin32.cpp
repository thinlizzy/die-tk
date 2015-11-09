#include "MemoWin32.h"
#include <memory>

namespace tk {

 MemoImpl::MemoImpl(HWND parentHwnd, ControlParams const & params):
	EditImpl(parentHwnd,params,ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL),
    linesList(*this)
{
}

MemoImpl * MemoImpl::clone() const
{
    auto result = std::make_unique<MemoImpl>(getParentHandle(),getControlData());
    cloneInto(*result);
    return result.release();
}

void MemoImpl::setText(NativeString const & text)
{
    auto p = text.wstr.find('\n');
    if( p == std::string::npos ) {
        EditImpl::setText(text);
        linesList.update(text);
    } else {
        // replace \n with \r\n
        auto textR = text.wstr.substr(0,p) + L"\r\n";
        for(;;) {
            ++p;
            auto p2 = text.wstr.find(L'\n',p);
            if( p2 == std::string::npos ) break;
            textR += text.wstr.substr(p,p2-p) + L"\r\n";
            p = p2;
        }
        textR += text.wstr.substr(p);
        EditImpl::setText(textR);
        linesList.update(textR);
    }
}

NativeControlStringList & MemoImpl::lines()
{
    return linesList;
}

void MemoImpl::scrollTo(unsigned lines)
{
    PostMessageW(hWnd,EM_LINESCROLL,0,lines);
}

}
