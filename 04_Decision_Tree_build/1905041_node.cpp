#include"1905041_defs.h"
#include"1905041_dec_tree.h"

/**
 * @brief   make a new leaf node with class_name as its classification
 * @warning must set leaf->parent and leaf->parent_choice of returned leaf node*
 *          at decision_tree() function
 * @param   class_name  the class name to put into leaf
 * @return  new leaf node
 */
node *mkleaf(std::string class_name)
{
    struct node *leaf = new struct node();

    leaf->child_list_head = nullptr; // no child
    leaf->is_leaf = true;
    leaf->next_sibling = nullptr; // no sibling at start
    leaf->classifcation = class_name;
    leaf->parent = nullptr;
    leaf->parent_node_choice = "";
    leaf->tested_attrib_name = ""; // no attribute is tested
    return leaf;
}

/**
 * @brief   make and return a node* for tree, with given attribute* name
 *          as tested_attribute name
 * @warning must set node->parent and node->parent_choice of returned node*
 *          at decision_tree() function
 *
 * @param   attrib  attribute that is tested in this node
 * @return  node* intermediate node in tree
 */
node *mk_tree_node(struct attribute *attrib)
{
    struct node *tree_node = new struct node();
    tree_node->child_list_head = nullptr; // no kids
    tree_node->classifcation = "";        // it is not a leaf
    tree_node->is_leaf = false;
    tree_node->tested_attrib_name = attrib->attrib_name;
    tree_node->next_sibling = nullptr;  // no siblings
    tree_node->parent = nullptr;        // no parent
    tree_node->parent_node_choice = ""; // no parent
    return tree_node;
}

/**
 * @brief   adds a child to parent node
 *
 * @param   parent
 * @param   kid
 */
void add_child(node *parent, node *kid)
{
    if (parent->child_list_head == nullptr)
    {
        // no child, set up child list
        parent->child_list_head = kid;
        parent->child_list_head->next_sibling = nullptr;
    }
    else
    {
        // there are children, start list traverse from head
        struct node *tail = parent->child_list_head;

        // loop till next_sibling is nullptr, i.e. last node reached
        while (tail->next_sibling)
        {
            tail = tail->next_sibling;
        }
        // put kid as next_sibling of last node
        tail->next_sibling = kid;
        kid->next_sibling = nullptr;
    }
}

/**
 * @brief   recursively print tree from given root
 *
 * @param   root    node*, root of tree
 * @param   level   tree depth, increases by 1 in recursive call
 */
void print_tree(struct node *root, int level)
{
    for (int i = 0; i < level; i++)
    {
        std::cout << "..";
    }
    std::cout << "(" << root->parent_node_choice << ")";
    if (root->is_leaf)
    {
        std::cout << "<<" << root->classifcation << ">>"
                  << "\n";
    }
    else
    {
        std::cout << root->tested_attrib_name << "\n\n";
        struct node *tail = root->child_list_head;
        while (tail)
        {
            print_tree(tail, level + 1);
            tail = tail->next_sibling;
        }
    }
}