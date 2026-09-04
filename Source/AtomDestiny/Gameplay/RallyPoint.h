#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <AtomDestiny/AtomDestiny.h>

#include "RallyPoint.generated.h"

class UMaterialInterface;
class USceneComponent;
class UStaticMesh;
class UStaticMeshComponent;

///
/// Map marker for a side rally point. Place on the level and set Side.
///
UCLASS(BlueprintType)
class ATOMDESTINY_API ARallyPoint : public AActor
{
    GENERATED_BODY()

public:
    ARallyPoint(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AtomDestiny|Side", meta = (WorldContext = "worldContextObject"))
    static ARallyPoint* FindForSide(const UObject* worldContextObject, EGameSide side);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AtomDestiny|Side")
    EGameSide GetSide() const { return m_side; }

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AtomDestiny|Side", meta = (DisplayName = "Side"))
    EGameSide m_side = EGameSide::Rebels;

    UPROPERTY(VisibleAnywhere, Category = "AtomDestiny|Side")
    TObjectPtr<USceneComponent> m_root;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type endPlayReason) override;

#if WITH_EDITOR
    virtual void OnConstruction(const FTransform& transform) override;
    virtual void PostLoad() override;
    virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

    void ApplyEditorFlagVisual() const;
    void HideFlagVisualForGame() const;

    UPROPERTY(VisibleAnywhere, Category = "AtomDestiny|Editor Visual", meta = (DisplayName = "Flag mesh"))
    TObjectPtr<UStaticMeshComponent> m_flagMesh;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Editor Visual", meta = (DisplayName = "Flag static mesh"))
    TSoftObjectPtr<UStaticMesh> m_flagStaticMesh;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Editor Visual", meta = (DisplayName = "Flag material"))
    TSoftObjectPtr<UMaterialInterface> m_flagMaterial;

    UPROPERTY(EditAnywhere, Category = "AtomDestiny|Editor Visual", meta = (DisplayName = "Team color parameter"))
    FName m_colorParameterName = TEXT("TeamColor");
};
