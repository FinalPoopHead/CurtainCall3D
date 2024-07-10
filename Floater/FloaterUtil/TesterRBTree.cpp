#include "TesterRBTree.h"
#include <iostream>
#include <random>

flt::test::TesterRBTree::TesterRBTree() : TesterRBTree(std::random_device()())
{
}

flt::test::TesterRBTree::TesterRBTree(unsigned int seed) :
	_inputDataCount(10000),
	_tree(_inputDataCount), _map(), _unorderedMap(),
	_inputData(),
	_timer(),
	nil(flt::RBTree<int, int>::s_nil)
{
	std::mt19937_64 rng;
	rng.seed(seed);
	std::uniform_int_distribution<int> dist(0, _inputDataCount * 100);

	_inputData.reserve(_inputDataCount);
	for (int i = 0; i < _inputDataCount; ++i)
	{
		_inputData.push_back(dist(rng));
	}

	std::wcout << L"중복 있는 데이터 " << _inputDataCount << L"개 생성 완료." << std::endl;
	_timer.Start();
	for (auto& e : _inputData)
	{
		_tree.Insert(e, e);
	}
	std::wcout << L"트리 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		_map.insert(std::make_pair(e, e));
	}
	std::wcout << L"맵 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		_unorderedMap.insert(std::make_pair(e, e));
	}
	std::wcout << L"해쉬 맵 삽입 시간 : " << _timer.GetLabTimeSeconds() << std::endl;
}

bool flt::test::TesterRBTree::Test()
{
	bool retval = true;

	// 단순 생성 삭제 테스트
	{
		flt::RBTree<int, int> tree;
	}

	std::vector<int> v;

	// 중위 순회, 오름차순 정렬
	Inorder(_tree._root, &v);

	// iterator++로 순회 시 중위순회인지?
	{
		std::vector<int> iterVec;
		iterVec.reserve(_inputDataCount);

		for (auto& e : _tree)
		{
			iterVec.push_back(e.key);
		}

		//std::cout << std::endl;

		for (int i = 0; i < v.size(); ++i)
		{
			if (v[i] != iterVec[i])
			{
				ASSERT(false, "중위 순회 오류");
				retval = retval && false;
			}
		}
	}

	v.clear();

	// 트리의 최대 깊이가 최저 깊이의 2배가 넘지 않는지 체크.
	retval = retval && CheckDepth(_tree);


	// 들어간 값을 제대로 찾는 지 테스트.
	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _tree.Find(e);
		if (it == _tree.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"트리 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	// 값을 하나씩 삭제하면서 삭제한 값이 제대로 삭제되었는지 테스트.
	{
		RBTree<int, int> eraseTestTree(_inputDataCount);
		for(auto& e : _inputData)
		{
			eraseTestTree.Insert(e, e);
		}

		retval == retval && CheckDepth(eraseTestTree);

		for (int deletedIndex = 0; deletedIndex < _inputData.size(); ++deletedIndex)
		{
			int remainingStartIndex = deletedIndex + 1;
			int key = _inputData[deletedIndex];
			eraseTestTree.erase(key);
			retval == retval && CheckDepth(eraseTestTree);

			// 삭제한 값이 제대로 삭제되었는지 테스트.
			{
				auto it = eraseTestTree.Find(key);
				if (it != eraseTestTree.end())
				{
					ASSERT(false, "값이 제대로 삭제 안됨. 아직 남아있음.");
					auto testFind = eraseTestTree.Find(key);
					retval = retval && false;
				}
			}

			// 나머지 값들이 제대로 들어있는지 테스트.
			{
				for (int i = remainingStartIndex; i < _inputData.size(); ++i)
				{
					int remainingkey = _inputData[i];
					auto it = eraseTestTree.Find(remainingkey);
					if (it == eraseTestTree.end())
					{
						// 입력 데이터에는 중복 데이터가 있을 수 있으므로 이 경우는 통과.
						bool isDeleted = false;
						for (int j = 0; j < remainingStartIndex; ++j)
						{
							if (_inputData[j] == remainingkey)
							{
								isDeleted = true;
							}
						}
						if (!isDeleted)
						{
							ASSERT(false, "값이 제대로 삭제 안됨. 있어야 할 데이터가 없음.");
							auto testFind = eraseTestTree.Find(remainingkey);
							retval = retval && false;
						}
					}
				}
			}
		}
	}
	



	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _map.find(e);
		if (it == _map.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"맵 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	for (auto& e : _inputData)
	{
		auto it = _unorderedMap.find(e);
		if (it == _unorderedMap.end())
		{
			ASSERT(false, "값이 다 안들어감.");
			retval = retval && false;
		}
	}
	std::wcout << L"해쉬 맵 탐색 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_tree.Clear();
	std::wcout << L"트리 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_map.clear();
	std::wcout << L"맵 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;

	_timer.Start();
	_unorderedMap.clear();
	std::wcout << L"해쉬 맵 삭제 시간 : " << _timer.GetLabTimeSeconds() << std::endl;
	return retval;
}

void flt::test::TesterRBTree::Preorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	outVector->push_back(pNode->key);

	Preorder(pNode->pLeft, outVector);
	Preorder(pNode->pRight, outVector);
}

void flt::test::TesterRBTree::Inorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	Inorder(pNode->pLeft, outVector);
	outVector->push_back(pNode->key);
	Inorder(pNode->pRight, outVector);
}

