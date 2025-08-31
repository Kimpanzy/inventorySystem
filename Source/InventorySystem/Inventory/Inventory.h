
#pragma once

#include "InventorySystem/DataTypes/InventorySlotData.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

class UUiWidget;
class UDataTable;
class UUiInventorySlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemAdded, FInventorySlotData, SlotData);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventory : public UActorComponent
{
	GENERATED_BODY()

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UInventory();
	void SetupUiClass();

	int FindSlotWithItem(FName Name, int QuantityToAdd);
	UFUNCTION()
	void AddItem(AItem* Item);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory slots", meta = (AllowPrivateAccess = true))
	int32 NumberOfSlots;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FInventorySlotData> InventorySlotData;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnItemAdded OnItemAdded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Data")
	UDataTable* InventoryDataTable;

	void DragAndDropUpdate(UUiInventorySlot* TargetSlot, UUiInventorySlot* SourceSlot);
	
	UFUNCTION()
	void DropItemFromSlot(UUiInventorySlot* ItemToDrop);

	
	void SpawnItem(FItemStats);
protected:
	virtual void BeginPlay() override;
private:
	UFUNCTION()
	void InitializeSlot();
	
	UFUNCTION()
	int CheckForEmptySlot();

	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUiWidget> UiClass;
	
	UFUNCTION(BlueprintCallable, Category = "UI")
	UUiWidget* GetUiRef() const { return UiRef; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="UI", meta = (AllowPrivateAccess = "true"))
	UUiWidget* UiRef = nullptr;

		
};
