#include <string>

#pragma pack(push, 1)
struct BMPHeader
{
    char signature[2];        /**< Сигнатура файла BMP. */
    uint32_t fileSize;        /**< Размер файла в байтах. */
    uint16_t reserved1;       /**< Зарезервировано для использования. */
    uint16_t reserved2;       /**< Зарезервировано для использования. */
    uint32_t dataOffset;      /**< Смещение, с которого начинаются данные изображения. */
    uint32_t headerSize;      /**< Размер заголовка в байтах. */
    int32_t width;            /**< Ширина изображения в пикселях. */
    int32_t height;           /**< Высота изображения в пикселях. */
    uint16_t planes;          /**< Количество плоскостей. */
    uint16_t bitsPerPixel;    /**< Глубина цвета пикселя в битах. */
    uint32_t compression;     /**< Тип сжатия. */
    uint32_t imageSize;       /**< Размер данных изображения. */
    int32_t xPixelsPerMeter;  /**< Горизонтальное разрешение в пикселях на метр. */
    int32_t yPixelsPerMeter;  /**< Вертикальное разрешение в пикселях на метр. */
    uint32_t colorsUsed;      /**< Количество используемых цветов изображения. */
    uint32_t colorsImportant; /**< Количество важных цветов изображения. */
};
#pragma pack(pop)

struct RGB
{
    uint8_t red;   /**< Компонента красного цвета. */
    uint8_t green; /**< Компонента зеленого цвета. */
    uint8_t blue;  /**< Компонента синего цвета. */
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : red(r), green(g), blue(b) {}
};

struct Coordinate
{
    int32_t x; /**< Координата x. */
    int32_t y; /**< Координата y. */
};

struct Operations
{
    std::string input_file;              /**< Путь к входному файлу. */
    std::string output_file = "out.bmp"; /**< Путь к выходному файлу (по умолчанию "out.bmp"). */
    bool info = false;                   /**< Флаг вывода информации о изображении. */
    bool hexagon = false;                 /**< Флаг отражения изображения. */
    Coordinate center;
    int radius = 0;
    int thickness = 0;
    RGB color;
    bool fill = false;
    RGB fill_color;
    bool copy = false;                   /**< Флаг копирования выделенной области. */
    Coordinate dest_left_up;            /**< Левая верхняя точка для вставки скопированной области. */
    Coordinate left_up; 
    Coordinate right_down;             
    bool color_replace = false;          /**< Флаг замены цвета. */
    RGB old_color;                       /**< Старый цвет, который будет заменен. */
    RGB new_color;                       /**< Новый цвет, на который будет заменен старый цвет. */
    bool ornament = false;                  /**< Флаг разделения изображения на части. */
    std::string pattern;
    int count = 0;
    bool compress = false;
    int num = 1;                    


    Operations()
    : input_file()
    , output_file("out.bmp")
    , info(false)
    , hexagon(false)               
    , center()
    , radius(0)
    , thickness(0)
    , color()
    , fill(false)
    , fill_color()
    , copy(false)                 
    , dest_left_up()           
    , left_up() 
    , right_down()            
    , color_replace(false)         
    , old_color()                
    , new_color()                   
    , ornament(false)                
    , pattern()
    , count(0) 
    {
    }
};