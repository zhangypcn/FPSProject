// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include"PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FPSPROJECT_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 射线长度
	UPROPERTY(EditAnywhere)
		float Reach = 500.0f;//单位cm

	// 获取射线起点位置
	FVector GetLineStartPosition();
	// 获取射线终点位置
	FVector GetLineEndPosition();
	// 获取射线碰撞的第一个物体信息
	FHitResult GetFirstPhysicsBobyInReach();

	// 输入组件
	UInputComponent *InputComponent = nullptr;
	// 物理把手组件
	UPhysicsHandleComponent *PhysicsHandle = nullptr;

	// 获取物理把手组件
	void SetPhysicsHandleComponent();
	// 获取输入组件
	void SetInputComponent();

	// 拿起物体
	void Grad();
	//放下物体
	void Release();

};
