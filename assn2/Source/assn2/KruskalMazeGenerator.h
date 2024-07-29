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

	using GraphNode = TPair<int, int>;
	using GraphEdge = TPair<GraphNode, GraphNode>;

	// @TODO: make static array len of 4, a node can have only upto 4 neighbours
	TMap<GraphNode, TArray<GraphNode>> AdjGraph;

private:
	void Kruskal(TMap<GraphNode, TArray<GraphNode>>& adjGraph);

	void ShuffleArray(TArray<GraphEdge>& Array);

	GraphNode GetRoot(const TMap<GraphNode, GraphNode>& connectivity, GraphNode node);

	void Union(TMap<GraphNode, GraphNode>& connectivity, GraphNode node1, GraphNode node2);
};