void flt::test::TesterRBTree::Postorder(Node* pNode, std::vector<int>* outVector)
{
	if (pNode == &nil)
	{
		return;
	}

	Postorder(pNode->pLeft, outVector);
	Postorder(pNode->pRight, outVector);
	outVector->push_back(pNode->key);
}

bool flt::test::TesterRBTree::CheckDepth(flt::RBTree<int, int>& tree)
{
	int minDepth = INT_MAX;
	int maxDepth = 0;
	bool retval = true;

	CheckDepthRecursive(tree._root, 0, &minDepth, &maxDepth);

	if (minDepth * 2 < maxDepth)
	{
		ASSERT(false, "깊이 차이가 2배 이상임.");
		retval = retval && false;
	}

	int blackDepth = -1;
	retval = retval && CheckBlackDepthRecursive(tree._root, 0, &blackDepth);
	ASSERT(retval, "검정 노드의 갯수가 다름.");

	return retval;
	//printf("depth | min | max | black\n");
	//printf("      | %03d | %03d |  %03d\n", maxDepth, minDepth, blackDepth);
}

void flt::test::TesterRBTree::CheckDepthRecursive(Node* pNode, int depth, int* outMinDepth, int* outMaxDepth)
{
	if (pNode == &nil)
	{
		*outMinDepth > depth ? *outMinDepth = depth : 0;
		*outMaxDepth < depth ? *outMaxDepth = depth : 0;
		return;
	}
	depth++;
	CheckDepthRecursive(pNode->pLeft, depth, outMinDepth, outMaxDepth);
	CheckDepthRecursive(pNode->pRight, depth, outMinDepth, outMaxDepth);
}

bool flt::test::TesterRBTree::CheckBlackDepthRecursive(Node* pNode, int depth, int* outBlackDepth)
{
	if (pNode == &nil)
	{
		if (*outBlackDepth == -1)
		{
			*outBlackDepth = depth;
		}
		else if (depth != *outBlackDepth)
		{
			ASSERT(false, "depth 다름.");
			return false;
		}
		return true;
	}

	if (pNode->color == Color::BLACK)
	{
		depth++;
	}

	bool retval = CheckBlackDepthRecursive(pNode->pLeft, depth, outBlackDepth);
	retval = retval && CheckBlackDepthRecursive(pNode->pRight, depth, outBlackDepth);

	return retval;
}
