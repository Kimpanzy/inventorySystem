// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNotification.generated.h"

class UTextBlock;
struct FInventorySlotData;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemNotification : public UUserWidget
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	class UImage* IMG_ItemIcon;
	
	UPROPERTY(BlueprintReadWrite,meta = (BindWidget))
	UTextBlock* TXT_ItemNameAmount;
	
	//UFUNCTION(BlueprintCallable)
	void SetupFromStats(FInventorySlotData* SlotData);
	
};
