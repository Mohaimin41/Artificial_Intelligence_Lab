#include <vector>
#include <string>

#ifndef DEFS
#define DEFS

struct attribute;
struct example;
struct node;

struct node*                    mkleaf(std::string class_name);
struct node*                    get_tree_node(
                                    std::vector<struct node*>*,
                                    struct attribute* attrib
                                    );
void                            add_child(
                                    struct node* parent, struct node* kid);                                    
std::string                     is_all_same_class(std::vector<struct example*>);
std::string                     get_majority_class(
                                    std::vector<struct example*>* examples
                                    );
void                            get_samples_matching_value(
                                    std::vector<struct example*>* new_samples,
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

#endif // DEFS
