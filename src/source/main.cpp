#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"

int main()
{
    const std::string input_file = "images/negr.bmp";

    BMP bmp(input_file);

    if (!bmp.isValid())
    {
        Logger::exit(1, invalid_bmp_message);
    }

    bmp.getInfo();

    bmp.ornament("semicircle", RGB(0,255,0), 5, 30);

    bmp.save("output.bmp");

    return EXIT_SUCCESS;
}