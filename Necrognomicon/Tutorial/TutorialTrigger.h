// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TutorialTrigger.generated.h"

UCLASS()
class NECROGNOMICON_API ATutorialTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATutorialTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* DefaultRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* Mesh;

	/// En caso de tener video, el indice del video asociado a la playlist
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TutorialParams")
		int VideoIndex;

	/// Mensaje asociado al tutorial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TutorialParams")
		FText Message;

	/// Para activar el o no el video en el tutorial
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TutorialParams")
		bool IsVideoEnabled;
	
	///En caso de tener un mesh, activa la rotacion del mismo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		bool IsRotationEnabled = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float Roll_X;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float Pitch_Y;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
		float Yaw_Z;

	/// Si este tutorial se repite en varios puntos del mismo nivel hay que marcarlo como true para
	/// que no vuelva a saltar si ya hemos activado uno de los que estan en el mapa
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		bool IsDuplicated = false;

	/// Asocia los triggers a un nivel. IMPORTANTE; en caso de estar duplicados este valor
	/// se usa para elimianr los triggers repetidos.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
		int LevelIndex;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FRotator _rotator;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UFUNCTION()
		virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	class GameInstance* _gameInstance;
};
