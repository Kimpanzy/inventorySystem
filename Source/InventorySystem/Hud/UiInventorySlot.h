// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/DataTypes/InventorySlotData.h"
#include "UiInventorySlot.generated.h"

class UInventory;
struct FInventorySlotData;
class UTextBlock;
class UImage;


/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UUiInventorySlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TXT_ItemAmount;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* IMG_ItemIcon;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TXT_Debug;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventory* InventoryRef;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	int32 SlotIndex;
	
	UPROPERTY()
	FInventorySlotData SlotData;
	
	void UpdateSlot(const FInventorySlotData* NewSlotData);
	
protected:
	virtual FReply  NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent, UDragDropOperation*& OutOperation) override;

	virtual FReply NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	
	bool IsInsideInventoryUI(const FVector2D& ScreenPosition);
	
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	bool bIsDragging = false;
	
};
