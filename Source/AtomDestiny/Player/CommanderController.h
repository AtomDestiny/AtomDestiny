#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AtomDestiny/AtomDestiny.h"

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

    void SetTrainingWidget(UTrainingMainWidget* widget);

    void TryPlaceUnitAtCursor();

    void OnSetupArmyModeChanged(bool setupArmy);

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

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action RClick"))
    UInputAction* m_actionRClick;

private:
    void UpdatePlacementPointer();
    bool IsGridPointerActive() const;
    bool TryGetGridCellUnderCursor(AFloorGrid*& outGrid, FVector& outCellCenter) const;
    bool ProjectToGround(const FVector& cellCenter, FVector& outGroundLocation) const;
    FRotator ComputeFacingRotation(const FVector& location, EGameSide placementSide) const;
    void AlignUnitGroundPoint(APawn* pawn, const FVector& groundLocation) const;

    UPROPERTY()
    TObjectPtr<APlacementPointer> m_placementPointer;

    TWeakObjectPtr<UTrainingMainWidget> m_trainingWidget;

    UPROPERTY()
    TArray<TWeakObjectPtr<APawn>> m_setupPlacedUnits;
};
