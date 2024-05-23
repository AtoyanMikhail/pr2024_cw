#include "operation_params.hpp"
#include "logger.hpp"
#include "messages.hpp"

std::vector<int> parseValues(const std::string& str)
{
    std::vector<int> values;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, '.'))
    {
        try
        {
            values.push_back(std::stoi(token));
        }
        catch (const std::invalid_argument& e)
        {
            Logger::exit(ERR_INVALID_ARGUMENT, invalid_argument_error + token );
        }
    }
    return values;
}

RGB parseRGB(const std::string& str)
{
    std::vector<int> values = parseValues(str);
    if (values.size() != 3) { Logger::exit(ERR_INVALID_ARGUMENT, invalid_color_format_error); }
    for (int value : values)
    {
        if (value < 0 || value > 255) { Logger::exit(ERR_INVALID_ARGUMENT, invalid_color_range_error + std::to_string(value)); }
    }
    return { static_cast<uint8_t>(values[0]), static_cast<uint8_t>(values[1]), static_cast<uint8_t>(values[2]) };
}

Coordinate parseCoordinate(const std::string& str)
{
    Coordinate coord;
    std::vector<int> values = parseValues(str);
    if (values.size() != 2) { Logger::exit(ERR_INVALID_ARGUMENT, invalid_color_format_error); }
    coord.x = values[0];
    coord.y = values[1];
    return coord;
}


void displayHelp()
{
    Logger::log(help_usage_description);
    Logger::log(help_usage_start);
    //=======================================
    Logger::log(copy_option_description);
    Logger::log(left_up_option_description);
    Logger::log(right_down_option_description);
    Logger::log(dest_left_up_option_description);
    //=======================================
    Logger::log(color_replace_option_description);
    Logger::log(old_color_option_description);
    Logger::log(new_color_option_description);
    //=======================================
    Logger::log(hexagon_option_description);
    Logger::log(radius_option_description);
    Logger::log(color_option_description);
    Logger::log(thickness_option_description);
    Logger::log(fill_option_description);
    Logger::log(fill_color_option_description);
    //=======================================
    Logger::log(ornament_option_description);
    //=======================================
    Logger::log(compress_option_description);
    Logger::log(num_option_description);
    Logger::log(output_option_description);
    Logger::log(input_option_description);
    Logger::log(help_option_description);
}


Operations parseCommandLine(int argc, char* argv[])
{
    Operations params;

    const std::map<int, std::function<void(const char*)>> optionHandlers = {
        { 'h',
          [&](const char*)
          {
              if (argc != 2) Logger::exit(ERR_INVALID_ARGUMENT, invalid_argument_error + "--help (-h)");
              displayHelp();
              Logger::exit(EXIT_SUCCESS, "");
          } },
        { 'i', [&](const char* option_argument) { params.input_file = option_argument; } },
        { 'o', [&](const char* option_argument) { params.output_file = option_argument; } },
        { 256, [&](const char*) { params.hexagon = true; } },
        { 257, [&](const char* option_argument) { params.center = parseCoordinate(option_argument); } },
        { 258, [&](const char* option_argument) { params.radius = parseValues(option_argument)[0]; } },
        { 259, [&](const char* option_argument) { params.thickness = parseValues(option_argument)[0]; } },
        { 260, [&](const char* option_argument) { params.color = parseRGB(option_argument); } },
        { 261, [&](const char*) { params.fill = true; } },
        { 262, [&](const char* option_argument) { params.fill_color = parseRGB(option_argument); } },
        { 263, [&](const char*) { params.copy = true; } },
        { 264, [&](const char* option_argument) { params.dest_left_up = parseCoordinate(option_argument); } },        
        { 265, [&](const char* option_argument) { params.left_up = parseCoordinate(option_argument); } },
        { 266, [&](const char* option_argument) { params.right_down = parseCoordinate(option_argument); } },
        { 267, [&](const char*) { params.color_replace = true; } },
        { 268, [&](const char* option_argument) { params.old_color = parseRGB(option_argument); } },
        { 269, [&](const char* option_argument) { params.new_color = parseRGB(option_argument); } },
        { 270, [&](const char*) { params.ornament = true; } },
        { 271, [&](const char* option_argument) { params.pattern = option_argument; } },
        { 272, [&](const char* option_argument) { params.count = parseValues(option_argument)[0]; } },
        { 273, [&](const char*) { params.info = true; } },
        { 274, [&](const char*) { params.compress = true; } },
        { 275, [&](const char* option_argument) { params.num = parseValues(option_argument)[0]; } },
        { 276, [&](const char*) { Logger::set_colors_enabled(true); } },
        { 277, [&](const char*) { params.info = true; } },
        
    };

    const char* short_options = "hi:o:";

    static struct option long_options[] = { { "help", no_argument, nullptr, 'h' }, { "input", required_argument, nullptr, 'i' }, { "output", required_argument, nullptr, 'o' }, { "hexagon", no_argument, nullptr, 256 }, { "center", required_argument, nullptr, 257 }, { "radius", required_argument, nullptr, 258 }, { "thickness", required_argument, nullptr, 259 }, { "color", required_argument, nullptr, 260 }, { "fill", no_argument, nullptr, 261 }, { "fill_color", required_argument, nullptr, 262 }, { "copy", no_argument, nullptr, 263 }, { "dest_left_up", required_argument, nullptr, 264 }, { "left_up", required_argument, nullptr, 265 }, { "right_down", required_argument, nullptr, 266 }, { "color_replace", no_argument, nullptr, 267 }, { "old_color", required_argument, nullptr, 268 }, { "new_color", required_argument, nullptr, 269 }, { "ornament", no_argument, nullptr, 270 }, { "pattern", required_argument, nullptr, 271 }, { "count", required_argument, nullptr, 272 }, { "info", no_argument, nullptr, 273 }, {"compress", no_argument, nullptr, 274}, {"num", required_argument, nullptr, 275}, { nullptr, 0, nullptr, 0 } };

    int opt;

    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1)
    {
        auto handler = optionHandlers.find(opt);
        if (handler != optionHandlers.end()) { handler->second(optarg); }
    }

    if (params.fill && !params.hexagon) { 
        Logger::warn(filling_a_nonexistent_hexagon_err); 
        params.fill = false;
    }

    if (params.input_file.empty())
    {
        if (optind == argc - 1) { params.input_file = argv[optind]; }
        else if (optind < argc - 1) { Logger::exit(ERR_INVALID_ARGUMENT, too_many_args_err); }
        else { Logger::exit(ERR_INVALID_ARGUMENT, invalid_bmp_message); }
    }

    if (params.input_file == params.output_file) { Logger::exit(ERR_INVALID_ARGUMENT, same_input_output_message); }

    return params;
}