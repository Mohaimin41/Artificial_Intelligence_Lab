#include "1905041_defs.h"
#include "1905041_dec_tree.h"

const std::string WHITESPACE = " \n\r\t\f\v";

// trim leading whitespace, return modified string or "" if all char whitespace
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

// trim trailing whitespace, return modified string
// or "" if all char whitespace
std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

// trim leading and trailing whitespace
// return modified string, or
// "" if all char were whitespace char.
// whitespace chars = " \\n\\r\\t\\f\\v"
std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

/**
 * @brief   return pointer to vector of attribute* with all possible attributes
 *          from given file
 *
 * @param   fileentry   metadata file,
 * \b format:
 * \n "--class values
 * \\n <class value1>, <class value2>, ...
 * \\n --attributes
 * \\n <attribute name>: value1, value2, ..., valueN."
 * @return  std::vector<struct attribute *> *   pointer to vector of attribute*
 *
 * @warning no checking done, file must have this format
 */
std::vector<struct attribute *> *init_attribs(
    const std::filesystem::directory_entry *fileentry)
{
    std::vector<struct attribute *> *ALL_ATTRIBS =
        new std::vector<struct attribute *>();
    std::string single_line;
    bool info_started = false;

    std::ifstream metadata_file((*fileentry).path().c_str());

    while (getline(metadata_file, single_line))
    {
        if (single_line == "--attributes") // next lines will be attributes
        {
            info_started = true;
            continue;
        }
        if (info_started && trim(single_line).size() != 0)
        {
            // two level tokenization
            std::string temp;
            std::stringstream ss_line(single_line);
            std::vector<std::string> stg1_tokens;
            std::vector<std::string> val_tokens;
            // split into name and values, tokenization on ':'
            while (getline(ss_line, temp, ':'))
            {
                stg1_tokens.push_back(temp);
            }

            // trim whitespace from values and remove trailing '.'
            temp = trim(stg1_tokens[1]);
            temp.pop_back();

            std::stringstream ss_vals(temp);
            // split the values, tokenization on ','
            while (getline(ss_vals, temp, ','))
            {
                val_tokens.push_back(trim(temp));
            }

            // make new attribute
            struct attribute *attrib = new attribute();
            attrib->attrib_values = new std::vector<std::string>();
            // name is first token from ':' tokenization
            attrib->attrib_name = stg1_tokens[0];
            // attribute values are pushed
            for (auto val : val_tokens)
            {
                attrib->attrib_values->push_back(val);
            }

            ALL_ATTRIBS->push_back(attrib);
        }
    }
    metadata_file.close();

    return ALL_ATTRIBS;
}

/**
 * @brief   return pointer to vector of example* populated
 *          using data from given file
 * @param   fileentry   data file,
 *                      \b format:
 *                      <attribute value>,...,<classification>\\n
 * @warning no checking done, file must have the format
 */
std::vector<struct example *> *init_samples(
    const std::filesystem::directory_entry *fileentry)
{
    std::vector<struct example *> *ALL_SAMPLES =
        new std::vector<struct example *>();
    std::string single_sample;

    std::ifstream data_file((*fileentry).path().c_str());

    while (getline(data_file, single_sample))
    {
        // tokenization
        std::stringstream ss_sample(single_sample);

        std::vector<std::string> tokens;
        std::string temp;

        while (getline(ss_sample, temp, ','))
        {
            tokens.push_back(trim(temp));
        }
        // new sample
        struct example *new_sample = new example();
        new_sample->ex_val_for_attrib = new std::vector<std::string>();

        // from first till penultimate tokens are attrib val
        for (int i = 0; i < tokens.size() - 1; i++)
        {
            new_sample->ex_val_for_attrib->push_back(trim(tokens[i]));
        }
        // last token is classification
        new_sample->classification = trim(tokens[tokens.size() - 1]);

        ALL_SAMPLES->push_back(new_sample);
    }

    data_file.close();
    return ALL_SAMPLES;
}

/**
 * @brief   prints char ch 80 times in a line, then a newline at end
 *
 * @param   ch
 */
void print_char_line(char ch)
{
    for (int i = 0; i < 80; i++)
    {
        std::cout << ch;
    }
    std::cout << "\n";
}

/**
 * @brief   prints char ch 120 times in a line, then a newline at end
 *          to the output file stream
 * @param   output_file
 * @param   ch
 */
void print_char_line_file(std::ofstream *output_file, char ch)
{
    for (int i = 0; i < 120; i++)
    {
        *(output_file) << ch;
    }
    *(output_file) << "\n";
}

/**
 * @brief   write initial headers to output file
 *
 * @param   output_file: ofstream pointer to file
 */
void start_output(std::ofstream *output_file)
{
    *(output_file) << "ITERATION\t"
                   << "TOTAL EXAMPLES\t"
                   << "TRAINING SET SIZE\t"
                   << "TEST SET SIZE\t"
                   << "\%OF TOTAL EXAMPLES TRAINED\t"
                   << "\%ACCURACY\n";
    print_char_line_file(output_file, '-');
}

/**
 * @brief   write mean and SD of accuracy and close filestream
 *
 * @param   output_file ofstream pointer to file
 * @param   all_accuracies: vector of all runs' accuracy
 */
void end_output(std::ofstream *output_file, std::vector<double> &all_accuracies)
{
    print_char_line_file(output_file, '-');
    *(output_file) << "\n";
    *(output_file) << "Mean Accuracy: " << get_average(all_accuracies) << "%\n";
    *(output_file) << "Standard Deviation of Accuracy: "
                   << get_standard_deviation(all_accuracies);
    output_file->close();
}
