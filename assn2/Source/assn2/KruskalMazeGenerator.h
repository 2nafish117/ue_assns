// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KruskalMazeGenerator.generated.h"

UCLASS()
class ASSN2_API AKruskalMazeGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	AKruskalMazeGenerator();

private:
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

};
