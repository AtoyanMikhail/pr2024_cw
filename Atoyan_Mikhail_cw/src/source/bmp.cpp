#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

#include <thread>

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

void BMP::initialize(int32_t width, int32_t height)
{
    // Clear header
    std::memset(&header, 0, sizeof(header));
    std::strcpy(header.signature, "BM");

    // Calculate bytes per pixel and row size
    const uint32_t bytesPerPixel = 3; // 24 bits per pixel
    const uint32_t rowSize = ((width * bytesPerPixel + 3) / 4) * 4; // Align row size to 4 bytes
    const uint32_t imageSize = rowSize * height;

    // Fill BMP header
    header.fileSize = sizeof(BMPHeader) + imageSize;
    header.dataOffset = sizeof(BMPHeader);
    header.headerSize = 40; // BITMAPINFOHEADER size
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bitsPerPixel = 24; // 24 bits per pixel
    header.compression = 0; // No compression
    header.imageSize = imageSize;
    header.xPixelsPerMeter = 2835; // 72 DPI
    header.yPixelsPerMeter = 2835; // 72 DPI
    header.colorsUsed = 0; // Use all colors
    header.colorsImportant = 0; // All colors are important

    // Allocate pixel data and initialize to black
    pixelData.assign(imageSize, 0);
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

void BMP::copy(const Coordinate& src_left_up, const Coordinate& src_right_down, const Coordinate& dest_left_up)
{
    int src_x_min = std::min(src_left_up.x, src_right_down.x);
    int src_x_max = std::max(src_left_up.x, src_right_down.x);
    int src_y_min = std::min(src_left_up.y, src_right_down.y);
    int src_y_max = std::max(src_left_up.y, src_right_down.y);

    for (int x = src_x_min; x <= src_x_max; x++)
    {
        for (int y = src_y_min; y <= src_y_max; y++) { setColor(dest_left_up.x + (x - src_x_min), dest_left_up.y + (y - src_y_min), getColor(x, y)); }
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

    if (pattern == "semicircles")
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
    return abs(float(x) + float(radius) / 2 - center.x) + abs(float(x) - float(radius) / 2 - center.x) + float(2 * abs(y - center.y)) / sqrt(3) < 2 * radius + 1;
}

void BMP::drawHexagon(const Coordinate center, const int radius, const RGB color)
{   
    for (int x = center.x - radius; x <= center.x; x++)
    {
        for (int y = center.y - radius; y <= center.y; y++)
        {
            if (isInHexagonArea(center, x, y, radius))
            {
                setColor(x, y, color);
                setColor(x, 2*center.y - y, color);
                setColor(2*center.x - x, y, color);
                setColor(2*center.x - x, 2*center.y - y, color);
            }
        }
    }
}

void BMP::drawThickLine(int x0, int y0, int x1, int y1, int thickness, const RGB color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    int radius = thickness / 2;
    int radiusSquared = radius * radius;
    int x, y;

    int iSquared, jSquared;
    for (x = -radius; x <= radius; ++x)
    {
        iSquared = x * x;
        for (y = -radius; y <= radius; ++y)
        {
            jSquared = y * y;
            if (iSquared + jSquared <= radiusSquared) { setColor(x0 + x, y0 + y, color); }
        }
    }

    while (x0 != x1 || y0 != y1)
    {
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }

        if (x0 + radius < 0 || y0 + radius < 0 || x0 - radius > header.width || y0 - radius > header.height) { continue; }

        for (x = -radius; x <= radius; ++x)
        {
            iSquared = x * x;
            for (y = -radius; y <= radius; ++y)
            {
                jSquared = y * y;
                if (iSquared + jSquared <= radiusSquared) { setColor(x0 + x, y0 + y, color); }
            }
        }
    }
}


void BMP::hexagon(const Coordinate center, const int radius, const int thickness, const RGB color,
                  const bool fill, const RGB fill_color)
{
    if (thickness <= 0 || radius <= 0)
        Logger::exit(ERR_INVALID_ARGUMENT, invalid_hexagon_parameters);

    if (fill){
        drawHexagon(center, radius, fill_color);
    }

    drawThickLine(center.x - radius, center.y, center.x - radius/2, center.y - radius*sqrt(3)/2, thickness, color);
    drawThickLine(center.x - radius/2, center.y - radius*sqrt(3)/2, center.x + radius/2, center.y - radius*sqrt(3)/2,  thickness, color);
    drawThickLine(center.x + radius/2, center.y - radius*sqrt(3)/2, center.x + radius, center.y, thickness, color);
    drawThickLine(center.x + radius, center.y, center.x + radius/2, center.y + radius*sqrt(3)/2 + 1, thickness, color);
    drawThickLine(center.x + radius/2, center.y + radius*sqrt(3)/2 + 1, center.x - radius/2, center.y + radius*sqrt(3)/2 + 1, thickness, color);
    drawThickLine(center.x - radius/2, center.y + radius*sqrt(3)/2 + 1, center.x - radius, center.y, thickness, color);
}

RGB BMP::getAverageColor(int x0, int y0, int compression_depth){
    int count = 0;
    int r = 0;
    int g = 0;
    int b = 0;

    for (int x = x0; x < x0 + compression_depth; x++){
        for (int y = y0; y < y0 + compression_depth; y++){
            if (x < 0 || y < 0 || x > header.width || y > header.height) { continue; }
            RGB current_pixel= getColor(x,y);
            r += current_pixel.red; 
            g += current_pixel.green;
            b += current_pixel.blue;
            count += 1;
        }
    }

    return RGB(r / count, g / count, b / count);
}


BMP BMP::compress(BMP bmp, int compression_depth) {
    bmp.initialize(header.width/compression_depth, header.height/compression_depth);
    
    if (compression_depth <= 1) {
        Logger::exit(ERR_INVALID_ARGUMENT, invaild_compression_depth);
    }
    
    for (int x = 0; x < header.width; x+=compression_depth){
        for (int y = 0; y < header.height; y+=compression_depth) {
            bmp.setColor(x / compression_depth, y / compression_depth, getAverageColor(x, y, compression_depth));
        }
    }
    return bmp;
}
