// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STU_Next_Location_Task.generated.h"


UCLASS()
class SHOOT_THEM_UP_2_API USTU_Next_Location_Task : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	USTU_Next_Location_Task();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai")
	float Radius = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai")
	FBlackboardKeySelector AimLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai")
	bool SelfCenter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ai", meta = (EditCondition = "!SelfCenter"))
	FBlackboardKeySelector CenterActorKey;

};
