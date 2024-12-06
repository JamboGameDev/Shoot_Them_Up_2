#pragma once

class STUUtils
{
public:
	// ��������� �������, ������� ���������� ��������� �� ���������� ��� ���������� pawn, ���� ����� �������.
	template <typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerPawn)
	{
		if (!PlayerPawn) return nullptr;

		const auto Component = PlayerPawn->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component); // ���������� cast � ���� ����������, ������� �� �����������.
	}
};
