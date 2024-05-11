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

void BMP::getInfo() const
{
    Logger::log(signature_message + std::string(header.signature, 2));
    Logger::log(file_size_message + std::to_string(header.fileSize) + " bytes");
    Logger::log(data_offset_message + std::to_string(header.dataOffset) +
                " bytes");
    Logger::log(header_size_message + std::to_string(header.headerSize) +
                " bytes");
    Logger::log(image_dimensions_message + std::to_string(header.width) + "x" +
                std::to_string(header.height));
    Logger::log(bits_per_pixel_message + std::to_string(header.bitsPerPixel));
    Logger::log(compression_message + std::to_string(header.compression));
    Logger::log(image_size_message + std::to_string(header.imageSize) + " bytes");
    Logger::log(pixels_per_meter_x_message +
                std::to_string(header.xPixelsPerMeter));
    Logger::log(pixels_per_meter_y_message +
                std::to_string(header.yPixelsPerMeter));
    Logger::log(colors_used_message + std::to_string(header.colorsUsed));
    Logger::log(important_colors_message +
                std::to_string(header.colorsImportant));
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

void BMP::save(const std::string &fileName)
{
    std::ofstream file(fileName, std::ios::binary);
    if (!file.is_open())
    {
        Logger::exit(ERR_FILE_WRITE_ERROR, failed_create_output_file + fileName);
        return;
    }

    int rowSize = ((header.width * header.bitsPerPixel + 31) / 32) * 4;

    int imageSize = rowSize * header.height;

    header.fileSize = header.dataOffset + imageSize;
    header.imageSize = imageSize;

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

void BMP::copy(const Coordinate &src_left_up, const Coordinate &src_right_down,
               const Coordinate &dest_left_up)
{
    if (src_left_up.x < 0 || src_left_up.y < 0 || dest_left_up.x <= 0 ||
        dest_left_up.x < 0)
    {
        Logger::exit(ERR_INVALID_ARGUMENT, invalid_copy_region);
        return;
    }
    int src_width = src_right_down.x - src_left_up.x;
    int src_height = src_right_down.y - src_left_up.y;
    int dest_width = header.width - dest_left_up.x;
    int dest_height = header.height - dest_left_up.y;

    if (src_width <= 0 || src_height <= 0 || dest_width <= 0 ||
        dest_height <= 0)
    {
        Logger::exit(ERR_INVALID_ARGUMENT, invalid_copy_region);
        return;
    }

    if (src_width > dest_width || src_height > dest_height)
    {
        Logger::warn(copy_exceeds_bounds_error);
    }
    else
    {
        int offsetX = 0;
        for (int x = src_left_up.x; x < src_right_down.x; x++)
        {
            int offsetY = 0;
            for (int y = src_left_up.y; y < src_right_down.y; y++)
            {
                setColor(dest_left_up.x + offsetX, dest_left_up.y + offsetY,
                         getColor(x, y));
                offsetY++;
            }
            offsetX++;
        }
    }
}

void BMP::drawRectangle(const Coordinate left_top, const Coordinate right_bottom, const RGB color)
{
    for (int x = left_top.x; x <= right_bottom.x; x++)
    {
        setColor(x, left_top.y, color);
        setColor(x, right_bottom.y, color);
    }

    for (int y = left_top.y; y <= right_bottom.y; y++)
    {
        setColor(left_top.x, y, color);
        setColor(right_bottom.x, y, color);
    }
}

void BMP::drawCircle(const Coordinate center, const int radius, const int thickness, const RGB color)
{
    for (int x = center.x - radius - thickness; x <= center.x + radius + thickness; x++)
    {
        for (int y = center.y - radius - thickness; y <= center.y + radius + thickness; y++)
        {
            if (pow(x - center.x, 2) + pow((y - center.y), 2) >= pow(radius, 2) &&
                pow(x - center.x, 2) + pow((y - center.y), 2) < pow(radius + thickness, 2))
            {
                setColor(x, y, color);
            }
        }
    }
}

void BMP::ornament(const std::string pattern, const RGB color, const int thickness = 0, const int count = 0)
{
    if (pattern == "circle")
    {
        struct Coordinate center = {header.width / 2, header.height / 2};
        int radius = std::min(header.height, header.width) / 2;

        for (int x = 0; x <= header.width; x++)
            for (int y = 0; y <= header.height; y++)
                if (pow((center.y - y), 2) + pow((center.x - x), 2) > pow(radius, 2))
                    setColor(x, y, color);

        return;
    }

    if (thickness <= 0 || count <= 0)
        Logger::exit(ERR_INVALID_ARGUMENT, invalid_ornament_parameters);

    if (pattern == "rectangle")
    {
        struct Coordinate left_top = {0, 0};
        struct Coordinate right_bottom = {header.width - 1, header.height - 1};

        for (int cnt = 0; cnt < count; cnt++)
        {

            if ((left_top.x + thickness >= right_bottom.x) || (left_top.y + thickness >= right_bottom.y))
            {
                Logger::warn(rectangle_overflow_warning);
                return;
            }

            for (int layer = 0; layer < thickness; layer++)
            {
                drawRectangle(left_top, right_bottom, color);
                left_top.x += 1;
                left_top.y += 1;
                right_bottom.x -= 1;
                right_bottom.y -= 1;
            }

            left_top.x += thickness;
            left_top.y += thickness;
            right_bottom.x -= thickness;
            right_bottom.y -= thickness;
        }
        return;
    }

    if (pattern == "semicircle")
    {
        int horizontal_radius = ceil(float(header.width) / count / 2) - thickness / 2;
        int vertical_radius = ceil(float(header.height) / count / 2) - thickness / 2;

        for (int oXcenter = horizontal_radius + thickness / 2; oXcenter - horizontal_radius < header.width; oXcenter += horizontal_radius * 2 + thickness)
        {
            struct Coordinate center = {oXcenter, 0};
            drawCircle(center, horizontal_radius, thickness, color);
            center.y = header.height;
            drawCircle(center, horizontal_radius, thickness, color);
        }

        for (int oYcenter = vertical_radius + thickness / 2; oYcenter - vertical_radius < header.height; oYcenter += vertical_radius * 2 + thickness)
        {
            struct Coordinate center = {0, oYcenter};
            drawCircle(center, vertical_radius, thickness, color);
            center.x = header.width;
            drawCircle(center, vertical_radius, thickness, color);
        }
        return;
    }

    Logger::exit(ERR_INVALID_ARGUMENT, invalid_ornament_pattern);
}

bool isInHexagonArea(const Coordinate center, int x, int y, int radius)
{
    // Просто конченная математическая формула для проверки на вхождение точки в область шестиугольника.
    return abs(float(x) + float(radius) / 2 - center.x) + abs(float(x) - float(radius) / 2 - center.x) + float(2 * abs(y - center.y)) / sqrt(3) < 2 * radius;
}

void BMP::drawHexagon(const Coordinate center, const int radius, const RGB color)
{
    for (int x = center.x - radius; x <= center.x + radius; x++)
    {
        for (int y = center.y - radius; y <= center.y + radius; y++)
        {
            if (isInHexagonArea(center, x, y, radius) and not isInHexagonArea(center, x, y, radius - 1))
            {
                setColor(x, y, color);
            }
        }
    }
}

void BMP::hexagon(const Coordinate center, const int radius, const int thickness, const RGB color,
                  const bool fill, const RGB fill_color)
{
    if (thickness <= 0 || radius <= 0)
        Logger::exit(ERR_INVALID_ARGUMENT, invalid_hexagon_parameters);

    int current_radius = radius + thickness - 1;

    while (current_radius >= radius)
    {
        drawHexagon(center, current_radius, color);
        current_radius--;
    }

    if (!fill)
        return;

    while (current_radius >= 0)
    {
        drawHexagon(center, current_radius, fill_color);
        current_radius--;
    }
}
