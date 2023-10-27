// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralPlane.h"
//#include "ProceduralMeshComponent.h"

// Sets default values
AProceduralPlane::AProceduralPlane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
    // Create the procedural mesh component.
    PlaneMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("PipeMesh"));
    //;RootComponent = PipeMesh;
    PlaneMesh->SetupAttachment(GetRootComponent());


}

// Called when the game starts or when spawned
void AProceduralPlane::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AProceduralPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProceduralPlane::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    Vertices.Empty();
    Triangles.Empty();
	PlaneMesh->ClearAllMeshSections();
	GeneratePlaneMesh();

}


void AProceduralPlane::GeneratePlaneMesh()
{
  

    for (int X = 0; X <= XSize; ++X)
    {
        for (int Y = 0; Y <= YSize; ++Y)
        {
            float Z = FMath::PerlinNoise2D(FVector2D(X * NoiseScale + 0.1, Y * NoiseScale + 0.1)) * ZMultiplier;
            Vertices.Add(FVector(X * Scale, Y * Scale, Z));
            UV0.Add(FVector2D(X * UVScale, Y * UVScale));
        }
    }

    int Vertex = 0;

    for (int X = 0; X < XSize; ++X)
    {
        for (int Y = 0; Y < YSize; ++Y)
        {
            Triangles.Add(Vertex);//Bottom left corner
            Triangles.Add(Vertex + 1);//Bottom right corner
            Triangles.Add(Vertex + YSize + 1);//Top left corner
            Triangles.Add(Vertex + 1);//Bottom right corner
            Triangles.Add(Vertex + YSize + 2);//Top right corner
            Triangles.Add(Vertex + YSize + 1);//Top left corner

            ++Vertex;
        }
        ++Vertex;
    }

    PlaneMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
    PlaneMesh->SetMaterial(0, Material);
}
