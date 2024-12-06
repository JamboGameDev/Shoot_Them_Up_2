// Shoot_Them_UP


#include "Ai/Services/STU_FindEnemy_Service.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtil.h"
#include "Components/STU_AI_PerceptionComponent.h"

USTU_FindEnemy_Service::USTU_FindEnemy_Service()
{
	NodeName = "Find Enemy";
}
//---------------------------------------------------------------------------------------------------------------------
void USTU_FindEnemy_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();
	if (BlackBoard)
	{
		const auto Controller = OwnerComp.GetAIOwner();
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTU_AI_PerceptionComponent>(Controller);
		if (PerceptionComponent)
		{
			BlackBoard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}


