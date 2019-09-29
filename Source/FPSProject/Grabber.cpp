// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetPhysicsHandleComponent();
	SetInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->SetTargetLocation(GetLineEndPosition());
	}
}

// 获取射线起点位置
FVector UGrabber::GetLineStartPosition()
{
	//射线起点位置和方向
	FVector PlayerViewPointLocation;//角色位置
	FRotator playViewPointRotation;//面朝方向
	//获取角色的位置和旋转方向
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, playViewPointRotation);
	return PlayerViewPointLocation;
}

// 获取射线终点位置
FVector UGrabber::GetLineEndPosition()
{
	//射线起点位置和方向
	FVector PlayerViewPointLocation;//角色位置
	FRotator playViewPointRotation;//面朝方向
	//获取角色的位置和旋转方向
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, playViewPointRotation);
	//射线终点位置（相对于世界坐标系）
	FVector LineTraceEndLocation = PlayerViewPointLocation + playViewPointRotation.Vector() * Reach;
	return LineTraceEndLocation;
}

// 获取射线碰撞的第一个物体信息
FHitResult UGrabber::GetFirstPhysicsBobyInReach()
{
	//绘制射线
	//DrawDebugLine(
	//	GetWorld(),
	//	GetLineStartPosition(),
	//	GetLineEndPosition(),
	//	FColor(255, 0, 0),
	//	false, 0.0f, 0.0f, 10.0f
	//);

	//碰撞物体
	FHitResult HitResult;
	FCollisionQueryParams QueryParams = FCollisionQueryParams("", false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetLineStartPosition(),
		GetLineEndPosition(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);

	AActor *ActorHit = HitResult.GetActor();
	if (ActorHit != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Name: %s"), *ActorHit->GetName());
	}
	return HitResult;
}

// 获取物理把手组件
void UGrabber::SetPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle HOT FOUND!"));
		return;
	}
}

// 获取输入组件
void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent HOT FOUND!"));
		return;
	}
	else
	{
		// 按键绑定
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grad);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}

}

void UGrabber::Grad()
{
	/* 测试射线是否击中 */
	//AActor *ActorHit = GetFirstPhysicsBobyInReach().GetActor();
	//if (ActorHit == nullptr)
	//{
	//	UE_LOG(LogTemp, Error, TEXT("ActorHit is NULL!"));
	//	return;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("GAME NAME: %s"), *ActorHit->GetName());

	FHitResult HitResut = GetFirstPhysicsBobyInReach();
	UPrimitiveComponent *ComponentToGrab = HitResut.GetComponent();
	if (HitResut.GetActor() != nullptr)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			HitResut.GetActor()->GetActorLocation(),
			HitResut.GetActor()->GetActorRotation()
		);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

