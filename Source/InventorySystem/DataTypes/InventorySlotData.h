#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
#include "InventorySlotData.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlotData : public FTableRowBase
{
	GENERATED_BODY()

	bool operator==(const FInventorySlotData& Other) const
	{
		return SlotIndex == Other.SlotIndex;
	}
	FInventorySlotData()
		 : bIsEmpty(true)
		 , SlotIndex(-1)
	 {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEmpty = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStats ItemData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SlotIndex;
	
	
};