// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOpenDoorRequst);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCloseDoorRequst);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �ŵ�λ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DoorLocation = FVector(0.0f, 0.0f, 0.0f);
	// ��λ��ƫ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Offset = FVector(0.0f, 0.0f, 0.0f);
	// ��������
	UPROPERTY(EditAnywhere)
		ATriggerVolume *TriggerArea = nullptr;
	// �����ж�Actor
	UPROPERTY(EditAnywhere)
		AActor *ActorThatOpen = nullptr;

	// �����ж�Mass ͨ�������жϿ���
	UPROPERTY(EditAnywhere)
		float MassToOpen = 20.0f;

	UPROPERTY(BlueprintAssignable)
		FOpenDoorRequst OnOpenDoorRequst;
	UPROPERTY(BlueprintAssignable)
		FCloseDoorRequst OnCloseDoorRequst;

private:

	// ����
	void OpenDoor();
	// ����
	void CloseDoor();
	// ��ȡ����������Actor������
	float GetTotalMassOfActors();
};
