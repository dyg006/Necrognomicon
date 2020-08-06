// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialOverlapActor.generated.h"

UCLASS()
class NECROGNOMICON_API ATutorialOverlapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialOverlapActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* DefaultRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UWidgetComponent* Tooltip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Widget")
		void CheckPlayerOverlap();
	UFUNCTION(BlueprintCallable, Category = "Widget")
		void StartFocus();
	UFUNCTION(BlueprintCallable, Category = "Widget")
		void EndFocus();

private:
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class ANecrognomiconCharacter* _necroCharacter;
	bool IsPlayerOverlaping = false;

};
