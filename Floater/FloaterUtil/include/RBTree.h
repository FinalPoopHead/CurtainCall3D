#pragma once
#include <functional>
#include "FixedSizeMemoryPool.h"
#include "FloaterMacro.h"
//#include "Allocator.h"


namespace flt
{
	namespace test
	{
		class TesterRBTree;
	}
	/// <summary>
	/// 레드블랙트리.
	/// 노드를 개별 동적할당 하지 않고 배열을 이용한다.
	/// 렌더러에서 Transfrom의 WorldMatrix를 계산할 때 같은 Transform의 matrix를 다시 계산하지 않기 위해 만듦.
	/// 삭제 연산은 필요없고 삽입, 검색, 초기화만 필요로 해서 이를 우선적으로 구현했다.
	/// iterator의 경우 중위 순회를 한다.
	/// 
	/// RBTree 규칙
	/// 1. 노드는 레드 혹은 블랙이다.
	/// 2. 루트 노드는 반드시 블랙이다.
	/// 3. 모든 NIL은 블랙이다.
	/// 4. 루트 노드부터 리프 노트까지 블랙의 갯수는 항상 같다.
	/// 5. 레드 노드의 자식은 모두 블랙이다.
	/// </summary>
	template<typename Key, typename Value>
	class RBTree
	{
		using ConstKey_t = std::conditional_t<std::is_pointer_v<Key>, const std::remove_pointer_t<Key>*, const Key>;
		using ConstKeyRef_t = std::conditional_t<std::is_pointer_v<Key>, ConstKey_t, ConstKey_t&>;
		using ConstValue_t = std::conditional_t<std::is_pointer_v<Value>, const std::remove_pointer_t<Value>*, const Value>;
		using ConstValueRef_t = std::conditional_t<std::is_pointer_v<Value>, ConstValue_t, ConstValue_t&>;
#pragma region Nasted
	private:
		enum class Color : char
		{
			RED,
			BLACK
		};
		struct Node
		{
			Node() : Node(Key{}, Value{}, Color::RED, & s_nil, & s_nil, & s_nil) {}
			Node(Key key, Value value) : Node(key, value, Color::RED, &s_nil, &s_nil, &s_nil) {}
			Node(Key key, Value value, Color color, Node* pParent, Node* pLeft, Node* pRight) :
				key(key), value(value), color(color),
				pParent(pParent), pLeft(pLeft), pRight(pRight)
			{
				if (pParent == nullptr)
				{
					this->pParent = &s_nil;
				}
				if (pLeft == nullptr)
				{
					this->pLeft = &s_nil;
				}
				if (pRight == nullptr)
				{
					this->pRight = &s_nil;
				}
			}

			Node* GetGrandParent()
			{
				if (pParent == &s_nil)
				{
					return &s_nil;
				}

				return pParent->pParent;
			}

			Node* GetUncle()
			{
				Node* pGrandParent = GetGrandParent();
				if (pGrandParent == &s_nil)
				{
					return &s_nil;
				}

				if (pGrandParent->pLeft == pParent)
				{
					return pGrandParent->pRight;
				}
				else
				{
					return pGrandParent->pLeft;
				}
			}

			Node* GetSibling()
			{
				if (pParent == &s_nil)
				{
					return &s_nil;
				}

				if (pParent->pLeft == this)
				{
					return pParent->pRight;
				}
				else
				{
					return pParent->pLeft;
				}
			}

			Key key;
			Value value;
			Color color;

			Node* pParent;
			Node* pLeft;
			Node* pRight;
		};

		class iterator
		{
			friend class RBTree<Key, Value>;
		public:
			iterator() : iterator(nullptr) {}
			iterator(Node* pNode) : _pNode(pNode) {}

			bool operator==(const iterator& other)
			{
				return _pNode == other._pNode;
			}

			bool operator!=(const iterator& other)
			{
				return !operator==(other);
			}

			Node& operator*()
			{
				return *_pNode;
			}
			Node* operator->()
			{
				return _pNode;
			}

			iterator& operator++()
			{
				if (_pNode->pRight != &s_nil)
				{
					_pNode = _pNode->pRight;
					while (_pNode->pLeft != &s_nil)
					{
						_pNode = _pNode->pLeft;
					}
				}
				else
				{
					Node* pPrev = _pNode;
					_pNode = _pNode->pParent;
					while (_pNode != &s_nil && _pNode->pRight == pPrev)
					{
						pPrev = _pNode;
						_pNode = _pNode->pParent;
					}
				}

				return *this;
			}

