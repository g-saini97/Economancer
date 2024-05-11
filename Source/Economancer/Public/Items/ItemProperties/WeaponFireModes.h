#pragma once

UENUM(BlueprintType)
enum class EWeaponFireMode : uint8
{
	EWFM_Single UMETA(DisplayName = "Melee"),
	EWFM_Burst UMETA(DisplayName = "HandGun"),
	EWFM_Auto UMETA(DisplayName = "Rifle"),

};