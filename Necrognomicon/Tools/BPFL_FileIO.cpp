// Fill out your copyright notice in the Description page of Project Settings.


#include "BPFL_FileIO.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

bool UBPFL_FileIO::FileExistsInProjectDir(FString FileName)
{
	FString directory = FPaths::ProjectDir();

	FString FileNameExists = directory + FileName;
	if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FileNameExists)) 
	{
		return true;
	}

	return false;
}
