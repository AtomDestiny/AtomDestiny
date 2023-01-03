#include "CommanderController.h"

//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"

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
