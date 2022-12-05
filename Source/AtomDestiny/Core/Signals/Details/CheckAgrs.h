#pragma once

#include <functional>

namespace AtomDestiny::Details
{
    template <typename T>
    struct GetArguments : GetArguments<decltype(&T::operator())>
    {
    };

    template <typename T, typename... Args>
    struct GetArguments<T (*)(Args ...)> : std::integral_constant<unsigned, sizeof...(Args)>
    {
    };

    template <typename T, typename C, typename... Args>
    struct GetArguments<T (C::*)(Args ...)> : std::integral_constant<unsigned, sizeof...(Args)>
    {
    };

    template <typename T, typename C, typename... Args>
    struct GetArguments<T (C::*)(Args ...) const> : std::integral_constant<unsigned, sizeof...(Args)>
    {
    };

    // bindings
    template <int>
    struct CheckArgs
    {
        template <typename T, typename Slot>
        void Connect(const T& slotObj, Slot&& slot) = delete; // specially not implemented
    };

    template <>
    struct CheckArgs<0>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj);
        }
    };

    template <>
    struct CheckArgs<1>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1);
        }
    };

    template <>
    struct CheckArgs<2>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2);
        }
    };

    template <>
    struct CheckArgs<3>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3);
        }
    };

    template <>
    struct CheckArgs<4>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4);
        }
    };

    template <>
    struct CheckArgs<5>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5);
        }
    };

    template <>
    struct CheckArgs<6>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6);
        }
    };

    template <>
    struct CheckArgs<7>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7);
        }
    };

    template <>
    struct CheckArgs<8>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7, std::placeholders::_8);
        }
    };

    template <>
    struct CheckArgs<9>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7, std::placeholders::_8,
                             std::placeholders::_9);
        }
    };

    template <>
    struct CheckArgs<10>
    {
        template <typename T, typename Slot>
        auto Connect(const T& slotObj, Slot&& slot)
        {
            return std::bind(std::forward<Slot>(slot), slotObj, std::placeholders::_1, std::placeholders::_2,
                             std::placeholders::_3, std::placeholders::_4, std::placeholders::_5,
                             std::placeholders::_6, std::placeholders::_7, std::placeholders::_8, std::placeholders::_9,
                             std::placeholders::_10);
        }
    };
} // namespace AtomDestiny::Details
