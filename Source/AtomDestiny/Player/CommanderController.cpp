#include "CommanderController.h"

#include "Misc/FloorGrid.h"
#include "Misc/PlacementPointer.h"
#include "UI/TrainingMainWidget.h"

#include "InputAction.h"
#include "InputMappingContext.h"

static void mapKey(UInputMappingContext* context, UInputAction* action, FKey key,
    bool isNegate = false, bool isSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ,
    bool isAddChord = false, UInputAction* chordAct = nullptr)
{
    auto& mapping = context->MapKey(action, key);

    UObject* outer = context->GetOuter();

    if (isNegate)
    {
        auto* negate = NewObject<UInputModifierNegate>(outer);
        mapping.Modifiers.Add(negate);
    }

    if (isAddChord)
    {
        auto *extTrig = NewObject<UInputTriggerChordAction>(outer);
        extTrig->ChordAction = chordAct;
        mapping.Triggers.Add(extTrig);
    }
    
    if (isSwizzle)
    {
        auto* swizzle = NewObject<UInputModifierSwizzleAxis>(outer);
        swizzle->Order = swizzleOrder;
        mapping.Modifiers.Add(swizzle);
    }
}

ACommanderController::ACommanderController() : APlayerController()
{
    bEnableMouseOverEvents = true;
    bEnableClickEvents = true;
    bShowMouseCursor = true;
    bShouldPerformFullTickWhenPaused = true;
}

void ACommanderController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    m_pawnMappingContext = NewObject<UInputMappingContext>(this);

    m_actionMove = NewObject<UInputAction>(this);
    m_actionMove->ValueType = EInputActionValueType::Axis3D;

    m_actionLook = NewObject<UInputAction>(this);
    m_actionLook->ValueType = EInputActionValueType::Axis2D;

    m_actionRoll = NewObject<UInputAction>(this);
    m_actionRoll->ValueType = EInputActionValueType::Axis1D;

    m_actionReset = NewObject<UInputAction>(this);
    m_actionReset->ValueType = EInputActionValueType::Boolean;

    m_actionLClick = NewObject<UInputAction>(this);
    m_actionLClick->ValueType = EInputActionValueType::Boolean;

    m_actionRClick = NewObject<UInputAction>(this);
    m_actionRClick->ValueType = EInputActionValueType::Boolean;

    mapKey(m_pawnMappingContext, m_actionLClick, EKeys::LeftMouseButton);
    mapKey(m_pawnMappingContext, m_actionRClick, EKeys::RightMouseButton);
    
    mapKey(m_pawnMappingContext, m_actionReset, EKeys::R);

    mapKey(m_pawnMappingContext, m_actionMove, EKeys::SpaceBar);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::LeftControl, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::D, false, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::A, true, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::W, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::S, true, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollUp, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollDown, true, true, EInputAxisSwizzle::ZYX);

    if (EnableMouseLook)
    {
        //mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseY);
        mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseY,
            false, false, EInputAxisSwizzle::YXZ, true, m_actionLClick);
        //mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseX, false, true);
        mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseX, false, true,
           EInputAxisSwizzle::YXZ, true, m_actionLClick);
    }

    mapKey(m_pawnMappingContext, m_actionRoll, EKeys::E, false);
    mapKey(m_pawnMappingContext, m_actionRoll, EKeys::Q, true);
}

void ACommanderController::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    m_placementPointer = GetWorld()->SpawnActor<APlacementPointer>(
        APlacementPointer::StaticClass(),
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        spawnParams);

    if (m_placementPointer != nullptr)
    {
        m_placementPointer->HidePointer();
    }
}

void ACommanderController::SetTrainingWidget(UTrainingMainWidget* widget)
{
    m_trainingWidget = widget;
}

bool ACommanderController::IsGridPointerActive() const
{
    return m_trainingWidget.IsValid() && m_trainingWidget->IsSetupArmyMode();
}

void ACommanderController::UpdatePlacementPointer()
{
    if (m_placementPointer == nullptr)
    {
        return;
    }

    if (!IsGridPointerActive())
    {
        m_placementPointer->HidePointer();
        return;
    }

    FHitResult hit;
    if (!GetHitResultUnderCursor(ECC_Visibility, false, hit))
    {
        m_placementPointer->HidePointer();
        return;
    }

    AFloorGrid* grid = Cast<AFloorGrid>(hit.GetActor());
    if (grid == nullptr)
    {
        m_placementPointer->HidePointer();
        return;
    }

    const FVector cellCenter = grid->SnapWorldLocationToCellCenter(hit.Location);
    m_placementPointer->ShowAt(cellCenter);
}

void ACommanderController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    UpdatePlacementPointer();
}
