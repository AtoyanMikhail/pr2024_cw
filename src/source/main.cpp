#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

int main()
{
    const std::string input_file = "24.bmp";

    // Загрузка изображения BMP
    BMP bmp(input_file);
    if (!bmp.isValid())
    {
        Logger::exit(1, invalid_bmp_message);
    }

    for (int i = 0; i < bmp.header.width; ++i)
    {
        bmp.setColor(i, 0, RGB(0, 255, 0));
    }

    bmp.save("output.bmp");

    return EXIT_SUCCESS;
}