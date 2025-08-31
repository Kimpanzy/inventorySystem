// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventorySystem/DataTypes/ItemData.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UItemData;

UCLASS()
class INVENTORYSYSTEM_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	
	virtual void Tick(float DeltaTime) override;
	AItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="ItemData",meta=(AllowPrivateAccess=true))
	FItemStats ItemStats;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
private:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh",meta=(AllowPrivateAccess=true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Sphere",meta=(AllowPrivateAccess=true))
	USphereComponent* Sphere;

	
};
