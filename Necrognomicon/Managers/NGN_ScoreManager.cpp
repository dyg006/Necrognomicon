// Fill out your copyright notice in the Description page of Project Settings.


#include "NGN_ScoreManager.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"

UNGN_ScoreManager::UNGN_ScoreManager()
{
	Reset();
}

UNGN_ScoreManager::~UNGN_ScoreManager()
{

}

void UNGN_ScoreManager::KilledGnome(FString Gnome, float Health, float ActualDamage, FString DamageCauser, FVector Location, int CurrentLevel)
{
	//UE_LOG(LogTemp, Warning, TEXT("Killed Gnome"));
	if (Health == 0) {
		KilledGnomes++;

		WeaponCounter(DamageCauser);
		TrapCounter(DamageCauser);

		if (Gnome.Contains("Enemy")) {
			BasicGnomes++;
		}
		else if (Gnome.Contains("Furious")) {
			FuriousGnomes++;
		}
		else if (Gnome.Contains("Strong")) {
			TankGnomes++;
		}

		// Lazamos evento de combos
		GnomeKilledEvent.Broadcast(DamageCauser);
	}

	//FString timeStamp = FDateTime::Now().ToString();
	float gameTimeInSeconds = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	FString sTime = FString::FromInt(UKismetMathLibrary::FTrunc(gameTimeInSeconds));
	FString sHealth = FString::FromInt(UKismetMathLibrary::FTrunc(Health));
	FString sEvent = Health == 0 ? "Killed Gnome" : "Hurted Gnome";
	
	// Causer
	FString sCauserClass = DamageCauser;
	FString sType = sCauserClass.Contains("Trap") ? "Trap" : "Weapon";
	FString sWeapon = "?";
	if (sType.Equals("Trap"))
	{
		sWeapon = GetTrapString(sCauserClass);
	}
	else if (sType.Equals("Weapon"))
	{
		sWeapon = GetWeaponString(sCauserClass);
	}

	FString sDamage = FString::FromInt(UKismetMathLibrary::FTrunc(ActualDamage));
	FString sLocationX = FString::FromInt(UKismetMathLibrary::FTrunc(Location.X));
	FString sLocationY = FString::FromInt(UKismetMathLibrary::FTrunc(Location.Y));
	FString sCurrentLevel = FString::FromInt(CurrentLevel);
	
	FString sGnome = Gnome;	
	FString sUID = FString::FromInt(uid);

	// Add new event info
	m_GnomesInfoString += sUID + ","
		+ sTime + "," 
		+ sEvent + "," 
		+ sGnome + "," 
		+ sHealth + "," 
		+ sType + "," 
		+ sWeapon + "," 
		+ sCauserClass + "," 
		+ sDamage + "," 
		+ sLocationX + "," 
		+ sLocationY + "," 
		+ sCurrentLevel + ","
		+ version + ","
		+ mode
		+ LINE_TERMINATOR;
}

