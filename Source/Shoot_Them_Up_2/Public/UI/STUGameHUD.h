// Shoot_Them_UP

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "STUGameHUD.generated.h"


UCLASS()
class SHOOT_THEM_UP_2_API ASTUGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf <UUserWidget> PlayerHudWidgetClass;

	virtual void BeginPlay() override;

private:
	void DrawCrossHair();
};
