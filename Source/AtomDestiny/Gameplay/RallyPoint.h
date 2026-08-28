#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Actor.h>
#include <AtomDestiny/AtomDestiny.h>

#include "RallyPoint.generated.h"

class USceneComponent;

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
};
