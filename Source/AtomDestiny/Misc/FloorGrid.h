// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
//#include "StaticmeshcompoPrimitiveComponent.h"
//#include "StaticmeshComponent/PlaneShapeComponent.h"
#include "FloorGrid.generated.h"

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
	
	UPROPERTY(EditAnywhere, meta=(DisplayName = "Material"))
	UMaterialInterface* m_material;

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateLine(int idx, FVector basePt, int width, int length, ELineAlignment alg);

	void CreatePlane(FVector basePt, int width, int height);

	UFUNCTION(BlueprintCallable)
	void BeginCursorOver(UPrimitiveComponent* comp);
	
private:
	UStaticMeshComponent *m_root;
	
	UProceduralMeshComponent* m_procMesh;

	//UPROPERTY(EditAnywhere)
	UProceduralMeshComponent *m_plane;

	TArray<FVector> m_vertices;
	
	TArray<int> m_indices;
	
	TArray<FVector2D> m_UV0;
	
};
