// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBase_Weapon.h"
#include "STULauncher_Weapon.generated.h"

class ASTUProjectile;

UCLASS()
class SHOOT_THEM_UP_2_API ASTULauncher_Weapon : public ASTUBase_Weapon
{
	GENERATED_BODY()

public:
	virtual void StartFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ASTUProjectile> ProjectileClass;

	virtual void MakeShot() override;
	
};
