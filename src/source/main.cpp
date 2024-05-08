#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

int main()
{
    const std::string input_file = "images/negr.bmp";
    
    // Загрузка изображения BMP
    BMP bmp(input_file);

    if (!bmp.isValid())
    {
        Logger::exit(1, invalid_bmp_message);
    }

    bmp.getInfo();

    const std::string pattern = "semicircle";

    bmp.ornament(pattern, RGB(0, 255, 0), 20, 7);

    bmp.save("output.bmp");

    return EXIT_SUCCESS;
}