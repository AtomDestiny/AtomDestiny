#pragma once

#include <AtomDestiny/Behaviour/DestroyBase.h>

#include "UnitScrapDestroy.generated.h"

///
/// Destroys unit with scrap.
/// It spawns explosion, than spawns scrap with added force.
/// Des-pawns explosion, scrap and destroys driven unit
/// 
UCLASS(ClassGroup=(AtomDestiny), meta=(BlueprintSpawnableComponent))
class ATOMDESTINY_API UUnitScrapDestroy final : public UDestroyBase
{
    GENERATED_BODY()

public:
    explicit UUnitScrapDestroy(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void Destroy() override;
    
private:
    
    // Rigid body min explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Min explosion power", AllowPrivateAccess = "true"))
    double m_minExplosionPower = 1000.0;
    
    // Rigid body max explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max explosion power", AllowPrivateAccess = "true"))
    double m_maxExplosionPower = 3000.0;
    
    // Rigid body explosion radius
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion radius", AllowPrivateAccess = "true"))
    double m_explosionRadius = 125.0;
    
    // Destroy time for parts after explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scrap parts destroy time", AllowPrivateAccess = "true"))
    double m_partsDestroyTime = 1.5;

    ///
    /// Scrap prefab should be represented by AActor and consists of exploded parts
    /// that emulates unit parts destruction.
    ///
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scrap prefab", AllowPrivateAccess = "true"))
    TSubclassOf<AActor> m_scrapPrefab;
    constexpr static uint32_t ScrapPreloadCount = 3;
};