			iterator operator++(int)
			{
				iterator temp = *this;
				++(*this);
				return temp;
			}

		private:
			Node* _pNode;
		};

#pragma endregion
	public:
		RBTree();
		RBTree(int capacity);
		~RBTree();

		bool Insert(Key& key, const Value& value);
		void erase(ConstKeyRef_t key);
		void erase(const iterator& it);
		iterator Find(ConstKeyRef_t key);

		Value& operator[](Key& key);

		// 모든 노드를 삭제한다.
		// 메모리는 해제하지 않으며 소멸자도 호출하지 않는다.
		void Clear();

		// iterator 관련 함수들
		iterator begin();
		iterator end();

	private:
		Node* FindRecursive(ConstKeyRef_t key, Node* pNode);

		Node* BSTInsert(Key& key, const Value& value);
		bool BSTInsertRecursive(Node* pCmpNode, Node* pNode, int* outDepth);

		void InsertCase1(Node* pNode);
		void InsertCase2(Node* pNode);
		void InsertCase3(Node* pNode);
		void InsertCase4(Node* pNode);
		void InsertCase5(Node* pNode);

		void RotateRight(Node* pNode);
		void RotateLeft(Node* pNode);

		void delete_case1(Node* pNode);
		void delete_case2(Node* pNode);
		void delete_case3(Node* pNode);
		void delete_case4(Node* pNode);
		void delete_case5(Node* pNode);
		void delete_case6(Node* pNode);


	private:
		std::function<bool(ConstKeyRef_t, ConstKeyRef_t)> _compareFunc;
		int _blackHigh;
		Node* _root;
		FixedSizeMemoryPool _memoryPool;
		int _size;

		static Node s_nil;

		friend class ::flt::test::TesterRBTree;
		friend struct RBTree<Key, Value>::Node;
		friend class RBTree<Key, Value>::iterator;
	};

	// static 멤버 변수 정의
	template<typename Key, typename Value>
	typename RBTree<Key, Value>::Node RBTree<Key, Value>::s_nil = Node{ Key{}, Value{}, Color::BLACK, nullptr, nullptr, nullptr };
}

