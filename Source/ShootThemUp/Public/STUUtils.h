#pragma once


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
};