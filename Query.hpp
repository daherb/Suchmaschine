#ifndef QUERY_H
#define QUERY_H

#include <vector>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "Util.hpp"

using namespace std;

// Quasi-abstract class for queries
template<class T>
class Query
{
  public: 
  // Parse the query
    virtual void parse(string query) {};
  // Return the result
    DocumentList<T> *get_result() { return NULL; };
};

#endif
