// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UiWidget.generated.h"

struct FInventorySlotData;
class AItem;
struct FItemStats;
class UItemNotification;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UUiWidget : public UUserWidget
{
	GENERATED_BODY()
	
	

public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* ContentHolder;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* VB_ItemNotificationBox;
	
	UPROPERTY(Meta = (BindWidget))
	class UUiInventoryUi* WBP_Inventory_UI;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> ItemNotification;

	
	void CreateNotification(FInventorySlotData* SlotData);
	
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	APlayerController* PC;

protected:
	virtual void NativeConstruct() override;
};

