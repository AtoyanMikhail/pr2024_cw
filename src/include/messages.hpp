#include <string>

#define ERR_FILE_NOT_FOUND 40

#define ERR_INCORRECT_FILE_FORMAT 41

#define ERR_FILE_WRITE_ERROR 42

#define ERR_INVALID_ARGUMENT 43

#define ERR_INSUFFICIENT_ARGUMENTS 45

const std::string hello_message = "Course work for option 5.7, created by Atoyan Mikhail.";

const std::string invalid_bmp_message = "Invalid bmp file!";

const std::string same_input_output_message = "Input file is the same as output file!";

const std::string success_message = "Success!";

const std::string invalid_signature_error = "Invalid BMP file signature";

const std::string invalid_dimensions_error = "Invalid BMP dimensions";

const std::string invalid_bpp_warning = "Invalid BMP bits per pixel, output image may be incorrect";

const std::string unsupported_compression_error = "Unsupported BMP compression type";

const std::string invalid_header_error = "BMP file header is invalid: ";

const std::string failed_create_output_file = "Failed to create output BMP file: ";

const std::string invalid_copy_region = "Invalid copy region or destination parameters";

const std::string copy_exceeds_bounds_error = "Copying region exceeds destination image boundaries.";

const std::string signature_message = "Signature: ";

const std::string file_size_message = "File size: ";

const std::string data_offset_message = "Data offset: ";

const std::string header_size_message = "Header size: ";

const std::string image_dimensions_message = "Image dimensions: ";

const std::string bits_per_pixel_message = "Bits per pixel: ";

const std::string compression_message = "Compression: ";

const std::string image_size_message = "Image size: ";

const std::string pixels_per_meter_x_message = "Pixels per meter (X axis): ";

const std::string pixels_per_meter_y_message = "Pixels per meter (Y axis): ";

const std::string colors_used_message = "Colors used: ";

const std::string important_colors_message = "Important colors: ";

const std::string invalid_ornament_pattern = "Invalid ornament pattern";

const std::string rectangle_overflow_warning = "The number of possible rectangles is exceeded";

const std::string invalid_ornament_parameters = "Ornament parametrs are invaild";

const std::string invalid_hexagon_parameters = "Hexagon parametrs are invaild";

const std::string invalid_argument_error = "Invalid argument for ";

const std::string invalid_color_format_error = "Invalid color format";

const std::string invalid_color_range_error = "Color out of range [0-255] got: ";

const std::string filling_a_nonexistent_hexagon_err = "Tried to fill a non-existent hexagon. Operation aborted";

const std::string too_many_args_err = "Too many arguments";

const std::string hexagon_warning = "=Hexagon operation is requested=";

const std::string color_replace_warning = "=Color replace operation is requested=";

const std::string ornamenet_warning = "=Ornament operation is requested=";

const std::string image_copy_warning = "=Image copy operation is requested=";

const std::string help_usage_description = "Usage: program_name [options] filename";

const std::string help_usage_start = "Options: ";

const std::string hexagon_option_description = "  --hexagon              Hexagon operation";

const std::string left_up_option_description = "  --left_up <x.y>        Coordinates of left-up corner";

const std::string right_down_option_description = "  --right_down <x.y>     Coordinates of right-down corner";

const std::string dest_left_up_option_description = "  --dest_left_up <x.y>   Coordinates of destination left-up corner";

const std::string old_color_option_description = "  --old_color <r.g.b>    Old color to replace";

const std::string new_color_option_description = "  --new_color <r.g.b>    New color to replace with";

const std::string color_option_description = "  --color <r.g.b>        Color of hexagon/ornament";

const std::string copy_option_description = "  --copy                 Copy operation";

const std::string color_replace_option_description = "  --color_replace        Color replace operation";

const std::string ornament_option_description = "  --split                Ornament operation";

const std::string radius_option_description = "  --radius <value>       Radius of hexagon";

const std::string thickness_option_description = "  --thickness <value>    Thickness of hexagon/ornament";

const std::string output_option_description = "  -o, --output <file>    Output file";

const std::string input_option_description = "  -i, --input <file>     Input file";

const std::string help_option_description = "  -h, --help             Display this information";

const std::string fill_option_description = "  --fill                 Filling hexagon with color";

const std::string fill_color_option_description = "  --fill_color           Color of hexagon's insides";


