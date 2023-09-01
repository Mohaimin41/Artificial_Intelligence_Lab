#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <cstdlib>

#ifndef DEFS
#define DEFS

struct attribute;
struct example;
struct node;

// 1905041_node.cpp 
struct node*                    mkleaf(std::string class_name);
struct node*                    mk_tree_node(struct attribute* attrib);
void                            add_child(
                                    struct node* parent, struct node* kid);
void                            print_tree(struct node* root, int level);   

// 1905041_dec_tree.cpp
std::string                     are_all_of_same_class(
                                    std::vector<struct example*>
                                    );
std::string                     get_majority_class(
                                    std::vector<struct example*>* examples
                                    );
std::vector<struct example*>*   get_samples_matching_value(
                                    std::vector<struct example*>* samples,
                                    struct attribute* attrib,
                                    std::string attrib_value
                                    );
double                          find_importance(
                                    struct attribute* attrib,
                                    std::vector<struct example*> *samples
                                    );
struct attribute*               get_best_attrib(
                                    std::vector<struct attribute*>* attribs,
                                    std::vector<struct example*> *samples
                                    );
struct node*                    decision_tree(
                                    std::vector<struct example*>* samples,
                                    std::vector<struct attribute*>* attribs,
                                    std::vector<struct example*>* par_samples
                                    );           

// 1905041_helper.cpp
std::vector<struct attribute*>* init_attribs(
                                    const std::filesystem::directory_entry *
                                    fileentry
                                    );
std::vector<struct example*>*   init_samples(
                                    const std::filesystem::directory_entry *
                                    fileentry
                                    );
void                            print_char_line(char ch);
void                            start_output(std::ofstream* output_file);
void                            end_output(
                                    std::ofstream* output_file,
                                    std::vector<double> &all_accuracies
                                    );

// 1905041_tester.cpp
double                          get_average(std::vector<double> &numbers);
double                          get_standard_deviation(
                                    std::vector<double> &numbers
                                    );
void                            split_examples(
                                    std::vector<struct example *> * training_set, 
                                    std::vector<struct example *> * testing_set,
                                    double percentage
                                    );
struct node *                   get_class(
                                    struct example *sample, struct node *root
                                    );
std::unordered_map<std::string, int>*   get_attrib_idx_map();
std::pair<int,int>              run_single_test(
                                    std::vector<struct example*>* training_set,
                                    struct node* root
                                );

#endif // DEFS
