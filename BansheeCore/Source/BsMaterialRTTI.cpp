//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#include "BsMaterialRTTI.h"
#include "BsMaterialManager.h"
#include "BsGpuParamDesc.h"

namespace BansheeEngine
{
	RTTITypeBase* MaterialFloatParam::getRTTIStatic() { return MaterialFloatParamRTTI::instance(); }
	RTTITypeBase* MaterialFloatParam::getRTTI() const { return MaterialFloatParam::getRTTIStatic(); }

	RTTITypeBase* MaterialVec2Param::getRTTIStatic() { return MaterialVec2ParamRTTI::instance(); }
	RTTITypeBase* MaterialVec2Param::getRTTI() const { return MaterialVec2Param::getRTTIStatic(); }

	RTTITypeBase* MaterialVec3Param::getRTTIStatic() { return MaterialVec3ParamRTTI::instance(); }
	RTTITypeBase* MaterialVec3Param::getRTTI() const { return MaterialVec3Param::getRTTIStatic(); }

	RTTITypeBase* MaterialVec4Param::getRTTIStatic() { return MaterialVec4ParamRTTI::instance(); }
	RTTITypeBase* MaterialVec4Param::getRTTI() const { return MaterialVec4Param::getRTTIStatic(); }

	RTTITypeBase* MaterialMat3Param::getRTTIStatic() { return MaterialMat3ParamRTTI::instance(); }
	RTTITypeBase* MaterialMat3Param::getRTTI() const { return MaterialMat3Param::getRTTIStatic(); }

	RTTITypeBase* MaterialMat4Param::getRTTIStatic() { return MaterialMat4ParamRTTI::instance(); }
	RTTITypeBase* MaterialMat4Param::getRTTI() const { return MaterialMat4Param::getRTTIStatic(); }

	RTTITypeBase* MaterialStructParam::getRTTIStatic() { return MaterialStructParamRTTI::instance(); }
	RTTITypeBase* MaterialStructParam::getRTTI() const { return MaterialStructParam::getRTTIStatic(); }

	RTTITypeBase* MaterialTextureParam::getRTTIStatic() { return MaterialTextureParamRTTI::instance(); }
	RTTITypeBase* MaterialTextureParam::getRTTI() const { return MaterialTextureParam::getRTTIStatic(); }

	RTTITypeBase* MaterialSamplerStateParam::getRTTIStatic() { return MaterialSamplerStateParamRTTI::instance(); }
	RTTITypeBase* MaterialSamplerStateParam::getRTTI() const { return MaterialSamplerStateParam::getRTTIStatic(); }

	RTTITypeBase* MaterialParams::getRTTIStatic() { return MaterialParamsRTTI::instance(); }
	RTTITypeBase* MaterialParams::getRTTI() const { return MaterialParams::getRTTIStatic(); }

	void MaterialRTTI::onSerializationStarted(IReflectable* obj)
	{
		Material* material = static_cast<Material*>(obj);
		std::shared_ptr<MaterialParams> params = bs_shared_ptr<MaterialParams, ScratchAlloc>();

		ShaderPtr shader = material->getShader();
		if(shader != nullptr)
		{
			const Map<String, String>& validParamNames = material->getValidParamNames();

			for(auto iter = validParamNames.begin(); iter != validParamNames.end(); ++iter)
			{
				GpuParamType type = shader->getParamType(iter->first);

				if(type == GPT_DATA)
				{
					const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->first);

					switch(paramDesc.type)
					{
					case GPDT_FLOAT1:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialFloatParam param;
								param.name = iter->first;
								param.value = material->getFloat(iter->first, i);
								param.arrayIdx = i;

								params->floatParams.push_back(param);
							}
						}
						break;
					case GPDT_FLOAT2:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialVec2Param param;
								param.name = iter->first;
								param.value = material->getVec2(iter->first, i);
								param.arrayIdx = i;

								params->vec2Params.push_back(param);
							}
						}
						break;
					case GPDT_FLOAT3:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialVec3Param param;
								param.name = iter->first;
								param.value = material->getVec3(iter->first, i);
								param.arrayIdx = i;

