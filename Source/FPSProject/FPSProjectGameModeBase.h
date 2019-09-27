// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API AFPSProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	// 此函数声明允许覆盖StartPlay()函数，以便在游戏开始时将日志消息打印到屏幕上。
	virtual void StartPlay() override;
};
