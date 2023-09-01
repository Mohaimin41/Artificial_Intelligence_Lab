#include <unordered_map>
#include <unordered_set>

#include "1905041_defs.h"
#include "1905041_dec_tree.h"

extern std::vector<struct attribute *> *ALL_ATTRIBS;

extern std::vector<struct example *> *ALL_EXAMPLES;

extern std::unordered_map<std::string, int> *ATTRIB_IDX_MAP;

/**
 * @brief   Get the string attribute to int index(in global ATTRIB array) map
 *
 * @return  std::unordered_map<std::string, int>*:
 *          map of attribute name to index
 */
std::unordered_map<std::string, int> *get_attrib_idx_map()
{
    std::unordered_map<std::string, int> *attrib_idx =
        new std::unordered_map<std::string, int>();
    for (int i = 0; i < ALL_ATTRIBS->size(); i++)
    {
        attrib_idx->insert(
            std::make_pair(
                ALL_ATTRIBS->at(i)->attrib_name, i));
    }

    return attrib_idx;
}

std::pair<int, int> run_single_test(std::vector<struct example *> *training_set, node *root)
{
    int mismatch = 0;
    int failure = 0;
    // test
    for (int k = 0; k < training_set->size(); k++)
    {
        struct node *res = get_class(training_set->at(k), root);
        if (res && res->is_leaf)
        {
            if (training_set->at(k)->classification != res->classifcation)
            {
                mismatch++;
            }
        }
        else
        {
            failure++;
        }
    }
    return std::make_pair(mismatch, failure);
}

/**
 * @brief   split and put examples from global vector* to training and test 
 *          example* vectors, training vector size corresponds to percentage
 * 
 * @param   training_set    pointer to vector of example*
 * @param   testing_set pointer to vector of example* 
 * @param   percentage  fraction of ALL_EXAMPLES that would be in training vector
 */
void split_examples(std::vector<struct example *> *training_set,
                    std::vector<struct example *> *testing_set,
                    double percentage)
{
    int end_index = std::round(ALL_EXAMPLES->size() * (percentage / 100.0));

    for (int i = 0; i < end_index; i++) {
        training_set->push_back(ALL_EXAMPLES->at(i));
    }
    for (int i = end_index; i < ALL_EXAMPLES->size(); i++) {
        testing_set->push_back(ALL_EXAMPLES->at(i));
    }
}
/**
 * @brief   Get the leaf node at decision tree for given example*
 *          or nullpointer in case of errors. Called \b recursively
 * @param   sample  the example* to classify
 * @param   root    node* root of decision tree
 * @return  struct node*:   the decision leaf or nullpointer
 */
struct node *get_class(struct example *sample, struct node *root)
{
    if (root->is_leaf)
    {
        return root;
    }
    else
    {
        std::string val_for_tested_attrb =
            sample->ex_val_for_attrib->at(
                ATTRIB_IDX_MAP->at(root->tested_attrib_name));

        struct node *child_list_iter = root->child_list_head;

        while (child_list_iter)
        {
            if (child_list_iter->parent_node_choice == val_for_tested_attrb)
            {
                return get_class(sample, child_list_iter);
            }
            child_list_iter = child_list_iter->next_sibling;
        }
        return nullptr;
    }
}
/**
 * @brief   Get the average of given numbers
 *
 * @param   numbers   vector of numbers
 * @return  double   average
 */
double get_average(std::vector<double> &numbers)
{
    double avg = 0;
    for (auto k : numbers)
    {
        avg += (k * 1.0) / (numbers.size());
    }
    return avg;
}

/**
 * @brief Get the standard deviation of given numbers
 *
 * @param   numbers   vector of numbers
 * @return  double  standard deviation
 */
double get_standard_deviation(std::vector<double> &numbers)
{
    double avg = get_average(numbers);
    double sd = 0;
    for (auto k : numbers)
    {
        // sum ( x_i - x_avg )^2 / N
        // = sum ( (x_i - x_avg )^2 / N )
        sd += ((k - avg) * (k - avg)) / (numbers.size() * 1.0);
    }
    // sd = sqrt ( sum ( x_i - x_avg )^2 / N)
    return std::sqrt(sd);
}