void UNGN_ScoreManager::AddMatchRow(FString Event, int CurrentLevel, FVector PlayerLocation, float HouseHealth, bool HordeActive, float Difficulty, FString CurrentWeapon)
{
	float gameTimeInSeconds = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	FString sTime = FString::FromInt(UKismetMathLibrary::FTrunc(gameTimeInSeconds));

	FString sCurrentLevel = FString::FromInt(UKismetMathLibrary::FTrunc(CurrentLevel));
	FString sKilledGnomes = FString::FromInt(KilledGnomes);
	FString sPlayerLocationX = FString::FromInt(UKismetMathLibrary::FTrunc(PlayerLocation.X));
	FString sPlayerLocationY = FString::FromInt(UKismetMathLibrary::FTrunc(PlayerLocation.Y));
	FString sUID = FString::FromInt(uid);
	FString sHouseHealth = FString::FromInt(UKismetMathLibrary::FTrunc(HouseHealth));
	FString sHordeActive = HordeActive ? "Yes" : "No";
	FString sDifficulty = FString::FromInt(UKismetMathLibrary::FTrunc(Difficulty));
	FString sNumDeaths = FString::FromInt(PlayerDeaths);

	m_MatchInfoString += sUID + ","
		+ sTime + ","
		+ Event + ","
		+ sCurrentLevel + ","
		+ sHordeActive + ","
		+ sDifficulty + ","
		+ sKilledGnomes + ","
		+ sPlayerLocationX + ","
		+ sPlayerLocationY + ","
		+ CurrentWeapon + ","
		+ sHouseHealth + ","
		+ sNumDeaths + ","
		+ version + ","
		+ mode
		+ LINE_TERMINATOR;
}
/*
	MailboxGnomes, SlingshotGnomes, BowlGnomes, MachinegunGnomes, MowerGnomes
*/
FString UNGN_ScoreManager::GetWeaponString(FString Causer)
{
	if (Causer.Contains("Tirachinas")) { return "Tirachinas"; }
	else if (Causer.Contains("CortaCesped")) {return "CortaCesped"; }
	else if (Causer.Contains("BolaBolos")) { return "BolaBolos"; }
	else if (Causer.Contains("AutoMachineGun")) { return "AutoMachineGun"; }
	else if (Causer.Contains("Melee")) { return "Buzon"; }
	else { return "?"; }
}

/*
	BearGnomes, BarrelGnomes, CoalGnomes, IceGnomes, DiscoGnomes
*/
FString UNGN_ScoreManager::GetTrapString(FString Causer)
{
	if (Causer.Contains("Bear")) { return "Bear"; }
	else if (Causer.Contains("Disco")) { return "Disco"; }
	else if (Causer.Contains("Coal")) { return "Coal"; }
	else if (Causer.Contains("Ice")) { return "Ice"; }
	else if (Causer.Contains("Barrel")) { return "Barrel"; }
	else { return "?"; }
}

void UNGN_ScoreManager::WeaponCounter(FString Causer)
{
	if (Causer.Contains("Tirachinas")) { SlingshotGnomes++; }
	else if (Causer.Contains("CortaCesped")) { MowerGnomes++; }
	else if (Causer.Contains("BolaBolos")) { BowlGnomes++; }
	else if (Causer.Contains("AutoMachineGun")) { MachinegunGnomes++; }
	else if (Causer.Contains("Melee")) { MailboxGnomes++; }
}

void UNGN_ScoreManager::TrapCounter(FString Causer)
{
	if (Causer.Contains("Bear")) { BearGnomes++; }
	else if (Causer.Contains("Disco")) { DiscoGnomes++; }
	else if (Causer.Contains("Coal")) { CoalGnomes++; }
	else if (Causer.Contains("Ice")) { IceGnomes++; }
	else if (Causer.Contains("Barrel")) { BarrelGnomes++; }
}

int UNGN_ScoreManager::GetUID()
{
	/*
	FString FinalFileName;
	bool bFileName = false;
	int i = 0;
	while (!bFileName) {
		FString FileNameExists = SAVE_FOLDER + GNOMES_FILE_NAME + FString::FromInt(i) + CSV_FILE_EXTENSION;
		if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileNameExists)) {
			FinalFileName = FileNameExists;
			bFileName = true;
			break;
		}
		i++;
	}
	return i;
	*/
	return FMath::Rand();
}

void UNGN_ScoreManager::Reset()
{
	m_GnomesInfoString = "uid,Time,Event,Gnome,Health,Type,Weapon,SubWeapon,Damage,LocationX,LocationY,Level,Version,Mode\n";
	m_MatchInfoString = "uid,MatchTime,MatchEvent,MatchLevel,HordeActive,Difficulty,KilledGnomes,PlayerPosX,PlayerPosY,CurrentWeapon,HouseHealth,sNumDeaths,Version,Mode\n";
	uid = GetUID();
	Score = 0.f;
	
	KilledGnomes = 0;
	
	MailboxGnomes = 0;
	SlingshotGnomes = 0;
	BowlGnomes = 0;
	MachinegunGnomes = 0;
	MowerGnomes = 0;
	BearGnomes = 0;
	BarrelGnomes = 0;
	CoalGnomes = 0;
	IceGnomes = 0;
	DiscoGnomes = 0;
	
	PlayerDeaths = 0;

	BasicGnomes = 0;
	FuriousGnomes = 0;
	TankGnomes = 0;
}

