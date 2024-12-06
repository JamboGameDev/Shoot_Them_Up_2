// Shoot_Them_UP


#include "Weapon/STURifle_Weapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

ASTURifle_Weapon::ASTURifle_Weapon()
{
	WeaponFXComponent = CreateAbstractDefaultSubobject<USTUWeaponFxComponent>("WeaponFXComponent");
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponFXComponent);

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::StartFire()
{
	InitMuzzleFX();
	GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifle_Weapon::MakeShot, TimeBetweenShots, true); // TimeBetweenShots параметр отвчеающий за частоту выполнения таймера. True значит таймер будет повторяться
	MakeShot();
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(ShotTimerHandle);
	SetMuzzleFXVisibilty(false);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::MakeShot()
{
	UE_LOG(LogTemp, Display, TEXT("MakeShot"));
	if (!GetWorld() || IsAmmoEmpty())
	{
		StopFire();
		return;
	}

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd))
	{
		StopFire();
		return;
	}

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	FVector TraceFxEnd = TraceEnd;
	
	if(WeaponFXComponent && HitResult.bBlockingHit)
	{
		TraceFxEnd = HitResult.ImpactPoint;
		MakeDamage(HitResult);
		WeaponFXComponent->PlayImpactFx(HitResult);

	}

	SpawnTraceFX(GetMuzzleWorldLocation(), TraceFxEnd);
	Decrease();
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTURifle_Weapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const auto HalfRad = FMath::DegreesToRadians(BulletSpread); //разброс выстрела пули.
	const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad); //функция, которая делает разброс.
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::MakeDamage(const FHitResult& HitResult)
{
	const auto DamageActor = HitResult.GetActor();
	if (!DamageActor) return; //Если указатель нулевой, то мы выходим из функции.

	DamageActor->TakeDamage(DamageAmount, FDamageEvent(), GetPlayerController(), this);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::InitMuzzleFX()
{
	if(!MuzzleFxComponent)
	{
		MuzzleFxComponent = SpawnMuzzleFx();
	}

	SetMuzzleFXVisibilty(true);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::SetMuzzleFXVisibilty(bool Visibile)
{
	if(MuzzleFxComponent)
	{
		MuzzleFxComponent->SetPaused(!Visibile);
		MuzzleFxComponent->SetVisibility(Visibile, true);
	}
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTURifle_Weapon::SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd)
{
	const auto TraceFxComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraceFx, TraceStart);
	if(TraceFxComponent)
	{
		TraceFxComponent->SetNiagaraVariableVec3(TraceTargetName, TraceEnd);
	}
}
//--------------------------------------------------------------------------------------------------------------------------