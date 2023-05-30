// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/FloorGrid.h"

#include "Core/Logger.h"


AFloorGrid::AFloorGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	m_root = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Root"));
	RootComponent = m_root;
	
	m_procMesh = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("Grid"));
	m_procMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	m_plane = ObjectInitializer.CreateDefaultSubobject<UProceduralMeshComponent>(this, TEXT("Plane"));
	m_plane->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	m_indices.Add(0);
	m_indices.Add(1);
	m_indices.Add(2);
	m_indices.Add(0);
	m_indices.Add(2);
	m_indices.Add(3);
}

// Called when the game starts or when spawned
void AFloorGrid::BeginPlay()
{
	Super::BeginPlay();
}

void AFloorGrid::CreateLine(int idx, FVector basePt, int width, int length, ELineAlignment alg)
{
	m_vertices.Empty();
	
	if (alg == ELineAlignment::XAligned)
	{
		m_vertices.Add(FVector(0,0,0) + basePt);
		m_vertices.Add(FVector(0,width,0) + basePt);
		m_vertices.Add(FVector(length,width,0) + basePt);
		m_vertices.Add(FVector(length,0,0) + basePt);
	}
	else
	{
		m_vertices.Add(FVector(0,0,0) + basePt);
		m_vertices.Add(FVector(0,length,0) + basePt);
		m_vertices.Add(FVector(width,length,0) + basePt);
		m_vertices.Add(FVector(width,0,0) + basePt);
	}
	
	m_procMesh->CreateMeshSection(
		idx, m_vertices, m_indices,
		TArray<FVector>(), m_UV0,
		TArray<FColor>(), TArray<FProcMeshTangent>(), false
		);
}

void AFloorGrid::CreatePlane(FVector basePt, int width, int height)
{
	TArray<FVector> planeVertices;
	planeVertices.Add(FVector(0,0,0) + basePt);
	planeVertices.Add(FVector(0,width,0) + basePt);
	planeVertices.Add(FVector(height,width,0) + basePt);
	planeVertices.Add(FVector(height,0,0) + basePt);
	
	m_plane->CreateMeshSection(
		0, planeVertices, m_indices,
		TArray<FVector>(), m_UV0,
		TArray<FColor>(), TArray<FProcMeshTangent>(), true
		);

	m_plane->OnBeginCursorOver.AddDynamic(this, &AFloorGrid::BeginCursorOver);

	m_plane->SetVisibility(false);
}

void AFloorGrid::BeginCursorOver(UPrimitiveComponent*)
{
	LOG_INFO(TEXT("FloorGrid: Hello!"));
}

// Called every frame
void AFloorGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFloorGrid::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	ConstructMesh();
}

void AFloorGrid::ConstructMesh()
{
	/// Grid
	m_procMesh->ClearAllMeshSections();
	int i = 0;

	float x = 0;
	FVector curBase {0,0,0};
	while (x <= m_sizeX)
	{
		CreateLine(i, curBase, m_lineWidth, m_sizeY, ELineAlignment::YAligned);
		++i;
		x += m_cellSize;
		curBase.X = x; 
	}

	curBase = FVector(0,0,0);
	float y = 0;
	
	while (y <= m_sizeY)
	{
		CreateLine(i, curBase, m_lineWidth, m_sizeX, ELineAlignment::XAligned);
		++i;
		y += m_cellSize;
		curBase.Y = y; 
	}
	
	for (int j = 0; j < i; ++j)
		m_procMesh->SetMaterial(j, m_material);
	
	m_procMesh->SetCastShadow(false);

	/// Plane

	curBase = FVector(0,0,0);
	CreatePlane(curBase, m_sizeY, m_sizeX);
}
