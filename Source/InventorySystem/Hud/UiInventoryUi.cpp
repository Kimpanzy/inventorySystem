// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Hud/UiInventoryUi.h"
#include "Components/TextBlock.h"
#include "InventorySystem/Hud/UiInventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "InventorySystem/Inventory/Inventory.h"


void UUiInventoryUi::NativeConstruct()
{
	Super::NativeConstruct();
	InitializeInventorySlot();
	InventoryRef->OnItemAdded.AddDynamic(this, &UUiInventoryUi::UpdateSlot);
}
void UUiInventoryUi::InitializeInventorySlot()
{
	if (auto* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		InventoryRef = Player->FindComponentByClass<UInventory>();
		
	}

	if (!Grid || !InventoryRef || !UiClass) return;
	
	Grid->ClearChildren();
	

	int32 AmountOfSlots = InventoryRef->NumberOfSlots;
	int32 Columns = 4;
	
	for (int32 i = 0; i < AmountOfSlots; i++)
	{
		auto* NewSlot = CreateWidget<UUiInventorySlot>(GetWorld(),UiClass);
		NewSlot->InventoryRef = InventoryRef;
		NewSlot->SlotIndex = i;
		NewSlot->UpdateSlot(nullptr);
		InventorySlots.AddUnique(NewSlot);
		
		if (NewSlot->TXT_Debug)
		{
			FText DebugText = NewSlot->TXT_Debug->GetText();
		}
		int32 Row = i / Columns;
		int32 Column = i % Columns;
		Grid->AddChildToUniformGrid(NewSlot,Row,Column);
	}
	
}

void UUiInventoryUi::UpdateSlot(FInventorySlotData SlotData)
{
	auto* SlotToUpdate = InventorySlots[SlotData.SlotIndex];
	if (SlotToUpdate)
	{
		SlotToUpdate->SlotData = SlotData;
		SlotToUpdate->UpdateSlot(&SlotData);
	}
}


void UUiInventoryUi::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);

	if (bIsDroppingItem)
	{
		bIsDroppingItem = false;
	}
}

void UUiInventoryUi::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (bIsDroppingItem)
	{
		bIsDroppingItem = true;
	}
}
