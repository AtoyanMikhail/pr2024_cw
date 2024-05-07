#include <string>

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

struct RGB
{
    uint8_t red;   /**< Компонента красного цвета. */
    uint8_t green; /**< Компонента зеленого цвета. */
    uint8_t blue;  /**< Компонента синего цвета. */
    RGB(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : red(r), green(g), blue(b) {}
};

struct Coordinate
{
    int x; /**< Координата x. */
    int y; /**< Координата y. */
};