#include "AtomDestinyCore.h"

struct AtomDestiny::Core::CoreData
{
};

AtomDestiny::Core::Core():
    m_impl(std::make_unique<CoreData>())
{
}

AtomDestiny::Core::~Core() = default;

AtomDestiny::Core& AtomDestiny::Core::Instance()
{
    static Core core;
    return core;
}
