#include "CommanderController.h"

//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"

#include "InputAction.h"
#include "InputMappingContext.h"

static void mapKey(UInputMappingContext* context, UInputAction* action, FKey key,
    bool isNegate = false, bool isSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ)
{
    auto& mapping = context->MapKey(action, key);

    UObject* outer = context->GetOuter();

    if (isNegate)
    {
        auto* negate = NewObject<UInputModifierNegate>(outer);
        mapping.Modifiers.Add(negate);
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

    m_actionRotate = NewObject<UInputAction>(this);
    m_actionRotate->ValueType = EInputActionValueType::Axis3D;

    m_actionReset = NewObject<UInputAction>(this);
    m_actionReset->ValueType = EInputActionValueType::Boolean;

    mapKey(m_pawnMappingContext, m_actionReset, EKeys::R);

    mapKey(m_pawnMappingContext, m_actionMove, EKeys::W);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::S, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::A, false, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::D, true, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::SpaceBar, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::LeftControl, true, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollUp, false, true, EInputAxisSwizzle::ZYX);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::MouseScrollDown, true, true, EInputAxisSwizzle::ZYX);

    if (EnableMouseLook)
    {
        mapKey(m_pawnMappingContext, m_actionRotate, EKeys::MouseY);
        mapKey(m_pawnMappingContext, m_actionRotate, EKeys::MouseX, true, true, EInputAxisSwizzle::ZYX);
    }

    mapKey(m_pawnMappingContext, m_actionRotate, EKeys::Q, false, true);
    mapKey(m_pawnMappingContext, m_actionRotate, EKeys::E, true, true);
}
