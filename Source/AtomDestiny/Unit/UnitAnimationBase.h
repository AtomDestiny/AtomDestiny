// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Components/ActorComponent.h>

#include <AtomDestiny/Core/Macros.h>
#include "AtomDestiny/Unit/Animation.h"

#include "UnitAnimationBase.generated.h"

UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitAnimationBase : public UActorComponent, public IAnimation
{
    GENERATED_BODY()

public:
    explicit UUnitAnimationBase(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

protected:
    void InitializeComponent() override;
    void Idle() ABSTRACT_METHOD;
    void Walk() ABSTRACT_METHOD;
    void Attack() ABSTRACT_METHOD;

    // Skeletal mesh that contains an animator
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Skeletal mesh with animation"))
    TWeakObjectPtr<USkeletalMeshComponent> m_skeletalMeshComponent;
};
