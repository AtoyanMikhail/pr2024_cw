#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

int main()
{
    const std::string input_file = "images/24.bmp";

    // Загрузка изображения BMP
    BMP bmp(input_file);

    bmp.getInfo();

    if (!bmp.isValid())
    {
        Logger::exit(1, invalid_bmp_message);
    }

    bmp.save("output.bmp");

    return EXIT_SUCCESS;
}