#include "System/PlanetGenerator.h"
//#include "ProceduralMeshComponent.h"

APlanetGenerator::APlanetGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	FName names[] = { TEXT("Mesh1"), TEXT("Mesh2"), TEXT("Mesh3"), TEXT("Mesh4"), TEXT("Mesh5"), TEXT("Mesh6") };

	for(int i = 0; i < 6; ++i)
	{
		FName name = TEXT("Mesh" + i);
		Meshes.Add(CreateDefaultSubobject<UProceduralMeshComponent>(names[i]));
		Meshes[i]->bUseAsyncCooking = true;
		Meshes[i]->SetupAttachment(Root);
	}

	//CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("CustomMesh");
	//RootComponent = CustomMesh;
	//CustomMesh->bUseAsyncCooking = true;
}

void APlanetGenerator::BeginPlay()
{
	Super::BeginPlay();

	//axisA = FVector(localUp.Y, localUp.Z, localUp.X);
	//axisB = FVector::CrossProduct(localUp, axisA);

	GenerateCubeMesh();
}

void APlanetGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlanetGenerator::AddTriangle(int32 V1, int32 V2, int32 V3)
{
	Triangles.Add(V1);
	Triangles.Add(V2);
	Triangles.Add(V3);
}

void APlanetGenerator::GenerateCubeMesh()
{
	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));
	 
	FVector directions[] = { FVector::UpVector, FVector::DownVector, FVector::LeftVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };

	for (int j = 0; j < Meshes.Num(); ++j)
	{
		UProceduralMeshComponent* mesh = Meshes[j];
		FVector localUp = directions[j];
		FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
		FVector axisB = FVector::CrossProduct(localUp, axisA);

		Vertices.Empty();
		Triangles.Empty();

		Vertices.Init(FVector::ZeroVector, Resolution * Resolution);
		Triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6);

		int i = 0;
		int triIndex = 0;
		for (int y = 0; y < Resolution; ++y)
		{
			for (int x = 0; x < Resolution; ++x)
			{
				FVector2D percent = FVector2D(x, y) / (Resolution - 1);
				FVector pointOnCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
				FVector pointOnUnitSphere = pointOnCube;
				pointOnUnitSphere.Normalize();
				Vertices[i] = pointOnUnitSphere;

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					Triangles[triIndex] = i;
					Triangles[triIndex + 1] = i + Resolution;
					Triangles[triIndex + 2] = i + Resolution + 1;

					Triangles[triIndex + 3] = i;
					Triangles[triIndex + 4] = i + Resolution + 1;
					Triangles[triIndex + 5] = i + 1;
					triIndex += 6;
				}

				++i;
			}
		}
		mesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
	}

	SetActorScale3D(FVector(100, 100, 100));

	/*

	Vertices.Add(FVector(0, -100, 0)); //lower left - 0
	Vertices.Add(FVector(0, -100, 100)); //upper left - 1
	Vertices.Add(FVector(0, 100, 0)); //lower right - 2 
	Vertices.Add(FVector(0, 100, 100)); //upper right - 3

	Vertices.Add(FVector(100, -100, 0)); //lower front left - 4
	Vertices.Add(FVector(100, -100, 100)); //upper front left - 5

	Vertices.Add(FVector(100, 100, 100)); //upper front right - 6
	Vertices.Add(FVector(100, 100, 0)); //lower front right - 7

	//Back face of cube
	AddTriangle(0, 2, 3);
	AddTriangle(3, 1, 0);

	//Left face of cube
	AddTriangle(0, 1, 4);
	AddTriangle(4, 1, 5);

	//Front face of cube
	AddTriangle(4, 5, 7);
	AddTriangle(7, 5, 6);

	//Right face of cube
	AddTriangle(7, 6, 3);
	AddTriangle(3, 2, 7);

	//Top face
	AddTriangle(1, 3, 5);
	AddTriangle(6, 5, 3);

	//bottom face
	AddTriangle(2, 0, 4);
	AddTriangle(4, 7, 2);

	TArray<FLinearColor> VertexColors;
	VertexColors.Add(FLinearColor(0.f, 0.f, 1.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 0.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.5f, 1.f, 0.5f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(1.f, 1.f, 0.f));
	VertexColors.Add(FLinearColor(0.f, 1.f, 1.f));

	CustomMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, TArray<FProcMeshTangent>(), true);
	
	
	
	*/
	
}