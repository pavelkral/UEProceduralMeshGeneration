// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomActor.h"

// Sets default values
ACustomActor::ACustomActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("My Static Mesh"));

	//UStaticMeshComponent* Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = SuperMesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder"));

	if (CylinderAsset.Succeeded())
	{
		SuperMesh->SetStaticMesh(CylinderAsset.Object);
		SuperMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
		SuperMesh->SetWorldScale3D(FVector(1.f));
	}
}

// Called when the game starts or when spawned
void ACustomActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACustomActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

