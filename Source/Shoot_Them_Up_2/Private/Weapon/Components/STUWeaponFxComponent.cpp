// Shoot_Them_UP


#include "Weapon/Components/STUWeaponFxComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"

// Sets default values for this component's properties
USTUWeaponFxComponent::USTUWeaponFxComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponFxComponent::PlayImpactFx(const FHitResult& Hit)
{
   auto ImpactData = DefaultImpactData;

   if(Hit.PhysMaterial.IsValid())
   {
      const auto PhysMat = Hit.PhysMaterial.Get();
      if(ImpactDataMap.Contains(PhysMat))
      {
         ImpactData = ImpactDataMap[PhysMat];
      }
     
   }

   // Spawn Niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactData.NiagaraEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

   // decal
   auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(),
                                          ImpactData.DecalData.Material,
                                          ImpactData.DecalData.Size,
                                          Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

   if(DecalComponent)
   {
      DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
   }
}

