#include "Misc/PlacementPointer.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInterface.h"

APlacementPointer::APlacementPointer()
{
    PrimaryActorTick.bCanEverTick = false;

    m_mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PointerMesh"));
    SetRootComponent(m_mesh);

    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_mesh->SetCastShadow(false);
    m_mesh->SetRelativeRotation(m_meshRotation);
}

void APlacementPointer::BeginPlay()
{
    Super::BeginPlay();

    if (m_pointerMesh.IsNull())
    {
        m_pointerMesh = TSoftObjectPtr<UStaticMesh>(
            FSoftObjectPath(TEXT("/Game/Models/Other/Pointer3D/Mesh/ArrowsPointer3D.ArrowsPointer3D")));
    }

    if (m_pointerMaterial.IsNull())
    {
        m_pointerMaterial = TSoftObjectPtr<UMaterialInterface>(
            FSoftObjectPath(TEXT("/Game/Models/Other/Pointer3D/Material/M_Pointer3D.M_Pointer3D")));
    }

    if (UStaticMesh* mesh = m_pointerMesh.LoadSynchronous())
    {
        m_mesh->SetStaticMesh(mesh);
    }

    if (UMaterialInterface* material = m_pointerMaterial.LoadSynchronous())
    {
        const int32 materialSlots = FMath::Max(1, m_mesh->GetNumMaterials());
        for (int32 slot = 0; slot < materialSlots; ++slot)
        {
            m_mesh->SetMaterial(slot, material);
        }
    }
}

void APlacementPointer::ShowAt(const FVector& worldLocation)
{
    FVector location = worldLocation;
    location.Z += m_heightOffset;

    SetActorLocation(location);
    SetActorHiddenInGame(false);
}

void APlacementPointer::HidePointer()
{
    SetActorHiddenInGame(true);
}
