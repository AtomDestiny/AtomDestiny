#include "CommanderController.h"

//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"
//#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputMappingContext.h"

#include "InputAction.h"
#include "InputMappingContext.h"

static void mapKey(UInputMappingContext* context, UInputAction* action, FKey key,
    bool bNegate = false, bool bSwizzle = false, EInputAxisSwizzle swizzleOrder = EInputAxisSwizzle::YXZ)
{
    auto& mapping = context->MapKey(action, key);

    UObject* outer = context->GetOuter();

    if (bNegate)
    {
        auto* negate = NewObject<UInputModifierNegate>(outer);
        mapping.Modifiers.Add(negate);
    }

    if (bSwizzle)
    {
        auto* swizzle = NewObject<UInputModifierSwizzleAxis>(outer);
        swizzle->Order = swizzleOrder;
        mapping.Modifiers.Add(swizzle);
    }
}

void ACommanderController::SetupInputComponent()
{
    Super::SetupInputComponent();

    pawnMappingContext = NewObject<UInputMappingContext>(this);

    actionMove = NewObject<UInputAction>(this);
    actionMove->ValueType = EInputActionValueType::Axis2D;

    mapKey(pawnMappingContext, actionMove, EKeys::W);
    mapKey(pawnMappingContext, actionMove, EKeys::S, true);
    mapKey(pawnMappingContext, actionMove, EKeys::A, true, true);
    mapKey(pawnMappingContext, actionMove, EKeys::D, false, true);

    /*pawnMappingContext->MapKey(actionMove, EKeys::W);
    auto& mapping = pawnMappingContext->MapKey(actionMove, EKeys::S);
    UInputModifierNegate* negate = NewObject<UInputModifierNegate>(this);
    mapping.Modifiers.Add(negate);*/
}
