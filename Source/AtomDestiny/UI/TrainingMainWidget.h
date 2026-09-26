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
    UFUNCTION(BlueprintCallable)
    void SetupUnits(const TArray<EADUnitType>& units);

    UFUNCTION(BlueprintCallable)
    void RefreshUnitCards();

    UFUNCTION(BlueprintCallable)
    void BeginArmySetup();

    /** Ends setup and starts the battle. Cannot return to setup until the map is reloaded. */
    UFUNCTION(BlueprintCallable)
    void EndArmySetup();

    UFUNCTION(BlueprintPure, Category = "Training")
    bool IsArmySetupActive() const { return m_bArmySetupActive; }

    /** Returns unit type from the currently selected card, if any. */
    UFUNCTION(BlueprintPure, Category = "Training")
    EADUnitType GetSelectedUnitType() const;

    UFUNCTION(BlueprintCallable, Category = "Training")
    void ReturnToMainMenu();

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UButton* BnBackToMenu;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UButton* BnClearAll;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UButton* BnStartBattle;

    UPROPERTY(EditAnywhere, meta = (BindWidget))
    UListView* UnitsList;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Main menu map"))
    FName m_mainMenuMapName = FName(TEXT("MainMenuMap"));

private:
    UFUNCTION()
    void OnBackToMenuClicked();

    UFUNCTION()
    void OnStartBattleClicked();

    UFUNCTION()
    void OnClearAllClicked();

    void ApplyOverlayHitBounds();

    bool m_bArmySetupActive = false;
};
