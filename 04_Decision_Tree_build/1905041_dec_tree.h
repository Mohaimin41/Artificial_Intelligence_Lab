#pragma once
#include "1905041_defs.h"

#ifndef DEC_TREE
#define DEC_TREE

/**
 * @brief   attribute struct, has name and all possible values of attribute
 */
struct attribute
{
    std::string attrib_name;                 // name of the attribute
    std::vector<std::string> *attrib_values; // all the possible values
};
/**
 * @brief   example struct, holds a single example's all attribute values
 *          and its classification
 */
struct example
{
    std::vector<std::string> *ex_val_for_attrib; // attribute values of example
    std::string classification; //  example's class
};
/**
 * @brief   decision tree node, holds tested attribute name,
 *          parent attribute value of its branch,
 *          pointer to parent, sibling & child node(s),
 *          if it is a leaf and the class if it is a leaf
 */
struct node
{
    node *parent;                   // pointer to parent node
    node *child_list_head;          // child node linked list header
    node *next_sibling;             // pointer to next sibling
    bool is_leaf;                   // is a leaf (decision) node
    std::string classifcation;      // class of node if a leaf
    std::string parent_node_choice; // the choice of parent node test
    std::string tested_attrib_name; // what it says
};

#endif // DEC_TREE