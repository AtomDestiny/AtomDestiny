#pragma once

#include "AtomDestiny/Unit/UnitAnimationBase.h"

#include "HunterAnimation.generated.h"

class UAnimSingleNodeInstance;

///
/// Represents move animation controller for Hunter unit
///
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UHunterAnimation final : public UUnitAnimationBase
{
    GENERATED_BODY()

public:
    void BeginPlay() override;

    void Idle() override;
    void Walk() override;
    void Attack() override;
    
private:
    TWeakObjectPtr<UAnimSingleNodeInstance> m_animation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Walking transition", AllowPrivateAccess = "true"))
    bool m_isWalking = false;
};
