/**
 * @file DocumentList.hpp
 * @author  Herbert Lange <herbert.lange@campus.lmu.de>
 * @version 1.0
 */

#ifndef DOCUMENTLIST_H
#define DOCUMENTLIST_H

/**
 * The DocumentList class is the abstract class to define the basic funcitonality of a document list.
 * @tparam T Datatype to represent a Document in the DocumentList
 */
template<class T>
class DocumentList
{
  public:
/**
 * Function to add a Document to the DocumentList. Needs to be 
 * overridden in concrete Implementations of an Index.
 * @param element The Document to add
 */
    virtual void add(T element) {};
/**
 * Function to remove a Document from the DocumentList. Needs to be 
 * overridden in concrete Implementations of an Index.
 * @param element The Document to remove
 */
    virtual void remove(T element) {};
/**
 * Function to intersect this DocumentList with a second one. Needs to be 
 * overridden in concrete Implementations of an Index.
 * @param dl The DocumentList to intersect with
 * @return Pointer to the intersected DocumentList
 */
    virtual DocumentList<T> *intersect(DocumentList<T> dl) { return NULL; };
/**
 * Function to unify this DocumentList with a second one. Needs to be 
 * overridden in concrete Implementations of an Index.
 * @param dl The DocumentList to unify with
 * @return Pointer to the unified DocumentList
 */
    virtual DocumentList<T> *unify(DocumentList<T> dl) { return NULL; };
/**
 * Function to complement this DocumentList according to a second one. Needs to be 
 * overridden in concrete Implementations of an Index.
 * @param element The DocumentList to complement with
 * @return Pointer to the complemented DocumentList
 */
    virtual DocumentList<T> *complement(DocumentList<T> dl) { return NULL; };
/**
 * Function to print the content of this DocumentList. Needs to be 
  * overridden in concrete Implementations of an DocumentList.
 */
    virtual void print() {};
};

#endif
