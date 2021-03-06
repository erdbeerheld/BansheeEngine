//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#include "BsGLSLProgramPipelineManager.h"
#include "BsGLSLGpuProgram.h"
#include "BsRenderStats.h"

namespace BansheeEngine
{
	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

	::std::size_t GLSLProgramPipelineManager::ProgramPipelineKeyHashFunction::operator()
		(const GLSLProgramPipelineManager::ProgramPipelineKey &key) const
	{
		std::size_t seed = 0;
		hash_combine(seed, key.vertexProgKey);
		hash_combine(seed, key.fragmentProgKey);
		hash_combine(seed, key.geometryProgKey);
		hash_combine(seed, key.hullProgKey);
		hash_combine(seed, key.domainProgKey);

		return seed;
	}

	bool GLSLProgramPipelineManager::ProgramPipelineKeyEqual::operator()
		(const GLSLProgramPipelineManager::ProgramPipelineKey &a, const GLSLProgramPipelineManager::ProgramPipelineKey &b) const
	{
		return a.vertexProgKey == b.vertexProgKey && a.fragmentProgKey == b.fragmentProgKey && a.geometryProgKey == b.geometryProgKey &&
			a.hullProgKey == b.hullProgKey && a.domainProgKey == b.domainProgKey;
	}

	GLSLProgramPipelineManager::~GLSLProgramPipelineManager()
	{
		for (auto& pipeline : mPipelines)
		{
			glDeleteProgramPipelines(1, &pipeline.second.glHandle);
			BS_INC_RENDER_STAT_CAT(ResDestroyed, RenderStatObject_PipelineObject);
		}
	}

	const GLSLProgramPipeline* GLSLProgramPipelineManager::getPipeline(GLSLGpuProgram* vertexProgram, GLSLGpuProgram* fragmentProgram, 
		GLSLGpuProgram* geometryProgram, GLSLGpuProgram* hullProgram, GLSLGpuProgram* domainProgram)
	{
		ProgramPipelineKey key;
		key.vertexProgKey = vertexProgram != nullptr ? vertexProgram->getProgramID() : 0;
		key.fragmentProgKey = fragmentProgram != nullptr ? fragmentProgram->getProgramID() : 0;
		key.geometryProgKey = geometryProgram != nullptr ? geometryProgram->getProgramID() : 0;
		key.hullProgKey = hullProgram != nullptr ? hullProgram->getProgramID() : 0;
		key.domainProgKey = domainProgram != nullptr ? domainProgram->getProgramID() : 0;

		auto iterFind = mPipelines.find(key);

		if(iterFind == mPipelines.end())
		{
			GLSLProgramPipeline newPipeline;

			glGenProgramPipelines(1, &newPipeline.glHandle);

			if(vertexProgram != nullptr)
			{
				glUseProgramStages(newPipeline.glHandle, GL_VERTEX_SHADER_BIT, vertexProgram->getGLHandle());
			}

			if(fragmentProgram != nullptr)
			{
				glUseProgramStages(newPipeline.glHandle, GL_FRAGMENT_SHADER_BIT, fragmentProgram->getGLHandle());
			}

			if(geometryProgram != nullptr)
			{
				glUseProgramStages(newPipeline.glHandle, GL_GEOMETRY_SHADER_BIT, geometryProgram->getGLHandle());
			}

			if(hullProgram != nullptr)
			{
				glUseProgramStages(newPipeline.glHandle, GL_TESS_CONTROL_SHADER_BIT, hullProgram->getGLHandle());
			}

			if(domainProgram != nullptr)
			{
				glUseProgramStages(newPipeline.glHandle, GL_TESS_EVALUATION_SHADER_BIT, domainProgram->getGLHandle());
			}

			mPipelines[key] = newPipeline;

			BS_INC_RENDER_STAT_CAT(ResCreated, RenderStatObject_PipelineObject);
			return &mPipelines[key];
		}
		else
			return &iterFind->second;
	}
}