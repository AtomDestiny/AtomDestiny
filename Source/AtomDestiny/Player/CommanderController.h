#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AtomDestiny/AtomDestiny.h"
#include "AtomDestiny/Gameplay/TacticsLayoutElement.h"

#include "CommanderController.generated.h"

class APlacementPointer;
class AFloorGrid;
class APawn;
class UInputAction;
class UInputMappingContext;
class UTrainingMainWidget;

///
/// Class which contains actions & Movement setup for CommanderPawn
///
UCLASS()
class ATOMDESTINY_API ACommanderController : public APlayerController
{
    GENERATED_BODY()
    
public:
    ACommanderController();
    
    virtual void SetupInputComponent() override;
    virtual void BeginPlay() override;
    virtual void PlayerTick(float DeltaTime) override;

    UInputMappingContext* GetPawnMappingContext() const { return m_pawnMappingContext; }

    UInputAction* GetActionMove() const { return m_actionMove; }
    UInputAction* GetActionLook() const { return m_actionLook; }
    UInputAction* GetActionRoll() const { return m_actionRoll; }
    UInputAction* GetActionReset() const { return m_actionReset; }
    UInputAction* GetActionLClick() const { return m_actionLClick; }
    UInputAction* GetActionEndSetupArmy() const { return m_actionEndSetupArmy; }

    void SetTrainingWidget(UTrainingMainWidget* widget);

    void TryPlaceUnitAtCursor();

    void TryFinishArmySetup();

    void OnSetupArmyModeChanged(bool setupArmy);

    bool IsArmySetupActive() const { return m_bArmySetupActive; }

    /** Drop placed unit refs before leaving Training (actors are removed by OpenLevel). */
    void ClearSetupUnits();

    /** Destroy all units placed during setup and clear the session layout. */
    void ClearAllSetupUnits();

    /** Clears pending despawn timers for all actors on the current map. */
    void ClearLevelDespawnTimers() const;

    /** Writes the current layout to GameInstance before leaving Training (after battle). */
    void PersistTacticsLayoutForNextVisit();

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable mouse look"))
    bool EnableMouseLook = true;

protected:
    
    UPROPERTY(EditAnywhere, meta = (DisplayName = "Pawn mapping context"))
    UInputMappingContext* m_pawnMappingContext;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action move"))
    UInputAction* m_actionMove;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action rotate"))
    UInputAction* m_actionLook;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action rotate"))
    UInputAction* m_actionRoll;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action reset"))
    UInputAction* m_actionReset;
    
    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action LClick"))
    UInputAction* m_actionLClick;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action end setup army"))
    UInputAction* m_actionEndSetupArmy;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action RClick"))
    UInputAction* m_actionRClick;

private:
    void UpdatePlacementPointer();
    bool IsGridPointerActive() const;
    bool TryGetGridCellUnderCursor(AFloorGrid*& outGrid, FVector& outCellCenter) const;
    bool ProjectToGround(const FVector& cellCenter, FVector& outGroundLocation) const;
    FRotator ComputeFacingRotation(const FVector& location, EGameSide placementSide) const;
    void AlignUnitGroundPoint(APawn* pawn, const FVector& groundLocation) const;

    APawn* SpawnTrainingUnitAt(
        EADUnitType unitType,
        EGameSide placementSide,
        const FVector& groundLocation,
        const FRotator& facingRotation);

    void SaveTacticsLayoutToGameInstance() const;

    void TryRestoreTacticsLayout();

    UPROPERTY()
    TObjectPtr<APlacementPointer> m_placementPointer;

    TWeakObjectPtr<UTrainingMainWidget> m_trainingWidget;

    bool m_bArmySetupActive = false;

    UPROPERTY()
    TArray<TWeakObjectPtr<APawn>> m_setupPlacedUnits;

    TArray<FTacticsLayoutElement> m_tacticsLayout;

    bool m_bTacticsLayoutRestored = false;

    uint8 m_restoreLayoutAttempts = 0;
};
