// Shoot_Them_UP


#include "Weapon/STUProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Components/STUWeaponFxComponent.h"

// Sets default values
ASTUProjectile::ASTUProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	CollisionComponent->InitSphereRadius(5.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //заметки
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block); //блокировка ракеты во всех каналах коллизии.
	CollisionComponent->bReturnMaterialOnMove = true;
	SetRootComponent(CollisionComponent);


	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
	MovementComponent->InitialSpeed = 2000.0f;
	MovementComponent->ProjectileGravityScale = 0.5f;

	WeaponFXComponent = CreateAbstractDefaultSubobject<USTUWeaponFxComponent>("WeaponFXComponent");

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUProjectile::BeginPlay()
{
	Super::BeginPlay();

	//if (!MovementComponent->InitialSpeed) return;

	check(MovementComponent);
	check(CollisionComponent);
	check(WeaponFXComponent);

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);  //указываем компоненту колизии каких акторов мы хотим игнорировать во время движения 
	CollisionComponent->OnComponentHit.AddDynamic(this, &ASTUProjectile::OnProjectileHit); //Делегат вызывается тогда, когда произошло столкновение с актором или другими компонентом. 
	SetLifeSpan(LifeSeconds);

}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!GetWorld()) return;

	MovementComponent->StopMovementImmediately();

	//Make Damage
	UGameplayStatics::ApplyRadialDamage(GetWorld(),
	                                   DamageAmount,
	                                   GetActorLocation(),
	                                   DamageRadius,
	                                   UDamageType::StaticClass(),//UDamageType::StaticClass() базовый класс. 
	                                  {GetOwner()},//масив акторов, который игнорируется при нанесении ущерба
	                                   this,
	                                   GetController(),
	                                   DoFullDamage);//наносится ли ущерб в каждой точке сферы одинково или будет равномерно увеличиваться от поверхности сферы к радиусу.

	//DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 24, FColor::Red, false, 5.0f);
	WeaponFXComponent->PlayImpactFx(Hit);

	Destroy();
}
//--------------------------------------------------------------------------------------------------------------------------
AController* ASTUProjectile::GetController() const
{
	const auto Pawn = Cast<APawn>(GetOwner()); //проверка для указателя GetOwner() является ли он APawn или нет и если является, то вернуть его Controller.
	return Pawn ? Pawn->GetController() : nullptr;//в дальнейшем при подсчете кто кого подстрелил, мы будем использовать указатель на данный контроллер.

}
