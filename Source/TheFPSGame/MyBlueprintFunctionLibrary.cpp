// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlueprintFunctionLibrary.h"

#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"

float UMyBlueprintFunctionLibrary::GetHeightAtLocation(AActor* Actor, FVector2D Point, bool bDrawDebugLines)
{
	
	UWorld* World{ Actor->GetWorld() };

	if (World)
	{
		FVector StartLocation{ Point.X, Point.Y, 1000 };	// Raytrace starting point.
		FVector EndLocation{ Point.X, Point.Y, 0 };			// Raytrace end point.

		// Raytrace for overlapping actors.
		FHitResult HitResult;
		World->LineTraceSingleByObjectType(
			OUT HitResult,
			StartLocation,
			EndLocation,
			FCollisionObjectQueryParams(ECollisionChannel::ECC_WorldStatic),
			FCollisionQueryParams()
		);

		// Draw debug line.
		if (bDrawDebugLines)
		{
			FColor LineColor;

			if (HitResult.GetActor()) LineColor = FColor::Red;
			else LineColor = FColor::Green;

			DrawDebugLine(
				World,
				StartLocation,
				EndLocation,
	 		LineColor,
				true,
				5.f,
				0.f,
				10.f
			);
		}

		// Return Z location.
		if (HitResult.GetActor()) return HitResult.ImpactPoint.Z;
	}

	return 0;
}