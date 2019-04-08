#pragma once
#include "Component.h"

class Light final : public Component<Light>
{
public:
	enum LightType : unsigned int
	{
		Directional,
		Point,
		Spot,
	};

public:
	Light(class Context* context, class GameObject* owner);
	~Light() = default;

	void Update() override;

	void SetLightType(const LightType type) { m_eLightType = type; }
	const LightType GetLightType() const { return m_eLightType; }

	void SetColor(const Color& color) { m_vLightColor = color; }
	const Color& GetColor() const { return m_vLightColor; }

	void SetRange(const float range) { m_fLightRange = range; }
	const float GetRange() const { return m_fLightRange; }

	class Transform* GetTransform() const { return m_pTransform; }

private:
	class Transform* m_pTransform;

	LightType m_eLightType;
	Color m_vLightColor;
	float m_fLightRange;
};