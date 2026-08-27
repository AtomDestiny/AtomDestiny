// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "AtomDestiny/AtomDestiny.h"

#include "FloorGrid.generated.h"

class UStaticMeshComponent;
class UMaterialInstanceDynamic;

enum class ELineAlignment
{
    XAligned, YAligned
};

UCLASS()
class ATOMDESTINY_API AFloorGrid : public AActor
{
    GENERATED_BODY()

public:
    AFloorGrid(const FObjectInitializer& ObjectInitializer);

    virtual void Tick(float DeltaTime) override;

    virtual void OnConstruction(const FTransform& Transform) override;

    UFUNCTION(BlueprintCallable)
    void ConstructMesh();

    /** World location of the center of a grid cell. */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    FVector GetCellCenterWorld(int32 cellX, int32 cellY) const;

    /** Snap a world hit location to the nearest in-bounds cell center. */
    FVector SnapWorldLocationToCellCenter(const FVector& worldLocation, int32* outCellX = nullptr, int32* outCellY = nullptr) const;

    /** Convert world location to cell indices; returns false if outside the grid. */
    UFUNCTION(BlueprintCallable, BlueprintPure)
    bool WorldToCell(const FVector& worldLocation, int32& outCellX, int32& outCellY) const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int32 GetCellCountX() const;

    UFUNCTION(BlueprintCallable, BlueprintPure)
    int32 GetCellCountY() const;

    /** Show or hide grid visuals and placement collision (Training setup mode). */
    UFUNCTION(BlueprintCallable)
    void SetupVisibility(bool visible);

    UFUNCTION(BlueprintCallable, BlueprintPure)
    EGameSide GetSide() const { return m_side; }

    UProceduralMeshComponent* GetInteractionPlane() const { return m_plane; }

    UPROPERTY(EditAnywhere, meta=(DisplayName = "Material"))
    UMaterialInterface* m_material;

    /** When enabled, grid lines use team color from Conflict Sides settings. */
    UPROPERTY(EditAnywhere, meta=(DisplayName = "Use side team color"))
    bool m_useSideTeamColor = true;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "Grid color", EditCondition = "!m_useSideTeamColor"))
    FLinearColor m_gridColor = FLinearColor(0.1f, 0.85f, 0.2f, 1.f);

    UPROPERTY(EditAnywhere, meta=(DisplayName = "Side"))
    EGameSide m_side = EGameSide::Rebels;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "X size"))
    int m_sizeX = 1000;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "Y size"))
    int m_sizeY = 500;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "CellSize"))
    int m_cellSize = 1000;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "NumCells"))
    int m_numCells = 10;

    UPROPERTY(EditAnywhere, meta=(DisplayName = "LineWidth"))
    float m_lineWidth = 10;

protected:
    virtual void BeginPlay() override;

    void CreateLine(int idx, FVector basePt, int width, int length, ELineAlignment alg);

    void CreatePlane(FVector basePt, int width, int height);

    void ConfigureInteractionPlane();

    void ApplyGridMaterial();

    void SyncGridColorFromSide();

private:
    UStaticMeshComponent* m_root;

    UProceduralMeshComponent* m_procMesh;

    UProceduralMeshComponent* m_plane;

    UPROPERTY(Transient)
    TObjectPtr<UMaterialInstanceDynamic> m_gridMaterialInstance;

    TArray<FVector> m_vertices;

    TArray<int> m_indices;

    TArray<FVector2D> m_UV0;
};
