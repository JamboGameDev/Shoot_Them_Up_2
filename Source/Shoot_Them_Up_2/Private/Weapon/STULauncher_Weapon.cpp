// Shoot_Them_UP


#include "Weapon/STULauncher_Weapon.h"
#include "Weapon/STUProjectile.h"

//--------------------------------------------------------------------------------------------------------------------------
void ASTULauncher_Weapon::StartFire()
{
	MakeShot();
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTULauncher_Weapon::MakeShot()
{

	if (!GetWorld() || IsAmmoEmpty()) return;

	FVector TraceStart, TraceEnd;
	if (!GetTraceData(TraceStart, TraceEnd)) return;

	FHitResult HitResult;
	MakeHit(HitResult, TraceStart, TraceEnd);

	//высчитывания вектора вдоль, которого надо запустить снаряд. Если переменная HitResult.bBlockingHit = true, то 
	//EndPoint примет значение HitResult.ImpactPoint, если HitResult не произошло, то берем точку TraceEnd.
	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);//заметки, отложенный BeginPlay
	if(Projectile) //если указатель на созданный актор не нулевой
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform); //то завершаем spawn актора.
	}
	Decrease();
	SpawnMuzzleFx();
}
//--------------------------------------------------------------------------------------------------------------------------