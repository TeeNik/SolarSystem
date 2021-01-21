#include "PlanetGeneration/PlanetMeshData.h"

void UPlanetMeshData::GenerateData(int resolution)
{
	Resolution = resolution;

	FVector directions[] = { FVector::UpVector, FVector::LeftVector, FVector::DownVector, FVector::RightVector, FVector::ForwardVector, FVector::BackwardVector };
	const int NumOfDirections = 6;

	Vertices.Init(FVector::ZeroVector, Resolution * Resolution * NumOfDirections);
	Triangles.Init(0, (Resolution - 1) * (Resolution - 1) * 6 * NumOfDirections);

	int index = 0;
	int triIndex = 0;

	for (int j = 0; j < NumOfDirections; ++j)
	{
		FVector localUp = directions[j];
		FVector axisA = FVector(localUp.Y, localUp.Z, localUp.X);
		FVector axisB = FVector::CrossProduct(localUp, axisA);

		for (int y = 0; y < Resolution; ++y)
		{
			for (int x = 0; x < Resolution; ++x)
			{
				FVector2D percent = FVector2D(x, y) / (Resolution - 1);
				FVector pointOnCube = localUp + (percent.X - .5f) * 2 * axisA + (percent.Y - .5f) * 2 * axisB;
				FVector pointOnUnitSphere = pointOnCube;
				pointOnUnitSphere.Normalize();

				Vertices[index] = pointOnUnitSphere;

				if (x != Resolution - 1 && y != Resolution - 1)
				{
					Triangles[triIndex] = index;
					Triangles[triIndex + 1] = index + Resolution;
					Triangles[triIndex + 2] = index + Resolution + 1;

					Triangles[triIndex + 3] = index;
					Triangles[triIndex + 4] = index + Resolution + 1;
					Triangles[triIndex + 5] = index + 1;
					triIndex += 6;
				}
				++index;
			}
		}
	}

	RecalculateNormals();
}

void UPlanetMeshData::RecalculateNormals()
{
	Normals.Init(FVector::ZeroVector, Vertices.Num());
	for (int i = 0; i < Triangles.Num(); i += 3)
	{
		FVector v1 = Vertices[Triangles[i + 1]] - Vertices[Triangles[i]];
		FVector v2 = Vertices[Triangles[i + 2]] - Vertices[Triangles[i]];
		FVector faceNormal = FVector::CrossProduct(v2, v1);
		faceNormal.Normalize();

		Normals[Triangles[i]] += faceNormal;
		Normals[Triangles[i + 1]] += faceNormal;
		Normals[Triangles[i + 2]] += faceNormal;
	}

	for (int i = 0; i < Normals.Num(); ++i)
	{
		Normals[i].Normalize();
	}
}