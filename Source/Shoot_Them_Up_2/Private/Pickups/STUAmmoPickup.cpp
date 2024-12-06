// Shoot_Them_UP


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBase_Weapon.h"
#include "GameFramework/Character.h"

#include "STUUtil.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All)

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn); //получаем указатель на health component
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn); //получаем указатель на weapon component
	if (!WeaponComponent) return false;
	

	return WeaponComponent->TryToAddAmmo(WeaponType, ClipsAmount);
}