/**
 * @file Query.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */
#ifndef QUERY_H
#define QUERY_H

#include <vector>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "Util.hpp"

using namespace std;

/**
 * The Query class is the abstract class to define the basic funcitonality of a query
 * @tparam T only used to determine the right type of DocumentList
 */
template<class T>
class Query
{
  public: 
 /**
  * Function to parse and execute a query stored in a string. Needs to be
  * overridden in concrete Implementations of an Query.
  *
  * @param query The Query String
  */
    virtual void parse(string query) {};
 /**
  * Function to return the Result of a previously executed Query. Needs to be
  * overridden in concrete Implementations of an Query.
  *
  * @return Pointer to a DocumentList representing the Result of the Query
  */
    DocumentList<T> *get_result() { return NULL; };
};

#endif
