#pragma once
#include "./include/ITester.h"
#include "./include/RBTree.h"
#include "./include/Timer.h"
#include <map>
#include <unordered_map>
#include <vector>

namespace flt
{
	namespace test
	{
		class TesterRBTree : public ITester
		{
			using Node = flt::RBTree<int, int>::Node;
			using Color = flt::RBTree<int, int>::Color;


		public:
			TesterRBTree();
			TesterRBTree(unsigned int seed);
			virtual ~TesterRBTree() {}

			virtual bool Test() override;

		private:
			void Preorder(Node* pNode, std::vector<int>* outVector);
			void Inorder(Node* pNode, std::vector<int>* outVector);
			void Postorder(Node* pNode, std::vector<int>* outVector);

			bool CheckDepth(flt::RBTree<int, int>& tree);

			void CheckDepthRecursive(Node* pNode, int depth, int* outMinDepth, int* outMaxDepth);
			bool CheckBlackDepthRecursive(Node* pNode, int depth, int* outBlackDepth);

			int _inputDataCount;
			RBTree<int, int> _tree;
			std::map<int, int> _map;
			std::unordered_map<int, int> _unorderedMap;
			std::vector<int> _inputData;

			Timer _timer;
			Node& nil;
		};
	}
}
