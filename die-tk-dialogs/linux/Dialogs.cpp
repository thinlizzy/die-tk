#include "die-tk-dialogs/Dialogs.h"
#include <unistd.h>
#include <cstring>
#include <algorithm>
#include "libsofd.h"
#include "messagebox.h"
#include "die-tk/linux/ResourceManager.h"
#include "die-tk/linux/WindowImplX11.h"

// this implementation uses a fork of https://github.com/x42/sofd
// and it uses https://github.com/Eleobert/MessageBox-X11 as well

namespace tk {
namespace dialog {

std::vector<std::pair<NativeString,NativeString>> const * filters;

bool endsWith(std::string const & extension, char const * basename, int len) {
	return
		extension.size() <= len
		&& std::equal(extension.begin(),extension.end(),basename+len-extension.size());
}

int filterFunction(char const * basename) {
	auto len = std::strlen(basename);
	for( auto && filter : *filters ) {
		if( endsWith(filter.first.str,basename,len) ) return 1;
	}
	return 0;
}

NativeString selectFile(Window & owner, SelectFileParams const & params, char const * buttonLabel) {
	if( ! params.path_.empty() ) {
		x_fib_configure(0,params.path_.str.c_str());
	}
	if( params.title_.empty() ) {
		x_fib_configure(1,buttonLabel);
	} else {
		x_fib_configure(1,params.title_.str.c_str());
	}
	if( params.showHidden_ ) {
		x_fib_cfg_buttons(1,1);
	}

	if( ! params.filters_.empty() ) {
		// x_fib_cfg_buttons(3,1);
		filters = &params.filters_;
		x_fib_cfg_filter_callback(&filterFunction);
	}

	auto dpy = ResourceManagerSingleton().instance().dpy;
	auto & impl = owner.getImpl();

	// TODO pass around buttonLabel
	x_fib_show(dpy,impl.windowId,buttonLabel,0,0);

	// poll until the dialog is closed
	char * filename = nullptr;
	for(;;) {
		XEvent event;
		while( XPending(dpy) > 0 ) {
			XNextEvent(dpy,&event);
			if( x_fib_handle_events(dpy, &event) ) {
				if( x_fib_status() > 0 ) {
					filename = x_fib_filename();
					break;
				}
			}
		}
		if( x_fib_status() ) {
			break;
		}
		usleep(80000);
	}

	// assign the filename to the resulting native string
	NativeString result;
	if( filename ) {
		result.str = std::string(filename);
	}
	free(filename);
	return result;
}

// implementations

NativeString selectFile(Window & owner, SelectFileParams const & params) {
	return selectFile(owner,params,"Open");
}

NativeString selectFileForSave(Window & owner, SelectFileParams const & params) {
	return selectFile(owner,params,"Save");
}

std::vector<NativeString> selectFiles(Window & owner, SelectFileParams const & params) {
	// TODO implement multiple files selection
	auto file = selectFile(owner,params,"Open");
	std::vector<NativeString> result;
	if( !file.empty() ) {
		result.push_back(file);
	}
	return result;
}

void showMessage(Window & owner, NativeString const & message) {
	// TODO see how to use owner.getImpl() as the transient window, like sofd does
	auto ok = L"OK";
	Button button;
	button.label = const_cast<wchar_t *>(ok);
	button.result = 1;
	// TODO test the conversion to wchar_t (might want to refactor Messagebox out of wchar_t)
	wchar_t * text = new wchar_t[message.str.size() + 1];
	std::copy(message.str.begin(),message.str.end(),text);
	Messagebox("Alert",text,&button,1);
}

}
}
