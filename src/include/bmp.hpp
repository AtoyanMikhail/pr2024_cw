#include "operation_params.hpp"
#include "structures.hpp"
#include <cstring>
#include <fstream>

class BMP
{
private:
    RGB getColor(int x, int y) const;

    void setColor(int x, int y, const RGB &newColor);

public:
    void getInfo() const;

    BMP(const std::string &fileName);

    bool isValid() const;

    void save(const std::string &fileName);

    void colorReplace(const RGB &color_replace_old_color, const RGB &color_replace_new_color);

    void copy(const Coordinate &src_left_up, const Coordinate &src_right_down,
              const Coordinate &dest_left_up);

private:
    BMPHeader header;            ///< Заголовок BMP файла.
    bool validateHeader() const; ///< Проверка корректности заголовка.

    std::vector<char> pixelData; ///< Пиксельные данные изображения.
};