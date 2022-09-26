#pragma once

#include "STUPlayerState.h"

class STUUtils
{

	public:
		template<typename T>
		static T* GetSTUPlayerComponent(AActor* PlyaerPawn)
		{
			if (!PlyaerPawn) return nullptr;

			const auto Component = PlyaerPawn->GetComponentByClass(T::StaticClass());
			return Cast<T>(Component);
		}

		bool static AreEnemies(AController* Controller1, AController* Controller2) 
		{ 
			if (!Controller1 || !Controller2 || Controller2 == Controller1) return false;

			const auto PlaerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
			const auto PlaerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

			return PlaerState1 && PlaerState2 && PlaerState2->GetTeamID() != PlaerState1->GetTeamID();
		}
};