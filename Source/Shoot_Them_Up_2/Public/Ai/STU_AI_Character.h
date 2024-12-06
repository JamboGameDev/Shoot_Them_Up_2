// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "Player/STUCharacter.h"
#include "STU_AI_Character.generated.h"

class UBehaviorTree;

UCLASS()
class SHOOT_THEM_UP_2_API ASTU_AI_Character : public ASTUCharacter
{
	GENERATED_BODY()

public:
	ASTU_AI_Character(const FObjectInitializer& ObjInit);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ai")
	UBehaviorTree* BehaviorTree;
};
