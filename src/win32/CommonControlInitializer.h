#ifndef COMMON_CONTROLS_INIT_H_jfgds59593lkn93hv
#define COMMON_CONTROLS_INIT_H_jfgds59593lkn93hv

namespace tk {

class CommonControlInitializerHelper {
public:
    CommonControlInitializerHelper(unsigned long dwICC);
};

template<unsigned long ICC>
class CommonControlInitializer {
protected:
    CommonControlInitializer()
    {
        static CommonControlInitializerHelper init(ICC);
    };
};

}

#endif
