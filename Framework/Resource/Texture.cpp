#include "Framework.h"
#include "Texture.h"


template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

Texture::Texture(Context * context) :
	Resource<Texture>(context)
{
	m_graphics = context->GetSubsystem<Graphics>();
}

void Texture::LoadFromFile(std::string & filename)
{
	//Set name as the filename
	size_t firstindex = m_name.find_last_of('/');
	if (firstindex >= m_name.length()) firstindex = 0;
	size_t lastindex = m_name.find_last_of('.');
	m_name = m_name.substr(firstindex, lastindex);

	//Load raw image data from file
	auto image = LoadRGBAImage(std::wstring(filename.begin(), filename.end()));

	//Settings
	ID3D11Device* device = m_graphics->GetDevice();
	D3D11_TEXTURE2D_DESC txtDesc = {};
	D3D11_SUBRESOURCE_DATA initialData = {};
	txtDesc.MipLevels = txtDesc.ArraySize = 1;
	txtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	txtDesc.SampleDesc.Count = 1;
	txtDesc.Usage = D3D11_USAGE_IMMUTABLE;
	txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	txtDesc.Width = m_width;
	txtDesc.Height = m_height;
	initialData.pSysMem = image.data();
	initialData.SysMemPitch = txtDesc.Width * sizeof(uint32_t);

	//Create shadow resource view from raw image data
	ComPtr<ID3D11Texture2D> tex;
	device->CreateTexture2D(&txtDesc, &initialData, tex.GetAddressOf());
	device->CreateShaderResourceView(tex.Get(), nullptr, m_shaderResourceView.ReleaseAndGetAddressOf());
}

std::vector<uint8_t> Texture::LoadRGBAImage(std::wstring filename)
{
	ComPtr<IWICImagingFactory> wicFactory;
	CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));

	ComPtr<IWICBitmapDecoder> decoder;
	wicFactory->CreateDecoderFromFilename(filename.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnDemand, decoder.GetAddressOf());
	
	ComPtr<IWICBitmapFrameDecode> frame;
	decoder->GetFrame(0, frame.GetAddressOf());

	frame->GetSize(&m_width, &m_height);

	WICPixelFormatGUID pixelFormat;
	frame->GetPixelFormat(&pixelFormat);

	unsigned int rowPitch = m_width * sizeof(uint32_t);
	unsigned int imageSize = rowPitch * m_height;

	std::vector<uint8_t> image;
	image.resize(size_t(imageSize));

	if (memcmp(&pixelFormat, &GUID_WICPixelFormat32bppRGBA, sizeof(GUID)) == 0)
	{
		frame->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data()));
	}
	else
	{
		ComPtr<IWICFormatConverter> formatConverter;
		wicFactory->CreateFormatConverter(formatConverter.GetAddressOf());

		BOOL canConvert = FALSE;
		formatConverter->CanConvert(pixelFormat, GUID_WICPixelFormat32bppRGBA, &canConvert);
		if (!canConvert)
		{
			throw std::exception("CanConvert");
		}

		formatConverter->Initialize(frame.Get(), GUID_WICPixelFormat32bppRGBA,
			WICBitmapDitherTypeErrorDiffusion, nullptr, 0, WICBitmapPaletteTypeMedianCut);

		formatConverter->CopyPixels(nullptr, rowPitch, imageSize, reinterpret_cast<BYTE*>(image.data()));
	}

	return image;
}