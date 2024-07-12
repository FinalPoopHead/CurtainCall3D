#pragma once

namespace flt
{
	template<typename Error>
	class Unexpected
	{
	};

	template<typename T, typename Error>
	class Expected
	{
		using UnexceptedType = Unexpected<Error>;

	public:

	private:
		bool _hasValue;
		union
		{
			T _val;
			Error _err;
		};

	};
}
