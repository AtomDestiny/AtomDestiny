#include "CoroutinesRunner.h"

#include <AtomDestiny/Core/Logger.h>

TOptional<uint64_t> AtomDestiny::CoroutinesRunner::RunCoroutine(FAsyncCoroutine&& coroutine, UObject* owner)
{
    const uint64_t id = ++Id;

    if (m_coroutines.Contains(id))
    {
        LOG_ERROR(TEXT("Coroutine id currently added, something went wrong. Can not run coroutine"));
        return NullOpt;
    }

    coroutine.ContinueWithWeak(owner, [this, id] {
        this->m_coroutines.Remove(id);
    });

    m_coroutines.Add(id, std::move(coroutine));
    return id;
}

bool AtomDestiny::CoroutinesRunner::StopCoroutine(const uint64_t id)
{
    if (const auto coroutine = m_coroutines.Find(id); coroutine != nullptr)
    {
        coroutine->Cancel();
        return static_cast<bool>(m_coroutines.Remove(id));
    }

    return false;
}

void AtomDestiny::CoroutinesRunner::StopAllCoroutines()
{
    for ([[maybe_unused]] auto& [id, coroutine] : m_coroutines)
    {
        coroutine.Cancel();
    }

    m_coroutines.Reset();
}
