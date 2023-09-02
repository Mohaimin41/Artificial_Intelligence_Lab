#include <unordered_set>
#include <unordered_map>
#include <cmath>

#include "1905041_defs.h"
#include "1905041_dec_tree.h"

extern std::vector<struct attribute *> *ALL_ATTRIBS;

/**
 * @brief   returns the class name if given vector* of example* has same
 *          class in all samples, empty string("") otherwise
 * @param   samples vector of example*
 * @return  std::string  name of the same class or "" otherwise
 */
std::string are_all_of_same_class(std::vector<struct example *> *samples)
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
        if (s->classification != same_class)
        {
            return std::string("");
        }
    }

    return same_class;
}

/**
 * @brief   Get the name of majority class in given vector* of example*
 *
 * @param   samples    vector* of example*
 * @return  std::string class name
 */
std::string get_majority_class(std::vector<struct example *> *samples)
{
    // classes that are present in given example set
    std::unordered_set<std::string> present_classes;
    // class name to class instance count mapping
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
 * @brief   returns new sample vector* from given samples vector* matching
 *          the given value for given attribute
 *
 * @param   samples source vector* of example*
 * @param   attrib  attribute* being tested
 * @param   attrib_value    string value of attribute
 * @return  std::vector<struct example *> *
 */
std::vector<struct example *> *get_samples_matching_value(
    std::vector<struct example *> *samples,
    struct attribute *attrib, std::string attrib_value)
{
    std::vector<struct example *> *new_samples =
        new std::vector<struct example *>();

    int idx_of_attrib = -1;
    // find index of attribute in global attribute vector
    for (int i = 0; i < ALL_ATTRIBS->size(); i++)
    {
        if (ALL_ATTRIBS->at(i)->attrib_name == attrib->attrib_name)
        {
            idx_of_attrib = i;
            break;
        }
    }
    // populate new samples vector
    for (auto s : *samples)
    {
        if (s->ex_val_for_attrib->at(idx_of_attrib) == attrib_value)
        {
            new_samples->push_back(s);
        }
    }
    return new_samples;
}

/**
 * @brief   calculate entropy on given list of samples, finds unique classes
 *          and calculates entropy on them(absent classes contribute 0 anyway)
 * @param   samples vector of example*
 * @return  doouble entropy value
 */
double find_entropy(std::vector<struct example *> *samples)
{
    if (samples->size() == 0) // no entropy if no example
    {
        return 0.0;
    }
    double entropy = 0.0;
    int num_samples = samples->size();

    // classes that are present in given example set
    std::unordered_set<std::string> present_classes;
    // class name to class instance count mapping
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

    // entropy of random variable V = H(V), with V having values v1...vk
    // each value has probability of apparing in the examples = P(vk)
    // H(V) = sum_k ( P(vk) log_2 ( 1 / P(vk) ) )
    // P(vk) = (num of sample with V=vk) / ( num of sample )
    for (auto elem : instance_count)
    {
        if (elem.second == 0)
        {
            continue;
        }
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
    if (samples->size() == 0) // shouldn't be reached but setting a fail safe
    {
        return 0;
    }

    // Entropy at current node
    double prev_entropy = find_entropy(samples);

    double rem = 0.0;
    // Attribute A has v1,...,vk distinct values
    // Remainder(Attribute A) =
    //          sum_k ( P(A=vk at current node) * Entropy(Child node with A=vk))
    // P(A = vk at current node ) = (num of example with A=vk)
    //                              / (num of example)
    for (auto val : *(attrib->attrib_values)) // loop over each value vk
    {
        // get the examples that would be in the child node with A=vk
        std::vector<struct example *> *splitted_sample =
            get_samples_matching_value(samples, attrib, val); // SPLITTED_SAMPLE

        int split_sample_count = splitted_sample->size();

        rem += ((split_sample_count * 1.0) / num_samples) *
               find_entropy(splitted_sample);
        delete splitted_sample; // SPLITTED_SAMPLE
    }
    return prev_entropy - rem; // Information Gain = Entropy(current node) - Remainder
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

/**
 * @brief   return root node* of decision tree, recursively called
 *
 * @param   samples   pointer to vector of example*
 * @param   attribs   pointer to vector of attribute*
 * @param   par_samples   pointer to parent's vector of example*
 * @return  node*
 */
node *decision_tree(
    std::vector<struct example *> *samples,
    std::vector<struct attribute *> *attribs,
    std::vector<struct example *> *par_samples)
{
    std::string name_of_same_class = are_all_of_same_class(samples);
    if (samples->empty())
    {
        // if examples is empty then return PLURALITY-VALUE(parent examples)
        std::string parent_majority_class = get_majority_class(par_samples);
        return mkleaf(parent_majority_class);
    }
    else if (name_of_same_class != "")
    {
        // if all examples have the same classification then
        // return the classification
        return mkleaf(name_of_same_class);
    }
    else if (attribs->empty())
    {
        // if attributes is empty then return PLURALITY-VALUE(examples)
        std::string given_sample_majority_class = get_majority_class(samples);
        return mkleaf(given_sample_majority_class);
    }
    else
    {
        // A ←argmax_(a ∈ attributes)( IMPORTANCE(a, examples) )
        struct attribute *best_attrib = get_best_attrib(attribs, samples);
        // tree ← a new decision tree with root test A
        struct node *subtree_root = mk_tree_node(best_attrib);
        // attributes − A
        std::vector<struct attribute *> *new_attribs =
            new std::vector<struct attribute *>(); // NEW_ATTRIBS

        for (auto prev_attr : *attribs)
        {
            if (prev_attr->attrib_name != best_attrib->attrib_name)
            {
                new_attribs->push_back(prev_attr);
            }
        }

        // for each value v_k of A do
        for (auto value : *(best_attrib->attrib_values))
        {
            // exs ← {e : e ∈ examples and e.A = v_k}
            std::vector<struct example *> *new_samples =
                get_samples_matching_value(samples, best_attrib, value); // NEW_SAMPLES

            // subtree ← DECISION-TREE-LEARNING(exs, attributes − A, examples)
            struct node *child = decision_tree(new_samples,
                                               new_attribs, samples);

            // add a branch to tree with label (A = v_k) and subtree subtree
            child->parent = subtree_root;
            child->parent_node_choice = value;
            add_child(subtree_root, child);
            delete new_samples; // NEW_SAMPLES
        }
        delete new_attribs; // NEW_ATTRIBS
        return subtree_root;
    }
    return nullptr;
}
