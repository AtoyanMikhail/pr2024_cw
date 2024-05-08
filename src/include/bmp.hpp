#include "operation_params.hpp"
#include "structures.hpp"
#include <cstring>
#include <fstream>
#include <math.h>

class BMP
{
private:
    RGB getColor(int x, int y) const;

    void setColor(int x, int y, const RGB &newColor);

    void drawLine(Coordinate first, Coordinate second, int thikness);

public:
    void getInfo() const;

    BMP(const std::string &fileName);

    bool isValid() const;

    void save(const std::string &fileName);

    void colorReplace(const RGB &color_replace_old_color, const RGB &color_replace_new_color);

    void copy(const Coordinate &src_left_up, const Coordinate &src_right_down,
              const Coordinate &dest_left_up);

    void ornament(const std::string pattern, const RGB colour, const int thikness, const int count);

private:
    BMPHeader header;            ///< Заголовок BMP файла.
    bool validateHeader() const; ///< Проверка корректности заголовка.

    std::vector<char> pixelData; ///< Пиксельные данные изображения.
};