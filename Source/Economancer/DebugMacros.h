#pragma once
#include "DrawDebugHelpers.h"

#define DEBUG_SPHERE_SPAWNER(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 32, FColor::Green, true);
#define DEBUG_SPHERE_SPAWNER_COLORED(Location, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 6.f, 32, Color, false, 2.0f);
#define DEBUG_SPHERE_SPAWNER_PER_FRAME(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 25.f, 32, FColor::Green, false);
#define DEBUG_LINE_SPAWNER(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, -1.f, 0, 0.5f);
#define DEBUG_LINE_SPAWNER_PER_FRAME(startLocation, endLocation) if(GetWorld()) DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f, 0, 0.5f);
#define DEBUG_POINT_SPAWNER(endLocation) if(GetWorld()) DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Yellow, true, -1.f, 0);
#define DEBUG_POINT_SPAWNER_PER_FRAME(endLocation) if(GetWorld()) DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Yellow, false, -1.f, 0);



// To be used at begin play
#define DEBUG_DRAW_FORWARD_VECTOR(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, true, -1.f, 0, 0.5f); \
		DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Yellow, true, -1.f, 0); \
	}

// for tick debug draws.
#define DEBUG_DRAW_FORWARD_VECTOR_PER_FRAME(startLocation, endLocation) if(GetWorld()) \
	{ \
		DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, -1.f, 0, 0.5f); \
		DrawDebugPoint(GetWorld(), endLocation, 15.f, FColor::Yellow, false, -1.f, 0); \
	}