
#include "InventorySystem/Hud/UiWidget.h"
#include "ItemNotification.h"
#include "UiInventoryUi.h"
#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"
#include "Components/VerticalBox.h"


void UUiWidget::CreateNotification(FInventorySlotData* SlotData)
{
	if (!ItemNotification || !VB_ItemNotificationBox) return;
	
	UItemNotification* NewNotif = CreateWidget<UItemNotification>(GetWorld(), ItemNotification);
	
	if (!NewNotif) return;

	NewNotif->SetupFromStats(SlotData);
	VB_ItemNotificationBox->AddChild(NewNotif);
}

void UUiWidget::ToggleInventory()
{
	if (!PC || !WBP_Inventory_UI) return;

	
	
	if (WBP_Inventory_UI->IsVisible())
	{
		WBP_Inventory_UI->SetVisibility(ESlateVisibility::Collapsed);

		PC->bShowMouseCursor = false;
			
		const FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}
	else
	{
		WBP_Inventory_UI->SetVisibility(ESlateVisibility::Visible);

		PC->bShowMouseCursor = true;
			
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(WBP_Inventory_UI->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PC->SetInputMode(InputMode);
	}
}


void UUiWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PC = GetWorld()->GetFirstPlayerController();
}
