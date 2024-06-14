#include "Image.h"
#include "../FloaterUtil/include/FloaterMacro.h"
#include "../FloaterUtil/include/FloaterType.h"
#include <filesystem>
#include <fstream>

flt::Image::Image() : Image(L"")
{

}

flt::Image::Image(const std::wstring& path) : 
	_path(path),
	_name(),
	_width(0),
	_height(0)
{
	if (path != L"")
	{
		Set(path);
	}
}

void flt::Image::Set(const std::wstring& path)
{
	ASSERT(std::filesystem::exists(path), "file not exist");
	_path = path;
	_name = std::filesystem::path(path).filename().wstring();

	Load();
}

void flt::Image::GetSize(int& width, int& height) const
{
	width = _width;
	height = _height;
}

void flt::Image::Load()
{
	std::wstring extention = std::filesystem::path(_path).extension().wstring();

	if (extention == L".bmp")
	{
		LoadBmp();
	}
	else if (extention == L".png")
	{
		LoadPng();
	}
	else if (extention == L".jpg")
	{
		LoadJpg();
	}
	else
	{
		ASSERT(false, "Unsupported image format");
	}
}

void flt::Image::LoadBmp()
{
#pragma pack(push, 1)
	struct BMPFileHeader {
		uint16_t fileType;
		uint32_t fileSize;
		uint16_t reserved1;
		uint16_t reserved2;
		uint32_t offsetData;
	};

	struct BMPInfoHeader {
		uint32_t size;
		int32_t width;
		int32_t height;
		uint16_t planes;
		uint16_t bitCount;
		uint32_t compression;
		uint32_t sizeImage;
		int32_t xPixelsPerMeter;
		int32_t yPixelsPerMeter;
		uint32_t colorsUsed;
		uint32_t colorsImportant;
	};
#pragma pack(pop)

	std::ifstream imgFile(_path, std::ios::binary);
	ASSERT(imgFile.is_open(), "Failed to open file");

	BMPFileHeader fileHeader;
	imgFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BMPFileHeader));

	BMPInfoHeader infoHeader;
	imgFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(BMPInfoHeader));

	_width = infoHeader.width;
	_height = infoHeader.height;
}

void flt::Image::LoadPng()
{
	std::ifstream imgFile(_path, std::ios::binary);
	ASSERT(imgFile.is_open(), "Failed to open file");

	uint8 header[8];
	imgFile.read(reinterpret_cast<char*>(header), sizeof(header));
	constexpr uint8 pngHeader[8] = { 137, 80, 78, 71, 13, 10, 26, 10 };
	ASSERT(memcmp(header, pngHeader, sizeof(pngHeader)) == 0, "Invalid png file");

	struct PNGChunk {
		uint32 length;
		char type[4];
	};

	while (imgFile)
	{
		PNGChunk chunk;
		imgFile.read(reinterpret_cast<char*>(&chunk), sizeof(PNGChunk));
		if (memcmp(chunk.type, "IHDR", 4) == 0)
		{
			uint32 width;
			uint32 height;
			imgFile.read(reinterpret_cast<char*>(&width), sizeof(uint32));
			imgFile.read(reinterpret_cast<char*>(&height), sizeof(uint32));
			width = (width >> 24) | ((width & 0xff0000) >> 8) | ((width & 0xff00) << 8) | (width << 24);
			height = (height >> 24) | ((height & 0xff0000) >> 8) | ((height & 0xff00) << 8) | (height << 24);

			_width = width;
			_height = height;
			break;
		}
		else
		{
			imgFile.seekg(chunk.length, std::ios::cur);
		}
	}

}

void flt::Image::LoadJpg()
{
	std::ifstream imgFile(_path, std::ios::binary);
	ASSERT(imgFile.is_open(), "Failed to open file");

	uint8 header[2];
	imgFile.read(reinterpret_cast<char*>(header), sizeof(header));
	ASSERT(header[0] == 0xFF && header[1] == 0xD8, "Invalid jpg file");

	while (imgFile)
	{
		uint8 marker;
		imgFile.read(reinterpret_cast<char*>(&marker), sizeof(uint8));
		if (marker != 0xFF)
		{
			continue;
		}

		imgFile.read(reinterpret_cast<char*>(&marker), sizeof(uint8));
		if (marker == 0xC0 || marker == 0xC2)
		{
			imgFile.seekg(3, std::ios::cur);
			uint16 height;
			uint16 width;
			imgFile.read(reinterpret_cast<char*>(&height), sizeof(uint16));
			imgFile.read(reinterpret_cast<char*>(&width), sizeof(uint16));
			_height = (height >> 8) | ((height & 0xff) << 8);
			_width = (width >> 8) | ((width & 0xff) << 8);

			//_height = _byteswap_ushort(height);
			//_width = _byteswap_ushort(width);
			break;
		}
		else
		{
			uint16 length;
			imgFile.read(reinterpret_cast<char*>(&length), sizeof(uint16));
			length = (length >> 8) | ((length & 0xff) << 8);
			//length = _byteswap_ushort(length);
			imgFile.seekg(length - 2, std::ios::cur);
		}
	}
}
