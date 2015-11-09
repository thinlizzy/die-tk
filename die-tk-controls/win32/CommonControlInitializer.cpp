#include "CommonControlInitializer.h"

#define _WIN32_IE 0x0400
#include <windows.h>
#include <commctrl.h>

#include <iostream>

namespace tk {

CommonControlInitializerHelper::CommonControlInitializerHelper(unsigned long dwICC)
{
    INITCOMMONCONTROLSEX initccsex = { sizeof(INITCOMMONCONTROLSEX) , dwICC };
    auto result = InitCommonControlsEx(&initccsex);
    if( ! result ) {
        // TODO log
        std::cerr << "InitCommonControlsEx failed for " << dwICC << std::endl;
    }
};

}
