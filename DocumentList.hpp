#ifndef DOCUMENTLIST_H
#define DOCUMENTLIST_H

// Quasi-abstract base class for document lists
template<class T>
class DocumentList
{
  public:
  // Add document
    virtual void add(T element) {};
  // Remove document
    virtual void remove(T element) {};
  // Intersect two document lists
    virtual DocumentList<T> *intersect(DocumentList<T> dl) { return NULL; };
  // Unify two document lists
    virtual DocumentList<T> *unify(DocumentList<T> dl) { return NULL; };
  // Complement of this in dl
    virtual DocumentList<T> *complement(DocumentList<T> dl) { return NULL; };
  // Print the content of the document list
    virtual void print() {};
};

#endif
