// Shoot_Them_UP


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBase_Weapon.h"
#include "GameFramework/Character.h"
#include "Animations/STUEquipFinishedAnimNotify.h"
#include "Animations/STU_Reload_Finished_AnimNotify.h"
#include "Animations/AnimUtils.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All)

constexpr static int32 WeaponNum = 2;

// Sets default values for this component's properties
USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapon items"), WeaponNum);
	
	CurrentWeaponIndex = 0;
	InitAnimations();
	SpawnWeapons();
	EquipWeapon(CurrentWeaponIndex);
}
//--------------------------------------------------------------------------------------------------------------------------
//Функция EndPlay вызывается у каждого компонента при вызове функции EndPlay родительского актора
//Вызывается функция EndPlay у всех компонентов Character и унижтожает их
void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr; // не должен указывать ни на какой актор.
	for(auto Weapon:Weapons)
	{
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		Weapon->Destroy();
	}
	Weapons.Empty();//очищаем массив Weapons 

	Super::EndPlay(EndPlayReason);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::SpawnWeapons()
{

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || !GetWorld()) return;


	for (auto OneWeaponData : WeaponData) //заметки
	{
		auto Weapon = GetWorld()->SpawnActor<ASTUBase_Weapon>(OneWeaponData.WeaponClass);
		if (!Weapon) continue;

		Weapon->OnEmptyClip.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
		Weapon->SetOwner(Character); //устанавливаем Owner для нашего Character.
		Weapons.Add(Weapon);

		AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocket);
	}
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::AttachWeaponToSocket(ASTUBase_Weapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
	if (!Weapon || !SceneComponent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);

}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex) //данную функцию вызываем каждый раз, когда хотим установить новое оружие
{
	if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
	{
		UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid Weapon Index"));
				 return;
	}

	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	/*CurrentWeapon указывает на текущее оружие, поэтому в следующий раз, когда мы вызовем функцию EquipWeapon
	c другим индексом перед тем как сменить указатель CurrentWeapon мы должны вернуть оружие за спину персонажа,
	то есть вызвать функцию AttachWeaponToSocket с именем сокета WeaponArmorySocket.*/
	if (CurrentWeapon)
	{  
		CurrentWeapon->StopFire();//остановка стрельбы при смене оружия.
		AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocket);
		EquipAnimProgress = true;
	}

	CurrentWeapon = Weapons[WeaponIndex]; //CurrentWeapon указывает на актор, который мы хотим использовать в качестве основного оружия 

	//FilterByPredicate - функциональный объект,который возвращает истину или ложь. Далее мы воспользовались лямбда функцией.
	const auto CurrentWeaponData = WeaponData.FilterByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass();});
	CurrentRealoadAnimMontage = CurrentWeaponData.Num() > 0 ? CurrentWeaponData[0].ReloadAnimMontage : nullptr;

	AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);

	PlayAnimMontage(EquipAnimMontage);
}
//--------------------------------------------------------------------------------------------------------------------------

void USTUWeaponComponent::StartFire()
{
	if (!CanFire()) return; // при смене оружия мы так же стрелять не можем.
	CurrentWeapon->StartFire();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::StopFire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->StopFire();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return; //можем ли мы поменять оружие, если не можем выходим из функции

	/*Чтобы CurrentWeaponIndex не вышла за пределы массива Weapons возьмем CurrentWeaponIndex + 1
	* по модулю длины массива. То есть, если значение счетчика будет равняться количеству элементов массива, то оно будет превращаться в 0
	количество элементов массива вычисляется с помощью функции Num*/
	CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::PlayAnimMontage(UAnimMontage* Animation)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	Character->PlayAnimMontage(Animation);
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::InitAnimations()
{
	auto EquipFinishedNotify = AnimUtils::FindNotifyByClass<USTUEquipFinishedAnimNotify>(EquipAnimMontage);
	if(EquipFinishedNotify)
	{
		EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
	}else
	{
		UE_LOG(LogWeaponComponent, Error, TEXT("Equip anim notify is forgotten to set"));
		checkNoEntry();
	}

	for (auto OneWeaponData : WeaponData)
	{
		auto ReloadFinishNotify = AnimUtils::FindNotifyByClass <USTU_Reload_Finished_AnimNotify> (OneWeaponData.ReloadAnimMontage);
		if (!ReloadFinishNotify)
		{
			UE_LOG(LogWeaponComponent, Error, TEXT("Reload anim notify is forgotten to set"));
			checkNoEntry();
		}

		ReloadFinishNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}

}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	EquipAnimProgress = false;
	
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (!Character || MeshComponent != Character->GetMesh()) return;

	ReloadAnimProgress = false;

}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanFire() const 
{
	return CurrentWeapon && !EquipAnimProgress && !ReloadAnimProgress; 
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanEquip() const 
{
	//Таким образом, return !EquipAnimProgress; возвращает true, если анимация смены оружия завершена или не выполняется, 
	// и false, если анимация в процессе. Это используется для проверки, можно ли сейчас сменить оружие.
	return !EquipAnimProgress && !ReloadAnimProgress;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::CanReaload() const 
{
	return CurrentWeapon && !EquipAnimProgress && !ReloadAnimProgress && CurrentWeapon->CanReload(); 
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::Reload()
{
	ChangeClip();
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::OnEmptyClip(ASTUBase_Weapon* AmmoEmptyWeapon)
{
	if(CurrentWeapon == AmmoEmptyWeapon)
	{
		ChangeClip();
	}
	else 
	{
		for (const auto Weapon : Weapons)
		{
			if(Weapon == AmmoEmptyWeapon)
			{
				Weapon->ChangeClip();
			}
			
		}
	}
	
}
//--------------------------------------------------------------------------------------------------------------------------
void USTUWeaponComponent::ChangeClip()
{
	if (!CanReaload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
	ReloadAnimProgress = true;
	PlayAnimMontage(CurrentRealoadAnimMontage);
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
	if(CurrentWeapon)
	{
		UIData = CurrentWeapon->GetUIData();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
	if(CurrentWeapon)
	{
		AmmoData = CurrentWeapon->GetAmmoData();
		return true;
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBase_Weapon> WeaponType, int32 ClipsAmount)
{
	for(const auto Weapon : Weapons)
	{
		if(Weapon && Weapon->IsA(WeaponType))//IsA - данная функция класса UObject принимает в качестве параметра тип и возвращает true, если текущий объект принадлежит данному типу.
		{
			return Weapon->TryToAddAmmo(ClipsAmount);
		}
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------------------------