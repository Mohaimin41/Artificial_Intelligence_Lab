#include "1905041_defs.h"

#ifndef DEC_TREE
#define DEC_TREE

std::vector<struct attribute*> ALL_ATTRIBS;

std::vector<struct node*> COMMON_NODES;

/**
 * @brief   attribute struct, has name and all possible values of attribute
 */
struct attribute
{
    std::string attrib_name;                // name of the attribute
    std::vector<std::string> attrib_values; // all the possible values
};
/**
 * @brief   example struct, holds a single example's all attribute values
 *          and its classification
 */
struct example
{
    std::vector<std::string> ex_val_for_attrib; // attribute values of example
    // index in global classes vector,
    // is the classification of this example
    int classval_arr_idx;
    std::string classification; //  example's class
};
/**
 * @brief   decision tree node, holds tested attribute,
 *          parent attribute value of its branch,
 *          pointer to parent & child node(s),
 *          if it is a leaf and the class if it is a leaf
 */
struct node
{
    bool is_leaf;                       // is a leaf (decision) node
    std::string parent_node_choice;     // the choice of parent node test
    struct attribute *tested_attribute; // the attribute being tested here
    node *parent;                       // pointer to parent node
    node *child_list_head;              // child node linked list header
    node *next_sibling;                 // pointer to next sibling
    std::string classifcation;          // class of node if a leaf
};

#endif // DEC_TREE