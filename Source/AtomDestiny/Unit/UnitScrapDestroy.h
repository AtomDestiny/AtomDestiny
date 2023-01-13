#pragma once

#include <AtomDestiny/Behaviour/DestroyBase.h>

#include "UnitScrapDestroy.generated.h"

///
/// Destroys unit with scrap.
/// It spawns explosion, than spawns scrap with added force.
/// Despawns explosion, scrap and destroys driven unit
/// 
UCLASS(ClassGroup=(AtomDestiny), Blueprintable)
class ATOMDESTINY_API UUnitScrapDestroy : public UDestroyBase
{
    GENERATED_BODY()

public:
    explicit UUnitScrapDestroy(const FObjectInitializer& objectInitializer = FObjectInitializer::Get());

    virtual void BeginPlay() override;
    virtual void Destroy() override;
    
private:
    
    // Rigidbody min explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Min explosion power", AllowPrivateAccess = "true"))
    double m_minExplosionPower = 100.0;
    
    // Rigidbody max explosion power. Random value between min and max values
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Max explosion power", AllowPrivateAccess = "true"))
    double m_maxExplosionPower = 500.0;
    
    // Rigidbody explosion radius
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Explosion radius", AllowPrivateAccess = "true"))
    double m_explosionRadius = 25.0;
    
    // Destroy time for parts after explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scrap parts destroy time", AllowPrivateAccess = "true"))
    double m_partsDestroyTime = 1.5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Scrap blueprint", AllowPrivateAccess = "true"))
    TSubclassOf<AActor> m_scrapBlueprint;
    uint32_t ScrapPreloadCount = 3;
};
