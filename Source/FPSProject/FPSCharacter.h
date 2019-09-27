// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <Engine.h>
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"


UCLASS()
class FPSPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// ����ǰ���ƶ�������
	UFUNCTION()
		void MoveForward(float Value);

	// ���������ƶ�������
	UFUNCTION()
		void MoveRight(float Value);

	// ���°���ʱ������Ծ���
	UFUNCTION()
		void StartJump();

	// �ɿ�����ʱ�����Ծ���
	UFUNCTION()
		void StopJump();

	// ���°���ʱ�����ƶ�
	UFUNCTION(BlueprintImplementableEvent, Category = MoveSpeed)
		void SpeedUp();

	// �ɿ�����ʱ�ָ�ԭ���ƶ�
	UFUNCTION(BlueprintImplementableEvent, Category = MoveSpeed)
		void ReSpeed();

	// FPS �����
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FPSCameraComponent;

	// ��һ�˳�ģ�ͣ��ֱۣ�������ӵ����ҿɼ�
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSMesh;

	// ��һ�˳�ģ�ͣ�������������ӵ����ҿɼ�
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FPSWeapon;

	// ������ĺ���
	UFUNCTION()
		void Fire();

	FTimerHandle TestHandel;	//��ʱ�����

	// �������䶨ʱ��
	UFUNCTION()
		void FireTimer();

	// ͣ��
	UFUNCTION()
		void FireStop();

	// �ӵ������� ������ͼ�޸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DelayTime")
		float DelayTime;

	// �������λ�õ�ǹ��ƫ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	// ���ɵķ�������
	// EditDefaultsOnly ˵������ζ�ŷ�������ֻ������ͼ�ϱ���ΪĬ�ϣ�����������ͼ��ÿ��ʵ�������á�
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

};