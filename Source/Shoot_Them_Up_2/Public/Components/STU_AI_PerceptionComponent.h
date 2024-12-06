// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STU_AI_PerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOT_THEM_UP_2_API USTU_AI_PerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()
	
public:
	AActor* GetClosestEnemy() const;
};
