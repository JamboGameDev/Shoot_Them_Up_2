// Shoot_Them_UP


#include "Ai/Services/STU_Fire_Service.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtil.h"
#include "Components/STUWeaponComponent.h"

USTU_Fire_Service::USTU_Fire_Service()
{
	NodeName = "Fire";
}
//---------------------------------------------------------------------------------------------------------------------
void USTU_Fire_Service::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto BlackBoard = OwnerComp.GetBlackboardComponent();

	const auto HasAim = BlackBoard && BlackBoard->GetValueAsObject(EnemyActorKey.SelectedKeyName);

	if(Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
		HasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire(); // condition fire
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds); 
}
//---------------------------------------------------------------------------------------------------------------------
