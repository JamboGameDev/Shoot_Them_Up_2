// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBase_Weapon.h"
#include "STURifle_Weapon.generated.h"

class USTUWeaponFxComponent;
class UNiagaraComponent;
class UNiagaraSystem;

UCLASS()
class SHOOT_THEM_UP_2_API ASTURifle_Weapon : public ASTUBase_Weapon
{
	GENERATED_BODY()

public:
	ASTURifle_Weapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float TimeBetweenShots = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float DamageAmount = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraceFx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	FString TraceTargetName = "TraceTarget";

	UPROPERTY(VisibleAnywhere, Category = "VFX")
	USTUWeaponFxComponent* WeaponFXComponent;

	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;

private:
	FTimerHandle ShotTimerHandle;

	UPROPERTY()
	UNiagaraComponent* MuzzleFxComponent;

	void MakeDamage(const FHitResult& HitResult);
	
	void InitMuzzleFX();
	void SetMuzzleFXVisibilty(bool Visibile);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);
};
