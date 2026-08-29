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

void UTrainingMainWidget::BeginArmySetup()
{
    if (m_bArmySetupActive)
    {
        return;
    }

    m_bArmySetupActive = true;

    if (BnSetupArmy != nullptr)
    {
        BnSetupArmy->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (BnStartBattle != nullptr)
    {
        BnStartBattle->SetVisibility(ESlateVisibility::Visible);
    }

    if (UnitsList != nullptr)
    {
        UnitsList->SetVisibility(ESlateVisibility::Visible);
    }

    UGameplayStatics::SetGamePaused(this, true);

    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        controller->OnSetupArmyModeChanged(true);
    }
}

void UTrainingMainWidget::EndArmySetup()
{
    if (!m_bArmySetupActive)
    {
        return;
    }

    m_bArmySetupActive = false;

    if (BnStartBattle != nullptr)
    {
        BnStartBattle->SetVisibility(ESlateVisibility::Collapsed);
    }

    if (UnitsList != nullptr)
    {
        UnitsList->SetVisibility(ESlateVisibility::Collapsed);
    }

    UGameplayStatics::SetGamePaused(this, false);

    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        controller->OnSetupArmyModeChanged(false);
    }
}

void UTrainingMainWidget::OnStartBattleClicked()
{
    EndArmySetup();
}

void UTrainingMainWidget::ReturnToMainMenu()
{
    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        if (!m_bArmySetupActive)
        {
            controller->PersistTacticsLayoutForNextVisit();
        }

        controller->ClearSetupUnits();
        controller->ClearLevelDespawnTimers();
    }

    if (m_bArmySetupActive)
    {
        UGameplayStatics::SetGamePaused(this, false);
    }

    UGameplayStatics::OpenLevel(this, m_mainMenuMapName);
}

void UTrainingMainWidget::OnBackToMenuClicked()
{
    ReturnToMainMenu();
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

    if (ACommanderController* controller = Cast<ACommanderController>(GetOwningPlayer()))
    {
        controller->SetTrainingWidget(this);
    }

    BeginArmySetup();

    if (UWorld* world = GetWorld())
    {
        world->GetTimerManager().SetTimerForNextTick(
            FTimerDelegate::CreateUObject(this, &UTrainingMainWidget::RefreshUnitCards));
    }

    if (BnStartBattle != nullptr)
    {
        BnStartBattle->OnClicked.AddDynamic(this, &UTrainingMainWidget::OnStartBattleClicked);
    }

    if (BnBackToMenu != nullptr)
    {
        BnBackToMenu->OnClicked.AddDynamic(this, &UTrainingMainWidget::OnBackToMenuClicked);
    }
}

void UTrainingMainWidget::NativeDestruct()
{
    if (BnStartBattle != nullptr)
    {
        BnStartBattle->OnClicked.RemoveDynamic(this, &UTrainingMainWidget::OnStartBattleClicked);
    }

    if (BnBackToMenu != nullptr)
    {
        BnBackToMenu->OnClicked.RemoveDynamic(this, &UTrainingMainWidget::OnBackToMenuClicked);
    }

    Super::NativeDestruct();
}

void UTrainingMainWidget::ApplyOverlayHitBounds()
{
    SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    SetCanvasSlotAutoSize(BnSetupArmy);
    SetCanvasSlotAutoSize(BnStartBattle);
    SetCanvasSlotAutoSize(BnBackToMenu);
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
