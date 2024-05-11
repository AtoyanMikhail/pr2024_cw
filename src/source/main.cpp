#include "bmp.hpp"
#include "logger.hpp"
#include "messages.hpp"
#include "operation_params.hpp"

#define IMG_DIR ""

int main(int argc, char *argv[])
{
    Logger::log(hello_message);

    // Парсинг параметров командной строки
    Operations params = parseCommandLine(argc, argv);

    const std::string input_file = IMG_DIR + params.input_file;

    // Загрузка изображения BMP
    BMP bmp(input_file);
    if (!bmp.isValid()) { Logger::exit(1, invalid_bmp_message); }

    // Вывод информации о изображении, если соответствующий флаг установлен
    if (params.info) { bmp.getInfo(); }

    // Рисование шестиугольника на изображении, если соответствующий флаг установлен
    if (params.hexagon)
    {
        Logger::warn(hexagon_warning);
        bmp.hexagon(params.center, params.radius, params.thickness, params.color, params.fill, params.fill_color);
        Logger::log(success_message);
    }

    // Замена цветов на изображении, если соответствующий флаг установлен
    if (params.color_replace)
    {
        Logger::warn(color_replace_warning);
        bmp.colorReplace(params.old_color, params.new_color);
        Logger::log(success_message);
    }

    // Рисование орнамента для изображения, если соответствующий флаг установлен
    if (params.ornament)
    {
        Logger::warn(ornamenet_warning);
        bmp.ornament(params.pattern, params.color, params.thickness, params.count);
        Logger::log(success_message);
    }

    // Копирование области изображения, если соответствующий флаг установлен
    if (params.copy)
    {
        Logger::warn(image_copy_warning);
        bmp.copy(params.left_up, params.right_down, params.dest_left_up);
        Logger::log(success_message);
    }

    // Сохранение изображения
    bmp.save(params.output_file);
    
    return EXIT_SUCCESS;
}