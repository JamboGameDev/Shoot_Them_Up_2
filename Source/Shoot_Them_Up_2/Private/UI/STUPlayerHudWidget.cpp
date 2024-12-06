// Shoot_Them_UP


#include "UI/STUPlayerHudWidget.h"
#include "Components/STUHealthComponent.h"
#include "Components/STUWeaponComponent.h"
#include "STUUtil.h"

bool USTUPlayerHudWidget::Initialize()
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	if(HealthComponent)
	{
		HealthComponent->OnHealthChanged.AddUObject(this, &USTUPlayerHudWidget::OnHealthChanged);
	}

	return Super::Initialize();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUPlayerHudWidget::OnHealthChanged(float Health, float HealthDelta)
{
	if(HealthDelta < 0.0f)
	{
		OnTakeDamage();
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------
float USTUPlayerHudWidget::GetHealthPercent() const
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());

	if (!HealthComponent) return 0.0f;

	return HealthComponent->GetHealthPercent();
	
} 
//--------------------------------------------------------------------------------------------------------------------------
bool USTUPlayerHudWidget::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponUIData(UIData);

}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUPlayerHudWidget::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(GetOwningPlayerPawn());
	if (!WeaponComponent) return false;

	return WeaponComponent->GetCurrentWeaponAmmoData(AmmoData);
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUPlayerHudWidget::IsPlayerAlive() const
{
	const auto HealthCompponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(GetOwningPlayerPawn());
	return HealthCompponent && !HealthCompponent->IsDead();
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUPlayerHudWidget::IsPlayerSpectating() const
{
	const auto Controller = GetOwningPlayer();//функция получение текущего контроллера игрока
	return Controller && Controller->GetStateName() == NAME_Spectating;
}
//--------------------------------------------------------------------------------------------------------------------------