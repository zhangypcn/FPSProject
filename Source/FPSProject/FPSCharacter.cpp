// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "FPSProject.h"

#include "FPSProjectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ����һ����һ�˳���������
	FPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	// �������������ӵ����������
	FPSCameraComponent->SetupAttachment(GetCapsuleComponent());
	// ��������������۾��Ϸ���Զ��
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f + BaseEyeHeight));
	// �� pawn �����������ת
	FPSCameraComponent->bUsePawnControlRotation = true;

	// Ϊӵ����Ҵ���һ����һ�˳�ģ�������
	FPSMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	// ��ģ�ͽ���ӵ����ҿɼ���
	// SetOnlyOwnerSee ������ģ�ͽ���ӵ�д� Character �� PlayerController �ɼ��� 
	FPSMesh->SetOnlyOwnerSee(true);
	// �� FPS ģ����ӵ� FPS �������
	FPSMesh->SetupAttachment(FPSCameraComponent);
	// ���ò��ֻ�����Ӱ��������һģ�ʹ��ڵļ���
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;

	// ӵ������޷�������ͨ�������˳ƣ�����ģ�͡�
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
		// ��ʾ������Ϣ���롣-1������ֵ���׸�������˵������������»�ˢ�´���Ϣ��
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using FPSCharacter."));
	}
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Called to bind functionality to input ���ú󽫹��ܰ󶨵�����
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���á��ƶ�����
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	// ���á��鿴����
	// Character �����Ѿ�������������Ҫ������AddControllerYawInput��AddControllerPitchInput
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	// ���á���������
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);

	// ���á����𡱰�
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::FireTimer);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::FireStop);

	// ���á����١���
	PlayerInputComponent->BindAction("SpeedUp", IE_Pressed, this, &AFPSCharacter::SpeedUp);
	PlayerInputComponent->BindAction("SpeedUp", IE_Released, this, &AFPSCharacter::ReSpeed);
}

void AFPSCharacter::MoveForward(float Value)
{
	// ��ȷ�ĸ������ǡ�ǰ����������¼�����ͼ��˷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AFPSCharacter::MoveRight(float Value)
{
	// ��ȷ�ĸ������ǡ����ҡ�������¼�����ͼ��˷����ƶ���
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

// �� Character �����������˽�ɫ��Ծ��֧�֡�
// ��ɫ��Ծ�� bPressedJump �����󶨡������Ҫִ�еĲ������ڰ�����Ծ����ʱ��bPressedJump��Ϊ true���ɿ���Ծ����ʱ��Ϊ false��
void AFPSCharacter::StartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::StopJump()
{
	bPressedJump = false;
}

// �����ƶ�����ͼ����ʵ�滻 (Thunk)
//void AFPSCharacter::SpeedUp()
//{
//}
//void AFPSCharacter::ReSpeed()
//{
//}

void AFPSCharacter::Fire()
{
	// ���Է�������
	if (ProjectileClass)
	{
		// ��ȡ������任
		FVector CameraLocation;//���λ��
		FRotator CameraRotation;//����Ƕ�
		GetActorEyesViewPoint(CameraLocation, CameraRotation);//��ȡ�ӵ�����
		// �� MuzzleOffset ��������ռ�任������ռ�
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// ��׼����΢��̧
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			// ��ǹ�ڴ����ɷ�����
			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		
			if (Projectile)
			{
				// ���÷�����ĳ�ʼ���
				FVector LaunchDirection;
				LaunchDirection = MuzzleRotation.Vector();//�ӵ����䷽��
				//if (MuzzleRotation.Vector().X > 0 && MuzzleRotation.Vector().X < 1)	//������ǰ��
				//	LaunchDirection += FVector(0, -0.25, 0);
				//else																	//ת����
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
