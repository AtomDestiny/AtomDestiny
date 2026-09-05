#pragma once

#include <Components/ActorComponent.h>
#include <Engine/EngineTypes.h>

#include "AtomDestiny/AtomDestiny.h"

#include "UnitSideColorDetails.generated.h"

///
/// Tints team-color detail meshes from Conflict Sides settings.
/// Details are picked manually and/or by component name (default: contains "side_part").
/// Base unit materials are not modified.
///
UCLASS(ClassGroup = (AtomDestiny), meta = (BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitSideColorDetails : public UActorComponent
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "AtomDestiny|Side")
    void ApplyForSide(EGameSide side);

    UFUNCTION(BlueprintCallable, Category = "AtomDestiny|Side")
    void SetHighlighted(bool bHighlighted);

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Side", meta = (DisplayName = "Detail meshes", UseComponentPicker))
    TArray<FComponentReference> m_detailMeshes;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Side", meta = (DisplayName = "Auto discover by name"))
    bool m_autoDiscoverByName = true;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Side", meta = (DisplayName = "Name contains (case insensitive)", EditCondition = "m_autoDiscoverByName"))
    FString m_autoDiscoverNameContains = TEXT("side_part");

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Side", meta = (DisplayName = "Material color parameter"))
    FName m_colorParameterName = TEXT("TeamColor");

private:
    void CollectDetailComponents(TArray<UActorComponent*>& outComponents) const;
    void ApplyDisplayColor() const;

    FLinearColor m_baseTeamColor = FLinearColor::White;
    bool m_bHighlighted = false;

    static constexpr float HighlightMultiplier = 1.85f;
};
