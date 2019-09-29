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

	// 门的位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DoorLocation = FVector(0.0f, 0.0f, 0.0f);
	// 门位置偏移量
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector Offset = FVector(0.0f, 0.0f, 0.0f);
	// 开门区域
	UPROPERTY(EditAnywhere)
		ATriggerVolume *TriggerArea = nullptr;
	// 开门判断Actor
	UPROPERTY(EditAnywhere)
		AActor *ActorThatOpen = nullptr;

	// 开门判断Mass 通过重量判断开门
	UPROPERTY(EditAnywhere)
		float MassToOpen = 20.0f;

	UPROPERTY(BlueprintAssignable)
		FOpenDoorRequst OnOpenDoorRequst;
	UPROPERTY(BlueprintAssignable)
		FCloseDoorRequst OnCloseDoorRequst;

private:

	// 开门
	void OpenDoor();
	// 关门
	void CloseDoor();
	// 获取区域内所有Actor的重量
	float GetTotalMassOfActors();
};
