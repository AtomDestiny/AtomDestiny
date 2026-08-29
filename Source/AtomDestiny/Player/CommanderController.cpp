#include "CommanderController.h"

#include "AtomDestinyGameStateBase.h"
#include "AtomDestiny/Gameplay/UnitStorage.h"
#include "AtomDestiny/Core/ObjectPool/Despawner.h"
#include "AtomDestiny/Core/ActorComponentUtils.h"
#include "AtomDestiny/Logic/Logic.h"
#include "AtomDestiny/Logic/UnitLogic.h"
#include "AtomDestiny/Unit/UnitState.h"
#include "AtomDestiny/Unit/UnitSideColorDetails.h"
#include "Misc/FloorGrid.h"
#include "Misc/PlacementPointer.h"
#include "UI/TrainingMainWidget.h"

#include "GameFramework/Pawn.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EngineUtils.h"
#include "TimerManager.h"
#include "UObject/UObjectIterator.h"

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

    m_actionEndSetupArmy = NewObject<UInputAction>(this);
    m_actionEndSetupArmy->ValueType = EInputActionValueType::Boolean;

    m_actionRClick = NewObject<UInputAction>(this);
    m_actionRClick->ValueType = EInputActionValueType::Boolean;

    mapKey(m_pawnMappingContext, m_actionLClick, EKeys::LeftMouseButton);
    mapKey(m_pawnMappingContext, m_actionRClick, EKeys::RightMouseButton);

    mapKey(m_pawnMappingContext, m_actionReset, EKeys::R);

    mapKey(m_pawnMappingContext, m_actionMove, EKeys::E);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::Q, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::D, false, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::A, true, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::W, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::S, true, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollUp, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollDown, true, true, EInputAxisSwizzle::ZYX);

    mapKey(m_pawnMappingContext, m_actionEndSetupArmy, EKeys::SpaceBar);

    if (EnableMouseLook)
    {
        //mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseY);
        mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseY,
            false, false, EInputAxisSwizzle::YXZ, true, m_actionLClick);
        //mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseX, false, true);
        mapKey(m_pawnMappingContext, m_actionLook, EKeys::MouseX, false, true,
           EInputAxisSwizzle::YXZ, true, m_actionLClick);
    }
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

void ACommanderController::ClearSetupUnits()
{
    for (const TWeakObjectPtr<APawn>& weakPawn : m_setupPlacedUnits)
    {
        APawn* pawn = weakPawn.Get();
        if (pawn == nullptr)
        {
            continue;
        }

        if (UDespawner* despawner = pawn->FindComponentByClass<UDespawner>())
        {
            despawner->ClearDespawnTimer();
        }
    }

    // Do not Destroy() here: OpenLevel unloads the Training map and removes actors.
    m_setupPlacedUnits.Empty();
}

void ACommanderController::ClearLevelDespawnTimers() const
{
    UWorld* world = GetWorld();
    if (world == nullptr)
    {
        return;
    }

    for (TObjectIterator<UDespawner> it; it; ++it)
    {
        if (it->GetWorld() != world)
        {
            continue;
        }

        it->ClearDespawnTimer();
    }
}

void ACommanderController::TryFinishArmySetup()
{
    if (!m_bArmySetupActive || !m_trainingWidget.IsValid())
    {
        return;
    }

    m_trainingWidget->EndArmySetup();
}

void ACommanderController::OnSetupArmyModeChanged(bool setupArmy)
{
    m_bArmySetupActive = setupArmy;

    if (UWorld* world = GetWorld())
    {
        for (TActorIterator<AFloorGrid> it(world); it; ++it)
        {
            it->SetupVisibility(setupArmy);
        }
    }

    if (setupArmy)
    {
        return;
    }

    if (m_placementPointer != nullptr)
    {
        m_placementPointer->HidePointer();
    }

    TArray<TWeakObjectPtr<APawn>> unitsToActivate = MoveTemp(m_setupPlacedUnits);
    m_setupPlacedUnits.Empty();

    if (unitsToActivate.Num() == 0 || GetWorld() == nullptr)
    {
        return;
    }

    GetWorld()->GetTimerManager().SetTimerForNextTick(
        FTimerDelegate::CreateWeakLambda(this, [unitsToActivate]()
        {
            for (const TWeakObjectPtr<APawn>& weakPawn : unitsToActivate)
            {
                APawn* pawn = weakPawn.Get();
                if (pawn == nullptr)
                {
                    continue;
                }

                if (UUnitLogic* logic = pawn->FindComponentByClass<UUnitLogic>())
                {
                    logic->ActivateAfterSetup();
                }
            }
        }));
}

bool ACommanderController::IsGridPointerActive() const
{
    return m_bArmySetupActive;
}

bool ACommanderController::TryGetGridCellUnderCursor(AFloorGrid*& outGrid, FVector& outCellCenter) const
{
    outGrid = nullptr;

    FHitResult hit;
    if (!GetHitResultUnderCursor(ECC_Visibility, false, hit))
    {
        return false;
    }

    outGrid = Cast<AFloorGrid>(hit.GetActor());
    if (outGrid == nullptr)
    {
        return false;
    }

    outCellCenter = outGrid->SnapWorldLocationToCellCenter(hit.Location);
    return true;
}

