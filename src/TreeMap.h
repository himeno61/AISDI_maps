#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H
 
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <queue>
 
namespace aisdi
{
 
template <typename KeyType, typename ValueType>
class TreeMap
{
public:
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
 
  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
 
private:
  struct Node
  {
    value_type data;
    Node* left;
    Node* right;
    Node* parent;

    Node(key_type key, mapped_type mapped): data(std::make_pair( key, mapped )), left(nullptr), right(nullptr), parent(nullptr)
    {}
 
    Node(value_type in) : Node(in.first,in.second) 
    {}
 
    ~Node() 
    { 
		delete left;
		delete right;  
	}
  };
  
  Node* root;
  size_type count;
 
public:
  TreeMap(): root(nullptr), count(0) 
  {}
 
  TreeMap(std::initializer_list<value_type> list): TreeMap()
  {
    for (auto it = list.begin(); it != list.end(); ++it)
      insert( new Node(*it) );
  }
 
  TreeMap(const TreeMap& other): TreeMap()
  {
    *this = other;
  }
 
  TreeMap(TreeMap&& other): TreeMap()
  {
    *this = std::move(other);
  }
 
  ~TreeMap()
  {
    erase();
    delete root;
  }
 
  TreeMap& operator=(const TreeMap& other)
  {
    if(this != &other) 
    {
      erase();
      delete root;
      
      std::queue<Node*> q;
      Node* temp;
 
      q.push(other.root);
      while(!q.empty()) 
      {
        temp = q.front();
        q.pop();
        if( temp != nullptr ) 
        {
          q.push( temp->left );
          q.push( temp->right );
          insert( new Node(temp->data) );
        }
      }
    }
    return *this;
  }
 
  TreeMap& operator=(TreeMap&& other)
  {
    if(this != &other) 
    {
		erase();
		delete root; 
		std::swap(root,other.root);
		std::swap(count,other.count);      
		other.root = nullptr;
		other.count = 0;
    }
    return *this;
  }
 
  bool isEmpty() const
  {
    return (!count) ;
  }
 
  mapped_type& operator[](const key_type& key)
  {
    Node* current = find_Node(key);
    if( current == nullptr ) 
    {
      current = new Node(key,mapped_type());
      insert( current );
    }
    return current->data.second;
  }
 
  const mapped_type& valueOf(const key_type& key) const 
  {
    const Node* current = find_Node(key);
    if(current != nullptr)
      return current->data.second;
    throw std::out_of_range("no such key - ValueOf");    
  }
 
  mapped_type& valueOf(const key_type& key)
  {
    Node* current = find_Node(key);
    if(current != nullptr)
		return current->data.second;
    throw std::out_of_range("no such key - valueOf");    
  }
 
  const_iterator find(const key_type& key) const
  {
    return const_iterator(this,find_Node(key));
  }
 
  iterator find(const key_type& key)
  {
    return iterator(this,find_Node(key));
  }
 
  void remove(const key_type& key)
  {
    remove(find(key));
  }
 
  void remove(const const_iterator& it)
  {
    if(this != it.tree || it == end())
      throw std::out_of_range ("remove");
    remove(it.node);
  }
 
  size_type getSize() const
  {
    return count;
  }
 
  bool operator==(const TreeMap& other) const
  {
    if( count != other.count )
      return false;
 
    for(auto it = begin(), it2 = other.begin(); it != end(); ++it, ++it2) {
      if(*it != *it2)
        return false;
    }
    return true;
  }
 
  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }
 
  iterator begin()
  {
    return iterator(this, min_TreeMap(root));
  }
 
  iterator end()
  {
    return iterator(this);
  }
 
  const_iterator cbegin() const
  {
    return const_iterator(this, min_TreeMap(root));
  }
 
  const_iterator cend() const
  {
    return const_iterator(this);
  }
 
  const_iterator begin() const
  {
    return cbegin();
  }
 
  const_iterator end() const
  {
    return cend();
  }
void add_element(key_type &key,mapped_type& val)
{
	Node temp(key,val);
	insert(temp);
}
private:
  void erase()
  {
    delete root;
    root = nullptr;
    count = 0;
  }

	
	
