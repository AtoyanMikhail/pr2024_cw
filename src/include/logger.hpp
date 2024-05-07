#include <iostream> // Include <iostream> for std::ostream

enum class Color
{
    RED,     /**< Красный цвет */
    GREEN,   /**< Зеленый цвет */
    YELLOW,  /**< Желтый цвет */
    BLUE,    /**< Синий цвет */
    MAGENTA, /**< Пурпурный цвет */
    CYAN,    /**< Голубой цвет */
    WHITE    /**< Белый цвет */
};

class Logger
{
private:
    static bool colors_enabled; /**< Флаг, определяющий, разрешены ли цвета в выводе. */

public:
    Logger(bool enable_colors);

    static void set_colors_enabled(bool enableColors);

    template <typename Message>
    static void log(const Message &message, Color color = Color::GREEN, std::ostream &stream = std::cout);

    static void warn(const std::string &message, std::ostream &stream = std::cout);

    static void error(const std::string &message, std::ostream &stream = std::cerr);

    static void exit(int exitCode, const std::string &exitMessage = "", std::ostream &stream = std::cerr);
};