// xtree internal header
#pragma once
#ifndef _XTREE_
#define _XTREE_
#ifndef RC_INVOKED
#include <functional>
#include <memory>
#include <stdexcept>

#ifdef _MSC_VER
 #pragma pack(push,_CRT_PACKING)
 #pragma warning(push,3)
 #pragma warning(disable:4127)
#endif  /* _MSC_VER */

_STD_BEGIN

		// TEMPLATE CLASS _Tree_nod
template<class _Traits>
	class _Tree_nod
		: public _Traits	// traits form ultimate base
	{	// base class for _Tree_ptr to hold allocator _Alnod
protected:
	struct _Node;
	friend struct _Node;
	typedef _Node *_Nodeptr;	// _Node allocator must have ordinary pointers

	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename _Traits::value_type value_type;

	struct _Node
		{	// tree node
		_Node(_Nodeptr _Larg, _Nodeptr _Parg, _Nodeptr _Rarg,
			const value_type& _Val, char _Carg)
			: _Left(_Larg), _Parent(_Parg), _Right(_Rarg),
				_Myval(_Val), _Color(_Carg), _Isnil(false)
			{	// construct a node with value
			}

		_Nodeptr _Left;	// left subtree, or smallest element if head
		_Nodeptr _Parent;	// parent, or root of tree if head
		_Nodeptr _Right;	// right subtree, or largest element if head
		value_type _Myval;	// the stored value, unused if head
		char _Color;	// _Red or _Black, _Black if head
		char _Isnil;	// true only if head (also nil) node
		};

	_Tree_nod(const key_compare& _Parg,
		allocator_type _Al)
		: _Traits(_Parg, _Al), _Alnod(_Al)
		{	// construct traits from _Parg and allocator from _Al
		}

	typename allocator_type::template rebind<_Node>::other
		_Alnod;	// allocator object for nodes
	};

		// TEMPLATE CLASS _Tree_ptr
template<class _Traits>
	class _Tree_ptr
		: public _Tree_nod<_Traits>
	{	// base class for _Tree_val to hold allocator _Alptr
 #if _HAS_ITERATOR_DEBUGGING
public:
 #else /* _HAS_ITERATOR_DEBUGGING */
protected:
 #endif /* _HAS_ITERATOR_DEBUGGING */
	typedef _Tree_nod<_Traits> _Mybase;
	typedef typename _Mybase::_Node _Node;
	typedef typename _Mybase::_Nodeptr _Nodeptr;
	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;

	_Tree_ptr(const key_compare& _Parg,
		allocator_type _Al)
		: _Tree_nod<_Traits>(_Parg, _Al), _Alptr(_Al)
		{	// construct base, and allocator from _Al
		}

	typename allocator_type::template rebind<_Nodeptr>::other
		_Alptr;	// allocator object for pointers to nodes
	};

		// TEMPLATE CLASS _Tree_val
template<class _Traits>
	class _Tree_val
		: public _Tree_ptr<_Traits>
	{	// base class for _Tree to hold allocator _Alval
protected:
	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;

	_Tree_val(const key_compare& _Parg,
		allocator_type _Al)
		: _Tree_ptr<_Traits>(_Parg, _Al), _Alval(_Al)
		{	// construct base, and allocator from _Al
		}

	allocator_type _Alval;	// allocator object for values stored in nodes
	};

		// TEMPLATE CLASS _Tree
