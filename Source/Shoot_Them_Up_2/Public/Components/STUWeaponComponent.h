// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STUCoreTypes.h"
#include "STUWeaponComponent.generated.h"

class ASTUBase_Weapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOT_THEM_UP_2_API USTUWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USTUWeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();
	void Reload();

	bool GetCurrentWeaponUIData(FWeaponUIData& UIData) const; // ������� �������, ������� ���������� UIData �������� ������.
	bool GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const;

	bool TryToAddAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType, int32 ClipsAmount);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TArray<FWeaponData> WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponEquipSocketName = "Weapon_Socet";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	FName WeaponArmorySocket = "Armory_Socket";

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UAnimMontage* EquipAnimMontage;


	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	ASTUBase_Weapon* CurrentWeapon = nullptr; //������ ��� ��� ����� ������ ��������� CurrentWeapon ����� �������� �� �������������� ��������� �� ������� Weapons.

	UPROPERTY()
	TArray<ASTUBase_Weapon*> Weapons; //���������������� ������, ��� ����� ��������� ��� ��������� �� ������� ������, ������� �� �����������.

	UPROPERTY()
	UAnimMontage* CurrentRealoadAnimMontage = nullptr;

	int32 CurrentWeaponIndex = 0; //�������� ������ �������� ������� �� ������� ������ ��������� ��� ��������� CurrentWeapon.
	bool EquipAnimProgress = false;
	bool ReloadAnimProgress = false;


	void InitAnimations();//������� ��� AnimNotify � ������������� �� ����.
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTUBase_Weapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 WeaponIndex);

	void PlayAnimMontage(UAnimMontage* Animation);

	void OnEquipFinished(USkeletalMeshComponent* MeshComponent); // ������� CallBack, ������� �� ������ ������� bind �� ������� OnNotified
	void OnReloadFinished(USkeletalMeshComponent* MeshComponent);

	bool CanFire() const;
	bool CanEquip() const;
	bool CanReaload() const;

	void OnEmptyClip(ASTUBase_Weapon* AmmoEmptyWeapon);
	void ChangeClip();

};
