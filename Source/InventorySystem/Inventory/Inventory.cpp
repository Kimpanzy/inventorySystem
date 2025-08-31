
#include "InventorySystem/Inventory/Inventory.h"

#include "InventorySystem/InventorySystemCharacter.h"
#include "InventorySystem/Hud/UiInventorySlot.h"
#include "InventorySystem/Hud/UiWidget.h"
#include "InventorySystem/Item/Item.h"

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
	NumberOfSlots = 10;
}

void UInventory::BeginPlay()
{
	Super::BeginPlay();
	InitializeSlot();
	SetupUiClass();
}
void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInventory::InitializeSlot()
{
	//Makes sure the array as enough slots
	InventorySlotData.SetNum(NumberOfSlots);
	
	for (int32 i = 0; i < NumberOfSlots; i++)
	{
		InventorySlotData[i].bIsEmpty = true;
		InventorySlotData[i].SlotIndex = i;
	}
}

int UInventory::CheckForEmptySlot()
{
	for (int32 i = 0; i < NumberOfSlots; i++)
	{
		if (InventorySlotData[i].bIsEmpty) return i;
	}
	return -1;
}

void UInventory::DragAndDropUpdate(UUiInventorySlot* TargetSlot, UUiInventorySlot* SourceSlot)
{
	if (!TargetSlot || !SourceSlot) return;
	
	int32 TargetIndex = TargetSlot->SlotIndex;
	int32 SourceIndex = SourceSlot->SlotIndex;

	if (!InventorySlotData.IsValidIndex(TargetIndex) || !InventorySlotData.IsValidIndex(SourceIndex))
		return;
	
	FInventorySlotData Temp = InventorySlotData[TargetIndex];
	InventorySlotData[TargetIndex] = InventorySlotData[SourceIndex];
	InventorySlotData[SourceIndex] = Temp;
	
	TargetSlot->UpdateSlot(&InventorySlotData[TargetIndex]);
	SourceSlot->UpdateSlot(&InventorySlotData[SourceIndex]);
}

void UInventory::DropItemFromSlot(UUiInventorySlot* ItemToDrop)
{
	UE_LOG(LogTemp, Display, TEXT("DropItemFromSlot"));
	
	FInventorySlotData ItemToSpawn = InventorySlotData[ItemToDrop->SlotIndex];

	SpawnItem(ItemToSpawn.ItemData);

	InventorySlotData[ItemToDrop->SlotIndex].bIsEmpty = true;
	ItemToDrop->UpdateSlot(&InventorySlotData[ItemToDrop->SlotIndex]);
}

void UInventory::SpawnItem(FItemStats ItemStats)
{
	UWorld* World = GetWorld();
	if (!World) return;

	auto* Player  = Cast<AInventorySystemCharacter>(GetOwner());
	if (!Player) return;

	if (!ItemStats.ItemBpClass.IsValid())
	{
		// Synchronously load if needed
		ItemStats.ItemBpClass.LoadSynchronous();
	}
	
	if (!ItemStats.ItemBpClass.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Item class for %s is not valid"), *ItemStats.ItemName.ToString());
		return;
	}
	
	FVector SpawnLocation = Player->GetActorLocation() + Player->GetActorForwardVector() * 100.f;
	FRotator SpawnRotation = FRotator::ZeroRotator;
	
	auto* SpawnedItem = World->SpawnActor<AItem>(
		ItemStats.ItemBpClass.Get(),
		SpawnLocation,
		SpawnRotation);

	SpawnedItem->ItemStats.CurrentQuantity = ItemStats.CurrentQuantity;
	SpawnedItem->ItemStats.ItemID = ItemStats.ItemID;
}

void UInventory::AddItem(AItem* Item)
{
	if (!Item) return;

	int QuantityToAdd = Item->ItemStats.CurrentQuantity;

	// First, try to stack into existing slots with the same item
	QuantityToAdd = FindSlotWithItem(Item->ItemStats.ItemID, QuantityToAdd);

	// Then, add to empty slots
	while (QuantityToAdd > 0)
	{
		int AvailableSlot = CheckForEmptySlot();

		if (AvailableSlot < 0) // No empty slots left
		{
			UE_LOG(LogTemp, Warning, TEXT("No empty slot available to add remaining %d of item %s"),
				QuantityToAdd, *Item->ItemStats.ItemName.ToString());
			break;
		}

		FItemStats* OutRow = InventoryDataTable->FindRow<FItemStats>(
			Item->ItemStats.ItemID,
			TEXT("Fetching item"));

		if (OutRow)
		{
			InventorySlotData[AvailableSlot].ItemData = *OutRow;
			InventorySlotData[AvailableSlot].bIsEmpty = false;
			InventorySlotData[AvailableSlot].SlotIndex = AvailableSlot;

			int Adding = FMath::Min(QuantityToAdd, InventorySlotData[AvailableSlot].ItemData.MaxQuantity);
			InventorySlotData[AvailableSlot].ItemData.CurrentQuantity = Adding;
			QuantityToAdd -= Adding;

			OnItemAdded.Broadcast(InventorySlotData[AvailableSlot]);
			
			if (UiRef)
			{
				UiRef->CreateNotification(&InventorySlotData[AvailableSlot]);
			}
		}
	}
}
void UInventory::SetupUiClass()
{
	if (UiClass)
	{
		UiRef = CreateWidget<UUiWidget>(GetWorld()->GetFirstPlayerController(), UiClass);
		if (UiRef)
		{
			UiRef->AddToViewport();
			
		}
	}
}

int UInventory::FindSlotWithItem(FName ItemID, int QuantityToAdd)
{
	for (int i = 0; i < InventorySlotData.Num(); i++)
	{
		if (!InventorySlotData[i].bIsEmpty && InventorySlotData[i].ItemData.ItemID == ItemID)
		{
			int CurrentQty = InventorySlotData[i].ItemData.CurrentQuantity;
			int MaxQty = InventorySlotData[i].ItemData.MaxQuantity;

			if (CurrentQty < MaxQty)
			{
				int SpaceLeft = MaxQty - CurrentQty;
				int Adding = FMath::Min(SpaceLeft, QuantityToAdd);

				InventorySlotData[i].ItemData.CurrentQuantity += Adding;
				QuantityToAdd -= Adding;

				OnItemAdded.Broadcast(InventorySlotData[i]);
				
				if (UiRef)
				UiRef->CreateNotification(&InventorySlotData[i]);
			}
		}
	}
	return QuantityToAdd;
}



