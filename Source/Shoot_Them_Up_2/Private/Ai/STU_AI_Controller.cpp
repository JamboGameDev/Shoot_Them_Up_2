// Shoot_Them_UP


#include "Ai/STU_AI_Controller.h"
#include "Ai/STU_AI_Character.h"
#include "Components/STU_AI_PerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASTU_AI_Controller::ASTU_AI_Controller()
{
	STU_AI_PerceptionComponent = CreateDefaultSubobject<USTU_AI_PerceptionComponent>("STUPerceptionComponent");
	SetPerceptionComponent(*STU_AI_PerceptionComponent); //чтобы преобразовать указатель в объект, нужно выполнить разыменовывание.
}
//---------------------------------------------------------------------------------------------------------------------
void ASTU_AI_Controller::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	const auto STUCharacter = Cast<ASTU_AI_Character>(InPawn);
	if(STUCharacter)
	{
		RunBehaviorTree(STUCharacter->BehaviorTree);
	}
}
//---------------------------------------------------------------------------------------------------------------------
void ASTU_AI_Controller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	const auto AimActor = GetFocusOnActor();
	SetFocus(AimActor);
}
//---------------------------------------------------------------------------------------------------------------------
AActor* ASTU_AI_Controller::GetFocusOnActor() const
{
	if(!GetFocusActor()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(FocusOnKeyName));
	
}
