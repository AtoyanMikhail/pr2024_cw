#include "bmp.hpp"

int main()
{
    const std::string input_file = "";

    // Загрузка изображения BMP
    BMP bmp(input_file);
    if (!bmp.isValid())
    {
        // Throw and invalide_file_error
        std::cout << "Пошёл нахуй\n";
        return 1;
    }

    for (int i = 0; i < bmp.header.width; ++i)
    {
        bmp.setColor(i, 0, RGB(0, 255, 0));
    }

    return EXIT_SUCCESS;
}