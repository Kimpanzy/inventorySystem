// Fill out your copyright notice in the Description page of Project Settings.


#include "InventorySystem/Hud/UiInventorySlot.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/DataTypes/InventorySlotData.h"
#include "InventorySystem/Inventory/Inventory.h"



void UUiInventorySlot::UpdateSlot(const FInventorySlotData* NewSlotData)
{
	if (NewSlotData)
	{
		SlotData = *NewSlotData;
	}
	else
	{
		auto* PlayerRef = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (PlayerRef)
		{
			InventoryRef = PlayerRef->FindComponentByClass<UInventory>();
		}

		if (!InventoryRef || !InventoryRef->InventorySlotData.IsValidIndex(SlotIndex)) return;
		
		SlotData = InventoryRef->InventorySlotData[SlotIndex];
	}
	
	if (SlotData.bIsEmpty)
	{
		IMG_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		TXT_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	IMG_ItemIcon->SetVisibility(ESlateVisibility::Visible);

	if (SlotData.ItemData.ItemIcon.IsValid() || SlotData.ItemData.ItemIcon.ToSoftObjectPath().IsValid())
	{
		if (UTexture2D* LoadedTexture = SlotData.ItemData.ItemIcon.Get())
		{
			IMG_ItemIcon->SetBrushFromTexture(LoadedTexture);
		}
		else
		{
			IMG_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		IMG_ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (SlotData.ItemData.MaxQuantity > 1)
	{
		TXT_ItemAmount->SetText(FText::AsNumber(SlotData.ItemData.CurrentQuantity));
		TXT_ItemAmount->SetVisibility(
			SlotData.ItemData.CurrentQuantity > 0 ?
			ESlateVisibility::Visible :
			ESlateVisibility::Collapsed
		);
	}
	else
	{
		TXT_ItemAmount->SetVisibility(ESlateVisibility::Collapsed);
	}
}


FReply UUiInventorySlot::NativeOnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!SlotData.bIsEmpty)
	{
		if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
		{
			bIsDragging = true;
			
			FEventReply EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(MouseEvent, this, EKeys::LeftMouseButton);
			return EventReply.NativeReply;
		}
	}
	return FReply::Unhandled();
}

void UUiInventorySlot::NativeOnDragDetected(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent,
	UDragDropOperation*& OutOperation)
{
	if (bIsDragging)
	{
		bIsDragging = false;

		
		UDragDropOperation* DragOp = NewObject<UDragDropOperation>();

		UUiInventorySlot* DragVisual = CreateWidget<UUiInventorySlot>(GetWorld(),GetClass());
		if (DragVisual)
		{
			DragVisual->UpdateSlot(&SlotData);
			DragVisual->SetIsEnabled(false);
			DragVisual->SetRenderOpacity(0.5f);
		}
		
		DragOp->DefaultDragVisual = DragVisual;
		DragOp->Payload = this;
		DragOp->Pivot = EDragPivot::MouseDown;
		
		OutOperation = DragOp;
	}
}

FReply UUiInventorySlot::NativeOnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
		UE_LOG(LogTemp, Warning, TEXT("Item dropped or click released"));
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

bool UUiInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (UUiInventorySlot* DraggedSlot = Cast<UUiInventorySlot>(InOperation->Payload))
	{
		if (InventoryRef)
		{
			InventoryRef->DragAndDropUpdate(this, DraggedSlot);
			return true;
		}
	}
	return false;
}

bool UUiInventorySlot::IsInsideInventoryUI(const FVector2D& ScreenPosition)
{
	if (auto* Root = GetRootWidget())
	{
		FGeometry Geometry = Root->GetCachedGeometry();

		FVector2D LocalCoord = Geometry.AbsoluteToLocal(ScreenPosition);

		FVector2D Size = Geometry.GetLocalSize();

		return LocalCoord.X >= 0.f && LocalCoord.Y >= 0.f &&
			   LocalCoord.X <= Size.X && LocalCoord.Y <= Size.Y;
	}
	return false;
}

void UUiInventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);

	if (UUiInventorySlot* DraggedSlot = Cast<UUiInventorySlot>(InOperation->Payload))
	{
		if (!IsInsideInventoryUI(InDragDropEvent.GetScreenSpacePosition()))
		{
			if (InventoryRef)
			{
				InventoryRef->DropItemFromSlot(DraggedSlot);
			}
		}
	}
}
