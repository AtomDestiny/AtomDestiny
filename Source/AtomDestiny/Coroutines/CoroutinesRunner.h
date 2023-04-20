#pragma once

#include <AtomDestiny/Coroutines/Coroutines.h>

#include <optional>

namespace AtomDestiny
{
    ///
    /// Represents manager that operates with running coroutines.
    /// You can use this runner to stop running coroutines.
    /// To start coroutine call RunCoroutine method. It will cache FAsyncCoroutine.
    /// To have a better access just derive CoroutinesRunner to your entity. 
    /// Warning. Use CoroutineRunner only with UObject. It should be possible to cast this to UObject
    ///
    class CoroutinesRunner
    {
    public:
        virtual ~CoroutinesRunner() = default;

        ///
        /// Runs new coroutine and returns its ID.
        /// You can use this ID to stop or manage coroutine execution.
        /// 
        [[maybe_unused]] TOptional<uint64_t> RunCoroutine(FAsyncCoroutine&& coroutine);
        [[maybe_unused]] bool StopCoroutine(const uint64_t id);
        void StopAllCoroutines();

    private:
        uint64_t m_id = 0;
        TMap<uint64_t, FAsyncCoroutine> m_coroutines;
    };
    
} // namespace AtomDestiny
