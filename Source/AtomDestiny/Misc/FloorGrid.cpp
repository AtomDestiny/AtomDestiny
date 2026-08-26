#include "Misc/FloorGrid.h"

#include "Components/StaticMeshComponent.h"

AFloorGrid::AFloorGrid(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;

    m_root = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Root"));
    SetRootComponent(m_root);

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

void AFloorGrid::BeginPlay()
{
    Super::BeginPlay();
    ConfigureInteractionPlane();
}

int32 AFloorGrid::GetCellCountX() const
{
    if (m_cellSize <= 0)
    {
        return 0;
    }

    return FMath::Max(1, FMath::CeilToInt(static_cast<float>(m_sizeX) / m_cellSize));
}

int32 AFloorGrid::GetCellCountY() const
{
    if (m_cellSize <= 0)
    {
        return 0;
    }

    return FMath::Max(1, FMath::CeilToInt(static_cast<float>(m_sizeY) / m_cellSize));
}

bool AFloorGrid::WorldToCell(const FVector& worldLocation, int32& outCellX, int32& outCellY) const
{
    if (m_cellSize <= 0)
    {
        return false;
    }

    const FVector local = GetActorTransform().InverseTransformPosition(worldLocation);
    if (local.X < 0.f || local.Y < 0.f || local.X > m_sizeX || local.Y > m_sizeY)
    {
        return false;
    }

    outCellX = FMath::FloorToInt(local.X / m_cellSize);
    outCellY = FMath::FloorToInt(local.Y / m_cellSize);

    const int32 maxCellX = GetCellCountX() - 1;
    const int32 maxCellY = GetCellCountY() - 1;
    if (outCellX < 0 || outCellY < 0 || outCellX > maxCellX || outCellY > maxCellY)
    {
        return false;
    }

    return true;
}

FVector AFloorGrid::GetCellCenterWorld(int32 cellX, int32 cellY) const
{
    const FVector localCenter(
        cellX * m_cellSize + m_cellSize * 0.5f,
        cellY * m_cellSize + m_cellSize * 0.5f,
        0.f);

    return GetActorTransform().TransformPosition(localCenter);
}

FVector AFloorGrid::SnapWorldLocationToCellCenter(const FVector& worldLocation, int32* outCellX, int32* outCellY) const
{
    int32 cellX = 0;
    int32 cellY = 0;
    if (!WorldToCell(worldLocation, cellX, cellY))
    {
        if (outCellX != nullptr)
        {
            *outCellX = -1;
        }

        if (outCellY != nullptr)
        {
            *outCellY = -1;
        }

        return worldLocation;
    }

    if (outCellX != nullptr)
    {
        *outCellX = cellX;
    }

    if (outCellY != nullptr)
    {
        *outCellY = cellY;
    }

    return GetCellCenterWorld(cellX, cellY);
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

    m_plane->SetVisibility(false);
    ConfigureInteractionPlane();
}

void AFloorGrid::ConfigureInteractionPlane()
{
    if (m_plane == nullptr)
    {
        return;
    }

    m_plane->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    m_plane->SetCollisionResponseToAllChannels(ECR_Ignore);
    m_plane->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    m_plane->SetGenerateOverlapEvents(false);
}

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
    {
        m_procMesh->SetMaterial(j, m_material);
    }

    m_procMesh->SetCastShadow(false);

    curBase = FVector(0,0,0);
    CreatePlane(curBase, m_sizeY, m_sizeX);
}
