#pragma once
#include "Resource.h"

class Texture final : public Resource<Texture>
{
public:
	Texture(class Context* context);
	~Texture() = default;

	void LoadFromFile(std::string& filename) override;

	inline const ID3D11ShaderResourceView* const GetTexture() { return m_shaderResourceView.Get(); }

private:
	std::vector<uint8_t> LoadRGBAImage(std::wstring filename);

private:
	unsigned int m_width;
	unsigned int m_height;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;

	class Graphics* m_graphics;
};