
#include "InventorySystem/Item/Item.h"
#include "Components/SphereComponent.h"
#include "InventorySystem/InventorySystemCharacter.h"
#include "InventorySystem/Inventory/Inventory.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;
	
	/*USceneComponent* Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;*/
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	

}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnEndOverlap);

	
}
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto* const Player = Cast<AInventorySystemCharacter>(OtherActor))
	{
		if (UInventory* Inventory = Player->GetInventoryComponent())
		{
			Inventory->AddItem(this);
		}
		Destroy();
	}
}

void AItem::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	
}



