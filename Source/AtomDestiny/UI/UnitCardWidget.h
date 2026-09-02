#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "AtomDestiny/Unit/Unit.h"

#include "UnitCardWidget.generated.h"

class UBorder;
class UButton;
class UImage;
class UListView;
class UTextBlock;
class UTexture2D;

UCLASS()
class ATOMDESTINY_API UUnitCardItem : public UObject
{
    GENERATED_BODY()

public:
    UPROPERTY()
    EADUnitType type = EADUnitType::None;

    UPROPERTY()
    FText title;

    UPROPERTY()
    TSoftObjectPtr<UTexture2D> icon;
};

/**
 * List entry card used to pick a unit for placement.
 * Visual layout lives in W_UnitCard; C++ only fills data.
 */
UCLASS(Blueprintable)
class ATOMDESTINY_API UUnitCardWidget : public UUserWidget, public IUserObjectListEntry
{
    GENERATED_BODY()

public:
    void SetTitle(const FString& title);
    void SetIcon(const TSoftObjectPtr<UTexture2D>& icon);
    EADUnitType GetUnitType() const { return m_unitType; }

    virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;

protected:
    virtual void NativePreConstruct() override;
    virtual void NativeOnItemSelectionChanged(bool bIsSelected) override;

    UPROPERTY(EditAnywhere)
    FString m_title;

    UPROPERTY()
    EADUnitType m_unitType = EADUnitType::None;

    UPROPERTY()
    TSoftObjectPtr<UTexture2D> m_pendingIcon;

    uint8 m_iconLoadAttempts = 0;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UBorder* CardBorder;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UButton* Button;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UTextBlock* Text;

    UPROPERTY(EditAnywhere, meta = (BindWidgetOptional))
    UImage* CardImage;

    UPROPERTY()
    class UListView* OwningListView;

private:
    int32 CardIndex = -1;

    void UpdateAppearance(const bool bIsActive) const;

    void ResolveWidgetReferences();
    void ApplyPendingIcon();
    UListView* GetOwningListView() const;
};
