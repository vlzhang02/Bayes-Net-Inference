/*
 * Node.cpp
 * by: Valerie Zhang
 *
 * Purpose: An implementation of Node class. A node stores all the information 
 *          of a variable in a Bayes Net
 */
#include "Node.h"

using namespace std;
/*
 * default constructor
 */
Node::Node()
{
    name = assignedValue = "NONE";
}
/*
 * secondary constructor
 */
Node::Node(string key, vector<string> value) 
{
    name = key;
    for (size_t i = 0; i < value.size(); i++) {
        values.push_back(value[i]);
    }
}
/*
 * destructor
 */
Node::~Node() 
{
    values.clear();
    parents.clear();
    children.clear();
}
/*
 * &operator=()
 * Purpose:     defines assignment operator to make a deep copy of the
 *              instance on the right hand side(rhs) into the instance on the
 *              left hand side(lhs)
 * Arguments:   reference to instance on the rhs
 * Returns:     reference to the lhs
 */
Node &Node::operator=(const Node &other) 
{
    if (this == &other) {return *this;}
    // empties Node on LHS
    values.clear();
    parents.clear();
    children.clear();
    // copies over values
    name = other.name;
    for (size_t i = 0; i < values.size(); i++) {
        values.push_back(other.values[i]);
    }
    for (size_t j = 0; j < parents.size(); j++) {
        parents.push_back(other.parents[j]);
    }
    for (size_t k = 0; k < children.size(); k++) {
        children.push_back(other.children[k]);
    }
    cpt = other.cpt;
    return *this;
}
/*
 * getName()
 * Purpose:     get name at certain index
 * Parameters:  index
 * Returns:     name
 */
string Node::getName()
{
    return name;
}
/*
 * getAssignedVal()
 * Purpose:     get assigned value at certain index
 * Parameters:  index
 * Returns:     assigned value
 */
string Node::getAssignedVal()
{
    return assignedValue;
}
/*
 * getNumVal()
 * Purpose:     get number of possible values
 * Parameters:  none
 * Returns:     number of possible values
 */
int Node::getNumVal() {
    return values.size();
}
/*
 * getIndex()
 * Purpose:     get index of specified value
 * Parameters:  value
 * Returns:     index
 */
 int Node::getIndex(string value) 
{
    for (size_t i = 0; i < values.size(); i++) {
        if (values[i] == value) {
            return i;
        }
    }
    return -1;
}
/*
 * getValue()
 * Purpose:     get value are particular index
 * Parameters:  index
 * Returns:     value
 */
string Node::getValue(int index)
{
    return values[index];
}
/*
 * getNumChildren()
 * Purpose:     get total number of children
 * Parameters:  none
 * Returns:     number of children
 */
int Node::getNumChildren() 
{
    return children.size();
}
/*
 * getChild()
 * Purpose:     get child at certain index
 * Parameters:  index
 * Returns:     name of child
 */
string Node::getChild(int index)
{
    return children[index];
}
/*
 * getNumParents()
 * Purpose:     get total number of parents
 * Parameters:  none
 * Returns:     number of parents
 */
int Node::getNumParents() 
{
    return parents.size();
}
/*
 * getParent()
 * Purpose:     get parent at certain index
 * Parameters:  index
 * Returns:     name of parent
 */
string Node::getParent(int index)
{
    return parents[index];
}
/*
 * getProbability()
 * Purpose:     get probability at particular 
 * Parameters:  string key and int index
 * Returns:     probability 
 */
double Node::getProbability(string key, int index) 
{
    return cpt.getProbability(key, index);
}
/*
 * setName()
 * Purpose:     sets name of node
 * Parameter:   string name
 * Returns:     none
 */
void Node::setName(string input)
{
    name = input;
}
/*
 * setValue()
 * Purpose:     sets assginedValue of node
 * Parameter:   string value
 * Returns:     none
 */
void Node::setVal(string input)
{
    assignedValue = input;
}
/*
 * addVal()
 * Purpose:     adds value to node
 * Parameters:  value to add
 * Returns:     none
 */
void Node::addVal(string value)
{
    values.push_back(value);
}
/*
 * addParent()
 * Purpose:     adds parent to node
 * Parameters:  parent to add
 * Returns:     none
 */
void Node::addParent(string parent)
{
    parents.push_back(parent);
}
/*
 * addChild()
 * Purpose:     adds child to node
 * Parameters:  child to add
 * Returns:     none
 */
void Node::addChild(string child)
{
    children.push_back(child);
}
/*
 * addToCPT()
 * Purpose:     adds info in string to CPT
 * Parameters:  string with info
 * Returns:     none
 */
void Node::addToCPT(string line)
{
    cpt.parseLine(line);
}