// Shoot_Them_UP


#include "Ai/STU_AI_Character.h"
#include "Ai/STU_AI_Controller.h"
#include "GameFramework/CharacterMovementComponent.h"

ASTU_AI_Character::ASTU_AI_Character(const FObjectInitializer& ObjInit) :Super(ObjInit)
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ASTU_AI_Controller::StaticClass();

	bUseControllerRotationYaw = false;
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
}
