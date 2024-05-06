#pragma once

// Small header to hold the weapon types

UENUM(BlueprintType)
enum class EWeaponType : uint8   
{
	EPS_Melee UMETA(DisplayName = "Melee"),
	EPS_HandGun UMETA(DisplayName = "HandGun"),
	EPS_Rifle UMETA(DisplayName = "Rifle"),
	EPS_Missle UMETA(DisplayName = "Missle")
};