// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralPipe.h"

// Sets default values
AProceduralPipe::AProceduralPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
    PrimaryActorTick.bCanEverTick = false;
    // Create the procedural mesh component.
    PMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PipeMesh"));
    //;RootComponent = PipeMesh;
    PMesh->SetupAttachment(GetRootComponent());

    // Set default values for pipe parameters.
    NumSegments = 10;
    Radius = 100.0f;
    SegmentLength = 100.0f;

}
// Called when the game starts or when spawned
void AProceduralPipe::BeginPlay()
{
    Super::BeginPlay();

}

// Called every frame
void AProceduralPipe::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}
void AProceduralPipe::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    Vertices.Empty();
    Triangles.Empty();
    PMesh->ClearAllMeshSections();
    GeneratePipeMesh();

    //GenerateCylinder();
    //GeneratePipeMesh();
}

void AProceduralPipe::GeneratePipeMesh()
{
    if (NumSegments <= 0 || Radius <= 0 || SegmentLength <= 0)
    {
        return;
    }


    for (int32 Segment = 0; Segment < NumSegments; Segment++)
    {
        float Angle = 2.0f * PI * (float)Segment / (float)NumSegments;
        float z = 0.0f;

        FVector StartTop = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, SegmentLength);
        FVector StartBottom = FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
        FVector EndTop = FVector(FMath::Cos(Angle + 2.0f * PI / NumSegments) * Radius, FMath::Sin(Angle + 2.0f * PI / NumSegments) * Radius, SegmentLength);
        FVector EndBottom = FVector(FMath::Cos(Angle + 2.0f * PI / NumSegments) * Radius, FMath::Sin(Angle + 2.0f * PI / NumSegments) * Radius, 0.0f);

        // Add vertices
        Vertices.Add(StartTop);
        Vertices.Add(StartBottom);
        Vertices.Add(EndTop);
        Vertices.Add(EndBottom);

        // Add triangles
        int32 BaseIndex = Segment * 4;
        Triangles.Add(BaseIndex);
        Triangles.Add(BaseIndex + 1);
        Triangles.Add(BaseIndex + 2);
        Triangles.Add(BaseIndex + 2);
        Triangles.Add(BaseIndex + 1);
        Triangles.Add(BaseIndex + 3);
    }

    // Create the mesh section
    PMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
    PMesh->SetMaterial(0, Material);
}



