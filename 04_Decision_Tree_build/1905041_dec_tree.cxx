#include <unordered_set>
#include <unordered_map>
#include <cmath>

#include "1905041_defs.h"
#include "1905041_dec_tree.h"

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
    leaf->tested_attribute = nullptr; // no attribute is tested
    leaf->next_sibling = nullptr;     // no sibling at start
    return leaf;
}

/**
 * @brief   Get the tree node object from given common nodes vector
 *          that has same attribute as given attrib, if none has the
 *          attribute, returns nullptr
 * @param   common_nodes    the ready made vector of node*
 * @param   attrib  the attribute returned node should have
 * @return  node* from common node vector
 */
node *get_tree_node(std::vector<struct node *> *common_nodes, attribute *attrib)
{
    for (auto n : *common_nodes)
    {
        if (n->tested_attribute->attrib_name == attrib->attrib_name)
        {
            return n;
        }
    }
    return nullptr;
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
        return;
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
 * @brief   returns the class name if given list of example are all of same
 *          class, empty string("") otherwise
 * @param   samples vector of example*
 * @return  std::string  name of the same class or "" otherwise
 */
std::string is_all_same_class(std::vector<struct example *> *samples)
{
    std::string same_class = "";
    if (!samples->size())
    {
        return same_class;
    }
    // set same_class with first class name
    same_class = samples->at(0)->classification;

    for (auto s : *samples)
    {
        // return "" on mismatch
        if (!(s->classification == same_class))
        {
            return std::string("");
        }
    }
    // no mismatch
    return same_class;
}

/**
 * @brief   Get the name of majority class in given set of samples
 *
 * @param   samples    vector of example*
 * @return  std::string class name
 */
std::string get_majority_class(std::vector<struct example *> *samples)
{
    // classes that are present in given example set
    std::unordered_set<std::string> present_classes;
    // class name to instance count mapping
    std::unordered_map<std::string, int> instance_count;
    for (auto e : *samples)
    {
        if (present_classes.find(e->classification) == present_classes.end())
        {
            // insert to present class set and initialize count
            present_classes.insert(e->classification);
            instance_count[e->classification] = 0;
        }

        instance_count[e->classification] += 1;
    }

    std::string r = "";
    int highest_count = INT32_MIN;

    for (auto element : instance_count)
    {
        if (element.second > highest_count)
        {
            r = element.first;
            highest_count = element.second;
        }
    }
    return r;
}

/**
 * @brief   populates new sample set from given samples set matching the given
 *          value for given attribute
 *
 * @param   new_samples empty vector of example* to be populated
 * @param   samples source vector of example*
 * @param   attrib  attribute being tested
 * @param   attrib_value    string value of attribute
 */
void get_samples_matching_value(
    std::vector<struct example *> *new_samples,
    std::vector<struct example *> *samples,
    struct attribute *attrib, std::string attrib_value)
{
    int idx_of_attrib = -1;
    // find index of attribute in global attribute vector
    for (int i = 0; i < ALL_ATTRIBS.size(); i++)
    {
        if (ALL_ATTRIBS[i]->attrib_name == attrib->attrib_name)
        {
            idx_of_attrib = i;
            break;
        }
    }
    // use the index to get value of attribute in example and compare
    for (auto s : *samples)
    {
        if (s->ex_val_for_attrib[idx_of_attrib] == attrib_value)
        {
            new_samples->push_back(s);
        }
    }
}

/**
 * @brief   calculate entropy on given list of samples, finds unique classes
 *          and calculates entropy on them(absent classes contribute 0 anyway)
 * @param   samples vector of example*
 * @return  doouble entropy value 
 */
double find_entropy(std::vector<struct example *> *samples)
{
    int num_samples = samples->size();
    double entropy = 0.0;

    // classes that are present in given example set
    std::unordered_set<std::string> present_classes;
    // class name to instance count mapping
    std::unordered_map<std::string, int> instance_count;
    for (auto e : *samples)
    {
        if (present_classes.find(e->classification) == present_classes.end())
        {
            // insert to present class set and initialize count
            present_classes.insert(e->classification);
            instance_count[e->classification] = 0;
        }

        instance_count[e->classification] += 1;
    }

    for (auto elem : instance_count)
    {
        entropy += ((elem.second * 1.0) / num_samples) *
                   std::log2(num_samples / (elem.second * 1.0));
    }

    return entropy;
}

/**
 * @brief   caculate importance(information gain) of attribute with given 
 *          list of examples
 * 
 * @param   attrib  attribute*
 * @param   samples vector of examples*
 * @return  double  importance value
 */
double find_importance(attribute *attrib,
                       std::vector<struct example *> *samples)
{
    int num_samples = samples->size();
    double prev_entropy = find_entropy(samples);

    double rem = 0.0;

    for (auto val : attrib->attrib_values)
    {
        std::vector<struct example *> splitted_sample;
        get_samples_matching_value(&splitted_sample, samples, attrib, val);

        int split_sample_count = splitted_sample.size();

        rem += ((split_sample_count * 1.0) / num_samples) *
               find_entropy(&splitted_sample);
    }

    return prev_entropy - rem;
}

/**
 * @brief   returns best attribute for making node from given vector of
 *          attribute* with respect to given vector of example*
 *
 * @param   attribs vector of attribute* holding attributes to be checked
 * @param   samples vector of examples*
 * @return  attribute*  best attribute or nullptr if any error
 */
attribute *get_best_attrib(
    std::vector<struct attribute *> *attribs,
    std::vector<struct example *> *samples)
{
    struct attribute *best_attrib = nullptr;
    double res = INT64_MIN;
    for (auto attr : *attribs)
    {
        double t = find_importance(attr, samples);
        if (t > res)
        {
            res = t;
            best_attrib = attr;
        }
    }
    return best_attrib;
}

node *decision_tree(
    std::vector<struct example *> *samples,
    std::vector<struct attribute *> *attribs,
    std::vector<struct example *> *par_samples)
{
    std::string name_of_same_class = is_all_same_class(samples);
    if (!(name_of_same_class == ""))
    {
        // if all examples have the same classification then
        // return the classification
        return mkleaf(name_of_same_class);
    }
    else if (samples->empty())
    {
        // if examples is empty then return PLURALITY-VALUE(parent examples)
        std::string parent_majority_class = get_majority_class(par_samples);
        return mkleaf(parent_majority_class);
    }
    else if (attribs->empty())
    {
        // if attributes is empty then return PLURALITY-VALUE(examples)
        std::string given_sample_majority_class = get_majority_class(samples);
        return mkleaf(given_sample_majority_class);
    }
    else
    {
        // A ←argmax_(a ∈ attributes)( IMPORTANCE(a, examples))
        struct attribute *best_attrib = get_best_attrib(attribs, samples);
        // tree ← a new decision tree with root test A
        struct node *subtree_root = get_tree_node(&COMMON_NODES, best_attrib);

        // attributes − A
        std::vector<struct attribute *> new_attribs;

        for (auto prev_attr : *attribs)
        {
            if (!(prev_attr->attrib_name == best_attrib->attrib_name))
            {
                new_attribs.push_back(prev_attr);
            }
        }
        //for each value v_k of A do
        for (auto value : best_attrib->attrib_values)
        {
            // exs ← {e : e ∈ examples and e.A = v_k}
            std::vector<struct example *> new_samples;
            get_samples_matching_value(&new_samples,
                                       samples, best_attrib, value);

            // subtree ← DECISION-TREE-LEARNING(exs, attributes − A, examples)   
            struct node *child = decision_tree(&new_samples,
                                               &new_attribs, samples);

            // add a branch to tree with label (A = v_k) and subtree subtree   
            child->parent = subtree_root;
            child->parent_node_choice = value;
            add_child(subtree_root, child);
        }
        return subtree_root;
    }
    return nullptr;
}
