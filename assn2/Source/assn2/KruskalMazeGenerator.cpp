// Fill out your copyright notice in the Description page of Project Settings.
#ifdef __clang__
#pragma clang optimize off
#else
#pragma optimize("", off)
#endif

#include "KruskalMazeGenerator.h"

#include <Components/InstancedStaticMeshComponent.h>

// Sets default values
AKruskalMazeGenerator::AKruskalMazeGenerator()
	: Material(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root MF"));
	check(SceneComponent != nullptr);
	this->RootComponent = SceneComponent;

	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Insatnced static Mesh Component"));
	InstancedMeshComponent->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	InstancedMeshComponent->SetStaticMesh(PlaneMeshRef.Object);
}

void AKruskalMazeGenerator::OnConstruction(const FTransform& Transform) {
	InstancedMeshComponent->ClearInstances();
	InstancedMeshComponent->SetMaterial(0, Material);

	// first direction
	for (size_t i = 0; i <= NumRows; ++i) {
		for (size_t j = 0; j < NumCols; ++j) {
			FRotator Rotation = FRotator(FQuat::MakeFromRotationVector(FVector(0, 1, 0) * FMath::DegreesToRadians(90)));

			FVector Translation = FVector(100 * i, 100 * j + 50, 50);
			const FTransform InstanceTransform = FTransform(Rotation, Translation);
			InstancedMeshComponent->AddInstance(InstanceTransform);
		}
	}

	// second direction
	for (size_t j = 0; j <= NumCols; ++j) {
		for (size_t i = 0; i < NumRows; ++i) {
			FQuat QRot = FQuat::MakeFromRotationVector(FVector(0, 1, 0) * FMath::DegreesToRadians(90));
			QRot = FQuat::MakeFromRotationVector(FVector(0, 0, 1) * FMath::DegreesToRadians(90)) * QRot;

			FRotator Rotation = FRotator(QRot);
			FVector Translation = FVector(100 * i + 50, 100 * j, 50);
			const FTransform InstanceTransform = FTransform(Rotation, Translation);
			InstancedMeshComponent->AddInstance(InstanceTransform);
		}
	}
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif