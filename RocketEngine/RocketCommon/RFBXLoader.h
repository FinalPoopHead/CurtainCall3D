#pragma once

/// .rfbx : assimp FBXLoader를 통해 로드한 파일을 저장하는 자체 포맷
/// RFBXLoader : .rfbx 파일을 로드하는 클래스
namespace Rocket::Core
{
	class RFBXLoader
	{
	public:
		RFBXLoader();
		~RFBXLoader();
	};
}
