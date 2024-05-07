#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

BMP::BMP(const std::string &fileName) : header(), pixelData()
{
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        file.close();
        Logger::exit(ERR_INCORRECT_FILE_FORMAT, invalid_header_error + fileName);
    }

    file.read(reinterpret_cast<char *>(&header), sizeof(header));

    if (!validateHeader())
    {
        file.close();
        Logger::exit(ERR_INCORRECT_FILE_FORMAT, invalid_header_error + fileName);
    }

    const uint32_t bytesPerPixel = header.bitsPerPixel / 8;
    const uint32_t rowSize = ((header.width * bytesPerPixel + 3) / 4) * 4;
    const uint32_t imageSize = rowSize * header.height;

    pixelData.resize(imageSize);

    file.seekg(header.dataOffset, std::ios_base::beg);

    file.read(reinterpret_cast<char *>(pixelData.data()), imageSize);

    file.close();
}

bool BMP::validateHeader() const
{
    if (std::strncmp(header.signature, "BM", 2) != 0)
    {
        Logger::exit(ERR_INCORRECT_FILE_FORMAT, invalid_signature_error);
        return false;

    }

    if (header.width <= 0 || header.height <= 0)
    {
        Logger::exit(ERR_INCORRECT_FILE_FORMAT, invalid_dimensions_error);
        return false;
    }

    if (header.bitsPerPixel != 24)
    {
        Logger::warn(invalid_bpp_warning);
    }

    if (header.compression != 0)
    {
        Logger::exit(ERR_INCORRECT_FILE_FORMAT, unsupported_compression_error);
        return false;
    }

    return true;
}

bool BMP::isValid() const
{
    return !pixelData.empty();
}

void BMP::save(const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        Logger::exit(ERR_FILE_WRITE_ERROR, failed_create_output_file + fileName);
        return;
    }

    const uint32_t bytesPerPixel = header.bitsPerPixel / 8;
    const uint32_t rowSize = ((header.width * bytesPerPixel + 3) / 4) * 4;

    file.write(reinterpret_cast<const char *>(&header), sizeof(header));

    for (int y = 0; y < header.height; ++y)
    {
        file.write(reinterpret_cast<const char *>(pixelData.data() + y * rowSize),
                   rowSize);
    }

    file.close();
}

void BMP::colorReplace(const RGB &color_replace_old_color, const RGB &color_replace_new_color)
{
    for (int y = 0; y < header.height; y++)
    {
        for (int x = 0; x < header.width; x++)
        {
            RGB current_color = getColor(x, y);
            if (current_color.red == color_replace_old_color.red &&
                current_color.green == color_replace_old_color.green &&
                current_color.blue == color_replace_old_color.blue)
            {
                setColor(x, y, color_replace_new_color);
            }
        }
    }
}

RGB BMP::getColor(int x, int y) const
{
    if (x < 0 || x >= header.width || y < 0 || y >= header.height)
        return RGB();

    const uint32_t bytesPerPixel = header.bitsPerPixel / 8;
    const uint32_t bytesPerRow = (bytesPerPixel * header.width + 3) & ~3;
    const uint32_t index =
        ((header.height - 1 - y) * bytesPerRow) + (x * bytesPerPixel);

    return RGB(pixelData[index + 2], pixelData[index + 1], pixelData[index]);
}

void BMP::setColor(int x, int y, const RGB &newColor)
{
    if (x < 0 || x >= header.width || y < 0 || y >= header.height)
        return;

    const uint32_t bytesPerPixel = header.bitsPerPixel / 8;
    const uint32_t bytesPerRow = (bytesPerPixel * header.width + 3) & ~3;
    const uint32_t index =
        ((header.height - 1 - y) * bytesPerRow) + (x * bytesPerPixel);

    pixelData[index] = newColor.blue;
    pixelData[index + 1] = newColor.green;
    pixelData[index + 2] = newColor.red;
}