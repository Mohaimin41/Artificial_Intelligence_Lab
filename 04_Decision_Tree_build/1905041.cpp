#include <iostream>
#include <iomanip>

#include "1905041_defs.h"
#include "1905041_dec_tree.h"

// global attribute* vector
std::vector<struct attribute *> *ALL_ATTRIBS;
// global example vector*
std::vector<struct example *> *ALL_EXAMPLES;
// map of attribute name to index of attribute name(in ALL_ATTRIB vector)
std::unordered_map<std::string, int> *ATTRIB_IDX_MAP;
// how many times running
int num_iters = 20;
// file to write iteration accuracy and other data
std::ofstream output_file("1905041_output.txt");

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cout << "Usage: .\\1905041.exe <data folder> ";
        std::cout << "<number of iterations>\n";
        return 0;
    }
    // populate global example*, attribute* vectors from files
    // in given directory
    std::string data_dir(argv[1]);
    try
    {
        const std::filesystem::path dir_path{data_dir};

        for (const auto &entry : std::filesystem::directory_iterator(dir_path))
        {

            if (entry.is_regular_file())
            {

                if (entry.path().string().find(".data") != std::string::npos)
                {
                    std::cout << "\nreading data file: "
                              << entry.path().string();
                    ALL_EXAMPLES = init_samples(&entry); // ALL_EXAMPLE
                }
                else if (
                    entry.path().string().find(".c45-names") != std::string::npos)
                {
                    std::cout << "\nreading metadata file: "
                              << entry.path().string();
                    ALL_ATTRIBS = init_attribs(&entry); // ALL_ATTRIBS
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error &err)
    {
        std::cerr << "filesystem error! " << err.what() << '\n';
        if (!err.path1().empty())
            std::cerr << "path1: " << err.path1().string() << '\n';
        if (!err.path2().empty())
            std::cerr << "path2: " << err.path2().string() << '\n';
        return 0;
    }
    catch (const std::exception &ex)
    {
        std::cerr << "general exception: " << ex.what() << '\n';
        return 0;
    }

    std::cout << "\nReading samples done: \n";
    std::cout << "Number of samples: " << ALL_EXAMPLES->size() << ", "
              << "Number of attributes: " << ALL_ATTRIBS->size() << ", "
              << ", Number of iterations to run: " << argv[2] << "\n";
    print_char_line('=');

    num_iters = std::atoi(argv[2]);

    // populate attribute to index map
    ATTRIB_IDX_MAP = get_attrib_idx_map();
    // vector to store all iteration accuracy
    std::vector<double> all_accuracies;
    // timer start variable
    std::chrono::steady_clock::time_point time_start;

    start_output(&output_file);
    // run iterations
    for (int i = 0; i < num_iters; i++)
    {
        // make training set
        std::vector<struct example *> *training_set =
            new std::vector<struct example *>();
        std::vector<struct example *> *testing_set =
            new std::vector<struct example *>();

        split_examples(training_set, testing_set, 80);
        std::cout << "Training size: " << training_set->size() << ", "
                  << ", Testing size: " << testing_set->size() << "\n";

        // reset timer
        time_start = std::chrono::steady_clock::now();

        // run algorithm
        struct node *root = decision_tree(training_set, ALL_ATTRIBS,
                                          training_set);

        // print_tree(root, 0);

        // test
        std::pair<int, int> single_run_result =
            run_single_test(testing_set, root);

        // report to stdout
        std::cout << "\t\t\t\t\tITERATION " << i + 1 << " DONE\n";
        std::cout << "Time taken to train and test: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         std::chrono::steady_clock::now() - time_start)
                         .count()
                  << " ms\n";

        double accuracy = (100.0 *
                           (testing_set->size() - single_run_result.first)) /
                          testing_set->size();
        all_accuracies.push_back(accuracy);

        std::cout << "Accuracy: " << accuracy << "%\n";
        print_char_line('=');

        // write to file
        output_file << std::left << std::setw(12) << i + 1 << std::setw(20)
                    << training_set->size() << std::setw(16)
                    << testing_set->size()
                    << std::setw(11) << accuracy << "\n";
    }

    end_output(&output_file, all_accuracies);

    delete ALL_ATTRIBS;  // ALL_ATTRIBS
    delete ALL_EXAMPLES; // ALL_EXAMPLES
    return 0;
}