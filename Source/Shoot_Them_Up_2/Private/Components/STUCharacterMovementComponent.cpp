// Shoot_Them_UP


#include "Components/STUCharacterMovementComponent.h"
#include "Player/STUCharacter.h"

float USTUCharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const ASTUCharacter* Player = Cast<ASTUCharacter>(GetPawnOwner());
	return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}

