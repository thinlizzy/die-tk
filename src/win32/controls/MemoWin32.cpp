#include "MemoWin32.h"

namespace tk {

 MemoImpl::MemoImpl(HWND parent_hWnd, ControlParams const & params):
	NativeControlImpl(parent_hWnd,params,"EDIT",ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL)
{
	if( ! params.text_.empty() ) {
		setText(params.text_);
	}
}

std::string MemoImpl::getText() const
{
    return NativeControlImpl::getText();
}

void MemoImpl::setText(std::string const & text)
{
    auto p = text.find('\n');
    if( p == std::string::npos ) {
        NativeControlImpl::setText(text);
    } else {
        // replace \n with \r\n
        auto textR = text.substr(0,p) + "\r\n";
        for(;;) {
            ++p;
            auto p2 = text.find('\n',p);
            if( p2 == std::string::npos ) break;
            textR += text.substr(p,p2-p) + "\r\n";
            p = p2;
        }
        textR += text.substr(p);
        NativeControlImpl::setText(textR);
    }
}

void MemoImpl::setReadOnly(bool readOnly)
{
    PostMessage(hWnd,EM_SETREADONLY,readOnly,0);
}


}