template<class _Traits>
	class _Tree
		: public _Tree_val<_Traits>
	{	// ordered red-black tree for [multi_]{map set}
public:
	typedef _Tree<_Traits> _Myt;
	typedef _Tree_val<_Traits> _Mybase;
	typedef typename _Traits::key_type key_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename _Traits::value_compare value_compare;
	typedef typename _Traits::value_type value_type;
	typedef typename _Traits::allocator_type allocator_type;

 #if _HAS_IMMUTABLE_SETS
	typedef typename _Traits::_ITptr _ITptr;
	typedef typename _Traits::_IReft _IReft;

 #else /* _HAS_IMMUTABLE_SETS */
	typedef typename allocator_type::pointer _ITptr;
	typedef typename allocator_type::reference _IReft;
 #endif /* _HAS_IMMUTABLE_SETS */

protected:

	typedef typename _Mybase::_Node _Node;
	typedef typename _Mybase::_Nodeptr _Nodeptr;

	typedef typename allocator_type::template rebind<_Nodeptr>::other
		_Nodeptr_alloc;
	typedef typename _Nodeptr_alloc::reference _Nodepref;

	typedef typename allocator_type::template rebind<key_type>::other
		_Key_alloc;
	typedef typename _Key_alloc::const_reference _Keyref;

	typedef typename allocator_type::template rebind<char>::other
		_Char_alloc;
	typedef typename _Char_alloc::reference _Charref;


	typedef typename allocator_type::reference _Vref;

	enum _Redbl
		{	// colors for link to parent
		_Red, _Black};

	static _Charref _Color(_Nodeptr _Pnode)
		{	// return reference to color in node
		return ((_Charref)(*_Pnode)._Color);
		}

	static _Charref _Isnil(_Nodeptr _Pnode)
		{	// return reference to nil flag in node
		return ((_Charref)(*_Pnode)._Isnil);
		}

	static _Keyref _Key(_Nodeptr _Pnode)
		{	// return reference to key in node
		return (_Mybase::_Kfn(_Myval(_Pnode)));
		}

	static _Nodepref _Left(_Nodeptr _Pnode)
		{	// return reference to left pointer in node
		return ((_Nodepref)(*_Pnode)._Left);
		}

	static _Nodepref _Parent(_Nodeptr _Pnode)
		{	// return reference to parent pointer in node
		return ((_Nodepref)(*_Pnode)._Parent);
		}

	static _Nodepref _Right(_Nodeptr _Pnode)
		{	// return reference to right pointer in node
		return ((_Nodepref)(*_Pnode)._Right);
		}

	static _Vref _Myval(_Nodeptr _Pnode)
		{	// return reference to value in node
		return ((_Vref)(*_Pnode)._Myval);
		}

public:
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type _Dift;
	typedef _Dift difference_type;
	typedef typename allocator_type::pointer _Tptr;
	typedef typename allocator_type::const_pointer _Ctptr;
	typedef typename allocator_type::reference _Reft;
	typedef _Tptr pointer;
	typedef _Ctptr const_pointer;
	typedef _Reft reference;
	typedef typename allocator_type::const_reference const_reference;

		// CLASS const_iterator
	class const_iterator;
	friend class const_iterator;

	class const_iterator
		: public _Bidit<value_type, _Dift, _Ctptr, const_reference>
	{	// iterator for nonmutable _Tree
	public:
		friend class _Tree<_Traits>;
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Dift difference_type;
		typedef _Ctptr pointer;
		typedef const_reference reference;

#if _SECURE_SCL
		typedef _Range_checked_iterator_tag _Checked_iterator_category;
#endif

		const_iterator()
			: _Ptr(0)
			{	// construct with null node pointer
			}

 #if _HAS_ITERATOR_DEBUGGING
 #define _TREE_CONST_ITERATOR(ppnode)	const_iterator(ppnode, this)

		const_iterator(_Nodeptr _Pnode, const _Myt *_Plist=NULL)
			: _Ptr(_Pnode)
			{	// construct with node pointer _Pnode
			this->_Adopt(_Plist);
			}

 #elif _SECURE_SCL
 #define _TREE_CONST_ITERATOR(ppnode)	const_iterator(ppnode, this)

		const_iterator(_Nodeptr _Pnode, const _Myt *_Plist)
			: _Ptr(_Pnode)
			{	// construct with node pointer _Pnode
			_SCL_SECURE_VALIDATE(_Plist != NULL);
			this->_Set_container(_Plist);
			}

 #else
 #define _TREE_CONST_ITERATOR(ppnode)	const_iterator(ppnode)

		explicit const_iterator(_Nodeptr _Pnode)
			: _Ptr(_Pnode)
			{	// construct with node pointer _Pnode
			}
 #endif /* _HAS_ITERATOR_DEBUGGING */

		const_reference operator*() const
			{	// return designated value

 #if _HAS_ITERATOR_DEBUGGING
			if (this->_Mycont == 0
				|| _Ptr == 0
				|| _Ptr == ((_Myt *)this->_Mycont)->_Myhead)
				{
				_DEBUG_ERROR("map/set iterator not dereferencable");
				_SCL_SECURE_OUT_OF_RANGE;
				}
 #else
			_SCL_SECURE_VALIDATE(this->_Has_container());
			_SCL_SECURE_VALIDATE_RANGE(_Ptr != ((_Myt *)(this->_Getmycont()))->_Myhead);
 #endif /* _HAS_ITERATOR_DEBUGGING */

			return (_Myval(_Ptr));
			}

		_Ctptr operator->() const
			{	// return pointer to class object
			return (&**this);
			}

		const_iterator& operator++()
			{	// preincrement
			_Inc();
			return (*this);
			}

		const_iterator operator++(int)
			{	// postincrement
			const_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
			}

		const_iterator& operator--()
			{	// predecrement
			_Dec();
			return (*this);
			}

		const_iterator operator--(int)
			{	// postdecrement
			const_iterator _Tmp = *this;
			--*this;
			return (_Tmp);
			}

		bool operator==(const const_iterator& _Right) const
			{	// test for iterator equality

 #if _HAS_ITERATOR_DEBUGGING
			if (this->_Mycont == 0 || this->_Mycont != _Right._Mycont)
				{
				_DEBUG_ERROR("map/set iterators incompatible");
				_SCL_SECURE_INVALID_ARGUMENT;
				}
 #else
			_SCL_SECURE_VALIDATE(this->_Has_container() && this->_Same_container(_Right));
 #endif /* _HAS_ITERATOR_DEBUGGING */

			return (_Ptr == _Right._Ptr);
			}

		bool operator!=(const const_iterator& _Right) const
			{	// test for iterator inequality
			return (!(*this == _Right));
			}

		void _Dec()
			{	// move to node with next smaller value

 #if _HAS_ITERATOR_DEBUGGING
			if (this->_Mycont == 0
				|| _Ptr == 0)
				{
				_DEBUG_ERROR("map/set iterator not decrementable");
				_SCL_SECURE_INVALID_ARGUMENT;
				}
 #else
			_SCL_SECURE_VALIDATE(this->_Has_container());
 #endif /* _HAS_ITERATOR_DEBUGGING */

			if (_Isnil(_Ptr))
			{
				_Ptr = _Right(_Ptr);	// end() ==> rightmost
   				if (_Isnil(_Ptr))
#if _HAS_ITERATOR_DEBUGGING
				{
					_DEBUG_ERROR("map/set iterator not decrementable");
					_SCL_SECURE_OUT_OF_RANGE;
				}
#elif _SECURE_SCL
				{
    				_SCL_SECURE_OUT_OF_RANGE;
				}
#else
				return;	// begin() shouldn't be incremented, don't move
#endif
			}
			else if (!_Isnil(_Left(_Ptr)))
				_Ptr = _Max(_Left(_Ptr));	// ==> largest of left subtree
			else
				{	// climb looking for left subtree
				_Nodeptr _Pnode;
				while (!_Isnil(_Pnode = _Parent(_Ptr))
					&& _Ptr == _Left(_Pnode))
					_Ptr = _Pnode;	// ==> parent while left subtree
				if (_Isnil(_Ptr))
 #if _HAS_ITERATOR_DEBUGGING
 					{
					_DEBUG_ERROR("map/set iterator not decrementable");
					_SCL_SECURE_OUT_OF_RANGE;
					}
 #elif _SECURE_SCL
					{
					_SCL_SECURE_OUT_OF_RANGE;
					}
 #else
					return;	// begin() shouldn't be incremented, don't move
 #endif
				else
					_Ptr = _Pnode;	// ==> parent if not head
				}
			}

		void _Inc()
			{	// move to node with next larger value

 #if _HAS_ITERATOR_DEBUGGING
			if (this->_Mycont == 0
				|| _Ptr == 0
				|| _Isnil(_Ptr))
				{
				_DEBUG_ERROR("map/set iterator not incrementable");
				_SCL_SECURE_OUT_OF_RANGE;
				}
 #else
			_SCL_SECURE_VALIDATE(this->_Has_container());
			if (_Isnil(_Ptr))
				{
				_SCL_SECURE_OUT_OF_RANGE;
				// end() shouldn't be incremented, don't move if _SCL_SECURE is not turned on
				}
 #endif /* _HAS_ITERATOR_DEBUGGING */

			else if (!_Isnil(_Right(_Ptr)))
				_Ptr = _Min(_Right(_Ptr));	// ==> smallest of right subtree
			else
				{	// climb looking for right subtree
				_Nodeptr _Pnode;
				while (!_Isnil(_Pnode = _Parent(_Ptr))
					&& _Ptr == _Right(_Pnode))
					_Ptr = _Pnode;	// ==> parent while right subtree
				_Ptr = _Pnode;	// ==> parent (head if end())
				}
			}

		_Nodeptr _Mynode() const
			{	// return node pointer
			return (_Ptr);
			}

 #if _HAS_ITERATOR_DEBUGGING
    public:
 #else /* _HAS_ITERATOR_DEBUGGING */
	protected:
 #endif /* _HAS_ITERATOR_DEBUGGING */
		_Nodeptr _Ptr;	// pointer to node
		};

		// CLASS iterator
	class iterator;
	friend class iterator;

	class iterator
		: public const_iterator
	{	// iterator for mutable _Tree
	public:
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Dift difference_type;
		typedef _ITptr pointer;
		typedef _IReft reference;

		iterator()
			{	// construct with null node pointer
			}

 #if _HAS_ITERATOR_DEBUGGING
 #define _TREE_ITERATOR(ppnode)	iterator(ppnode, this)

		iterator(_Nodeptr _Pnode, const _Myt *_Plist=NULL)
			: const_iterator(_Pnode, _Plist)
			{	// construct with node pointer _Pnode
			}

 #elif _SECURE_SCL
 #define _TREE_ITERATOR(ppnode)	iterator(ppnode, this)

		iterator(_Nodeptr _Pnode, const _Myt *_Plist)
			: const_iterator(_Pnode, _Plist)
			{	// construct with node pointer _Pnode
			}

 #else /* _HAS_ITERATOR_DEBUGGING */
 #define _TREE_ITERATOR(ppnode)	iterator(ppnode)

		explicit iterator(_Nodeptr _Pnode)
			: const_iterator(_Pnode)
			{	// construct with node pointer _Pnode
			}
 #endif /* _HAS_ITERATOR_DEBUGGING */

		reference operator*() const
			{	// return designated value
			return ((reference)**(const_iterator *)this);
			}

		pointer operator->() const
			{	// return pointer to class object
			return (&**this);
			}

		iterator& operator++()
			{	// preincrement
			++(*(const_iterator *)this);
			return (*this);
			}

		iterator operator++(int)
			{	// postincrement
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
			}

		iterator& operator--()
			{	// predecrement
			--(*(const_iterator *)this);
			return (*this);
			}

		iterator operator--(int)
			{	// postdecrement
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
			}
		};

	typedef std::reverse_iterator<iterator> reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	typedef pair<iterator, bool> _Pairib;
	typedef pair<iterator, iterator> _Pairii;
	typedef pair<const_iterator, const_iterator> _Paircc;

	explicit _Tree(const key_compare& _Parg,
		const allocator_type& _Al)
		: _Mybase(_Parg, _Al)
		{	// construct empty tree
		_Init();
		}

	_Tree(const value_type *_First, const value_type *_Last,
		const key_compare& _Parg, const allocator_type& _Al)
		: _Mybase(_Parg, _Al)
		{	// construct tree from [_First, _Last) array
		_Init();
		_TRY_BEGIN
		insert(_First, _Last);
		_CATCH_ALL
		_Tidy();
		_RERAISE;
		_CATCH_END
		}

	_Tree(const _Myt& _Right)
		: _Mybase(_Right.key_comp(), _Right.get_allocator())
		{	// construct tree by copying _Right
		_Init();
		_TRY_BEGIN
		_Copy(_Right);
		_CATCH_ALL
		_Tidy();
		_RERAISE;
		_CATCH_END
		}

	~_Tree()
		{	// destroy tree
		_Tidy();
		}

	_Myt& operator=(const _Myt& _Right)
		{	// replace contents from _Right
		if (this != &_Right)
			{	// worth doing
			erase(begin(), end());
			this->comp = _Right.comp;
			_Copy(_Right);
			}
		return (*this);
		}

	iterator begin()
		{	// return iterator for beginning of mutable sequence
		return (_TREE_ITERATOR(_Lmost()));
		}

	const_iterator begin() const
		{	// return iterator for beginning of nonmutable sequence
		return (_TREE_CONST_ITERATOR(_Lmost()));
		}

	iterator end()
		{	// return iterator for end of mutable sequence
		return (_TREE_ITERATOR(_Myhead));
		}

	const_iterator end() const
		{	// return iterator for end of nonmutable sequence
		return (_TREE_CONST_ITERATOR(_Myhead));
		}

	iterator _Make_iter(const_iterator _Where) const
		{	// make iterator from const_iterator
		return (iterator(_TREE_ITERATOR(_Where._Ptr)));
		}

	reverse_iterator rbegin()
		{	// return iterator for beginning of reversed mutable sequence
		return (reverse_iterator(end()));
		}

	const_reverse_iterator rbegin() const
		{	// return iterator for beginning of reversed nonmutable sequence
		return (const_reverse_iterator(end()));
		}

	reverse_iterator rend()
		{	// return iterator for end of reversed mutable sequence
		return (reverse_iterator(begin()));
		}

	const_reverse_iterator rend() const
		{	// return iterator for end of reversed nonmutable sequence
		return (const_reverse_iterator(begin()));
		}

	size_type size() const
		{	// return length of sequence
		return (_Mysize);
		}

	size_type max_size() const
		{	// return maximum possible length of sequence
		return (this->_Alval.max_size());
		}

	bool empty() const
		{	// return true only if sequence is empty
		return (size() == 0);
		}

	allocator_type get_allocator() const
		{	// return allocator object for values
		return (this->_Alval);
		}

	key_compare key_comp() const
		{	// return object for comparing keys
		return (this->comp);
		}

	value_compare value_comp() const
		{	// return object for comparing values
		return (value_compare(key_comp()));
		}

	_Pairib insert(const value_type& _Val)
		{	// try to insert node with value _Val
		_Nodeptr _Trynode = _Root();
		_Nodeptr _Wherenode = _Myhead;
		bool _Addleft = true;	// add to left of head if tree empty
		while (!_Isnil(_Trynode))
			{	// look for leaf to insert before (_Addleft) or after
			_Wherenode = _Trynode;
			_Addleft = _DEBUG_LT_PRED(this->comp,
				this->_Kfn(_Val), _Key(_Trynode));
			_Trynode = _Addleft ? _Left(_Trynode) : _Right(_Trynode);
			}

		if (this->_Multi)
			return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
		else
			{	// insert only if unique
			iterator _Where = _TREE_ITERATOR(_Wherenode);
			if (!_Addleft)
				;	// need to test if insert after is okay
			else if (_Where == begin())
				return (_Pairib(_Insert(true, _Wherenode, _Val), true));
			else
				--_Where;	// need to test if insert before is okay

			if (_DEBUG_LT_PRED(this->comp,
				_Key(_Where._Mynode()), this->_Kfn(_Val)))
				return (_Pairib(_Insert(_Addleft, _Wherenode, _Val), true));
			else
				return (_Pairib(_Where, false));
			}
		}

	iterator insert(const_iterator _Where,
		const value_type& _Val)
		{	// try to insert node with value _Val using _Where as a hint

 #if _HAS_ITERATOR_DEBUGGING
		if (_Where._Mycont != this)
			_DEBUG_ERROR("map/set insert iterator outside range");
 #endif /* _HAS_ITERATOR_DEBUGGING */

		const_iterator _Next;

		if (size() == 0)
			return (_Insert(true, _Myhead, _Val));	// insert into empty tree
		else if (this->_Multi)
			{	// insert even if duplicate
			if (_Where == begin())
				{	// insert at beginning if before first element
				if (!_DEBUG_LT_PRED(this->comp,
					_Key(_Where._Mynode()), this->_Kfn(_Val)))
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (_Where == end())
				{	// insert at end if after last element
				if (!_DEBUG_LT_PRED(this->comp,
					this->_Kfn(_Val), _Key(_Rmost())))
					return (_Insert(false, _Rmost(), _Val));
				}
			else if (!_DEBUG_LT_PRED(this->comp,
				_Key(_Where._Mynode()), this->_Kfn(_Val))
				&& !_DEBUG_LT_PRED(this->comp,
					this->_Kfn(_Val), _Key((--(_Next = _Where))._Mynode())))
				{	// insert before _Where
				if (_Isnil(_Right(_Next._Mynode())))
					return (_Insert(false, _Next._Mynode(), _Val));
				else
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (!_DEBUG_LT_PRED(this->comp,
				this->_Kfn(_Val), _Key(_Where._Mynode()))
				&& (++(_Next = _Where) == end()
					|| !_DEBUG_LT_PRED(this->comp,
						_Key(_Next._Mynode()), this->_Kfn(_Val))))
				{	// insert after _Where
				if (_Isnil(_Right(_Where._Mynode())))
					return (_Insert(false, _Where._Mynode(), _Val));
				else
					return (_Insert(true, _Next._Mynode(), _Val));
				}
			}
		else
			{	// insert only if unique
			if (_Where == begin())
				{	// insert at beginning if before first element
				if (_DEBUG_LT_PRED(this->comp,
					this->_Kfn(_Val), _Key(_Where._Mynode())))
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (_Where == end())
				{	// insert at end if after last element
				if (_DEBUG_LT_PRED(this->comp,
					_Key(_Rmost()), this->_Kfn(_Val)))
					return (_Insert(false, _Rmost(), _Val));
				}
			else if (_DEBUG_LT_PRED(this->comp,
				this->_Kfn(_Val), _Key(_Where._Mynode()))
				&& _DEBUG_LT_PRED(this->comp,
					_Key((--(_Next = _Where))._Mynode()), this->_Kfn(_Val)))
				{	// insert before _Where
				if (_Isnil(_Right(_Next._Mynode())))
					return (_Insert(false, _Next._Mynode(), _Val));
				else
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (_DEBUG_LT_PRED(this->comp,
				_Key(_Where._Mynode()), this->_Kfn(_Val))
				&& (++(_Next = _Where) == end()
					|| _DEBUG_LT_PRED(this->comp,
						this->_Kfn(_Val), _Key(_Next._Mynode()))))
				{	// insert after _Where
				if (_Isnil(_Right(_Where._Mynode())))
					return (_Insert(false, _Where._Mynode(), _Val));
				else
					return (_Insert(true, _Next._Mynode(), _Val));
				}
			}

		return (insert(_Val).first);	// try usual insert if all else fails
		}

	template<class _Iter>
		void insert(_Iter _First, _Iter _Last)
		{	// insert [_First, _Last) one at a time

 #if _HAS_ITERATOR_DEBUGGING
		_DEBUG_RANGE(_First, _Last);
 #endif /* _HAS_ITERATOR_DEBUGGING */

		for (; _First != _Last; ++_First)
			insert(*_First);
		}

	iterator erase(const_iterator _Where)
		{	// erase element at _Where

 #if _HAS_ITERATOR_DEBUGGING
		if (_Where._Mycont != this || _Isnil(_Where._Mynode()))
			_DEBUG_ERROR("map/set erase iterator outside range");
		_Nodeptr _Erasednode = _Where._Mynode();	// node to erase
		++_Where;	// save successor iterator for return
		_Orphan_ptr(*this, _Erasednode);

 #else /* _HAS_ITERATOR_DEBUGGING */
		if (_Isnil(_Where._Mynode()))
			_THROW(out_of_range, "invalid map/set<T> iterator");
		_Nodeptr _Erasednode = _Where._Mynode();	// node to erase
		++_Where;	// save successor iterator for return
 #endif /* _HAS_ITERATOR_DEBUGGING */

		_Nodeptr _Fixnode;	// the node to recolor as needed
		_Nodeptr _Fixnodeparent;	// parent of _Fixnode (which may be nil)
		_Nodeptr _Pnode = _Erasednode;

		if (_Isnil(_Left(_Pnode)))
			_Fixnode = _Right(_Pnode);	// must stitch up right subtree
		else if (_Isnil(_Right(_Pnode)))
			_Fixnode = _Left(_Pnode);	// must stitch up left subtree
		else
			{	// two subtrees, must lift successor node to replace erased
			_Pnode = _Where._Mynode();	// _Pnode is successor node
			_Fixnode = _Right(_Pnode);	// _Fixnode is its only subtree
			}

		if (_Pnode == _Erasednode)
			{	// at most one subtree, relink it
			_Fixnodeparent = _Parent(_Erasednode);
			if (!_Isnil(_Fixnode))
				_Parent(_Fixnode) = _Fixnodeparent;	// link up

			if (_Root() == _Erasednode)
				_Root() = _Fixnode;	// link down from root
			else if (_Left(_Fixnodeparent) == _Erasednode)
				_Left(_Fixnodeparent) = _Fixnode;	// link down to left
			else
				_Right(_Fixnodeparent) = _Fixnode;	// link down to right

			if (_Lmost() == _Erasednode)
				_Lmost() = _Isnil(_Fixnode)
					? _Fixnodeparent	// smallest is parent of erased node
					: _Min(_Fixnode);	// smallest in relinked subtree

			if (_Rmost() == _Erasednode)
				_Rmost() = _Isnil(_Fixnode)
					? _Fixnodeparent	// largest is parent of erased node
					: _Max(_Fixnode);	// largest in relinked subtree
			}
		else
			{	// erased has two subtrees, _Pnode is successor to erased
			_Parent(_Left(_Erasednode)) = _Pnode;	// link left up
			_Left(_Pnode) = _Left(_Erasednode);	// link successor down

			if (_Pnode == _Right(_Erasednode))
				_Fixnodeparent = _Pnode;	// successor is next to erased
			else
				{	// successor further down, link in place of erased
				_Fixnodeparent = _Parent(_Pnode);	// parent is successor's
				if (!_Isnil(_Fixnode))
					_Parent(_Fixnode) = _Fixnodeparent;	// link fix up
				_Left(_Fixnodeparent) = _Fixnode;	// link fix down
				_Right(_Pnode) = _Right(_Erasednode);	// link successor down
				_Parent(_Right(_Erasednode)) = _Pnode;	// link right up
				}

			if (_Root() == _Erasednode)
				_Root() = _Pnode;	// link down from root
			else if (_Left(_Parent(_Erasednode)) == _Erasednode)
				_Left(_Parent(_Erasednode)) = _Pnode;	// link down to left
			else
				_Right(_Parent(_Erasednode)) = _Pnode;	// link down to right

			_Parent(_Pnode) = _Parent(_Erasednode);	// link successor up
			std::swap(_Color(_Pnode), _Color(_Erasednode));	// recolor it
			}

		if (_Color(_Erasednode) == _Black)
			{	// erasing black link, must recolor/rebalance tree
			for (; _Fixnode != _Root() && _Color(_Fixnode) == _Black;
				_Fixnodeparent = _Parent(_Fixnode))
				if (_Fixnode == _Left(_Fixnodeparent))
					{	// fixup left subtree
					_Pnode = _Right(_Fixnodeparent);
					if (_Color(_Pnode) == _Red)
						{	// rotate red up from right subtree
						_Color(_Pnode) = _Black;
						_Color(_Fixnodeparent) = _Red;
						_Lrotate(_Fixnodeparent);
						_Pnode = _Right(_Fixnodeparent);
						}

					if (_Isnil(_Pnode))
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Color(_Left(_Pnode)) == _Black
						&& _Color(_Right(_Pnode)) == _Black)
						{	// redden right subtree with black children
						_Color(_Pnode) = _Red;
						_Fixnode = _Fixnodeparent;
						}
					else
						{	// must rearrange right subtree
						if (_Color(_Right(_Pnode)) == _Black)
							{	// rotate red up from left sub-subtree
							_Color(_Left(_Pnode)) = _Black;
							_Color(_Pnode) = _Red;
							_Rrotate(_Pnode);
							_Pnode = _Right(_Fixnodeparent);
							}

						_Color(_Pnode) = _Color(_Fixnodeparent);
						_Color(_Fixnodeparent) = _Black;
						_Color(_Right(_Pnode)) = _Black;
						_Lrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
						}
					}
				else
					{	// fixup right subtree
					_Pnode = _Left(_Fixnodeparent);
					if (_Color(_Pnode) == _Red)
						{	// rotate red up from left subtree
						_Color(_Pnode) = _Black;
						_Color(_Fixnodeparent) = _Red;
						_Rrotate(_Fixnodeparent);
						_Pnode = _Left(_Fixnodeparent);
						}
					if (_Isnil(_Pnode))
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Color(_Right(_Pnode)) == _Black
						&& _Color(_Left(_Pnode)) == _Black)
						{	// redden left subtree with black children
						_Color(_Pnode) = _Red;
						_Fixnode = _Fixnodeparent;
						}
					else
						{	// must rearrange left subtree
						if (_Color(_Left(_Pnode)) == _Black)
							{	// rotate red up from right sub-subtree
							_Color(_Right(_Pnode)) = _Black;
							_Color(_Pnode) = _Red;
							_Lrotate(_Pnode);
							_Pnode = _Left(_Fixnodeparent);
							}

						_Color(_Pnode) = _Color(_Fixnodeparent);
						_Color(_Fixnodeparent) = _Black;
						_Color(_Left(_Pnode)) = _Black;
						_Rrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
						}
					}

			_Color(_Fixnode) = _Black;	// ensure stopping node is black
			}

		this->_Alnod.destroy(_Erasednode);	// destroy, free erased node
		this->_Alnod.deallocate(_Erasednode, 1);

		if (0 < _Mysize)
			--_Mysize;

		return (_Make_iter(_Where));	// return successor iterator
		}

	iterator erase(const_iterator _First, const_iterator _Last)
		{	// erase [_First, _Last)
		if (_First == begin() && _Last == end())
			{	// erase all
			clear();
			return (begin());
			}
		else
			{	// partial erase, one at a time
			while (_First != _Last)
				erase(_First++);
			return (_Make_iter(_First));
			}
		}

	size_type erase(const key_type& _Keyval)
		{	// erase and count all that match _Keyval
		_Pairii _Where = equal_range(_Keyval);
		size_type _Num = 0;
		_Distance(_Where.first, _Where.second, _Num);
		erase(_Where.first, _Where.second);
		return (_Num);
		}

	void erase(const key_type *_First, const key_type *_Last)
		{	// erase all that match array of keys [_First, _Last)
		_DEBUG_RANGE(_First, _Last);
		while (_First != _Last)
			erase(*_First++);
		}

	void clear()
		{	// erase all

 #if _HAS_ITERATOR_DEBUGGING
		this->_Orphan_ptr(*this, 0);
 #endif /* _HAS_ITERATOR_DEBUGGING */

		_Erase(_Root());
		_Root() = _Myhead, _Mysize = 0;
		_Lmost() = _Myhead, _Rmost() = _Myhead;
		}

	iterator find(const key_type& _Keyval)
		{	// find an element in mutable sequence that matches _Keyval
		iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->comp,
				_Keyval, _Key(_Where._Mynode()))
					? end() : _Where);
		}

	const_iterator find(const key_type& _Keyval) const
		{	// find an element in nonmutable sequence that matches _Keyval
		const_iterator _Where = lower_bound(_Keyval);
		return (_Where == end()
			|| _DEBUG_LT_PRED(this->comp,
				_Keyval, _Key(_Where._Mynode()))
					? end() : _Where);
		}

	size_type count(const key_type& _Keyval) const
		{	// count all elements that match _Keyval
		_Paircc _Ans = equal_range(_Keyval);
		size_type _Num = 0;
		_Distance(_Ans.first, _Ans.second, _Num);
		return (_Num);
		}

	iterator lower_bound(const key_type& _Keyval)
		{	// find leftmost node not less than _Keyval in mutable tree
		return (_TREE_ITERATOR(_Lbound(_Keyval)));
		}

	const_iterator lower_bound(const key_type& _Keyval) const
		{	// find leftmost node not less than _Keyval in nonmutable tree
		return (_TREE_CONST_ITERATOR(_Lbound(_Keyval)));
		}

	iterator upper_bound(const key_type& _Keyval)
		{	// find leftmost node greater than _Keyval in mutable tree
		return (_TREE_ITERATOR(_Ubound(_Keyval)));
		}

	const_iterator upper_bound(const key_type& _Keyval) const
		{	// find leftmost node greater than _Keyval in nonmutable tree
		return (_TREE_CONST_ITERATOR(_Ubound(_Keyval)));
		}

	_Pairii equal_range(const key_type& _Keyval)
		{	// find range equivalent to _Keyval in mutable tree
		return (_Eqrange(_Keyval));
		}

	_Paircc equal_range(const key_type& _Keyval) const
		{	// find range equivalent to _Keyval in nonmutable tree
		return (_Eqrange(_Keyval));
		}

	void swap(_Myt& _Right)
		{	// exchange contents with _Right
		if (this == &_Right)
			;	// same object, do nothing
		else if (get_allocator() == _Right.get_allocator())
			{	// same allocator, swap control information

 #if _HAS_ITERATOR_DEBUGGING
			this->_Swap_all(_Right);
 #endif /* _HAS_ITERATOR_DEBUGGING */

			this->_Swap_aux(_Right);

			std::swap(this->comp, _Right.comp);
			std::swap(_Myhead, _Right._Myhead);
			std::swap(_Mysize, _Right._Mysize);
			}
		else
			{	// different allocator, do multiple assigns
			this->_Swap_aux(_Right);

			_Myt _Tmp = *this;

			*this = _Right;
			_Right = _Tmp;
			}
		}

protected:
	void _Copy(const _Myt& _Right)
		{	// copy entire tree from _Right
		_Root() = _Copy(_Right._Root(), _Myhead);
		_Mysize = _Right.size();
		if (!_Isnil(_Root()))
			{	// nonempty tree, look for new smallest and largest
			_Lmost() = _Min(_Root());
			_Rmost() = _Max(_Root());
			}
		else
			_Lmost() = _Myhead, _Rmost() = _Myhead;	// empty tree
		}

	_Nodeptr _Copy(_Nodeptr _Rootnode, _Nodeptr _Wherenode)
		{	// copy entire subtree, recursively
		_Nodeptr _Newroot = _Myhead;	// point at nil node

		if (!_Isnil(_Rootnode))
			{	// copy a node, then any subtrees
			_Nodeptr _Pnode = _Buynode(_Myhead, _Wherenode, _Myhead,
				_Myval(_Rootnode), _Color(_Rootnode));
			if (_Isnil(_Newroot))
				_Newroot = _Pnode;	// memorize new root

			_TRY_BEGIN
			_Left(_Pnode) = _Copy(_Left(_Rootnode), _Pnode);
			_Right(_Pnode) = _Copy(_Right(_Rootnode), _Pnode);
			_CATCH_ALL
			_Erase(_Newroot);	// subtree copy failed, bail out
			_RERAISE;
			_CATCH_END
			}

		return (_Newroot);	// return newly constructed tree
		}

	_Paircc _Eqrange(const key_type& _Keyval) const
		{	// find leftmost node not less than _Keyval
		_Nodeptr _Pnode = _Root();
		_Nodeptr _Lonode = _Myhead;	// end() if search fails
		_Nodeptr _Hinode = _Myhead;	// end() if search fails

		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Key(_Pnode), _Keyval))
				_Pnode = _Right(_Pnode);	// descend right subtree
			else
				{	// _Pnode not less than _Keyval, remember it
				if (_Isnil(_Hinode)
						&& _DEBUG_LT_PRED(this->comp, _Keyval, _Key(_Pnode)))
					_Hinode = _Pnode;	// _Pnode greater, remember it
				_Lonode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}

		_Pnode = _Isnil(_Hinode) ? _Root()
			: _Left(_Hinode);	// continue scan for upper bound
		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Keyval, _Key(_Pnode)))
				{	// _Pnode greater than _Keyval, remember it
				_Hinode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}
			else
				_Pnode = _Right(_Pnode);	// descend right subtree

		const_iterator _First = _TREE_CONST_ITERATOR(_Lonode);
		const_iterator _Last = _TREE_CONST_ITERATOR(_Hinode);
		return (_Paircc(_First, _Last));
		}

	_Pairii _Eqrange(const key_type& _Keyval)
		{	// find leftmost node not less than _Keyval
		_Nodeptr _Pnode = _Root();
		_Nodeptr _Lonode = _Myhead;	// end() if search fails
		_Nodeptr _Hinode = _Myhead;	// end() if search fails

		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Key(_Pnode), _Keyval))
				_Pnode = _Right(_Pnode);	// descend right subtree
			else
				{	// _Pnode not less than _Keyval, remember it
				if (_Isnil(_Hinode)
						&& _DEBUG_LT_PRED(this->comp, _Keyval, _Key(_Pnode)))
					_Hinode = _Pnode;	// _Pnode greater, remember it
				_Lonode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}

		_Pnode = _Isnil(_Hinode) ? _Root()
			: _Left(_Hinode);	// continue scan for upper bound
		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Keyval, _Key(_Pnode)))
				{	// _Pnode greater than _Keyval, remember it
				_Hinode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}
			else
				_Pnode = _Right(_Pnode);	// descend right subtree

		iterator _First = _TREE_ITERATOR(_Lonode);
		iterator _Last = _TREE_ITERATOR(_Hinode);
		return (_Pairii(_First, _Last));
		}

	void _Erase(_Nodeptr _Rootnode)
		{	// free entire subtree, recursively
		for (_Nodeptr _Pnode = _Rootnode; !_Isnil(_Pnode); _Rootnode = _Pnode)
			{	// free subtrees, then node
			_Erase(_Right(_Pnode));
			_Pnode = _Left(_Pnode);
			this->_Alnod.destroy(_Rootnode);	// destroy, free erased node
			this->_Alnod.deallocate(_Rootnode, 1);
			}
		}

	void _Init()
		{	// create head/nil node and make tree empty
		_Myhead = _Buynode();
		_Isnil(_Myhead) = true;
		_Root() = _Myhead;
		_Lmost() = _Myhead, _Rmost() = _Myhead;
		_Mysize = 0;
		}

	iterator _Insert(bool _Addleft, _Nodeptr _Wherenode,
		const value_type& _Val)
		{	// add node with value next to _Wherenode, to left if _Addnode
		if (max_size() - 1 <= _Mysize)
			_THROW(length_error, "map/set<T> too long");
		_Nodeptr _Newnode = _Buynode(_Myhead, _Wherenode, _Myhead,
			_Val, _Red);

		++_Mysize;
		if (_Wherenode == _Myhead)
			{	// first node in tree, just set head values
			_Root() = _Newnode;
			_Lmost() = _Newnode, _Rmost() = _Newnode;
			}
		else if (_Addleft)
			{	// add to left of _Wherenode
			_Left(_Wherenode) = _Newnode;
			if (_Wherenode == _Lmost())
				_Lmost() = _Newnode;
			}
		else
			{	// add to right of _Wherenode
			_Right(_Wherenode) = _Newnode;
			if (_Wherenode == _Rmost())
				_Rmost() = _Newnode;
			}

		for (_Nodeptr _Pnode = _Newnode; _Color(_Parent(_Pnode)) == _Red; )
			if (_Parent(_Pnode) == _Left(_Parent(_Parent(_Pnode))))
				{	// fixup red-red in left subtree
				_Wherenode = _Right(_Parent(_Parent(_Pnode)));
				if (_Color(_Wherenode) == _Red)
					{	// parent has two red children, blacken both
					_Color(_Parent(_Pnode)) = _Black;
					_Color(_Wherenode) = _Black;
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Pnode = _Parent(_Parent(_Pnode));
					}
				else
					{	// parent has red and black children
					if (_Pnode == _Right(_Parent(_Pnode)))
						{	// rotate right child to left
						_Pnode = _Parent(_Pnode);
						_Lrotate(_Pnode);
						}
					_Color(_Parent(_Pnode)) = _Black;	// propagate red up
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Rrotate(_Parent(_Parent(_Pnode)));
					}
				}
			else
				{	// fixup red-red in right subtree
				_Wherenode = _Left(_Parent(_Parent(_Pnode)));
				if (_Color(_Wherenode) == _Red)
					{	// parent has two red children, blacken both
					_Color(_Parent(_Pnode)) = _Black;
					_Color(_Wherenode) = _Black;
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Pnode = _Parent(_Parent(_Pnode));
					}
				else
					{	// parent has red and black children
					if (_Pnode == _Left(_Parent(_Pnode)))
						{	// rotate left child to right
						_Pnode = _Parent(_Pnode);
						_Rrotate(_Pnode);
						}
					_Color(_Parent(_Pnode)) = _Black;	// propagate red up
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Lrotate(_Parent(_Parent(_Pnode)));
					}
				}

		_Color(_Root()) = _Black;	// root is always black
		return (_TREE_ITERATOR(_Newnode));
		}

	_Nodeptr _Lbound(const key_type& _Keyval) const
		{	// find leftmost node not less than _Keyval
		_Nodeptr _Pnode = _Root();
		_Nodeptr _Wherenode = _Myhead;	// end() if search fails

		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Key(_Pnode), _Keyval))
				_Pnode = _Right(_Pnode);	// descend right subtree
			else
				{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}

		return (_Wherenode);	// return best remembered candidate
		}

	_Nodeptr& _Lmost() const
		{	// return leftmost node in nonmutable tree
		return (_Left(_Myhead));
		}

	void _Lrotate(_Nodeptr _Wherenode)
		{	// promote right node to root of subtree
		_Nodeptr _Pnode = _Right(_Wherenode);
		_Right(_Wherenode) = _Left(_Pnode);

		if (!_Isnil(_Left(_Pnode)))
			_Parent(_Left(_Pnode)) = _Wherenode;
		_Parent(_Pnode) = _Parent(_Wherenode);

		if (_Wherenode == _Root())
			_Root() = _Pnode;
		else if (_Wherenode == _Left(_Parent(_Wherenode)))
			_Left(_Parent(_Wherenode)) = _Pnode;
		else
			_Right(_Parent(_Wherenode)) = _Pnode;

		_Left(_Pnode) = _Wherenode;
		_Parent(_Wherenode) = _Pnode;
		}

	static _Nodeptr _Max(_Nodeptr _Pnode)
		{	// return rightmost node in subtree at _Pnode
		while (!_Isnil(_Right(_Pnode)))
			_Pnode = _Right(_Pnode);
		return (_Pnode);
		}

	static _Nodeptr _Min(_Nodeptr _Pnode)
		{	// return leftmost node in subtree at _Pnode
		while (!_Isnil(_Left(_Pnode)))
			_Pnode = _Left(_Pnode);
		return (_Pnode);
		}

	_Nodeptr& _Rmost() const
		{	// return rightmost node in nonmutable tree
		return (_Right(_Myhead));
		}

	_Nodeptr& _Root() const
		{	// return root of nonmutable tree
		return (_Parent(_Myhead));
		}

	void _Rrotate(_Nodeptr _Wherenode)
		{	// promote left node to root of subtree
		_Nodeptr _Pnode = _Left(_Wherenode);
		_Left(_Wherenode) = _Right(_Pnode);

		if (!_Isnil(_Right(_Pnode)))
			_Parent(_Right(_Pnode)) = _Wherenode;
		_Parent(_Pnode) = _Parent(_Wherenode);

		if (_Wherenode == _Root())
			_Root() = _Pnode;
		else if (_Wherenode == _Right(_Parent(_Wherenode)))
			_Right(_Parent(_Wherenode)) = _Pnode;
		else
			_Left(_Parent(_Wherenode)) = _Pnode;

		_Right(_Pnode) = _Wherenode;
		_Parent(_Wherenode) = _Pnode;
		}

	_Nodeptr _Ubound(const key_type& _Keyval) const
		{	// find leftmost node greater than _Keyval
		_Nodeptr _Pnode = _Root();
		_Nodeptr _Wherenode = _Myhead;	// end() if search fails

		while (!_Isnil(_Pnode))
			if (_DEBUG_LT_PRED(this->comp, _Keyval, _Key(_Pnode)))
				{	// _Pnode greater than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}
			else
				_Pnode = _Right(_Pnode);	// descend right subtree

		return (_Wherenode);	// return best remembered candidate
		}

 #if _HAS_ITERATOR_DEBUGGING
	void _Orphan_ptr(_Myt& _Cont, _Nodeptr _Ptr) const
		{	// orphan iterators with specified node pointers
		_Lockit _Lock(_LOCK_DEBUG);
		const_iterator **_Pnext = (const_iterator **)&_Cont._Myfirstiter;
		while (*_Pnext != 0)
			if ((*_Pnext)->_Ptr == _Myhead
				|| _Ptr != 0 && (*_Pnext)->_Ptr != _Ptr)
				_Pnext = (const_iterator **)&(*_Pnext)->_Mynextiter;
			else
				{	// orphan the iterator
				(*_Pnext)->_Mycont = 0;
				*_Pnext = (const_iterator *)(*_Pnext)->_Mynextiter;
				}
		}
 #endif /* _HAS_ITERATOR_DEBUGGING */

	_Nodeptr _Buynode()
		{	// allocate a head/nil node
		_Nodeptr _Wherenode = this->_Alnod.allocate(1);
		int _Linkcnt = 0;

		_TRY_BEGIN
		this->_Alptr.construct(&_Left(_Wherenode), 0);
		++_Linkcnt;
		this->_Alptr.construct(&_Parent(_Wherenode), 0);
		++_Linkcnt;
		this->_Alptr.construct(&_Right(_Wherenode), 0);
		_CATCH_ALL
		if (1 < _Linkcnt)
			this->_Alptr.destroy(&_Parent(_Wherenode));
		if (0 < _Linkcnt)
			this->_Alptr.destroy(&_Left(_Wherenode));
		this->_Alnod.deallocate(_Wherenode, 1);
		_RERAISE;
		_CATCH_END
		_Color(_Wherenode) = _Black;
		_Isnil(_Wherenode) = false;
		return (_Wherenode);
		}

	_Nodeptr _Buynode(_Nodeptr _Larg, _Nodeptr _Parg,
		_Nodeptr _Rarg, const value_type& _Val, char _Carg)
		{	// allocate a node with pointers, value, and color
		_Nodeptr _Wherenode = this->_Alnod.allocate(1);
		_TRY_BEGIN
		new (_Wherenode) _Node(_Larg, _Parg, _Rarg, _Val, _Carg);
		_CATCH_ALL
		this->_Alnod.deallocate(_Wherenode, 1);
		_RERAISE;
		_CATCH_END
		return (_Wherenode);
		}

	void _Tidy()
		{	// free all storage
		erase(begin(), end());
		this->_Alptr.destroy(&_Left(_Myhead));
		this->_Alptr.destroy(&_Parent(_Myhead));
		this->_Alptr.destroy(&_Right(_Myhead));
		this->_Alnod.deallocate(_Myhead, 1);
		_Myhead = 0, _Mysize = 0;
		}

	static void _Xran()
		{	// report an out_of_range error
		_THROW(out_of_range, "invalid map/set<T> iterator");
		}

	static void _Xinvarg()
		{	// report an invalid_argument error
		_THROW(invalid_argument, "invalid map/set<T> argument");
		}

	_Nodeptr _Myhead;	// pointer to head node
	size_type _Mysize;	// number of elements
	};

	// _Tree implements a performant swap
