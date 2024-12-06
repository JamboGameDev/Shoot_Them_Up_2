#pragma once

class STUUtils
{
public:
	// Шаблонная функция, которая возвращает указатель на конкретный тип компонента pawn, если такой имеется.
	template <typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component); // возвращаем cast к типу компонента, который мы запрашиваем.
	}
};
