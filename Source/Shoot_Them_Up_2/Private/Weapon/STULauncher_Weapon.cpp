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

	//������������ ������� �����, �������� ���� ��������� ������. ���� ���������� HitResult.bBlockingHit = true, �� 
	//EndPoint ������ �������� HitResult.ImpactPoint, ���� HitResult �� ���������, �� ����� ����� TraceEnd.
	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

	const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
	ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);//�������, ���������� BeginPlay
	if(Projectile) //���� ��������� �� ��������� ����� �� �������
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform); //�� ��������� spawn ������.
	}
	Decrease();
	SpawnMuzzleFx();
}
//--------------------------------------------------------------------------------------------------------------------------