#include "UI/TrainingMainWidget.h"

#include "AtomDestiny/Gameplay/UnitStorage.h"
#include "AtomDestiny/Gameplay/UnitCatalog.h"
#include "AtomDestiny/UI/UnitCardWidget.h"
#include "Player/CommanderController.h"

#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ListView.h"
#include "Components/Widget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

namespace
{
    const ESlateVisibility VisibilityBool[] = { ESlateVisibility::Hidden, ESlateVisibility::Visible };

    void SetCanvasSlotAutoSize(UWidget* widget)
    {
        if (widget == nullptr)
        {
            return;
        }

        if (UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(widget->Slot))
        {
            slot->SetAutoSize(true);
        }
    }
}

void UTrainingMainWidget::RefreshUnitCards()
{
    AtomDestiny::EnsureUnitCatalogLoaded();
    SetupUnits(AtomDestiny::UnitStorage::Instance().GetUnits());
}

void UTrainingMainWidget::SetupUnits(const TArray<EADUnitType>& units)
{
    if (UnitsList == nullptr)
    {
        return;
    }

    UnitsList->ClearListItems();

    AtomDestiny::UnitStorage& storage = AtomDestiny::UnitStorage::Instance();

    for (const EADUnitType type : units)
    {
        UUnitCardItem* item = NewObject<UUnitCardItem>(this);
        item->type = type;
        item->title = storage.GetDisplayName(type);

        if (const TOptional<FUnitInfo> info = storage.GetInfo(type); info.IsSet())
        {
            item->icon = info->icon;
        }

        UnitsList->AddItem(item);
    }

    if (UnitsList->GetNumItems() > 0 && UnitsList->GetSelectedItem() == nullptr)
    {
        UnitsList->SetSelectedIndex(0);
    }
}

void UTrainingMainWidget::ChangeMode(bool setupArmy)
{
    m_flSetupArmy = setupArmy;

    if (BnSetupArmy != nullptr)
    {
        BnSetupArmy->SetVisibility(VisibilityBool[!setupArmy]);
    }

    if (BnEndSetupArmy != nullptr)
    {
        BnEndSetupArmy->SetVisibility(VisibilityBool[setupArmy]);
    }

    if (UnitsList != nullptr)
    {
        UnitsList->SetVisibility(VisibilityBool[setupArmy]);
    }

    UGameplayStatics::SetGamePaused(this, setupArmy);

    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        controller->OnSetupArmyModeChanged(setupArmy);
    }
}

void UTrainingMainWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    ApplyOverlayHitBounds();
}

void UTrainingMainWidget::NativeConstruct()
{
    Super::NativeConstruct();
    ApplyOverlayHitBounds();

    RefreshUnitCards();

    if (UWorld* world = GetWorld())
    {
        world->GetTimerManager().SetTimerForNextTick(
            FTimerDelegate::CreateUObject(this, &UTrainingMainWidget::RefreshUnitCards));
    }

    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        controller->SetTrainingWidget(this);
    }
}

void UTrainingMainWidget::ApplyOverlayHitBounds()
{
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    SetCanvasSlotAutoSize(BnSetupArmy);
    SetCanvasSlotAutoSize(BnEndSetupArmy);
}

EADUnitType UTrainingMainWidget::GetSelectedUnitType() const
{
    if (UnitsList == nullptr)
    {
        return EADUnitType::None;
    }

    const UUnitCardItem* item = Cast<UUnitCardItem>(UnitsList->GetSelectedItem());
    if (item == nullptr)
    {
        return EADUnitType::None;
    }

    return item->type;
}
