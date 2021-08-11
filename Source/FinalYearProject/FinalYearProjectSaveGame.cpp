// Fill out your copyright notice in the Description page of Project Settings.


#include "FinalYearProjectSaveGame.h"
#include "FinalYearProject.h"

UFinalYearProjectSaveGame::UFinalYearProjectSaveGame()
{
	SaveSlotName = TEXT("SaveGame");
	UserIndex = 0;

	CurrentTime = 8.0f;
}