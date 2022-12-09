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
    m_actionMove->ValueType = EInputActionValueType::Axis2D;

    mapKey(m_pawnMappingContext, m_actionMove, EKeys::W);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::S, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::A, true, true);
    mapKey(m_pawnMappingContext, m_actionMove, EKeys::D, false, true);

    /*pawnMappingContext->MapKey(actionMove, EKeys::W);
    auto& mapping = pawnMappingContext->MapKey(actionMove, EKeys::S);
    UInputModifierNegate* negate = NewObject<UInputModifierNegate>(this);
    mapping.Modifiers.Add(negate);*/
}
