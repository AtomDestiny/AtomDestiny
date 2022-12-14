#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "CommanderController.generated.h"

///
/// Class which contains actions & Movement setup for CommanderPawn
///
UCLASS()
class ATOMDESTINY_API ACommanderController : public APlayerController
{
    GENERATED_BODY()
    
public:
    virtual void SetupInputComponent() override;

    class UInputMappingContext* GetPawnMappingContext() const { return m_pawnMappingContext; }

    class UInputAction* GetActionMove() const { return m_actionMove; }
    class UInputAction* GetActionRotate() const { return m_actionRotate; }
    class UInputAction* GetActionReset() const { return m_actionReset; }

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Enable mouse look"))
    bool EnableMouseLook = true;

protected:
    
    UPROPERTY(EditAnywhere, meta = (DisplayName = "Pawn mapping context"))
    UInputMappingContext* m_pawnMappingContext;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action move"))
    UInputAction* m_actionMove;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action rotate"))
    UInputAction* m_actionRotate;

    UPROPERTY(EditAnywhere, meta = (DisplayName = "Action reset"))
    UInputAction* m_actionReset;
};
