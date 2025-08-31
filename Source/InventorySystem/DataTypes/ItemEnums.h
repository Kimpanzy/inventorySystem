#pragma once

#include "ItemEnums.generated.h"

UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Consumable UMETA(DisplayName = "Consumable"),
	Equipment  UMETA(DisplayName = "Equipment"),
	Other      UMETA(DisplayName = "Other")
};
