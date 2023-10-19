// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "CustomStruct.generated.h"

/**
 * 
 */
UCLASS()
class D3D_UE5_API UCustomStruct : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};

USTRUCT(BlueprintType)
struct FCStruct 
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
    float test;
};