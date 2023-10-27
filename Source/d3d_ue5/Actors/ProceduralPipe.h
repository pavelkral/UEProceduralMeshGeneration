// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "ProceduralPipe.generated.h"

UCLASS()
class D3D_UE5_API AProceduralPipe : public AActor
{
	GENERATED_BODY()
	
public:	
	AProceduralPipe();
private:

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	int32 NumSegments;

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Pipe Settings")
	float SegmentLength;

	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* PMesh;

	void OnConstruction(const FTransform& Transform);

	void GeneratePipeMesh();
protected:

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
