// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STU_FindEnemy_Service.generated.h"

/**
 * 
 */
UCLASS()
class SHOOT_THEM_UP_2_API USTU_FindEnemy_Service : public UBTService
{
	GENERATED_BODY()

public:
	USTU_FindEnemy_Service();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	FBlackboardKeySelector EnemyActorKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
