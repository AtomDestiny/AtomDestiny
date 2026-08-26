#include "UI/UnitCardWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/ListView.h"
#include "Engine/Texture2D.h"
#include "Shader/Preshader2.h"

void UUnitCardWidget::SetTitle(const FString& title)
{
    m_title = title;

    if (Text != nullptr)
    {
        Text->SetText(FText::FromString(title));
    }
}

void UUnitCardWidget::SetIcon(const TSoftObjectPtr<UTexture2D>& icon)
{
    ResolveWidgetReferences();

    if (CardImage == nullptr)
    {
        return;
    }

    if (icon.IsNull())
    {
        CardImage->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    UTexture2D* texture = icon.LoadSynchronous();
    if (texture == nullptr)
    {
        CardImage->SetVisibility(ESlateVisibility::Collapsed);
        return;
    }

    CardImage->SetBrushFromTexture(texture, true);
    CardImage->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UUnitCardWidget::UpdateAppearance(const bool bIsActive) const
{
    if (!CardBorder)
        return;

    const FLinearColor BorderColor = bIsActive ? FLinearColor::Yellow : FLinearColor::Gray;
    CardBorder->SetBrushColor(BorderColor);
}

void UUnitCardWidget::ResolveWidgetReferences()
{
    if (WidgetTree == nullptr || CardImage != nullptr)
    {
        return;
    }

    CardImage = Cast<UImage>(WidgetTree->FindWidget(FName("CardImage")));
}

void UUnitCardWidget::NativePreConstruct()
{
    Super::NativePreConstruct();
    SetTitle(m_title);
    UpdateAppearance(false);
}

void UUnitCardWidget::NativeOnItemSelectionChanged(bool bIsSelected)
{
    IUserObjectListEntry::NativeOnItemSelectionChanged(bIsSelected);

    UpdateAppearance(bIsSelected);
}

void UUnitCardWidget::NativeOnListItemObjectSet(UObject* listItemObject)
{
    const UUnitCardItem* Item = Cast<UUnitCardItem>(listItemObject);
    if (Item == nullptr)
    {
        return;
    }

    m_unitType = Item->type;
    SetTitle(Item->title.ToString());
    SetIcon(Item->icon);
}

UListView* UUnitCardWidget::GetOwningListView() const
{
    return Cast<UListView>(GetOuter());
}
