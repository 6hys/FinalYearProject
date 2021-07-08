// Fill out your copyright notice in the Description page of Project Settings.


#include "Plant.h"

// Sets default values
APlant::APlant(FName name, int sockets, FString path)
{
	m_Name = name;
	m_Sockets = sockets;
	m_FilePath = path;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Empty constructor
APlant::APlant()
{
	m_Name = "Empty";
	m_Sockets = 0;
	m_FilePath = "";
}

// Called when the game starts or when spawned
void APlant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FName APlant::GetName()
{
	return m_Name;
}

int APlant::GetSockets()
{
	return m_Sockets;
}

FString APlant::GetSocketName()
{
	if (m_Sockets > 1)
	{
		return c_MultipleSockets;
	}
	else
	{
		return c_SingleSocket;
	}
}

FString APlant::GetFilePath()
{
	return m_FilePath;
}



