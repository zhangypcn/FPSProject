// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPSProject.h"

#include "FPSProjectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 创建一个第一人称摄像机组件
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// 将摄像机组件附加到胶囊体组件
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// 将摄像机放置在眼睛上方不远处
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// 用 pawn 控制摄像机旋转
	FPSCameraComponent->bUsePawnControlRotation = true;

	// 为拥有玩家创建一个第一人称模型组件。
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// 该模型仅对拥有玩家可见。
	// SetOnlyOwnerSee 表明此模型仅对拥有此 Character 的 PlayerController 可见。 
	FPSMesh->SetOnlyOwnerSee(true);
	// 将 FPS 模型添加到 FPS 摄像机。
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// 禁用部分环境阴影，保留单一模型存在的假象。
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// 拥有玩家无法看到普通（第三人称）身体模型。
	GetMesh()->SetOwnerNoSee(true);

	FPSWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonWeapon"));
	FPSWeapon->SetOnlyOwnerSee(true);
	FPSWeapon->SetupAttachment(FPSCameraComponent);
	FPSWeapon->SetupAttachment(FPSCameraComponent);
	FPSWeapon->bCastDynamicShadow = false;
	FPSWeapon->CastShadow = false;
	//FPSWeapon->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	//FPSWeapon->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));

	DelayTime = 0.5f;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GEngine)
	{
		// 显示调试信息五秒。-1“键”值（首个参数）说明我们无需更新或刷新此消息。
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input 调用后将功能绑定到输入
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 设置“移动”绑定
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// 设置“查看”绑定
	// Character 基类已经定义了两个必要函数：AddControllerYawInput、AddControllerPitchInput
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// 设置“动作”绑定
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	// 设置“开火”绑定
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::FireTimer);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::FireStop);

	// 设置“加速”绑定
	PlayerInputComponent->BindAction("SpeedUp", IE_Pressed, this, &AFPSCharacter::SpeedUp);
	PlayerInputComponent->BindAction("SpeedUp", IE_Released, this, &AFPSCharacter::ReSpeed);
}

void AFPSCharacter::MoveForward(float Value)
{
	// 明确哪个方向是“前进”，并记录玩家试图向此方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// 明确哪个方向是“向右”，并记录玩家试图向此方向移动。
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

// 在 Character 基类中内置了角色跳跃的支持。
// 角色跳跃与 bPressedJump 变量绑定。因此需要执行的操作是在按下跳跃动作时将bPressedJump设为 true，松开跳跃动作时设为 false。
void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

// 加速移动在蓝图中形实替换 (Thunk)
//void AFPSCharacter::SpeedUp()
//{
//}
//void AFPSCharacter::ReSpeed()
//{
//}

void AFPSCharacter::Fire()
{
	// 尝试发射物体
	if (ProjectileClass)
	{
		// 获取摄像机变换
		FVector CameraLocation;//相机位置
		FRotator CameraRotation;//相机角度
		GetActorEyesViewPoint(CameraLocation, CameraRotation);//获取视点坐标
		// 将 MuzzleOffset 从摄像机空间变换到世界空间
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 将准星稍微上抬
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// 在枪口处生成发射物
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		
			if (Projectile)
			{
				// 设置发射物的初始轨道
				FVector LaunchDirection;
				LaunchDirection = MuzzleRotation.Vector();//子弹发射方向
				//if (MuzzleRotation.Vector().X > 0 && MuzzleRotation.Vector().X < 1)	//面向正前方
				//	LaunchDirection += FVector(0, -0.25, 0);
				//else																	//转到后方
				//	LaunchDirection += FVector(0, 0.25, 0);
				Projectile->FireInDirection(LaunchDirection);
			}
			Projectile = NULL;
		}
	}
}

void AFPSCharacter::FireTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TestHandel, this, &AFPSCharacter::Fire, DelayTime, true);
}

void AFPSCharacter::FireStop()
{
	GetWorld()->GetTimerManager().ClearTimer(TestHandel);
}
