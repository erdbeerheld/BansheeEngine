#pragma once

#include "CmPrerequisites.h"

namespace CamelotEngine
{
	class RenderCommand
	{
	public:
		RenderCommand(RenderOperation& ro, PassPtr& pass, PassParametersPtr& passParams)
			:mRO(ro), mPass(pass), mPassParameters(passParams)
		{ }

		const RenderOperation& getRenderOperation() const { return mRO; }
		PassPtr getPass() const { return mPass; }
		PassParametersPtr getPassParameters() const { return mPassParameters; }

	private:
		RenderOperation mRO; // TODO Low priority - Keeping this by value is unnecessary overhead
		PassPtr mPass;
		PassParametersPtr mPassParameters;
	};
}