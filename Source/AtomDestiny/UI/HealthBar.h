#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "HealthBar.generated.h"

class UProgressBar;

/**
 * A HealthBar which is auto-oriented in the screen space
 */
UCLASS(Blueprintable)
class ATOMDESTINY_API UHealthBar : public UUserWidget
{
    GENERATED_BODY()

public:
    
    UFUNCTION(BlueprintCallable)
    void SetHealthPercent(float value);

    UFUNCTION(BlueprintCallable)
    void SetEnergyPercent(float value);
    
protected:
    UPROPERTY(meta = (BindWidget, DisplayName = "Health bar blueprint"))
    UProgressBar* m_health;

    UPROPERTY(meta = (BindWidget, DisplayName = "Shield bar blueprint"))
    UProgressBar* m_energy;
};