void UNGN_ScoreManager::SaveCSV()
{
	if (SaveScoreToCSV) {
		SaveString(GNOMES_FILE_NAME + FString::FromInt(uid), CSV_FILE_EXTENSION, m_GnomesInfoString);
		SaveString(MATCH_FILE_NAME + FString::FromInt(uid), CSV_FILE_EXTENSION, m_MatchInfoString);
	}
	Reset();
}

void UNGN_ScoreManager::PlayerDied()
{
	PlayerDeaths++;
}

void UNGN_ScoreManager::UpdateStreakCombo(int currentCombo)
{
	if (MaxStreak < currentCombo)
		MaxStreak = currentCombo;
}

void UNGN_ScoreManager::UpdateMailCombo(int currentCombo)
{
	if (MaxMailCombo < currentCombo)
		MaxMailCombo = currentCombo;
}

void UNGN_ScoreManager::UpdateSlingshotCombo(int currentCombo)
{
	if (MaxSlingshotCombo < currentCombo)
		MaxSlingshotCombo = currentCombo;
}

void UNGN_ScoreManager::UpdateBarrelCombo(int currentCombo)
{
	if (MaxBarrelCombo < currentCombo)
		MaxBarrelCombo = currentCombo;
}

void UNGN_ScoreManager::UpdateBowlingBallCombo(int currentCombo)
{
	if (MaxBowlingBallCombo < currentCombo)
		MaxBowlingBallCombo = currentCombo;
}

void UNGN_ScoreManager::UpdateHitsCombo(int currentCombo)
{
	if (MaxGunHitsCombo < currentCombo)
		MaxGunHitsCombo = currentCombo;
}

void UNGN_ScoreManager::LoadGlobalCombos(int Streak, int MailCombo, int SlingshotCombo, int BarrelCombo, int BowlingBallCombo, int HitsCombo)
{
	MaxStreakEver = Streak;
	MaxMailComboEver = MailCombo;
	MaxSlingshotComboEver = SlingshotCombo;
	MaxBarrelComboEver = BarrelCombo;
	MaxBowlingBallComboEver = BowlingBallCombo;
	MaxGunHitsComboEver = HitsCombo;
}

void UNGN_ScoreManager::UpdateGlobalCombos()
{
	if (MaxStreakEver < MaxStreak)
		MaxStreakEver = MaxStreak;
	
	if (MaxMailComboEver < MaxMailCombo)
		MaxMailComboEver = MaxMailCombo;

	if (MaxSlingshotComboEver < MaxSlingshotCombo)
		MaxSlingshotComboEver = MaxSlingshotCombo;

	if (MaxBarrelComboEver < MaxBarrelCombo)
		MaxBarrelComboEver = MaxBarrelCombo;

	if (MaxBowlingBallComboEver < MaxBowlingBallCombo)
		MaxBowlingBallComboEver = MaxBowlingBallCombo;

	if (MaxGunHitsComboEver < MaxGunHitsCombo)
		MaxGunHitsComboEver = MaxGunHitsCombo;
}

bool UNGN_ScoreManager::SaveString(const FString FileName, const FString FileExtension, const FString Content)
{
	FString FinalFileName = SAVE_FOLDER + FileName + FileExtension;

	//UE_LOG(LogTemp, Warning, TEXT("SaveStringToFile %s"), *FinalFileName);
	return FFileHelper::SaveStringToFile(Content, *FinalFileName);

}