bool ACommanderController::ProjectToGround(const FVector& cellCenter, FVector& outGroundLocation) const
{
    const FVector traceStart = cellCenter + FVector(0.f, 0.f, 5000.f);
    const FVector traceEnd = cellCenter - FVector(0.f, 0.f, 5000.f);

    FHitResult hit;
    FCollisionQueryParams params(SCENE_QUERY_STAT(UnitPlacementGround), false, GetPawn());
    if (m_placementPointer != nullptr)
    {
        params.AddIgnoredActor(m_placementPointer);
    }

    if (!GetWorld()->LineTraceSingleByChannel(hit, traceStart, traceEnd, ECC_WorldStatic, params))
    {
        return false;
    }

    outGroundLocation = hit.ImpactPoint;
    return true;
}

FRotator ACommanderController::ComputeFacingRotation(const FVector& location, const EGameSide placementSide) const
{
    const AAtomDestinyGameStateBase* gameState = GetWorld()->GetGameState<AAtomDestinyGameStateBase>();
    if (gameState == nullptr)
    {
        return FRotator::ZeroRotator;
    }

    const AActor* rallyPoint = gameState->GetRallyPoint(placementSide);
    if (rallyPoint == nullptr)
    {
        return FRotator::ZeroRotator;
    }

    FVector direction = rallyPoint->GetActorLocation() - location;
    direction.Z = 0.f;
    if (direction.IsNearlyZero())
    {
        return FRotator::ZeroRotator;
    }

    FRotator rotation = direction.Rotation();
    rotation.Yaw = FMath::GridSnap(rotation.Yaw, 90.f);
    rotation.Pitch = 0.f;
    rotation.Roll = 0.f;
    return rotation;
}

void ACommanderController::AlignUnitGroundPoint(APawn* pawn, const FVector& groundLocation) const
{
    if (pawn == nullptr)
    {
        return;
    }

    const UUnitState* unitState = pawn->FindComponentByClass<UUnitState>();
    if (unitState == nullptr)
    {
        pawn->SetActorLocation(groundLocation);
        return;
    }

    const TWeakObjectPtr<USceneComponent> groundPoint = unitState->GetGroundPoint<USceneComponent>();
    if (!groundPoint.IsValid())
    {
        pawn->SetActorLocation(groundLocation);
        return;
    }

    const FVector offset = groundLocation - groundPoint->GetComponentLocation();
    pawn->SetActorLocation(pawn->GetActorLocation() + offset);
}

void ACommanderController::TryPlaceUnitAtCursor()
{
    if (!IsGridPointerActive() || m_trainingWidget == nullptr)
    {
        return;
    }

    const EADUnitType unitType = m_trainingWidget->GetSelectedUnitType();
    if (unitType == EADUnitType::None)
    {
        return;
    }

    AFloorGrid* grid = nullptr;
    FVector cellCenter = FVector::ZeroVector;
    if (!TryGetGridCellUnderCursor(grid, cellCenter))
    {
        return;
    }

    const EGameSide placementSide = grid->GetSide();

    FVector groundLocation = FVector::ZeroVector;
    if (!ProjectToGround(cellCenter, groundLocation))
    {
        return;
    }

    const AtomDestiny::UnitStorage& storage = AtomDestiny::UnitStorage::Instance();
    const TOptional<FUnitInfo> unitInfo = storage.GetInfo(unitType);
    if (!unitInfo.IsSet() || unitInfo->prefab == nullptr)
    {
        return;
    }

    const FRotator facingRotation = ComputeFacingRotation(groundLocation, placementSide);

    FTransform spawnTransform(facingRotation, groundLocation);
    APawn* pawn = GetWorld()->SpawnActorDeferred<APawn>(
        unitInfo->prefab,
        spawnTransform,
        this,
        nullptr,
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

    if (pawn == nullptr)
    {
        return;
    }

    if (const TScriptInterface<ILogic> logic = AtomDestiny::Utils::GetInterface<ILogic>(pawn))
    {
        logic->SetSide(placementSide);
    }

    if (UUnitLogic* unitLogic = pawn->FindComponentByClass<UUnitLogic>())
    {
        unitLogic->PrepareForSetupPlacement();
    }

    pawn->FinishSpawning(spawnTransform);
    AlignUnitGroundPoint(pawn, groundLocation);
    pawn->SetActorRotation(facingRotation);

    if (UUnitSideColorDetails* sideColorDetails = pawn->FindComponentByClass<UUnitSideColorDetails>())
    {
        sideColorDetails->ApplyForSide(placementSide);
    }

    m_setupPlacedUnits.Add(pawn);
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

    AFloorGrid* grid = nullptr;
    FVector cellCenter = FVector::ZeroVector;
    if (!TryGetGridCellUnderCursor(grid, cellCenter))
    {
        m_placementPointer->HidePointer();
        return;
    }

    m_placementPointer->ShowAt(cellCenter);
}

void ACommanderController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    UpdatePlacementPointer();
}