//----------------------------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------------------------

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::erase(ConstKeyRef_t key)
{
	Node* pNode = Find(key)._pNode;

	if (pNode == &s_nil)
	{
		return;
	}

	if (pNode->pLeft != &s_nil && pNode->pRight != &s_nil)
	{
		Node* pNext = pNode->pRight;
		while (pNext->pLeft != &s_nil)
		{
			pNext = pNext->pLeft;
		}

		pNode->key = pNext->key;
		pNode->value = pNext->value;
		pNode = pNext;
	}

	Node* pChild = (pNode->pLeft == &s_nil) ? pNode->pRight : pNode->pLeft;

	pChild->pParent = pNode->pParent;

	if (pNode->pParent->pLeft == pNode)
	{
		pNode->pParent->pLeft = pChild;
	}
	else if (pNode->pParent->pRight == pNode)
	{
		pNode->pParent->pRight = pChild;
	}

	if (pNode->color == Color::BLACK)
	{
		if (pChild->color == Color::RED)
		{
			pChild->color = Color::BLACK;
		}
		else
		{
			delete_case1(pChild);
		}
	}

	if(pNode->pParent == &s_nil)
	{
		_root = pChild;
	}

	_memoryPool.Free(pNode);
	--_size;
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::erase(const iterator& it)
{
	erase(it->key);
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::RotateRight(Node* pNode)
{
	Node* pLeft = pNode->pLeft;
	Node* pParent = pNode->pParent;

	if (pLeft->pRight != &s_nil)
	{
		pLeft->pRight->pParent = pNode;
	}

	pNode->pLeft = pLeft->pRight;
	pNode->pParent = pLeft;
	pLeft->pRight = pNode;
	pLeft->pParent = pParent;

	if (pParent != &s_nil)
	{
		if (pParent->pLeft == pNode)
		{
			pParent->pLeft = pLeft;
		}
		else
		{
			pParent->pRight = pLeft;
		}
	}
	else
	{
		_root = pLeft;
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::RotateLeft(Node* pNode)
{
	Node* pRight = pNode->pRight;
	Node* pParent = pNode->pParent;

	if (pRight->pLeft != &s_nil)
	{
		pRight->pLeft->pParent = pNode;
	}

	pNode->pRight = pRight->pLeft;
	pNode->pParent = pRight;
	pRight->pLeft = pNode;
	pRight->pParent = pParent;

	if (pParent != &s_nil)
	{
		if (pParent->pLeft == pNode)
		{
			pParent->pLeft = pRight;
		}
		else
		{
			pParent->pRight = pRight;
		}
	}
	else
	{
		_root = pRight;
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase1(Node* pNode)
{
	// 1. Node가 루트노드일 경우 자신을 블랙으로 바꾸고 종료.
	if (pNode == _root)
	{
		pNode->color = Color::BLACK;
	}
	else
	{
		InsertCase2(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase2(Node* pNode)
{
	// 2. 부모가 블랙일 경우 종료.
	if (pNode->pParent->color == Color::BLACK)
	{
		return;
	}
	else
	{
		InsertCase3(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase3(Node* pNode)
{
	ASSERT(pNode->pParent->color == Color::RED, "부모의 색은 빨강 이어야합니다.");

	Node* pUncle = pNode->GetUncle();

	if (pUncle != &s_nil && pUncle->color == Color::RED)
	{
		pNode->pParent->color = Color::BLACK;
		pUncle->color = Color::BLACK;

		Node* pGrandParent = pNode->GetGrandParent();
		pGrandParent->color = Color::RED;

		InsertCase1(pGrandParent);
	}
	else
	{
		InsertCase4(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase4(Node* pNode)
{
	Node* pGrandParent = pNode->GetGrandParent();

	// 내가 오른쪽 자식이고, 부모는 왼쪽 자식일 경우
	if (pNode == pNode->pParent->pRight && pNode->pParent == pGrandParent->pLeft)
	{
		RotateLeft(pNode->pParent);
		pNode = pNode->pLeft;
	}
	// 내가 왼쪽 자식이고 부모는 오른쪽 자식일 경우
	else if (pNode == pNode->pParent->pLeft && pNode->pParent == pGrandParent->pRight)
	{
		RotateRight(pNode->pParent);
		pNode = pNode->pRight;
	}

	InsertCase5(pNode);
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::InsertCase5(Node* pNode)
{
	Node* pGrandParent = pNode->GetGrandParent();

	pNode->pParent->color = Color::BLACK;
	pGrandParent->color = Color::RED;

	if (pNode == pNode->pParent->pLeft)
	{
		RotateRight(pGrandParent);
	}
	else
	{
		RotateLeft(pGrandParent);
	}
}


template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case1(Node* pNode)
{
	if (pNode->pParent != &s_nil)
	{
		delete_case2(pNode);
	}
}


template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case2(Node* pNode)
{
	Node* pSibling = pNode->GetSibling();
	if (pSibling->color == Color::RED)
	{
		pNode->pParent->color = Color::RED;
		pSibling->color = Color::BLACK;

		if (pNode == pNode->pParent->pLeft)
		{
			RotateLeft(pNode->pParent);
		}
		else
		{
			RotateRight(pNode->pParent);
		}
	}
	delete_case3(pNode);
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case3(Node* pNode)
{
	Node* pSibling = pNode->GetSibling();

	if (pNode->pParent->color == Color::BLACK &&
		pSibling->color == Color::BLACK &&
		pSibling->pLeft->color == Color::BLACK &&
		pSibling->pRight->color == Color::BLACK)
	{
		pSibling->color = Color::RED;
		delete_case1(pNode->pParent);
	}
	else
	{
		delete_case4(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case4(Node* pNode)
{
	Node* pSibling = pNode->GetSibling();

	if (pNode->pParent->color == Color::RED &&
		pSibling->color == Color::BLACK &&
		pSibling->pLeft->color == Color::BLACK &&
		pSibling->pRight->color == Color::BLACK)
	{
		pSibling->color = Color::RED;
		pNode->pParent->color = Color::BLACK;
	}
	else
	{
		delete_case5(pNode);
	}
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case5(Node* pNode)
{
	Node* pSibling = pNode->GetSibling();

	if (pSibling->color == Color::BLACK)
	{
		if (pNode == pNode->pParent->pLeft &&
			pSibling->pRight->color == Color::BLACK &&
			pSibling->pLeft->color == Color::RED)
		{
			pSibling->color = Color::RED;
			pSibling->pLeft->color = Color::BLACK;
			RotateRight(pSibling);
		}
		else if (pNode == pNode->pParent->pRight &&
			pSibling->pLeft->color == Color::BLACK &&
			pSibling->pRight->color == Color::RED)
		{
			pSibling->color = Color::RED;
			pSibling->pRight->color = Color::BLACK;
			RotateLeft(pSibling);
		}
	}

	delete_case6(pNode);
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::delete_case6(Node* pNode)
{
	Node* pSibling = pNode->GetSibling();

	pSibling->color = pNode->pParent->color;
	pNode->pParent->color = Color::BLACK;

	if (pNode == pNode->pParent->pLeft)
	{
		pSibling->pRight->color = Color::BLACK;
		RotateLeft(pNode->pParent);
	}
	else
	{
		pSibling->pLeft->color = Color::BLACK;
		RotateRight(pNode->pParent);
	}
}


template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::Insert(Key& key, const Value& value)
{
	Node* node = BSTInsert(key, value);

	if (node == nullptr)
	{
		return false;
	}

	InsertCase1(node);
	++_size;
	return true;
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::begin()
{
	Node* pNode = _root;

	if (pNode == &s_nil)
	{
		return iterator{ pNode };
	}

	while (pNode->pLeft != &s_nil)
	{
		pNode = pNode->pLeft;
	}

	return iterator{ pNode };
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::end()
{
	return iterator{ &s_nil };
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::Node* flt::RBTree<Key, Value>::BSTInsert(Key& key, const Value& value)
{
	void* pMemory = _memoryPool.Alloc();
	if (pMemory == nullptr)
	{
		return nullptr;
	}

	Node* pNode = new (pMemory) Node{ key, value, Color::RED, &s_nil, &s_nil, &s_nil };

	int depth = 1;

	if (_root == &s_nil)
	{
		_root = pNode;
		return pNode;
	}

	if (BSTInsertRecursive(_root, pNode, &depth))
	{
		return pNode;
	}

	// 삽입에 실패했을 경우.
	_memoryPool.Free(pNode);
	return nullptr;
}

template<typename Key, typename Value>
bool flt::RBTree<Key, Value>::BSTInsertRecursive(Node* pCmpNode, Node* pNode, int* outDepth)
{
	(*outDepth)++;

	// key중복은 허용 안함.
	if (pCmpNode->key == pNode->key)
	{
		*outDepth = 0;
		return false;
	}

	Node** ppNextCmpNode = nullptr;
	if (_compareFunc(pCmpNode->key, pNode->key))
	{
		ppNextCmpNode = &pCmpNode->pRight;
	}
	else
	{
		ppNextCmpNode = &pCmpNode->pLeft;
	}

	if (*ppNextCmpNode == &s_nil)
	{
		*ppNextCmpNode = pNode;
		pNode->pParent = pCmpNode;
		return true;
	}

	return BSTInsertRecursive(*ppNextCmpNode, pNode, outDepth);
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::Node* flt::RBTree<Key, Value>::FindRecursive(ConstKeyRef_t key, flt::RBTree<Key, Value>::Node* pNode)
{
	// NIL 노드를 만난다면 종료
	if (pNode == &s_nil)
	{
		return &s_nil;
	}

	ConstKeyRef_t cmpKey = pNode->key;

	// key를 찾았다면 종료
	if (cmpKey == key)
	{
		return pNode;
	}

	// 기본적으로 cmpKey < key 일 경우 왼쪽 노드.
	// 반대의 경우 오른쪽 노드.
	if (_compareFunc(cmpKey, key))
	{
		return FindRecursive(key, pNode->pRight);
	}
	else
	{
		return FindRecursive(key, pNode->pLeft);
	}
}

template<typename Key, typename Value>
Value& flt::RBTree<Key, Value>::operator[](Key& key)
{
	Insert(key, Value{});

	return (Find(key))->value;
}

template<typename Key, typename Value>
flt::RBTree<Key, Value>::iterator flt::RBTree<Key, Value>::Find(ConstKeyRef_t key)
{
	return iterator{ FindRecursive(key, _root) };
}

template<typename Key, typename Value>
void flt::RBTree<Key, Value>::Clear()
{
	_root = &s_nil;
	_memoryPool.FreeAll();
	_size = 0;
}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::RBTree() :
	RBTree(1024)
{

}


template<typename Key, typename Value>
flt::RBTree<Key, Value>::RBTree(int capacity) :
	_compareFunc([](ConstKeyRef_t a, ConstKeyRef_t b) {return a < b; }),
	_blackHigh(0),
	_root(&s_nil),
	_memoryPool(sizeof(Node), capacity),
	_size(0)
{

}



template<typename Key, typename Value>
flt::RBTree<Key, Value>::~RBTree()
{

}
