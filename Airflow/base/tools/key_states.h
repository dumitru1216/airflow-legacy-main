#pragma once
#include <atomic>
#include <wtypes.h>
#include <functional>

#include "utils_macro.h"

class c_key_states {
protected:
    std::atomic<short> array_states[256] = {};
public:

};

declare_feature_ptr(key_states);