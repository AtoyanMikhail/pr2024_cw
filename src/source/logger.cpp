#include "logger.hpp"

bool Logger::colors_enabled = false;

void set_color(const Color color, std::ostream &stream = std::cout)
{
    switch (color)
    {
    case Color::RED:
        stream << "\033[31m";
        break;
    case Color::GREEN:
        stream << "\033[32m";
        break;
    case Color::YELLOW:
        stream << "\033[33m";
        break;
    case Color::BLUE:
        stream << "\033[34m";
        break;
    case Color::MAGENTA:
        stream << "\033[35m";
        break;
    case Color::CYAN:
        stream << "\033[36m";
        break;
    case Color::WHITE:
        stream << "\033[37m";
        break;
    }
}

void reset_color(std::ostream &stream = std::cout)
{
    stream << "\033[0m"; // Reset color
}

Logger::Logger(bool enableColors) { set_colors_enabled(enableColors); }

void Logger::set_colors_enabled(bool enableColors) { colors_enabled = enableColors; }
template <typename Message>
void Logger::log(const Message &message, Color color, std::ostream &stream)
{
    if (colors_enabled)
    {
        set_color(color, stream);
    }

    stream << message << std::endl;

    if (colors_enabled)
    {
        reset_color(stream);
    }
}

void Logger::warn(const std::string &message, std::ostream &stream) { log(message, Color::YELLOW, stream); }

void Logger::error(const std::string &message, std::ostream &stream) { log(message, Color::RED, stream); }

void Logger::exit(int exitCode, const std::string &exitMessage, std::ostream &stream)
{
    if (!exitMessage.empty())
    {
        error(exitMessage, stream);
    }
    std::exit(exitCode);
}