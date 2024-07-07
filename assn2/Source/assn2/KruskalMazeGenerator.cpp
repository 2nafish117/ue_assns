// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "KruskalMazeGenerator.h"

// Sets default values
AKruskalMazeGenerator::AKruskalMazeGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root MF"));
	check(SceneComponent != nullptr);
	this->RootComponent = SceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("dont forget the fkn TEXT macro"));
	StaticMeshComponent->SetupAttachment(RootComponent);
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif