#pragma once

#include <vector>
#include <algorithm>
#include <utility>

#include "Details/CheckAgrs.h"

#define signals
#define slots
#define emit

namespace AtomDestiny
{
    // signal - slot control entity
    class Connector;

    struct ISignal
    {
        virtual ~ISignal() = default;
        virtual void Drop(void* object) = 0;
    };

    namespace Details
    {
        using ObjectPointer = void*;

        struct ConnectorForwarder
        {
            template <typename Object>
            static void Disconnect(const ISignal* signal, const Object& object);
        };
        
    }  // namespace details

    class IConnectable
    {
    protected:
        ~IConnectable()
        {
            for (const auto signal : m_signals)
            {
                if (signal != nullptr)
                    Details::ConnectorForwarder::Disconnect(signal, this);
            }
        }

    private:
        std::vector<ISignal*> m_signals;
        friend class Connector;
    };

    ///
    /// Base pseudo signal
    ///
    template <typename T>
    class Signal;

    ///
    /// Signals needed specializations
    ///
    template <typename Return, typename... InArgs>
    class Signal<Return(InArgs...)> final : public ISignal
    {
    public:
        using Argument = std::function<Return(InArgs...)>;
        using Signature = Return(InArgs...);
        using Slots = std::vector<std::pair<Details::ObjectPointer, Argument>>;

        ///
        /// Generates signal.
        ///
        template <typename... Args>
        void operator()(Args&&... args) const
        {
            for ([[maybe_unused]] auto& [obj, func] : m_slots)
            {
                (void)obj;
                
                if (func)
                    func(std::forward<Args>(args)...);
            }
        }

        Signal() = default;
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;

        Signal(Signal&& signal) noexcept
        : m_slots(std::move(signal.m_slots))
        {
            signal.m_slots.clear();
        }

        Signal& operator=(Signal&& signal) noexcept
        {
            m_slots = std::move(signal.m_slots);
            signal.m_slots.clear();

            return *this;
        }

        virtual ~Signal() override
        {
            (*this) = nullptr;
        }

    private:
        template <typename T>
        auto& Add(T&& s, Details::ObjectPointer obj = nullptr)
        {
            Argument arg = std::forward<T>(s);

            if (!arg)
                return *this;

            m_slots.push_back(std::make_pair(obj, std::move(arg)));
            return *this;
        }

        // clears all signal slots
        auto& operator=(const void* ptr)
        {
            if (ptr == nullptr)
                m_slots.clear();

            return *this;
        }

        std::size_t Size() const noexcept
        {
            return m_slots.size();
        }

        Slots& Content() noexcept
        {
            return m_slots;
        }

        const Slots& Content() const noexcept
        {
            return m_slots;
        }

        virtual void Drop(void* object) override
        {
            std::vector<typename Slots::iterator> iterators;

            for (auto iterator = m_slots.begin(); iterator != m_slots.end(); ++iterator)
            {
                if (iterator->first == Details::ObjectPointer{ object })
                    iterators.push_back(iterator);
            }

            for (auto& iter : iterators)
                m_slots.erase(iter);
        }

        // all connected slots
        Slots m_slots;

        friend class Connector;

        template <typename T>
        friend class Signal;
    };

    ///
    /// Signal for function prototype
    ///
    template <typename T>
    class Signal<std::function<T>> final : public ISignal
    {
    public:
        using Argument = std::function<T>;
        using Signature = T;
        using Slots = std::vector<std::pair<Details::ObjectPointer, Argument>>;

        ///
        /// @brief Generates signal.
        /// @param args Any count of template parameters.
        ///
        template <typename... Args>
        void operator()(Args&&... args) const
        {
            m_signal(std::forward<Args>(args)...);
        }

        // creation
        Signal() = default;
        Signal(const Signal&) = delete;
        Signal& operator=(const Signal&) = delete;

        Signal(Signal&& signal) noexcept
        : m_signal(std::move(signal.m_signal))
        {
        }

        Signal& operator=(Signal&& signal) noexcept
        {
            m_signal = std::move(signal.m_signal);
            return *this;
        }

        virtual ~Signal() override
        {
            (*this) = nullptr;
        }

    private:
        // adds slot to signal
        template <typename U>
        auto& Add(U&& s, Details::ObjectPointer obj = nullptr)
        {
            m_signal.Add(std::forward<U>(s), obj);
            return *this;
        }

        // clears all slots
        auto& operator=(void* ptr)
        {
            m_signal = ptr;
            return *this;
        }

        std::size_t Size() const noexcept
        {
            return m_signal.Size();
        }

        Slots& Content() noexcept
        {
            return m_signal.Content();
        }

        const Slots& Content() const noexcept
        {
            return m_signal.Content();
        }

        virtual void Drop(void* object) override
        {
            m_signal.Drop(object);
        }

        Signal<Signature> m_signal;
        friend class Connector;
    };
    
    ///
    /// Signal - slot connection entity
    ///
    class Connector
    {
        template <typename Object>
        static Details::ObjectPointer CheckConnection(const ISignal* signal, const Object& object, std::true_type)
        {
            IConnectable* connectable = reinterpret_cast<IConnectable*>(object);
            connectable->m_signals.push_back(const_cast<ISignal*>(signal));

            return Details::ObjectPointer{connectable};
        }

