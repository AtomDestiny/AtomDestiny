#pragma once

#include <UE5Coro/Public/UE5Coro.h>

namespace AtomDestiny
{
    ///
    /// Represents manager that operates with running coroutines.
    /// You can use this runner to stop running coroutines.
    /// To start coroutine call RunCoroutine method. It will cache FAsyncCoroutine.
    /// To have a better access just derive CoroutineRunner to your entity.
    ///
    class CoroutineRunner
    {
    public:
        void RunCoroutine(FAsyncCoroutine&& coroutine);
        void StopAllCoroutines();

    private:
        uint64_t m_id = 0;
        TMap<uint64_t, FAsyncCoroutine> m_coroutines;
    };
}
