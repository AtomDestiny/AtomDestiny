#include "RallyPoint.h"

#include <AtomDestiny/Gameplay/AtomDestinySideSettings.h>
#include <AtomDestiny/Gameplay/SideStorage.h>
#include <AtomDestiny/Core/Logger.h>

#include <Components/SceneComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Engine/StaticMesh.h>
#include <EngineUtils.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <Materials/MaterialInterface.h>
#include <UObject/ConstructorHelpers.h>

namespace
{
    FLinearColor GetSideTeamColor(const EGameSide side)
    {
        if (side == EGameSide::None)
        {
            return FLinearColor::White;
        }

        if (AtomDestiny::SideStorage::Instance().Contains(side))
        {
            return AtomDestiny::SideStorage::Instance().GetTeamColor(side);
        }

        if (const UAtomDestinySideSettings* settings = GetDefault<UAtomDestinySideSettings>())
        {
            if (const FSideDefinition* definition = settings->GetSides().Find(side))
            {
                return definition->teamColor;
            }
        }

        return FLinearColor::White;
    }

    void ConfigureEditorOnlyMesh(UStaticMeshComponent* component)
    {
        if (component == nullptr)
            return;

        component->SetHiddenInGame(true);
        component->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        component->SetCastShadow(false);
    }

    bool ApplyMesh(
        UStaticMeshComponent* component,
        const TSoftObjectPtr<UStaticMesh>& meshAsset,
        const TSoftObjectPtr<UMaterialInterface>& materialAsset)
    {
        if (component == nullptr || meshAsset.IsNull())
        {
            return false;
        }

        UStaticMesh* mesh = meshAsset.LoadSynchronous();
        if (mesh == nullptr)
        {
#if WITH_EDITOR
            LOG_WARNING(
                TEXT("RallyPoint '%s': flag mesh not found at '%s'. Import Flag.fbx in Content Browser."),
                *component->GetOwner()->GetActorNameOrLabel(),
                *meshAsset.ToSoftObjectPath().ToString());
#endif
            return false;
        }

        component->SetStaticMesh(mesh);

        if (materialAsset.IsNull())
        {
            return true;
        }

        if (UMaterialInterface* material = materialAsset.LoadSynchronous())
        {
            const int32 materialSlots = FMath::Max(1, component->GetNumMaterials());

            for (int32 slot = 0; slot < materialSlots; ++slot)
            {
                component->SetMaterial(slot, material);
            }
        }

        return true;
    }

    void ApplyTeamColorToMesh(
        UStaticMeshComponent* component,
        const FLinearColor& teamColor,
        const FName colorParameterName)
    {
        if (component == nullptr || colorParameterName.IsNone())
        {
            return;
        }

        const int32 materialSlots = component->GetNumMaterials();
        for (int32 slot = 0; slot < materialSlots; ++slot)
        {
            UMaterialInterface* sourceMaterial = component->GetMaterial(slot);
            if (sourceMaterial == nullptr)
            {
                continue;
            }

            if (UMaterialInstanceDynamic* dynamicMaterial = component->CreateDynamicMaterialInstance(slot, sourceMaterial))
            {
                dynamicMaterial->SetVectorParameterValue(colorParameterName, teamColor);
            }
        }
    }
}

ARallyPoint::ARallyPoint(const FObjectInitializer& objectInitializer):
    AActor(objectInitializer)
{
    m_root = objectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
    SetRootComponent(m_root);

    m_flagMesh = objectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("FlagMesh"));
    m_flagMesh->SetupAttachment(m_root);
    ConfigureEditorOnlyMesh(m_flagMesh);

    m_flagStaticMesh = TSoftObjectPtr<UStaticMesh>(
        FSoftObjectPath(TEXT("/Game/Models/Other/Flag/Mesh/Flag.Flag")));

    m_flagMaterial = TSoftObjectPtr<UMaterialInterface>(
        FSoftObjectPath(TEXT("/Game/Models/Other/Flag/Mesh/M_Flag.M_Flag")));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> defaultFlagMesh(
        TEXT("/Game/Models/Other/Flag/Mesh/Flag.Flag"));

    if (defaultFlagMesh.Succeeded())
    {
        m_flagMesh->SetStaticMesh(defaultFlagMesh.Object);
    }

    static ConstructorHelpers::FObjectFinder<UMaterialInterface> defaultFlagMaterial(
        TEXT("/Game/Models/Other/Flag/Mesh/M_Flag.M_Flag"));

    if (defaultFlagMaterial.Succeeded())
    {
        m_flagMesh->SetMaterial(0, defaultFlagMaterial.Object);
    }
}

void ARallyPoint::BeginPlay()
{
    Super::BeginPlay();
    HideFlagVisualForGame();
}

void ARallyPoint::EndPlay(const EEndPlayReason::Type endPlayReason)
{
    Super::EndPlay(endPlayReason);

#if WITH_EDITOR
    ApplyEditorFlagVisual();
#endif
}

#if WITH_EDITOR
void ARallyPoint::OnConstruction(const FTransform& transform)
{
    Super::OnConstruction(transform);
    ApplyEditorFlagVisual();
}

void ARallyPoint::PostLoad()
{
    Super::PostLoad();

    if (!IsTemplate())
        ApplyEditorFlagVisual();
}

void ARallyPoint::PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent)
{
    Super::PostEditChangeProperty(propertyChangedEvent);
    ApplyEditorFlagVisual();
}
#endif

void ARallyPoint::ApplyEditorFlagVisual() const
{
    if (m_flagMesh == nullptr)
        return;

    ApplyMesh(m_flagMesh, m_flagStaticMesh, m_flagMaterial);
    ApplyTeamColorToMesh(m_flagMesh, GetSideTeamColor(m_side), m_colorParameterName);

    if (GetWorld() != nullptr && GetWorld()->IsGameWorld())
    {
        HideFlagVisualForGame();
        return;
    }

    m_flagMesh->SetHiddenInGame(true);
    m_flagMesh->SetVisibility(true, true);
}

void ARallyPoint::HideFlagVisualForGame() const
{
    if (m_flagMesh == nullptr)
        return;

    m_flagMesh->SetHiddenInGame(true);
}

ARallyPoint* ARallyPoint::FindForSide(const UObject* worldContextObject, const EGameSide side)
{
    if (worldContextObject == nullptr || side == EGameSide::None)
        return nullptr;

    const auto world = worldContextObject->GetWorld();
    if (world == nullptr)
        return nullptr;

    for (TActorIterator<ARallyPoint> iterator(world); iterator; ++iterator)
    {

        if (const auto rallyPoint = *iterator;
            rallyPoint != nullptr && rallyPoint->GetSide() == side)
        {
            return rallyPoint;
        }
    }

    return nullptr;
}
