// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/DataTypes/InventorySlotData.h"
#include "UiInventoryUi.generated.h"

struct FInventorySlotData;
class UUiWidget;
class UUiInventorySlot;
class AInventorySystemCharacter;
class UInventory;
class UImage;
class UUniformGridPanel;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UUiInventoryUi : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UUniformGridPanel* Grid;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	UImage* Image;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InventoryComponent ref")
	UInventory* InventoryRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player ref")
	AInventorySystemCharacter* PlayerRef;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUiInventorySlot> UiClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<UUiInventorySlot*> InventorySlots;
	
	UFUNCTION(BlueprintCallable)
	void InitializeInventorySlot();

	UFUNCTION(BlueprintCallable)
	void UpdateSlot(FInventorySlotData SlotData);
	

	UPROPERTY()
	FInventorySlotData TempSlotData;
	
	UPROPERTY()
	UUiInventorySlot* DragAndDropSlot;
	
	UPROPERTY()
	bool bIsDroppingItem;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
