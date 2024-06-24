#pragma once
#include <functional>



namespace flt
{
	class FLTweenNode;
	
	class FLTween
	{
	public:
		FLTween();
		~FLTween();

		void Update(float deltaSeconds);

	private:
		FLTweenNode* _head;
		FLTweenNode* _current;
	};
}