        template <typename Object>
        static Details::ObjectPointer CheckConnection(const ISignal*, const Object& object, std::false_type)
        {
            return Details::ObjectPointer{object};
        }

    public:
        explicit Connector() = delete;
        Connector(const Connector&) = delete;
        Connector& operator=(const Connector&) = delete;
        Connector(Connector&&) = delete;
        Connector& operator=(Connector&&) = delete;

        ~Connector() = default;

        ///
        /// Connects signal pointer with lambda or function.
        ///
        template <template <typename> typename Signal, typename T>
        static void Connect(const Signal<T>* signal, typename Signal<T>::Argument slot)
        {
            const_cast<Signal<T>*>(signal)->Add(slot);
        }

        ///
        /// Connects const signal pointer with objects method.
        ///
        template <template <typename> typename Signal, typename T, typename Object, typename Slot>
        static void Connect(const Signal<T>* signal, const Object& slotObj, Slot&& slot)
        {
            constexpr int size = Details::GetArguments<Slot>();

            auto obj = Connector::CheckConnection(static_cast<const ISignal*>(signal), slotObj, std::is_base_of<IConnectable, std::remove_pointer_t<Object>>());
            const_cast<Signal<T>*>(signal)->Add(Details::CheckArgs<size>().Connect(slotObj, std::forward<Slot>(slot)), obj);
        }

        ///
        /// Connects two signals with each other.
        ///
        template <template <typename> typename Signal, typename T>
        static void Connect(const Signal<T>* lhs, const Signal<T>* rhs)
        {
            if (lhs == rhs)
                return;

            auto closure = [=](auto... args) -> void {
                if (rhs)
                    (*rhs)(std::forward<decltype(args)>(args)...);
            };

            std::function<typename Signal<T>::Signature> func = closure;
            Connector::Connect(lhs, std::move(func));
        }

        ///
        /// Disconnects signal with objects slots.
        ///
        template <template <typename> typename Signal, typename T, typename Object, typename Slot>
        static bool Disconnect(const Signal<T>* signal, const Object& slotObj, Slot&& slot)
        {
            if (!slotObj)
                return false;

            constexpr int size = Details::GetArguments<Slot>();
            std::function<T> binder = Details::CheckArgs<size>().Connect(slotObj, std::forward<Slot>(slot));

            auto& content = const_cast<Signal<T>*>(signal)->Content();
            auto iterator = std::find_if(content.begin(), content.end(), [&](const auto& pair) {
                auto& [object, function] = pair;

                if (object)
                {
                    if (object == (slotObj))
                        return function.target_type().hash_code() == binder.target_type().hash_code();
                }

                return false;
            });

            if (iterator != content.end())
            {
                content.erase(iterator);
                return true;
            }

            return false;
        }

        ///
        /// Disconnects signal pointer with lambda or function.
        ///
        template <template <typename> typename Signal, typename T>
        static bool Disconnect(const Signal<T>* signal, typename Signal<T>::Argument slot)
        {
            auto& content = const_cast<Signal<T>*>(signal)->Content();
            auto iterator = std::find_if(content.begin(), content.end(), [&](const auto& pair) {
                auto& [object, function] = pair;

                if (!object)
                    return function.target_type().hash_code() == slot.target_type().hash_code();

                return false;
            });

            if (iterator != content.end())
            {
                content.erase(iterator);
                return true;
            }

            return false;
        }

        ///
        /// Drops all signal connections.
        ///
        template <template <typename> typename Signal, typename T>
        static bool Disconnect(const Signal<T>* signal)
        {
            const auto signalPtr = const_cast<Signal<T>*>(signal);
            *(signalPtr) = nullptr;

            return signalPtr->Content().empty();
        }

        ///
        /// Drops all slots for object from signal interface.
        ///
        template <typename Object, typename = std::enable_if_t<std::is_pointer_v<Object> && std::is_class_v<std::remove_pointer_t<Object>>>>
        static void Disconnect(const ISignal* signal, const Object& object)
        {
            const_cast<ISignal*>(signal)->Drop(Details::ObjectPointer{ object });
        }

        ///
        /// Drops all slots from subscribed object.
        /// 
        template <template <typename> typename Signal, typename T, typename Object,
                  typename = std::enable_if_t<std::is_pointer_v<Object> && std::is_class_v<std::remove_pointer_t<Object>>>>
        static void Disconnect(const Signal<T>* signal, const Object& object)
        {
            Connector::Disconnect(static_cast<ISignal*>(signal), object);
        }

        ///
        /// Returns signal callbacks size.
        ///
        template <template <typename> typename Signal, typename T>
        static std::size_t Callbacks(const Signal<T>* signal)
        {
            return signal->Size();
        }
    };

    // forward realization
    template <typename Object>
    void Details::ConnectorForwarder::Disconnect(const ISignal* signal, const Object& object)
    {
        Connector::Disconnect(signal, object);
    }
    
}  // namespace AtomDestiny
