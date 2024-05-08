#pragma once
#pragma once
/// <summary>
/// Small .h file just to hold the AI's state enum, mainly to make the animBPS work correctly
/// </summary>

UENUM(BlueprintType)

enum class EAIState : uint8   /// enum class makes a scoped enum, so in order to use the enum we will have to fully qualify it with the :: operator	
{
	EAIS_Uneqipped UMETA(DisplayName = "Uneqipped"),
	EAIS_EquippedOneHanded UMETA(DisplayName = "EquippedOneHanded"),
	EAIS_EquippedTwoHanded UMETA(DisplayName = "EquippedTwoHanded"),
	EAIS_EquipedAkimbo UMETA(DisplayName = "EquipedAkimbo")
};