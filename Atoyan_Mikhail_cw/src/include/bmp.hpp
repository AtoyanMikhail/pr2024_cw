#include "operation_params.hpp"
#include <cstring>
#include <fstream>
#include <math.h>

class BMP
{
private:
    RGB getColor(int x, int y) const;

    void setColor(int x, int y, const RGB &newColor);

    void drawHexagon(const Coordinate center, const int radius, const RGB color);

    void drawCircle(const Coordinate center, const int radius, const int thickness, const RGB color);

    void drawRectangle(const Coordinate left, const Coordinate right, const RGB color);

    void drawThickLine(int x0, int y0, int x1, int y1, int thickness, const RGB color);

    RGB getAverageColor(int x, int y, int compression_depth);
public:
    BMP(const std::string &fileName);

    bool isValid() const;

    void save(const std::string &fileName);

    void getInfo() const;

    void initialize(int32_t width, int32_t height);

    void hexagon(const Coordinate center, const int radius, const int thickness, const RGB color,
                 const bool fill = false, const RGB fill_color = {0, 0, 0});

    void copy(const Coordinate &src_left_up, const Coordinate &src_right_down,
              const Coordinate &dest_left_up);

    void colorReplace(const RGB &color_replace_old_color, const RGB &color_replace_new_color);

    void ornament(const std::string pattern, const RGB colour, const int thikness, const int count);

    BMP compress(BMP bmp, int compression_depth);

private:
    BMPHeader header;            ///< Заголовок BMP файла.
    bool validateHeader() const; ///< Проверка корректности заголовка.

    std::vector<char> pixelData; ///< Пиксельные данные изображения.
};