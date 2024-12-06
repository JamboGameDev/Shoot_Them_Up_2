// Fill out your copyright notice in the Description page of Project Settings.


#include "STUGameModeBase.h"
#include "Player/STUCharacter.h"
#include "Player/STUPlayerController.h"
#include "UI/STUGameHUD.h"


ASTUGameModeBase::ASTUGameModeBase() 
{
   DefaultPawnClass = ASTUCharacter::StaticClass();
   PlayerControllerClass = ASTUPlayerController::StaticClass();
   HUDClass = ASTUGameHUD::StaticClass();
}