template <class _Traits>
	class _Move_operation_category<_Tree<_Traits> >
	{
	public:
		typedef _Swap_move_tag _Move_cat;
	};

		// _Tree TEMPLATE OPERATORS
template<class _Traits> inline
	bool operator==(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test for _Tree equality
	return (_Left.size() == _Right.size()
		&& equal(_Left.begin(), _Left.end(), _Right.begin()));
	}

template<class _Traits> inline
	bool operator!=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test for _Tree inequality
	return (!(_Left == _Right));
	}

template<class _Traits> inline
	bool operator<(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test if _Less < _Right for _Trees
	return (lexicographical_compare(_Left.begin(), _Left.end(),
		_Right.begin(), _Right.end()));
	}

template<class _Traits> inline
	bool operator>(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test if _Less > _Right for _Trees
	return (_Right < _Left);
	}

template<class _Traits> inline
	bool operator<=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test if _Less <= _Right for _Trees
	return (!(_Right < _Left));
	}

template<class _Traits> inline
	bool operator>=(const _Tree<_Traits>& _Left, const _Tree<_Traits>& _Right)
	{	// test if _Less >= _Right for _Trees
	return (!(_Left < _Right));
	}
_STD_END

#ifdef _MSC_VER
 #pragma warning(default:4127)
 #pragma warning(pop)
 #pragma pack(pop)
#endif  /* _MSC_VER */

#endif /* RC_INVOKED */
#endif /* _XTREE_ */

/*
 * This file is derived from software bearing the following
 * restrictions:
 *
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this
 * software and its documentation for any purpose is hereby
 * granted without fee, provided that the above copyright notice
 * appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation.
 * Hewlett-Packard Company makes no representations about the
 * suitability of this software for any purpose. It is provided
 * "as is" without express or implied warranty.
 */

/*
 * Copyright (c) 1992-2007 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
 V5.03:0009 */
