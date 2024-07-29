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

	InstancedMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Instanced static Mesh Component"));
	InstancedMeshComponent->SetupAttachment(RootComponent);

	// load fallback static mesh in cpp
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshRef(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	PlaneMesh = PlaneMeshRef.Object;

	if (WallMesh == nullptr) {
		WallMesh = PlaneMesh;
	}
	InstancedMeshComponent->SetStaticMesh(WallMesh);
}

void AKruskalMazeGenerator::OnConstruction(const FTransform& Transform) {
	if (WallMesh == nullptr) {
		WallMesh = PlaneMesh;
	}
	InstancedMeshComponent->SetStaticMesh(WallMesh);
	
	if (InstancedMeshComponent->GetInstanceCount() > 0) {
		InstancedMeshComponent->ClearInstances();
	}

	InstancedMeshComponent->SetMaterial(0, Material);

	AdjGraph.Reset();
	Kruskal(AdjGraph);

	const int numRows = NumRows;
	const int numCols = NumCols;

	// first direction
	for (size_t i = 0; i <= numRows; ++i) {
		for (size_t j = 0; j < numCols; ++j) {
			int x = i;
			int y = j;

			int nx = i - 1;
			int ny = j;

			TArray<GraphNode>* adjList = AdjGraph.Find(GraphNode(x, y));
			if(adjList == nullptr || adjList->Find(GraphNode(nx, ny)) == INDEX_NONE) {
				// draw if not neighbours
				FRotator Rotation = FRotator(FQuat::MakeFromRotationVector(FVector(0, 1, 0) * FMath::DegreesToRadians(90)));

				FVector Translation = FVector(100 * i, 100 * j + 50, 50);
				const FTransform InstanceTransform = FTransform(Rotation, Translation);
				InstancedMeshComponent->AddInstance(InstanceTransform);				
			}
		}
	}

	// second direction
	for (size_t j = 0; j <= numCols; ++j) {
		for (size_t i = 0; i < numRows; ++i) {
			int x = i;
			int y = j;

			int nx = i;
			int ny = j - 1;

			TArray<GraphNode>* adjList = AdjGraph.Find(GraphNode(x, y));
			if (adjList == nullptr || adjList->Find(GraphNode(nx, ny)) == INDEX_NONE) {
				// draw if not neighbours
				FQuat QRot = FQuat::MakeFromRotationVector(FVector(0, 1, 0) * FMath::DegreesToRadians(90));
				QRot = FQuat::MakeFromRotationVector(FVector(0, 0, 1) * FMath::DegreesToRadians(90)) * QRot;

				FRotator Rotation = FRotator(QRot);
				FVector Translation = FVector(100 * i + 50, 100 * j, 50);
				const FTransform InstanceTransform = FTransform(Rotation, Translation);
				InstancedMeshComponent->AddInstance(InstanceTransform);				
			}
		}
	}
}

// quick shuffle from https://forums.unrealengine.com/t/equivalent-of-bp-shuffle-array-for-c-tarrays/349431
void AKruskalMazeGenerator::ShuffleArray(TArray<GraphEdge>& Array)
{
	if (Array.Num() > 0)
	{
		int32 LastIndex = Array.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				Array.Swap(i, Index);
			}
		}
	}
}

AKruskalMazeGenerator::GraphNode AKruskalMazeGenerator::GetRoot(const TMap<GraphNode, GraphNode>& previous, GraphNode node) {
	const GraphNode* prev = previous.Find(node);
	check(prev != nullptr);

	while (*prev != node) {
		node = *prev;
		prev = previous.Find(*prev);
	}

	return *prev;
};

void AKruskalMazeGenerator::Union(TMap<GraphNode, GraphNode>& previous, GraphNode node1, GraphNode node2) {
	GraphNode root1 = GetRoot(previous, node1);
	GraphNode root2 = GetRoot(previous, node2);

	// prev or root2 is now root1
	previous.Add(root2, root1);
};

void AKruskalMazeGenerator::Kruskal(TMap<GraphNode, TArray<GraphNode>>& adjGraph) {
	const int numRows = NumRows;
	const int numCols = NumCols;

	UE_LOG(LogTemp, Log, TEXT("running kruskal with rows=%d cols=%d"), numRows, numCols);

	TArray<GraphEdge> edges;

	TMap<GraphNode, GraphNode> previous;
	previous.Reserve(numCols * numRows);

	// construct edge list, connectivity and adjGraph
	for (int x = 0; x < numRows; ++x) {
		for (int y = 0; y < numCols; ++y) {
			GraphNode node = GraphNode(x, y);

			auto Safe = [numRows, numCols](int x, int y) { return x >= 0 && x < numRows && y >= 0 && y < numCols; };

			if (Safe(x - 1, y)) {
				GraphNode neighbor = GraphNode(x - 1, y);
				edges.Add(GraphEdge(node, neighbor));
			}

			if (Safe(x + 1, y)) {
				GraphNode neighbor = GraphNode(x + 1, y);
				edges.Add(GraphEdge(node, neighbor));
			}

			if (Safe(x, y - 1)) {
				GraphNode neighbor = GraphNode(x, y - 1);
				edges.Add(GraphEdge(node, neighbor));
			}

			if (Safe(x, y + 1)) {
				GraphNode neighbor = GraphNode(x, y + 1);
				edges.Add(GraphEdge(node, neighbor));
			}

			int i = numCols * x + y;
			previous.Add(node, node);
			adjGraph.Add(node, TArray<GraphNode>());
		}
	}

	// @TODO: shuffle edges
	ShuffleArray(edges);

	// build adjGraph
	for (const auto& edge : edges) {
		GraphNode start = edge.Key;
		GraphNode end = edge.Value;

		if (GetRoot(previous, start) != GetRoot(previous, end)) {
			TArray<GraphNode>* startList = adjGraph.Find(start);
			check(startList);
			startList->Add(end);
			
			TArray<GraphNode>* endList = adjGraph.Find(end);
			check(endList);
			endList->Add(start);

			Union(previous, start, end);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("built adj graph"));
}

#ifdef __clang__
#pragma clang optimize on
#else
#pragma optimize("", on)
#endif