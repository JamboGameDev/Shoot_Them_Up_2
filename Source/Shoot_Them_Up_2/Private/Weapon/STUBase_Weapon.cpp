// Shoot_Them_UP


#include "Weapon/STUBase_Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

//--------------------------------------------------------------------------------------------------------------------------
ASTUBase_Weapon::ASTUBase_Weapon()
{
 
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::BeginPlay()
{
	Super::BeginPlay();

	check(WeaponMesh);
	checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));

	CurrentAmmo = DefaultAmmo;
	
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::StartFire()
{
	
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::StopFire()
{
	
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::MakeShot()
{
	
}
//--------------------------------------------------------------------------------------------------------------------------
APlayerController* ASTUBase_Weapon::GetPlayerController() const
{
	const auto Player = Cast<ACharacter>(GetOwner());
	if (!Player) return nullptr;

	return Player->GetController<APlayerController>(); //≈сли указатель на Player валидный, то возвращаем указатель, который возвращает функци€ GetController<>
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto STUCharacter = Cast<ACharacter>(GetOwner());
	if (!STUCharacter) return false;

	if(STUCharacter->IsPlayerControlled())
	{
		const auto Controller = GetPlayerController();
		if (!Controller) return false;//≈сли указатель на контроллер nullptr, то мы должны вернуть false, потому что наша функци€ возвращает bool.

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------
FVector ASTUBase_Weapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd) const
{
	FVector ViewLocation;
	FRotator ViewRotation;
	if(!GetPlayerViewPoint(ViewLocation, ViewRotation)) return false;

	TraceStart = ViewLocation;
	const FVector ShootDirection = ViewRotation.Vector();
	TraceEnd = TraceStart + ShootDirection * TraceMaxDistance;
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::MakeHit (FHitResult& HitResult, FVector& TraceStart, FVector& TraceEnd)
{
	if (!GetWorld()) return;

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner()); //игнор актора, чтобы не стрел€ть в себ€.
	CollisionParams.bReturnPhysicalMaterial = true;

	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::Decrease()
{
	if(CurrentAmmo.Bullets == 0)
	{
		UE_LOG(LogBaseWeapon, Warning, TEXT("Clip is Empty"));
		return;
	}

	CurrentAmmo.Bullets--;

	if (IsClipEmpty() && !IsAmmoEmpty())
	{
		StopFire();
		OnEmptyClip.Broadcast(this);
	}
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsClipEmpty();
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::IsClipEmpty() const
{
	return CurrentAmmo.Bullets == 0;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::ChangeClip()
{
	
	if (!CurrentAmmo.Infinite)
	{
		if(CurrentAmmo.Clips == 0)
			{
			  UE_LOG(LogBaseWeapon, Warning, TEXT("No more Clips"));
			  return;
			}
		CurrentAmmo.Clips--;
	}
	CurrentAmmo.Bullets = DefaultAmmo.Bullets;
	UE_LOG(LogBaseWeapon, Display, TEXT("----Change_Clip----"));
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUBase_Weapon::LogAmmo()
{
	FString AmmoInfo = "Ammo: " + FString::FromInt(CurrentAmmo.Bullets) + " / ";
	AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips); //Ћибо количество обойм, если арсенал конечен, либо текст "Infinite", если арсенал бесконечен.
	UE_LOG(LogBaseWeapon, Display, TEXT("%s"), * AmmoInfo);

}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::IsAmmoFull() const
{

	return CurrentAmmo.Clips == DefaultAmmo.Clips && //
		CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUBase_Weapon::TryToAddAmmo(int32 ClipsAmount)
{
	if (CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0) return false;

	if(IsAmmoEmpty())
	{
		UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was empty"))
		CurrentAmmo.Clips = FMath::Clamp(ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnEmptyClip.Broadcast(this);
	}
	else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
	{
		const auto NextClipAmount = CurrentAmmo.Clips + ClipsAmount;
		if (DefaultAmmo.Clips - NextClipAmount >= 0)
		{
			CurrentAmmo.Clips = NextClipAmount;
			UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"))
		}
		else
		{
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
			UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full"))
		}
	}
	else
	{
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
		CurrentAmmo.Clips = DefaultAmmo.Clips;
		UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"))
	}
	return true;
}
//--------------------------------------------------------------------------------------------------------------------------
UNiagaraComponent* ASTUBase_Weapon::SpawnMuzzleFx()
{
	return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFx,
																WeaponMesh,
																MuzzleSocketName,
																FVector::ZeroVector,
																FRotator::ZeroRotator,
																EAttachLocation::SnapToTarget, true);
}
