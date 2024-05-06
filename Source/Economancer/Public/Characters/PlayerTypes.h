#pragma once
/// <summary>
/// Small .h file just to hold the player state enum
/// </summary>

UENUM(BlueprintType)
enum class EPlayerState : uint8   /// enum class makes a scoped enum, so in order to use the enum we will have to fully qualify it with the :: operator	
{
	EPS_Uneqipped UMETA(DisplayName = "Uneqipped"),
	EPS_EquippedOneHanded UMETA(DisplayName = "EquippedOneHanded"),
	EPS_EquippedTwoHanded UMETA(DisplayName = "EquippedTwoHanded"),
	EPS_EquipedAkimbo UMETA(DisplayName = "EquipedAkimbo")
};