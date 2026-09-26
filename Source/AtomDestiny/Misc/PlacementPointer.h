#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlacementPointer.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;

/**
 * Visual marker snapped to a FloorGrid cell center.
 */
UCLASS()
class ATOMDESTINY_API APlacementPointer : public AActor
{
    GENERATED_BODY()

public:
    APlacementPointer();

    void ShowAt(const FVector& worldLocation);
    void HidePointer();

    UStaticMeshComponent* GetMeshComponent() const { return m_mesh; }

protected:
    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* m_mesh;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Pointer mesh"))
    TSoftObjectPtr<UStaticMesh> m_pointerMesh;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Pointer material"))
    TSoftObjectPtr<UMaterialInterface> m_pointerMaterial;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Height offset"))
    float m_heightOffset = 5.f;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Mesh rotation"))
    FRotator m_meshRotation = FRotator::ZeroRotator;
};
