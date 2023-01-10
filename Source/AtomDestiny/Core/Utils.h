#pragma once

#include <type_traits>

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>

#define NOT_NULLPTR_CALL(value, methodName, ...) \
    static_assert(std::is_pointer_v<decltype(value)>, "Type should be a pointer"); \
    if (value != nullptr) { \
        value->methodName(__VA_ARGS__); \
    } \
