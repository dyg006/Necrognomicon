// Fill out your copyright notice in the Description page of Project Settings.


#include "NecrognomiconCheatManager.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

void UNecrognomiconCheatManager::TestMyCheatManager()
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Hello My Cheat Manager!"));
	}
}

void UNecrognomiconCheatManager::TestWithParam(FString arg1)
{
	if (GEngine) {
		FString msg = "Hello " + arg1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, msg);
	}
}
