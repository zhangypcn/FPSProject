// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProject.h"
#include "FPSProjectGameModeBase.h"

void AFPSProjectGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		// ��ʾ������Ϣ���롣
		// -1������ֵ���׸�������˵������������»�ˢ�´���Ϣ
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));
	}

}