/**
 * @file Index.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */

#ifndef INDEX_H
#define INDEX_H

#include <fstream>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "DocumentList.hpp"

using namespace std;

/**
 * The Index class is the abstract class to define the basic funcitonality of a inverted search index
 * @tparam T only used to determine the right type of DocumentList
 */
template<class T>
class Index
{
public:
 /**
  * Function to add a file described by a filename to the index 
  *
  * @param filename Name of a file to add
  */
    void insert(string filename);
 /**
  * Abstract Function to add a file by using a input stream to the index. Needs to be
  * overridden in concrete Implementations of an Index.
  *
  * @param *in Pointer to a input stream associated to the file
  * @param filename The filename of the file associated with the in stream
  */
    virtual void insert(ifstream *in, string filename) {};
 /**
  * Function to return the number of documents in the index
  *
  */
    int count() const;
 /**
  * Abstract Function to return the DocumentList of documents containing the search term. Needs to be 
  * overridden in concrete Implementations of an Index
  *
  * @param key Search term to retrieve documents containing it
  * @return DocumentList of documents containing \p key
  */
    virtual DocumentList<T> *retrieve(string key) { return NULL; };
  protected:

 /**
  * Number of documents in the Index
  */
    int doccount;
};

template<class T>
void Index<T>::insert(string filename)
{
#ifdef DEBUG
  cout << "Adding file " << filename << " to index" << endl;
#endif
  // Open filestream
  ifstream infile;
  infile.open(filename,ifstream::in);
  // Call the other insert method with the filestream
  insert(&infile,filename);
}

template<class T>
int Index<T>::count() const
{
  return doccount();
}

#endif
