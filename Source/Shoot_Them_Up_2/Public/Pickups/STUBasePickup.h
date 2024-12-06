// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUBasePickup.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class SHOOT_THEM_UP_2_API ASTUBasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ASTUBasePickup();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	float TimeRespawn = 5.0f;

	virtual void BeginPlay() override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	
	virtual void PickupWasTaken();

private:
	float RotationYaw = 0.0f;
	float GenerationYaw();

	virtual void Tick(float DeltaTime) override;

	virtual bool GivePickupTo(APawn* PlayerPawn);
	
	
	void Respawn();

};
