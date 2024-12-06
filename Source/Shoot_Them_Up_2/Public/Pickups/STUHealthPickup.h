// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Pickups/STUBasePickup.h"
#include "STUHealthPickup.generated.h"


UCLASS()
class SHOOT_THEM_UP_2_API ASTUHealthPickup : public ASTUBasePickup
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pickup", meta = (ClampMin = "1.0", ClampMax = "100.0"))
	float HealthAmount = 100;

private:
	virtual bool GivePickupTo(APawn* PlayerPawn) override;
	
};
