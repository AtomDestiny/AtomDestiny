#include "SideDestination.h"

#include <AtomDestiny/Gameplay/AtomDestinySideSettings.h>

#include <Components/ArrowComponent.h>
#include <Components/BillboardComponent.h>
#include <Components/SceneComponent.h>
#include <EngineUtils.h>

ASideDestination::ASideDestination(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
    m_root = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
    SetRootComponent(m_root);

#if WITH_EDITORONLY_DATA
    m_sprite = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
    m_arrow = CreateEditorOnlyDefaultSubobject<UArrowComponent>(TEXT("Arrow"));

    if (m_sprite != nullptr)
    {
        m_sprite->SetupAttachment(m_root);
        m_sprite->bIsScreenSizeScaled = true;
    }

    if (m_arrow != nullptr)
    {
        m_arrow->SetupAttachment(m_root);
        m_arrow->bIsScreenSizeScaled = true;
        m_arrow->bTreatAsASprite = true;
    }
#endif

    SetHidden(true);
    SetCanBeDamaged(false);
}

#if WITH_EDITOR
void ASideDestination::OnConstruction(const FTransform& transform)
{
    Super::OnConstruction(transform);

    if (m_arrow == nullptr)
        return;

    // Side storage is filled at game startup only, so use project settings in editor
    if (const UAtomDestinySideSettings* settings = GetDefault<UAtomDestinySideSettings>())
    {
        if (const FSideDefinition* definition = settings->GetSides().Find(m_side))
            m_arrow->SetArrowColor(definition->teamColor);
    }
}
#endif

ASideDestination* ASideDestination::FindForSide(const UObject* worldContextObject, EGameSide side)
{
    if (worldContextObject == nullptr || side == EGameSide::None)
        return nullptr;

    const UWorld* world = worldContextObject->GetWorld();

    if (world == nullptr)
        return nullptr;

    for (TActorIterator<ASideDestination> iterator(world); iterator; ++iterator)
    {
        if (iterator->GetSide() == side)
            return *iterator;
    }

    return nullptr;
}
