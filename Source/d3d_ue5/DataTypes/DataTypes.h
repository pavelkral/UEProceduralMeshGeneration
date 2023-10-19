// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes.generated.h"
/**
 * 
 */
#define SCREEN_LOG(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT(text), fstring))

UENUM(BlueprintType)
enum class EUnitState : uint8
{
    Idle UMETA(DisplayName = "Idle"),
    Walking UMETA(DisplayName = "Walking"),
    Running UMETA(DisplayName = "Running")
};

USTRUCT(BlueprintType)
struct FUnitStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
    float UnitHealth;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
    float UniId;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MyCategory)
    EUnitState state;

};