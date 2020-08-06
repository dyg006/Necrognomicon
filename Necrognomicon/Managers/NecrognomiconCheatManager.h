// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "NecrognomiconCheatManager.generated.h"

/**
 * 
 */

UCLASS()
class NECROGNOMICON_API UNecrognomiconCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(exec)
	virtual void TestMyCheatManager();

	UFUNCTION(exec)
	virtual void TestWithParam(FString arg1);

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "restart"))
	void Restart();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "crono"))
	void Crono(const FString& command);

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "antonio"))
	void Antonio();  //sorpresa

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "ngn_reset"))
	void NGN_Reset(const FString& level);

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "gameover"))
	void GameOver();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "victory"))
	void Victory();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "abretesesamo"))
	void AbreteSesamo();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "cierratesesamo"))
	void CierrateSesamo();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "next"))
	void Next();

	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "chucknorris"))
	void ChuckNorris();

	// Same as ChuckNorris
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "raluy"))
	void Raluy();

	// Same as God
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "zazo"))
	void Zazo();

	// Same as NGN_Reset
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "level"))
	void Level(const FString& level);

	// same as DestroyPawns
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "vela"))
	void Vela();

	// same as DestroyPawns
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "juan"))
	void Juan();

	// same as DestroyPawns
	UFUNCTION(exec, BlueprintImplementableEvent, meta = (DisplayName = "marc"))
	void Marc();
};
