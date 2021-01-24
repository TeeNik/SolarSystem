#include "PlanetGeneration/PlanetGenerator.h"
#include "PlanetGeneration/ColorGenerator.h"
#include "PlanetGeneration/ShapeGenerator.h"
#include "KismetProceduralMeshLibrary.h"
#include "PlanetGeneration/PlanetMeshData.h"

#include "Kismet/KismetMathLibrary.h"

APlanetGenerator::APlanetGenerator()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	ShapeGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("ShapeGenerator"));
	ColorGenerator = CreateDefaultSubobject<UColorGenerator>(TEXT("ColorGenerator"));
	CloudGenerator = CreateDefaultSubobject<UShapeGenerator>(TEXT("CloudGenerator"));

	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->bUseAsyncCooking = true;

	CloudMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CloudMesh"));
	CloudMesh->SetupAttachment(Root);
	CloudMesh->bUseAsyncCooking = true;
}

void APlanetGenerator::CallMeshGeneration()
{
	if (ShouldRegenerate)
	{
		GenerateCubeMesh();
	}
}

void APlanetGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateCubeMesh();
}

void APlanetGenerator::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CallMeshGeneration();
}

int64 APlanetGenerator::GetUnixTime()
{
	FDateTime timeUtc = FDateTime::UtcNow();
	return timeUtc.ToUnixTimestamp() * 1000 + timeUtc.GetMillisecond();
}

void APlanetGenerator::GenerateCubeMesh()
{
	int64 startTime = GetUnixTime();

	ShapeGenerator->Init();
	ColorGenerator->Init();
	CloudGenerator->Init();

	UPlanetMeshData* planetMeshData = NewObject<UPlanetMeshData>();
	planetMeshData->GenerateData(Resolution);

	UPlanetMeshData* cloudMeshData = NewObject<UPlanetMeshData>();
	cloudMeshData->GenerateData(Resolution);

	TArray<FVector2D> uv;
	uv.Init(FVector2D::ZeroVector, planetMeshData->Vertices.Num());

	TArray<FVector2D> cloudUV;
	cloudUV.Init(FVector2D::ZeroVector, cloudMeshData->Vertices.Num());

	TArray<FVector> spherePoints = planetMeshData->Vertices;

	for (int i = 0; i < planetMeshData->Vertices.Num(); ++i)
	{
		FVector pointOnUnitSphere = planetMeshData->Vertices[i];
		float unscaledElevation = ShapeGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
		planetMeshData->Vertices[i] = pointOnUnitSphere * ShapeGenerator->CalculateScaledElevation(unscaledElevation);

		int biomeIndex = unscaledElevation > 0 ? ColorGenerator->BiomeIndexFromPoint(pointOnUnitSphere) : 0;
		uv[i] = FVector2D(unscaledElevation, biomeIndex);

		float clouds = CloudGenerator->CalculateUnscaledElevation(pointOnUnitSphere);
		cloudUV[i].Y = clouds;
	}

	planetMeshData->RecalculateNormals();

	TArray<FLinearColor> colors;
	colors.Init(FColor::White, planetMeshData->Vertices.Num());

	for (int i = 0; i < colors.Num(); ++i)
	{
		float biomIndex = uv[i].Y;
		float min = biomIndex > 0 ? 0 : ShapeGenerator->MinMax.Key;
		float max = biomIndex > 0 ? ShapeGenerator->MinMax.Value : 0;
		//UE_LOG(LogTemp, Log, TEXT("biom %f %f %f %f"), biomIndex, min, max, uv[i].X);
		if(biomIndex > 0)
			colors[i] = ColorGenerator->GetColorFromPoint(spherePoints[i], uv[i].X, min, max);
		else 
			colors[i] = ColorGenerator->GetColor(biomIndex,  uv[i].X, min, max);
	}

	UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
	dynamicMaterial->SetVectorParameterValue(TEXT("ElevationMinMax"), FVector(ShapeGenerator->MinMax.Key, ShapeGenerator->MinMax.Value, 0));

	Mesh->CreateMeshSection_LinearColor(0, planetMeshData->Vertices, planetMeshData->Triangles, planetMeshData->Normals, uv, colors, TArray<FProcMeshTangent>(), true);
	Mesh->SetMaterial(0, dynamicMaterial);

	CloudMesh->CreateMeshSection_LinearColor(0, cloudMeshData->Vertices, cloudMeshData->Triangles, cloudMeshData->Normals, cloudUV, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
	CloudMesh->SetMaterial(0, CloudMaterial);
	CloudMesh->SetRelativeScale3D(FVector(1.05f));

	UE_LOG(LogTemp, Log, TEXT("Time spend: %d"), GetUnixTime() - startTime);
}