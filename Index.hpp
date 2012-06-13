#ifndef INDEX_H
#define INDEX_H

#include <fstream>
#include <string>

#ifdef DEBUG
#include <iostream>
#endif

#include "DocumentList.hpp"

using namespace std;

// Quasi-abstract class for a search index
template<class T>
class Index
{
  public:
  // Add files either by filename or by filestream
    void insert(string filename);
    virtual void insert(ifstream *in) {};
  // Return number of documents in the index
    int count() const;
  // Return document list for a term
    virtual DocumentList<T> *retrieve(string key) { return NULL; };
  protected:
  // Number of Documents in index
    int doccount=0;
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
