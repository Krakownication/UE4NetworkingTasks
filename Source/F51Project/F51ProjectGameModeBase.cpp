// Fill out your copyright notice in the Description page of Project Settings.

#include "F51ProjectGameModeBase.h"
#include "Public/MyPlayerCharacter.h"
#include "Public/MyPlayerController.h"
#include "Public/MyPlayerHUD.h"

AF51ProjectGameModeBase::AF51ProjectGameModeBase() : AGameModeBase()
 {
     PlayerControllerClass = AMyPlayerController::StaticClass();
 
     DefaultPawnClass = AMyPlayerCharacter::StaticClass();

     //GameStateClass = ACustomGameState::StaticClass();

     HUDClass = AMyPlayerHUD::StaticClass();

     //ReplaySpectatorPlayerControllerClass = ACustomReplaySpectatorPlayerController::StaticClass();
     //SpectatorClass = ACustomSpectatorClass::StaticClass();
 }