  void insert(Node* newNode)
  {
    if (root == nullptr) {
      root = newNode;
    }
    else {
 
      Node* current = root;
      while (true) {
        if (newNode->data.first > current->data.first) {
          if (current->right == nullptr) {
            current->right = newNode;
            newNode->parent = current;
            break;
          }
          else
            current = current->right;
        }
        else if (newNode->data.first < current->data.first) {
          if (current->left == nullptr) {
            current->left = newNode;
            newNode->parent = current;
            break;
          }
          else
            current = current->left;
        }
        else {
          delete newNode;
          return;
        }
      }
    }
    count++;
    return;
  }
 
  void remove(Node* node)
  {
    if (node->left == nullptr) 
    {
      replace(node, node->right);
    }
    else if (node->right == nullptr)
    {
      replace(node, node->left);
    }
    else 
    { 
		auto tmp = node->right;
		while (tmp->left != nullptr)
			tmp = tmp->left;
		replace(tmp, tmp->right);
		replace(node, tmp);
    }
    count--;
    delete node;
  }
 
  void replace(Node* A, Node* B)
  {
    
    if (A->parent == nullptr)       
		root = B;
    else if (A == A->parent->left)  
		A->parent->left = B;
    else                            
		A->parent->right = B;
 
    if (B != nullptr) 
    {     
      B->parent = A->parent;
      if(A->right != nullptr && A->right != B)
        B->right = A->right;
      if(A->left != nullptr && A->left != B)
        B->left = A->left;
    }
    A->parent = nullptr;
    A->left   = nullptr;
    A->right  = nullptr;
  }
 
  Node* find_Node(const key_type& key) const
  {
    Node* node = root;
    while ( node != nullptr ) {
      if (key > node->data.first) {
        node = node->right;
      }
      else if (key < node->data.first) {
        node = node->left;
      }
      else
        break;
    }
    return node;
  }
 
  Node* min_TreeMap(Node* node) const
  {
    if(node != nullptr)
      while(node->left != nullptr)
        node = node->left;
    return node;
  }
 
};
 
template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
 
private:
  const TreeMap *tree;
  Node *node;
  friend void TreeMap<KeyType, ValueType>::remove(const const_iterator&);
 
public:
  explicit ConstIterator(const TreeMap *tree = nullptr, Node *node = nullptr) : tree(tree), node(node) {}
 
  ConstIterator(const ConstIterator& other) : ConstIterator(other.tree, other.node) {}
 
  ConstIterator& operator++()
  {
    if(tree == nullptr || node == nullptr) {
      throw std::out_of_range("operator++");
    }
    else if( node->right != nullptr ) {
        node = node->right;
        while(node->left != nullptr)
          node = node->left;
    }
    else {
        while(true) {
          if(node->parent == nullptr) {
            node = nullptr;
            break;
          }
          if(node->parent->left == node) {
            node = node->parent;
            break;
          }
          node = node->parent;
        }
    }
 
    return *this;
  }
 
  ConstIterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }
 
  ConstIterator& operator--()
  {
    if(tree == nullptr || tree->root == nullptr) {
      throw std::out_of_range("operator--");
    }
    else if(node == nullptr) {
        node = tree->root;
        while(node->right != nullptr)
          node = node->right;
    }
    else if(node->left != nullptr ) {
        node = node->left;
        while(node->right != nullptr)
          node = node->right;
    }
    else {
        while(true) {
          if(node->parent == nullptr) {
            throw std::out_of_range("operator--");
          }
          if(node->parent->right == node) {
            node = node->parent;
            break;
          }
          node = node->parent;
        }
    }
 
    return *this;
  }
 
  ConstIterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }
 
  reference operator*() const
  {
    if(tree == nullptr || node == nullptr)
      throw std::out_of_range("operator*");
    return node->data;
  }
 
  pointer operator->() const
  {
    return &this->operator*();
  }
 
  bool operator==(const ConstIterator& other) const
  {
    return tree == other.tree && node == other.node;
  }
 
  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};
 
template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;
 
  explicit Iterator(TreeMap *tree = nullptr, Node *node = nullptr) : ConstIterator(tree, node) {}
 
  Iterator(const ConstIterator& other) : ConstIterator(other) {}
 
  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }
 
  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }
 
  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }
 
  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }
 
  pointer operator->() const
  {
    return &this->operator*();
  }
 
  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};
 
}
 
#endif /* AISDI_MAPS_MAP_H */
