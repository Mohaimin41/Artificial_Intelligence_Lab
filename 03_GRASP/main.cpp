#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <random>
#include <algorithm>
#include <chrono>

#include "graph_simulation.h"

int known_solns[TOTAL_GRAPHS] = {0};

char *known_best_soln_filename = (char *)"1905041_known_best_soln.csv";
char *output_csv_filename = (char *)"1905041_output.csv";

int main(int argc, char *argv[])
{
    std::ofstream output_file(output_csv_filename);

    read_best_solns(known_best_soln_filename, known_solns);

    start_output(&output_file);

    try
    {
        const std::filesystem::path pathToShow{argc >= 2 ? argv[1] : std::filesystem::current_path()};

        for (const auto &entry : std::filesystem::directory_iterator(pathToShow))
        {
            if (entry.is_regular_file())
            {
                // find out graph name, need for output csv and best solution index
                std::string graph_name = entry.path().filename().string();

                int known_best_soln = known_solns[stoi(
                    (graph_name.substr(0, graph_name.length() - 4))
                        .substr(1))];

                struct graph_data graph;

                // read graph to graph array and store vertices and edges in graph_data
                read_graph(&entry, &graph);
                std::cout << "Read " << graph_name << "\n";

                // run the algoes
                int randomized1_val = avg_randomized1(&graph);
                std::cout << N_RUN << " runs of randomized1 algorithm ended, result:  "
                          << randomized1_val << "\n";

                int greedy1_val = greedy1(&graph);
                std::cout << "Greedy1 algorithm running ended, result:  "
                          << greedy1_val << "\n";

                int semigreedy_alpha_val = avg_semigreedy_alpha(&graph);
                std::cout << N_RUN << " runs of semigreedy1 algorithm ended, result:  "
                          << semigreedy_alpha_val << "\n";

                GRASP_return_data res = GRASP(GRASP_ITERATIONS, &graph);
                std::cout << GRASP_ITERATIONS << " runs of GRASP algorithm ended, result:  "
                          << res.local_search_iter_count << ","
                          << res.local_search_best_val << ","
                          << res.grasp_iter_count << ","
                          << res.grasp_best_val << "\n\n";

                // write to output file
                output_file << graph_name.substr(0, graph_name.length() - 4) << ",";
                output_file << graph.vertices << ","
                            << graph.edges << ","
                            << randomized1_val << ","
                            << greedy1_val << ","
                            << semigreedy_alpha_val << ","
                            << res.local_search_iter_count << ","
                            << res.local_search_best_val << ","
                            << res.grasp_iter_count << ","
                            << res.grasp_best_val << ","
                            << known_best_soln << " \n";
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
    }
    catch (const std::exception &ex)
    {
        std::cerr << "general exception: " << ex.what() << '\n';
    }
    return 0;
}