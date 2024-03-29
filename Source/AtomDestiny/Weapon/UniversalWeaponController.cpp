﻿#include "UniversalWeaponController.h"

#include <AtomDestiny/Core/ObjectPool/ActorPool.h>
#include <AtomDestiny/Core/ActorComponentUtils.h>

#include <AtomDestiny/Projectile/Projectile.h>
#include <AtomDestiny/Particle/Particle.h>

#include <AtomDestiny/Core/Logger.h>

UUniversalWeaponController::UUniversalWeaponController(const FObjectInitializer& objectInitializer):
    UWeaponBase(objectInitializer)
{
    SetTickEnabled(true);
}

double UUniversalWeaponController::GetFireRate() const
{
    return m_shotCount * m_shootingPositions.Num() / (m_shotDelay * (m_shotCount * m_shootingPositions.Num() - 1) + m_reloadTime);
}

bool UUniversalWeaponController::IsSeeTarget() const
{
    if (!m_target.IsValid())
    {
        return false;
    }
    
    if (!m_useRaycast)
    {
        return true;
    }
    
    if (!m_scanPosition.IsValid())
    {
        bool isTargetAtSight = false;
        
        for (const auto& position : m_shootingPositions)
        {
            isTargetAtSight = CheckRaycastToTarget(position->GetComponentLocation(), m_target);
        }

        return isTargetAtSight;
    }
    
    return CheckRaycastToTarget(m_scanPosition->GetComponentLocation(), m_target);
}

void UUniversalWeaponController::Fire(float deltaTime)
{
    if (!m_target.IsValid() || m_forceFireDisable)
    {
        return;
    }

    const bool isValidShotDistance = (m_target->GetActorLocation() - m_weaponComponent->GetComponentLocation()).SquaredLength() > m_minShotSqrtDistance;
    
    if (m_rotatedWeapon)
    {
        RotateToTarget(deltaTime);

        if (!m_firing && isValidShotDistance && m_isRotatedOnTarget)
        {
            RunCoroutine(MakeShot());
        }
    }
    else
    {
        if (m_weaponAnimation != nullptr && !m_weaponAnimation->IsReady())
        {
            return;
        }
        
        if (!m_firing && isValidShotDistance)
        {
            RunCoroutine(MakeShot());
        }
    }
}

void UUniversalWeaponController::BeginPlay()
{
    Super::BeginPlay();
    AtomDestiny::ObjectPool::Instance().Preload(m_shotParticlePrefab);
}

void UUniversalWeaponController::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);
    RotateToRoot(deltaTime);
}

FAsyncCoroutine UUniversalWeaponController::MakeShot()
{
    m_firing = true;
    const TWeakObjectPtr<AActor> currentEnemy = m_target; // we need to store copy because of coroutines
    
    for (int32 i = 0; i < m_shotCount; ++i)
    {
        for (int32 shootingIndex = 0; shootingIndex < m_shootingPositions.Num(); ++shootingIndex)
        {
            if (!currentEnemy.IsValid() || !IsValid(m_projectilePrefab))
            {
                co_await FiringDelay();
                co_return;
            }

            const USceneComponent* shotPosition = m_shootingPositions[shootingIndex];
            const FVector onTarget = currentEnemy->GetActorLocation() - shotPosition->GetComponentLocation();
            FHitResult hitResult;
            
            if (CheckRaycastToTarget(shotPosition->GetComponentLocation(), currentEnemy, &hitResult))
            {
                TWeakObjectPtr<AActor> spawnedProjectile = AtomDestiny::ObjectPool::Instance().Spawn(m_projectilePrefab.GetDefaultObject(),
                    shotPosition->GetComponentLocation(), shotPosition->GetComponentQuat());
                
                const auto projectile = Cast<IProjectile>(spawnedProjectile.Get());

                if (projectile == nullptr)
                {
                    LOG_WARNING(TEXT("Projectile prefab does not realize IProjectile interface"));

                    co_await FiringDelay();
                    co_return;
                }

                projectile->SetPoints(FProjectilePoints{
                    .startPosition = shotPosition->GetComponentLocation(),
                    .endPosition = currentEnemy->GetActorLocation(),
                    .impactPosition = hitResult.Location,
                    .normal = -onTarget
                });

                projectile->SetParameters(GetParameters());
                projectile->Launch();

                if (m_useProjectileAsChild)
                {
                    spawnedProjectile->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
                }
                
                if (m_weaponAnimation != nullptr)
                {
                    m_weaponAnimation->Animate();
                }

                AtomDestiny::ObjectPool::Instance().Spawn(m_shotParticlePrefab, shotPosition->GetComponentLocation(), shotPosition->GetComponentQuat());
            }
            
            if (shootingIndex < m_shootingPositions.Num() - 1)
            {
                co_await Coroutines::Latent::Seconds(m_delayBetweenShots);
            }
        }

        if (i < (m_shotCount - 1))
        {
            co_await Coroutines::Latent::Seconds(m_shotDelay);
        }
    }

    co_await FiringDelay();
}
