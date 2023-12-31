// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralCylinder.h"


AProceduralCylinder::AProceduralCylinder()
{
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	RootComponent = mesh;
}

void AProceduralCylinder::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	GenerateMesh();
}

void AProceduralCylinder::BeginPlay()
{
	Super::BeginPlay();

	GenerateMesh();
}

void AProceduralCylinder::GenerateMesh()
{
	TArray<FVector> Vertices;
	TArray<int32> Triangles;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FColor> VertexColors;

	GenerateCylinder(Vertices, Triangles, Normals, UVs, Tangents, Height, Radius, CrossSectionCount, bCapEnds, bDoubleSided, bSmoothNormals);

	mesh->ClearAllMeshSections();
	mesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, VertexColors, Tangents, false);
}

void AProceduralCylinder::GenerateCylinder(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals, TArray<FVector2D>& UVs, TArray<FProcMeshTangent>& Tangents, float InHeight, float InWidth, int32 InCrossSectionCount, bool bInCapEnds, bool bInDoubleSided, bool bInSmoothNormals)
{
	// -------------------------------------------------------
	// Basic setup
	int VertexIndex = 0;
	int32 NumVerts = InCrossSectionCount * 4; // InCrossSectionCount x 4 verts per face

	// Count extra vertices if double sided
	if (bInDoubleSided)
	{
		NumVerts = NumVerts * 2;
	}

	// Count vertices for caps if set
	if (bInCapEnds)
	{
		NumVerts += 2 * (InCrossSectionCount - 1) * 3;
	}

	//reset
	Triangles.Reset();
	Vertices.Reset();
	Vertices.AddUninitialized(NumVerts);
	Normals.Reset();
	Normals.AddUninitialized(NumVerts);
	Tangents.Reset();
	Tangents.AddUninitialized(NumVerts);
	UVs.Reset();
	UVs.AddUninitialized(NumVerts);

	// -------------------------------------------------------
	// Generatio
	const float AngleBetweenQuads = (2.0f / (float)(InCrossSectionCount)) * PI;
	const float VMapPerQuad = 1.0f / (float)InCrossSectionCount;
	FVector Offset = FVector(0, 0, InHeight);

	// Start by building up vertices that make up the cylinder sides
	for (int32 QuadIndex = 0; QuadIndex < InCrossSectionCount; QuadIndex++)
	{
		float Angle = (float)QuadIndex * AngleBetweenQuads;
		float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

		// Set up the vertices
		FVector p0 = FVector(FMath::Cos(Angle) * InWidth, FMath::Sin(Angle) * InWidth, 0.f);
		FVector p1 = FVector(FMath::Cos(NextAngle) * InWidth, FMath::Sin(NextAngle) * InWidth, 0.f);
		FVector p2 = p1 + Offset;
		FVector p3 = p0 + Offset;

		// Set up the quad triangles
		int VertIndex1 = VertexIndex++;
		int VertIndex2 = VertexIndex++;
		int VertIndex3 = VertexIndex++;
		int VertIndex4 = VertexIndex++;

		Vertices[VertIndex1] = p0;
		Vertices[VertIndex2] = p1;
		Vertices[VertIndex3] = p2;
		Vertices[VertIndex4] = p3;

		// Now create two triangles from those four vertices
		// The order of these (clockwise/counter-clockwise) dictates which way the normal will face. 
		Triangles.Add(VertIndex4);
		Triangles.Add(VertIndex3);
		Triangles.Add(VertIndex1);

		Triangles.Add(VertIndex3);
		Triangles.Add(VertIndex2);
		Triangles.Add(VertIndex1);

		// UVs
		UVs[VertIndex1] = FVector2D(VMapPerQuad * QuadIndex, 0.0f);
		UVs[VertIndex2] = FVector2D(VMapPerQuad * (QuadIndex + 1), 0.0f);
		UVs[VertIndex3] = FVector2D(VMapPerQuad * (QuadIndex + 1), 1.0f);
		UVs[VertIndex4] = FVector2D(VMapPerQuad * QuadIndex, 1.0f);

		// Normals
		FVector NormalCurrent = FVector::CrossProduct(Vertices[VertIndex1] - Vertices[VertIndex3], Vertices[VertIndex2] - Vertices[VertIndex3]).GetSafeNormal();

		if (bInSmoothNormals)
		{
			// To smooth normals you give the vertices a different normal value than the polygon they belong to, gfx hardware then knows how to interpolate between those.
			// I do this here as an average between normals of two adjacent polygons
			// TODO re-use calculations between loop iterations (do them once and cache them!), no need to calculate same values every time :)
			float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
			FVector p4 = FVector(FMath::Cos(NextNextAngle) * InWidth, FMath::Sin(NextNextAngle) * InWidth, 0.f);

			// p1 to p4 to p2
			FVector NormalNext = FVector::CrossProduct(p1 - p2, p4 - p2).GetSafeNormal();
			FVector AverageNormalRight = (NormalCurrent + NormalNext) / 2;
			AverageNormalRight = AverageNormalRight.GetSafeNormal();

			float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
			FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * InWidth, FMath::Sin(PreviousAngle) * InWidth, 0.f);

			// p0 to p3 to pMinus1
			FVector NormalPrevious = FVector::CrossProduct(p0 - pMinus1, p3 - pMinus1).GetSafeNormal();
			FVector AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
			AverageNormalLeft = AverageNormalLeft.GetSafeNormal();

			Normals[VertIndex1] = AverageNormalLeft;
			Normals[VertIndex2] = AverageNormalRight;
			Normals[VertIndex3] = AverageNormalRight;
			Normals[VertIndex4] = AverageNormalLeft;
		}
		else
		{
			// If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
			Normals[VertIndex1] = NormalCurrent;
			Normals[VertIndex2] = NormalCurrent;
			Normals[VertIndex3] = NormalCurrent;
			Normals[VertIndex4] = NormalCurrent;
		}

		// Tangents (perpendicular to the surface)
		FVector SurfaceTangent = p0 - p1;
		SurfaceTangent = SurfaceTangent.GetSafeNormal();
		Tangents[VertIndex1] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex2] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex3] = FProcMeshTangent(SurfaceTangent, true);
		Tangents[VertIndex4] = FProcMeshTangent(SurfaceTangent, true);

		// If double sides, create extra polygons but face the normals the other way.
		if (bInDoubleSided)
		{
			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			VertIndex4 = VertexIndex++;

			Vertices[VertIndex1] = p0;
			Vertices[VertIndex2] = p1;
			Vertices[VertIndex3] = p2;
			Vertices[VertIndex4] = p3;

			Triangles.Add(VertIndex1);
			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex4);

			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex4);
		}

		if (QuadIndex != 0 && bInCapEnds)
		{
			// Cap is closed by triangles that start at 0, then use the points at the angles for the other corners

			// Bottom
			FVector capVertex0 = FVector(FMath::Cos(0.0f) * InWidth, FMath::Sin(0.0f) * InWidth, 0.f);
			FVector capVertex1 = FVector(FMath::Cos(Angle) * InWidth, FMath::Sin(Angle) * InWidth, 0.f);
			FVector capVertex2 = FVector(FMath::Cos(NextAngle) * InWidth, FMath::Sin(NextAngle) * InWidth, 0.f);

			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			Vertices[VertIndex1] = capVertex0;
			Vertices[VertIndex2] = capVertex1;
			Vertices[VertIndex3] = capVertex2;

			Triangles.Add(VertIndex1);
			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex3);

			FVector2D UV1 = FVector2D(FMath::Sin(0.0f), FMath::Cos(0.0f));
			FVector2D UV2 = FVector2D(FMath::Sin(Angle), FMath::Cos(Angle));
			FVector2D UV3 = FVector2D(FMath::Sin(NextAngle), FMath::Cos(NextAngle));

			UVs[VertIndex1] = UV1;
			UVs[VertIndex2] = UV2;
			UVs[VertIndex3] = UV3;

			// Top
			capVertex0 = capVertex0 + Offset;
			capVertex1 = capVertex1 + Offset;
			capVertex2 = capVertex2 + Offset;

			VertIndex1 = VertexIndex++;
			VertIndex2 = VertexIndex++;
			VertIndex3 = VertexIndex++;
			Vertices[VertIndex1] = capVertex0;
			Vertices[VertIndex2] = capVertex1;
			Vertices[VertIndex3] = capVertex2;

			Triangles.Add(VertIndex3);
			Triangles.Add(VertIndex2);
			Triangles.Add(VertIndex1);

			UVs[VertIndex1] = UV1;
			UVs[VertIndex2] = UV2;
			UVs[VertIndex3] = UV3;
		}
	}
}