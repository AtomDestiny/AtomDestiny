#pragma once

#include <AtomDestiny/Coroutines/Coroutines.h>

#include <Runtime/Core/Public/Misc/Optional.h>

namespace AtomDestiny
{
    ///
    /// Represents manager that operates with running coroutines.
    /// You can use this runner to stop running coroutines.
    /// To start coroutine call RunCoroutine method. It will cache FAsyncCoroutine.
    /// To have a better access just derive CoroutinesRunner to your entity. 
    ///
    class CoroutinesRunner
    {
    public:
        ///
        /// Runs new coroutine and returns its ID.
        /// You can use this ID to stop or manage coroutine execution.
        /// 
        [[maybe_unused]] TOptional<uint64_t> RunCoroutine(FAsyncCoroutine&& coroutine, UObject* owner);
        [[maybe_unused]] bool StopCoroutine(const uint64_t id);
        void StopAllCoroutines();

    private:
        inline static uint64_t Id = 0;
        TMap<uint64_t, FAsyncCoroutine> m_coroutines;
    };
} // namespace AtomDestiny
