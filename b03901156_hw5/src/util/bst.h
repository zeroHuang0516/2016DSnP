/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//

template <class T>
class BSTreeNode
{
   friend class BSTree<T>;
   friend class BSTree<T>::iterator;
   	
   	BSTreeNode(const T&data, BSTreeNode<T>*parent=NULL, BSTreeNode<T>*left=NULL, BSTreeNode<T>* right=NULL){
   		this->_data = data;
   		this->left =left;
   		this->right=right;
   		this->parent=parent;
   	}
   	
   	BSTreeNode<T>* left;
   	BSTreeNode<T>* right;
   	BSTreeNode<T>* parent;
   	T _data;
};


template <class T>
class BSTree
{
   // TODO: design your own class!!
public:
	BSTree(){
		_size=0;
		root = new BSTreeNode<T>(T(), NULL);
	}
	~BSTree(){
		clear();
    delete root;
	}




	 class iterator
   {
      friend class BSTree;

   public:
      iterator(BSTreeNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return _node->_data; }
      T& operator * () { return _node->_data; }
      iterator& operator ++ () { _node=successor(_node); return *(this); }
      iterator operator ++ (int) { 
         iterator tmp = iterator(*this);
         _node=successor(_node);
         return tmp; 
      }
      iterator& operator -- () { _node=ancestor(_node); return *(this); }
      iterator operator -- (int) { 
         iterator tmp = iterator(*this);
         _node=ancestor(_node);
         return tmp; 
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
     	BSTreeNode<T>* _node;
     	BSTreeNode<T>* MINnode(BSTreeNode<T>* node)const{
        while(node->left!=NULL){
          node = node->left;
        }
        return node;
      }
      BSTreeNode<T>* MAXnode(BSTreeNode<T>* node)const{
        while(node->right!=NULL){
          node = node->right;
        }
        return node;
      }
     	BSTreeNode<T>* successor(BSTreeNode<T>* node)const{
     		if(node->right!=NULL){
     			return MINnode(node->right);
     		}
     		else{
     			BSTreeNode<T>* tmp=node->parent;
     			while(tmp != NULL && node == tmp->right){
     				node = tmp;
     				tmp = tmp->parent;
     			}
     			return tmp;
     		}
     	}
     	BSTreeNode<T>* ancestor(BSTreeNode<T>* node)const{
     		if(node->left!=NULL){
     			return MAXnode(node->left);
     		}
     		else{
     			if(node==node->parent->right){
     				return node->parent;
     			}
     			else{
     				BSTreeNode<T>* tmp = node->parent;
     				while(tmp != NULL && node==tmp->left){
     					node = tmp;
     					tmp = tmp->parent;
     				}
     				return tmp;
     			}
     			
     		}
     	}
   };


   iterator begin() const { 
      return iterator(MINnode(this->root));
   }
   iterator end() const { 
      return iterator((this->root));
   }
   bool empty() const {
      return (_size ==0);
   }
   size_t size() const { 
      return _size;
   }

   void insert(const T& x){
   		if(empty()){
   			root->left = new BSTreeNode<T>(x, root);
   		}
   		else{
   			addNode(root->left, x);
   		}
   		++_size;
   }
   void pop_front() {
      if(!empty()){
        erase(begin());
      }
   }
   void pop_back() {
    iterator it = --end();
      if(!empty()){
      	erase(it);
      }
   }

   // return false if nothing to erase

   bool erase(iterator pos) { 
    //cout<<"### "<<_size<<endl;
      if(!empty()){
         BSTreeNode<T>* node = pos._node;
         int degreeOfCurNode = degreeOf(node);
         if(degreeOfCurNode == 0){ 				//the deleting node have no child
         		if(node==node->parent->left){	//is the left child
         			node->parent->left = NULL;
         		}
         		else{							//is the right child
         			node->parent->right = NULL;
         		}
         		--_size;
         		delete node;
         		return true;
         }
         else if(degreeOfCurNode == 1){				//the deleting node have one child
         		if(node->left){						//has left child
         			transplant(node, node->left);
         		}
         		else{								//has right child
         			transplant(node, node->right);
         		}
         		--_size;
         		delete node;
         		return true;
         }
         else if(degreeOfCurNode == 2){
         		BSTreeNode<T>* tmp = MINnode(node->right);
         		if(tmp->parent!=node){
         			transplant(tmp, tmp->right);
         			tmp->right = node->right;
         			tmp->right->parent = tmp;
         		}
         		transplant(node, tmp);
         		tmp->left = node->left;
         		tmp->left->parent = tmp;
            --_size;
            return true;
         }
        
      }
      return false; 
      
   }
   bool erase(const T& x) { 
      if(!empty()){
      	iterator it= Find(x, this->root->left);
      	if(it!=NULL){
      		return erase(it);
      	}
      	else{
      		return false;
      	}
      }
        return false; 
      
   }

   
   void clear() {
      if(!empty()){
      	 deleteTree(root->left);
      }
      _size=0;
      root->left=NULL;
      root->right=NULL;
   }  // delete all nodes except for the dummy node

   void sort() const {}
   void print() const{}


private:
	BSTreeNode<T> *root;
	size_t _size;

	int degreeOf(BSTreeNode<T>* node)const{
		if(!node->left && !node->right){
			return 0;
		}
		else if(node->left && node->right){
			return 2;
		}
		else{
			return 1;
		}
	}


	void deleteTree(BSTreeNode<T>* node){
		if(node->left){
			deleteTree(node->left);
		}
		if(node->right){
			deleteTree(node->right);
		}
		delete node;
	}
	void addNode(BSTreeNode<T> *root,const T& data){
		if(data < (root->_data) ){
			if(!root->left){
				root->left = new BSTreeNode<T>(data,root);
			}
			else{
				addNode(root->left, data);
			}
		}
		else{
			if(!root->right){
				root->right = new BSTreeNode<T>(data,root);
			}
			else{
				addNode(root->right, data);
			}
		}
	}
	iterator Find(const T& x, BSTreeNode<T>* cur)const{
   		if(x==cur->_data){
   			return iterator(cur);
   		}
   		else if(x<cur->_data){
   			if(cur->left){
   				return Find(x,cur->left);
   			}
   			else{
   				return NULL;
   			}
   		}
   		else{
   			if(cur->right){
   				return Find(x, cur->right);
   			}
   			else{
   				return NULL;
   			}
   		}
   }
    void transplant(BSTreeNode<T>*u,BSTreeNode<T>*v){
   		if(u==u->parent->left){
   			u->parent->left=v;
   		}
   		else{
   			u->parent->right=v;
   		}
   		if(v){
   			v->parent=u->parent;
   		}
   }

   BSTreeNode<T>* MINnode(BSTreeNode<T>* node)const{
        while(node->left!=NULL){
          node = node->left;
        }
        return node;
      }
   BSTreeNode<T>* MAXnode(BSTreeNode<T>* node)const{
        while(node->right!=NULL){
          node = node->right;
        }
        return node;
    }

};

#endif // BST_H
