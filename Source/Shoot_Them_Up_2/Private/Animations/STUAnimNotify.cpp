// Shoot_Them_UP


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotified.Broadcast(MeshComp);//Если анимация проигрывается не для нашего скелета, то мы данный AnimNotyfy обрабатывать не будем.
	Super::Notify(MeshComp, Animation);
}