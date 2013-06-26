#include "MemoWin32.h"

namespace tk {

 MemoImpl::MemoImpl(Window & parent, ControlParams const & params):
	NativeControlImpl(parent,params,L"EDIT",ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
	}
}

void MemoImpl::setText(die::NativeString const & text)
{
    auto p = text.wstr.find('\n');
    if( p == std::string::npos ) {
        NativeControlImpl::setText(text);
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
        NativeControlImpl::setText(textR);
    }
}

void MemoImpl::setReadOnly(bool readOnly)
{
    PostMessage(hWnd,EM_SETREADONLY,readOnly,0);
}

}
