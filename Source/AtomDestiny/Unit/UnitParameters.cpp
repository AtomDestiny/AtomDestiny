#include "UnitParameters.h"

#include <AtomDestiny/ObjectState/Destroyable.h>

#include <AtomDestiny/Core/Logger.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

#include "Core/Logger.h"

UUnitParameters::UUnitParameters(const FObjectInitializer& objectInitializer):
    UObjectStateBase(objectInitializer)
{
    SetTickEnabled(true);
}

void UUnitParameters::AddDamage(double damage, EWeaponType type, AActor* owner)
{
    if (m_currentHealth <= 0)
    {
        LOG_ERROR(TEXT("Current unit heath equal or lower that zero. Something went wrong"));
        return;
    }

    // calculate result damage change current health
    const double resultDamage = GetResultDamage(type, damage);
    m_currentHealth -= resultDamage;

    CheckHealthState();
}

void UUnitParameters::BeginPlay()
{
    Super::BeginPlay();

    if (m_healthBarWidget != nullptr)
        m_healthBarWidget->SetEnergyVisible(false);
    else
        LOG_WARNING(TEXT("[BeginPlay] Health bar widget was not set"));
}

void UUnitParameters::TickComponent(float deltaTime, ELevelTick levelTick, FActorComponentTickFunction* func)
{
    Super::TickComponent(deltaTime, levelTick, func);
    
    RenderHealthBar();
}

void UUnitParameters::RenderHealthBar()
{
    if (m_healthBarWidget == nullptr || !m_healthBarWidget.IsValid() || m_hideBar)
        return;

    m_healthBarWidget->SetHealthPercent(m_currentHealth / m_currentMaxHealth);
    m_healthBarWidget->SetHealthVisible(m_currentHealth != m_currentMaxHealth);
}

void UUnitParameters::CheckHealthState()
{
    // check current health on death
    if (m_currentHealth <= 0 && !m_isDead)
    {
        const TScriptInterface<IDestroyable> destroyable = GET_INTERFACE(Destroyable);

        if (destroyable == nullptr)
        {
            LOG_ERROR(TEXT("There is no IDestroyable component on Unit, so Unit params can not imitate a Destroy"));
            return;
        }

        if (!destroyable->IsDestroyed())
            destroyable->Destroy();

        m_isDead = true;
    }
}
