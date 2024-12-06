// Shoot_Them_UP


#include "Player/STUCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/STUCharacterMovementComponent.h"
#include "Components/STUHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/STUWeaponComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseCharacter, All, All)

// Sets default values
ASTUCharacter::ASTUCharacter(const FObjectInitializer& ObjInit)
	:Super(ObjInit.SetDefaultSubobjectClass<USTUCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->SocketOffset = FVector(0.0f, -70.0f, 80.0f);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<USTUHealthComponent>("HealthComponent");
	HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
	HealthTextComponent->SetupAttachment(GetRootComponent());
	HealthTextComponent->SetOwnerNoSee(true);

	WeaponComponent = CreateDefaultSubobject<USTUWeaponComponent>("WeaponComponent");
}
//--------------------------------------------------------------------------------------------------------------------------
// Called when the game starts or when spawned
void ASTUCharacter::BeginPlay()
{
	Super::BeginPlay();

	check(HealthComponent);
	check(HealthTextComponent);
	check(GetCharacterMovement());

	OnHealthChanged(HealthComponent->GetHealth(), 0.0f);
	HealthComponent->OnDeath.AddUObject(this, &ASTUCharacter::OnDeath);
	HealthComponent->OnHealthChanged.AddUObject(this, &ASTUCharacter::OnHealthChanged);

	LandedDelegate.AddDynamic(this, &ASTUCharacter::OnGroundLanded);
	
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::OnHealthChanged(float Health, float HealthDelta)
{
	HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}
// Called every frame
void ASTUCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
//--------------------------------------------------------------------------------------------------------------------------
// Called to bind functionality to input
void ASTUCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASTUCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTUCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASTUCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ASTUCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASTUCharacter::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASTUCharacter::OnStartRunning);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASTUCharacter::OnStopRunning);
	PlayerInputComponent->BindAction("Fire",IE_Pressed, WeaponComponent, &USTUWeaponComponent::StartFire);
	PlayerInputComponent->BindAction("Fire",IE_Released, WeaponComponent, &USTUWeaponComponent::StopFire);
	PlayerInputComponent->BindAction("ChangeWeapon",IE_Pressed, WeaponComponent, &USTUWeaponComponent::NextWeapon);
	PlayerInputComponent->BindAction("ReloadWeapon",IE_Pressed, WeaponComponent, &USTUWeaponComponent::Reload);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::MoveForward(float Amount)
{
	IsMovingForward = Amount > 0.0f; //the variable will be equal to true when the value Amount is greater than zero
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorForwardVector(), Amount);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::MoveRight(float Amount)
{
	if (Amount == 0.0f) return;
	AddMovementInput(GetActorRightVector(), Amount);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::OnStartRunning()
{
	WantsToRun = true;
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::OnStopRunning()
{
	WantsToRun = false;
}
//--------------------------------------------------------------------------------------------------------------------------
bool ASTUCharacter::IsRunning() const
{
	return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}
//--------------------------------------------------------------------------------------------------------------------------
float ASTUCharacter::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.0f;
	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);
	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::OnDeath()
{
	//PlayAnimMontage(DeathAnimMontage);

	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(LifeSpanOnDeath);
	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponComponent->StopFire();

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}
//--------------------------------------------------------------------------------------------------------------------------
void ASTUCharacter::OnGroundLanded(const FHitResult& Hit)
{
	const auto FallVelocityZ = -GetCharacterMovement()->Velocity.Z; //this sign "-" means multiple -1

	if (FallVelocityZ < LandedDamageVelocity.X) return;

	const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);
	UE_LOG(LogBaseCharacter, Display, TEXT("FinalDamage: %f"), FinalDamage);
	TakeDamage(FinalDamage, FDamageEvent{}, nullptr, nullptr);
}
//--------------------------------------------------------------------------------------------------------------------------
