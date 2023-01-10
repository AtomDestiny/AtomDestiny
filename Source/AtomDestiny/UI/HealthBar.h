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

    UFUNCTION(BlueprintCallable)
    void SetHealthVisible(bool visible);

    UFUNCTION(BlueprintCallable)
    void SetEnergyVisible(bool visible);
    
protected:
    UPROPERTY(meta = (BindWidget, DisplayName = "Health bar (widget part)"))
    UProgressBar* m_health;

    UPROPERTY(meta = (BindWidget, DisplayName = "Shield bar (widget part)"))
    UProgressBar* m_energy;
};
