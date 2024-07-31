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

	virtual void OnConstruction(const FTransform& Transform);

private:
	UPROPERTY(EditAnywhere)
	UInstancedStaticMeshComponent* InstancedMeshComponent;

	UPROPERTY(EditAnywhere)
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere)
	UMaterial* Material;

	UPROPERTY(EditAnywhere)
	int NumRows = 3;

	UPROPERTY(EditAnywhere)
	int NumCols = 3;

	UStaticMesh* PlaneMesh;

	using FGraphNode = TPair<int, int>;
	using FGraphEdge = TPair<FGraphNode, FGraphNode>;

	TMap<FGraphNode, TArray<FGraphNode>> AdjGraph;

private:
	void Kruskal(TMap<FGraphNode, TArray<FGraphNode>>& adjGraph);

	void ShuffleArray(TArray<FGraphEdge>& Array);

	FGraphNode GetRoot(const TMap<FGraphNode, FGraphNode>& connectivity, FGraphNode node);

	void Union(TMap<FGraphNode, FGraphNode>& connectivity, FGraphNode node1, FGraphNode node2);
};
