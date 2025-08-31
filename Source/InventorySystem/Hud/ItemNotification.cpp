

#include "InventorySystem/Hud/ItemNotification.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "InventorySystem/DataTypes/InventorySlotData.h"


void UItemNotification::SetupFromStats(FInventorySlotData* SlotData)
{
	if (TXT_ItemNameAmount)
	{
		
		FText NameWithAmount = FText::Format(
			FText::FromString("{0} x{1}"),
			SlotData->ItemData.ItemName,
			FText::AsNumber(SlotData->ItemData.CurrentQuantity)
		);
		TXT_ItemNameAmount->SetText(NameWithAmount);
	}

	if (IMG_ItemIcon)
	{
		if (SlotData->ItemData.ItemIcon.IsValid() || SlotData->ItemData.ItemIcon.ToSoftObjectPath().IsValid())
		{
			UTexture2D* LoadedTexture = SlotData->ItemData.ItemIcon.LoadSynchronous();
			if (LoadedTexture)
			{
				IMG_ItemIcon->SetBrushFromTexture(LoadedTexture);
				IMG_ItemIcon->SetVisibility(ESlateVisibility::Visible);
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
	}
}
