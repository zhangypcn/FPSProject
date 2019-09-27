// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProject.h"
#include "FPSProjectGameModeBase.h"

void AFPSProjectGameModeBase::StartPlay()
{
	Super::StartPlay();

	if (GEngine)
	{
		// 显示调试信息五秒。
		// -1“键”值（首个参数）说明我们无需更新或刷新此消息
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Hello World, this is FPSGameMode!"));
	}

}