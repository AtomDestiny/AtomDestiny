#include "CoroutineRunner.h"

#include <AtomDestiny/Core/Logger.h>

void AtomDestiny::CoroutineRunner::RunCoroutine(FAsyncCoroutine&& coroutine)
{
    const uint64_t id = ++m_id;

    if (m_coroutines.Contains(id))
    {
        LOG_ERROR(TEXT("Coroutine id currently added, something went wrong. Can not run coroutine"));
        return;
    }

    coroutine.OnCompletion().AddLambda([this, id] {
        if (this != nullptr) {
            m_coroutines.Remove(id);
        }
    });

    m_coroutines.Add(id, std::move(coroutine));
}

void AtomDestiny::CoroutineRunner::StopAllCoroutines()
{
    for ([[maybe_unused]] auto& [id, coroutine] : m_coroutines)
    {
        coroutine.OnCompletion().Clear();
        coroutine.Cancel();
    }

    m_coroutines.Reset();
}
