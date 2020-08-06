// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Paths.h"
#include "TimerManager.h"
#include "NGN_ScoreManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGnomeKilledEvent, FString, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeleeComboEvent, int, GnomesCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMachineGunComboEvent, bool, IsComboEnd);

/**
 * 
 */
UCLASS(Blueprintable)
class NECROGNOMICON_API UNGN_ScoreManager : public UObject
{
	GENERATED_BODY()

private:

	//const FString SAVE_FOLDER = FPaths::ProjectDir() + "Saved/";
	const FString SAVE_FOLDER = FPaths::ProjectDir();
	const FString GNOMES_FILE_NAME = "gnomes";
	const FString MATCH_FILE_NAME = "match";
	const FString CSV_FILE_EXTENSION = ".csv";

	FString m_GnomesInfoString;
	FString m_MatchInfoString;

	int uid;

public:
	UNGN_ScoreManager();
	~UNGN_ScoreManager();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	bool SaveScoreToCSV = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	float MatchTimerInterval = 5.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	FString version;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	FString mode;

	// final score
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	float Score;
	// global killed count
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int KilledGnomes;
	// weapons killed count
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int MailboxGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int SlingshotGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int BowlGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int MachinegunGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int MowerGnomes;
	// trap killed count
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int BearGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int BarrelGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int CoalGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int IceGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int DiscoGnomes;
	// player total deaths
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int PlayerDeaths;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int BasicGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int FuriousGnomes;
	UPROPERTY(BlueprintReadOnly, Category = "Score")
	int TankGnomes;
	//Combos counter
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxStreak = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxStreakEver;
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxMailCombo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxMailComboEver;
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxSlingshotCombo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxSlingshotComboEver;
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxBarrelCombo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxBarrelComboEver;
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxBowlingBallCombo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxBowlingBallComboEver;
	UPROPERTY(BlueprintReadOnly, Category = "Combos")
	int MaxGunHitsCombo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Combos")
	int MaxGunHitsComboEver;

	/*
	* TakeDamage gnome event
	*/
	void KilledGnome(FString Gnome, float Health, float ActualDamage, FString DamageCauser, FVector Location, int CurrentLevel);

	/*
	* Match timer event
	*/
	void AddMatchRow(FString Event, int CurrentLevel, FVector PlayerLocation, float HouseHealth, bool HordeActive, float Difficulty, FString CurrentWeapon);

	void SaveCSV();

	void PlayerDied();

	void Reset();

	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
	FGnomeKilledEvent GnomeKilledEvent;

	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
	FMeleeComboEvent MeleeComboEvent;
	
	UPROPERTY(VisibleAnywhere, BlueprintAssignable, BlueprintCallable)
	FMachineGunComboEvent MachineGunComboEvent;
	
public:

	//Update combos
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateStreakCombo(int currentCombo);
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateMailCombo(int currentCombo);
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateSlingshotCombo(int currentCombo);
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateBarrelCombo(int currentCombo);
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateBowlingBallCombo(int currentCombo);
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void UpdateHitsCombo(int currentCombo);

	/*
	* De momento no vamos a mostar la información global guardada asi que no serian necesarias
	* estas funciones, vamos a dejarlas por si en un futuro se quiere mostar esta info
	*/
	UFUNCTION(BlueprintCallable, Category = "Combos")
	void LoadGlobalCombos(int Streak, int MailCombo, int SlingshotCombo,
		int BarrelCombo, int BowlingBallCombo, int HitsCombo);
	void UpdateGlobalCombos();

private:

	bool SaveString(const FString FileName, const FString FileExtension, const FString Content);

	FString GetWeaponString(FString Causer);
	FString GetTrapString(FString Causer);
	void WeaponCounter(FString Causer);
	void TrapCounter(FString Causer);

	int GetUID();
};
