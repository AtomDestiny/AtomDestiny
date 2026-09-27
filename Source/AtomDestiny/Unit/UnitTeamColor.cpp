#include "UnitTeamColor.h"

#include "AtomDestiny/Core/ActorComponentUtils.h"
#include "AtomDestiny/Gameplay/SideStorage.h"
#include "AtomDestiny/Logic/Logic.h"

#include <Components/PrimitiveComponent.h>

namespace
{
    const FName TeamColorParameter = TEXT("TeamColor");
}

void AtomDestiny::TeamColor::Apply(const AActor* actor, const EGameSide side)
{
    if (actor == nullptr || side == EGameSide::None)
        return;

    const FVector4 color { SideStorage::Instance().GetTeamColor(side) };

    for (UPrimitiveComponent* primitive : Utils::GetComponents<UPrimitiveComponent>(actor))
    {
        // Does nothing for primitives without TeamColor custom primitive data in materials
        primitive->SetVectorParameterForCustomPrimitiveData(TeamColorParameter, color);
    }
}

void UUnitTeamColor::BeginPlay()
{
    Super::BeginPlay();

    if (const TScriptInterface<ILogic> logic = AtomDestiny::Utils::GetInterface<ILogic>(GetOwner()))
    {
        m_sideChangedHandle = logic->OnSideChanged().AddUObject(this, &UUnitTeamColor::OnSideChanged);
        OnSideChanged(logic->GetSide());
    }
}

void UUnitTeamColor::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    if (const TScriptInterface<ILogic> logic = AtomDestiny::Utils::GetInterface<ILogic>(GetOwner()))
        logic->OnSideChanged().Remove(m_sideChangedHandle);

    Super::EndPlay(endPlayReason);
}

void UUnitTeamColor::OnSideChanged(const EGameSide side) const
{
    AtomDestiny::TeamColor::Apply(GetOwner(), side);
}
