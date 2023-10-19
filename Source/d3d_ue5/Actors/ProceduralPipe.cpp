// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralPipe.h"

// Sets default values
AProceduralPipe::AProceduralPipe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

