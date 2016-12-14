/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node=_node->_next; return *(this); }
      iterator operator ++ (int) { 
         iterator tmp = *(this);
         _node=_node->_next;
         return tmp; 
      }
      iterator& operator -- () { _node=_node->_prev; return *(this); }
      iterator operator -- (int) { 
         iterator tmp = *(this);
         _node=_node->_prev;
         return *(this); 
      }
      iterator& operator = (const iterator& i) { 
         (*this)._node = i._node;
         return *(this); 
      }
      bool operator != (const iterator& i) const {
          return((*this)._node != i._node);
      }
      bool operator == (const iterator& i) const {
         return((*this)._node == i._node); 
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { 
      return iterator(_head->_next);
   }
   iterator end() const { 
      return iterator(_head);
   }
   bool empty() const {
      return begin()==end(); 
   }
   size_t size() const { 
      size_t _size=0;
      for(iterator it=begin();it!=end();++it){
         ++_size;
      } 
      return _size;
   }

   void push_back(const T& x) { 
      DListNode<T>* w = _head->_prev;
      DListNode<T>* v = new DListNode<T>(x, w, _head);
      w->_next = v;
      _head->_prev = v;
   }
   void pop_front() {
      if(!empty()){
         DListNode<T>* v = _head->_next;
         _head->_next = v->_next;
         v->_next->_prev = _head;
         delete v;
      }
   }
   void pop_back() {
      if(!empty()){
         DListNode<T>* v = _head->_prev;
         v->_prev->_next = _head;
         _head->_prev = v->_prev;
         delete v;
      }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
      if(!empty()){
         DListNode<T>* v = pos._node;
         pos._node->_prev->_next = v->_next;
         pos._node->_next->_prev = v->_prev;
         delete v;
         return true;
      }
      return false; 
   }
   bool erase(const T& x) { 
      iterator it = findNode(x);
      if(it!=NULL){
         return erase(it);
      }
      else{
         return false;
      }
   }

   void clear() {
      for(iterator it = begin();it!=end();++it){
         DListNode<T>*tmp = it._node;
         delete tmp;
      }
      _head->_prev=_head->_next=_head;
   }  // delete all nodes except for the dummy node

   void sort() const {
      
      iterator min;
      for(iterator cur = begin(); cur!=end();++cur){
         min = cur;
         for(iterator it =cur; it!=end();++it){
            if(it._node->_data < min._node->_data){
               min = it;
            }
         }
         swap(min._node, cur._node);
      }
   }

private:
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted
   void swap(DListNode<T>* a, DListNode<T>*b)const{
      T data = a->_data;
      a->_data = b->_data;
      b->_data = data; 
   }


   // [OPTIONAL TODO] helper functions; called by public member functions
    iterator findNode(const T& x)const
   {
      for(iterator it = begin();it!=end();++it)
      {
         if(it._node->_data == x)
            return it;
      }
      return NULL;
   }
};

#endif // DLIST_H
