#pragma once

#include "InventorySystem/DataTypes/ItemEnums.h"
#include "CoreMinimal.h"
#include "ItemData.generated.h"

class UTexture2D;
class AItem;

USTRUCT(BlueprintType)
struct FItemStats : public FTableRowBase
{
	GENERATED_BODY()

	FItemStats()
		: MaxQuantity(0)
		, CurrentQuantity(0)
	{}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemCategory ItemCategory;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentQuantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> ItemIcon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftClassPtr<AItem> ItemBpClass;
};