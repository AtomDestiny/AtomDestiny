#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AtomDestiny/Unit/Unit.h"

#include "TrainingMainWidget.generated.h"

class UButton;
class UListView;

/**
 * Overlay HUD for the Training map.
 */
UCLASS(Abstract, Blueprintable)
class ATOMDESTINY_API UTrainingMainWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, meta = (DisplayName = "SetupArmyFlag"))
    bool m_flSetupArmy = false;

    UFUNCTION(BlueprintCallable)
    void SetupUnits(const TArray<EADUnitType>& units);

    UFUNCTION(BlueprintCallable)
    void ChangeMode(bool setupArmy);

    UFUNCTION(BlueprintPure, Category = "Training")
    bool IsSetupArmyMode() const { return m_flSetupArmy; }

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UButton* BnSetupArmy;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UButton* BnEndSetupArmy;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UListView* UnitsList;

private:
    void ApplyOverlayHitBounds();
};
