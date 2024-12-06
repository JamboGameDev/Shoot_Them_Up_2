// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUAmmoPickup.generated.h"

class ASTUBase_Weapon;

UCLASS()
class SHOOT_THEM_UP_2_API ASTUAmmoPickup : public ASTUBasePickup
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "10.0"))
	int32 ClipsAmount = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup")
	TSubclassOf<ASTUBase_Weapon> WeaponType; 

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
	
};
