#pragma once

#include <GameFramework/Actor.h>

#include <AtomDestiny/AtomDestiny.h>

#include "SideDestination.generated.h"

class UArrowComponent;
class UBillboardComponent;

///
/// Level marker of a side destination.
/// Units of the side move there by default until they meet an enemy.
/// Visible in editor only.
///
UCLASS(BlueprintType, hideCategories = (Rendering, Physics, Collision, Input, HLOD))
class ATOMDESTINY_API ASideDestination : public AActor
{
    GENERATED_BODY()

public:
    explicit ASideDestination(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    // Returns first destination of the side on the level
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AtomDestiny|Side", meta = (WorldContext = "worldContextObject"))
    static ASideDestination* FindForSide(const UObject* worldContextObject, EGameSide side);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AtomDestiny|Side")
    EGameSide GetSide() const { return m_side; }

protected:
#if WITH_EDITOR
    virtual void OnConstruction(const FTransform& transform) override;
#endif

    // Side, which units use this destination
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AtomDestiny|Side", meta = (DisplayName = "Side"))
    EGameSide m_side = EGameSide::Rebels;

    UPROPERTY(VisibleAnywhere, Category = "AtomDestiny|Side")
    TObjectPtr<USceneComponent> m_root;

#if WITH_EDITORONLY_DATA
    UPROPERTY()
    TObjectPtr<UBillboardComponent> m_sprite;

    UPROPERTY()
    TObjectPtr<UArrowComponent> m_arrow;
#endif
};
