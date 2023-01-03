#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CommanderController.generated.h"

class UInputAction;
class UInputMappingContext;

///
/// Class which contains actions & Movement setup for CommanderPawn
///
UCLASS()
class ATOMDESTINY_API ACommanderController : public APlayerController
{
    GENERATED_BODY()
    
public:
    virtual void SetupInputComponent() override;

    UInputMappingContext* GetPawnMappingContext() const { return m_pawnMappingContext; }

    UInputAction* GetActionMove() const { return m_actionMove; }
    UInputAction* GetActionLook() const { return m_actionLook; }
    UInputAction* GetActionRoll() const { return m_actionRoll; }
    UInputAction* GetActionReset() const { return m_actionReset; }

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
};
