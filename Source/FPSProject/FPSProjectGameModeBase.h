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
	
	// �˺�������������StartPlay()�������Ա�����Ϸ��ʼʱ����־��Ϣ��ӡ����Ļ�ϡ�
	virtual void StartPlay() override;
};
