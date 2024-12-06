// Shoot_Them_UP


#include "Pickups/STUBasePickup.h"
#include "Components/SphereComponent.h"
#include "Components/STUWeaponComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogBasePickup, All, All)
// Sets default values
ASTUBasePickup::ASTUBasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(100.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //заметки
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SetRootComponent(CollisionComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyStaticMeshComponent"));
	StaticMesh->SetupAttachment(CollisionComponent);
	StaticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBasePickup::BeginPlay()
{
	Super::BeginPlay();
	
	check(CollisionComponent);

	GenerationYaw();
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorLocalRotation(FRotator(0.0f, RotationYaw, 0.0f));
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBasePickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	const auto Pawn = Cast<APawn>(OtherActor);
	
	if(GivePickupTo(Pawn))
	{
		PickupWasTaken();
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBasePickup::GivePickupTo(APawn* PlayerPawn)
{
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBasePickup::PickupWasTaken()
{
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if(GetRootComponent())
	{
		
		SetActorEnableCollision(false);
		GetRootComponent()->SetVisibility(false, true);
		
	}

	FTimerHandle RespawnTimerHandle;
	GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASTUBasePickup::Respawn, TimeRespawn);
	
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBasePickup::Respawn()
{
	GenerationYaw();

	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	if(GetRootComponent())
	{
		SetActorEnableCollision(true);
		GetRootComponent()->SetVisibility(true, true);
	}
}
//--------------------------------------------------------------------------------------------------------------------------
float ASTUBasePickup::GenerationYaw()
{
	const auto Direction = FMath::RandBool() ? 1.0f : -1.0f;
	RotationYaw = FMath::FRandRange(1.0f, 2.0f) * Direction;
	return 0.0f;
}
//--------------------------------------------------------------------------------------------------------------------------