								params->vec3Params.push_back(param);
							}
						}
						break;
					case GPDT_FLOAT4:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialVec4Param param;
								param.name = iter->first;
								param.value = material->getVec4(iter->first, i);
								param.arrayIdx = i;

								params->vec4Params.push_back(param);
							}
						}
						break;
					case GPDT_MATRIX_3X3:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialMat3Param param;
								param.name = iter->first;
								param.value = material->getMat3(iter->first, i);
								param.arrayIdx = i;

								params->mat3Params.push_back(param);
							}
						}
						break;
					case GPDT_MATRIX_4X4:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialMat4Param param;
								param.name = iter->first;
								param.value = material->getMat4(iter->first, i);
								param.arrayIdx = i;

								params->mat4Params.push_back(param);
							}
						}
						break;
					case GPDT_STRUCT:
						{
							for(UINT32 i = 0; i < paramDesc.arraySize; i++)
							{
								MaterialStructParam param;
								param.name = iter->first;
								param.value = material->getStructData(iter->first, i);
								param.arrayIdx = i;
								param.elementSize = paramDesc.elementSize;

								params->structParams.push_back(param);
							}
						}
						break;
					default:
						BS_EXCEPT(InternalErrorException, "Cannot serialize this paramater type: " + toString(paramDesc.type));
					}
				}
				else if(type == GPT_OBJECT)
				{
					const SHADER_OBJECT_PARAM_DESC& paramDesc = shader->getObjectParamDesc(iter->first);

					if(Shader::isSampler(paramDesc.type))
					{
						MaterialSamplerStateParam param;
						param.name = iter->first;
						param.value = material->getSamplerState(iter->first);

						params->samplerStateParams.push_back(param);
					}
					else if(Shader::isTexture(paramDesc.type))
					{
						MaterialTextureParam param;
						param.name = iter->first;
						param.value = material->getTexture(iter->first);

						params->textureParams.push_back(param);
					}
					else if(Shader::isBuffer(paramDesc.type))
					{
						BS_EXCEPT(NotImplementedException, "Buffers can't be serialized yet."); // TODO
					}
					else
					{
						BS_EXCEPT(InternalErrorException, "Cannot serialize this paramater type: " + toString(paramDesc.type));
					}
				}
				else
					BS_EXCEPT(InternalErrorException, "Invalid parameter type.");

			}
		}

		material->mRTTIData = params;
	}

	void MaterialRTTI::onSerializationEnded(IReflectable* obj)
	{
		Material* material = static_cast<Material*>(obj);

		material->mRTTIData = nullptr; // This will delete temporary data as it's stored in a unique ptr
	}

	void MaterialRTTI::onDeserializationStarted(IReflectable* obj)
	{
		// Do nothing
	}

	void MaterialRTTI::onDeserializationEnded(IReflectable* obj)
	{
		Material* material = static_cast<Material*>(obj);
		if(material->mRTTIData.empty())
			return;

		material->initBestTechnique();

		std::shared_ptr<MaterialParams> params = any_cast<std::shared_ptr<MaterialParams>>(material->mRTTIData);

		ShaderPtr shader = material->getShader();
		if(shader != nullptr)
		{
			for(auto iter = params->floatParams.begin(); iter != params->floatParams.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_FLOAT1 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setFloat(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->vec2Params.begin(); iter != params->vec2Params.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_FLOAT2 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setVec2(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->vec3Params.begin(); iter != params->vec3Params.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_FLOAT3 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setVec3(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->vec4Params.begin(); iter != params->vec4Params.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_FLOAT4 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setVec4(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->mat3Params.begin(); iter != params->mat3Params.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_MATRIX_3X3 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setMat3(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->mat4Params.begin(); iter != params->mat4Params.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_MATRIX_4X4 || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize)
					continue;

				material->setMat4(iter->name, iter->value, iter->arrayIdx);
			}

			for(auto iter = params->structParams.begin(); iter != params->structParams.end(); ++iter)
			{
				if(!shader->hasDataParam(iter->name))
					continue;

				const SHADER_DATA_PARAM_DESC& paramDesc = shader->getDataParamDesc(iter->name);

				if(paramDesc.type != GPDT_STRUCT || iter->arrayIdx < 0 || iter->arrayIdx >= paramDesc.arraySize || iter->elementSize != paramDesc.elementSize)
					continue;

				material->setStructData(iter->name, iter->value.data.get(), iter->value.size, iter->arrayIdx);
			}

			for(auto iter = params->samplerStateParams.begin(); iter != params->samplerStateParams.end(); ++iter)
			{
				if(!shader->hasObjectParam(iter->name))
					continue;

				const SHADER_OBJECT_PARAM_DESC& paramDesc = shader->getObjectParamDesc(iter->name);

				if(!Shader::isSampler(paramDesc.type))
					continue;

				material->setSamplerState(iter->name, iter->value);
			}

			for(auto iter = params->textureParams.begin(); iter != params->textureParams.end(); ++iter)
			{
				if(!shader->hasObjectParam(iter->name))
					continue;

				const SHADER_OBJECT_PARAM_DESC& paramDesc = shader->getObjectParamDesc(iter->name);

				if(!Shader::isTexture(paramDesc.type))
					continue;

				material->setTexture(iter->name, iter->value);
			}
		}

		material->mRTTIData = nullptr; // This will delete temporary data as it's stored in a unique ptr
	}

	std::shared_ptr<IReflectable> MaterialRTTI::newRTTIObject()
	{
		return MaterialManager::instance().create();
	}
}