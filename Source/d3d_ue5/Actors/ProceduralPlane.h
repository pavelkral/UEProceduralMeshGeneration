// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralPlane.generated.h"



UCLASS()
class D3D_UE5_API AProceduralPlane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralPlane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;

private:
	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* PipeMesh;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* ProceduralMesh;

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	int32 NumSegments;

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	float SegmentLength;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int XSize = 5;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	int YSize = 5;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float ZMultiplier = 1.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float NoiseScale = 1.0f;

	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float Scale = 100.0f;
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.000001))
	float UVScale = 0.0f;

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;


	void OnConstruction(const FTransform& Transform);

	void GeneratePipeMesh();
	void GeneratePlaneMesh();


public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
