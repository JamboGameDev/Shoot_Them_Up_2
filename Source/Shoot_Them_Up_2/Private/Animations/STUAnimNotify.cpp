// Shoot_Them_UP


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);//���� �������� ������������� �� ��� ������ �������, �� �� ������ AnimNotyfy ������������ �� �����.
	Super::Notify(MeshComp, Animation);
}