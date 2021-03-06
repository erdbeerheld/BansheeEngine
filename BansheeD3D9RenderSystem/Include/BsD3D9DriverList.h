//__________________________ Banshee Project - A modern game development toolkit _________________________________//
//_____________________________________ www.banshee-project.com __________________________________________________//
//________________________ Copyright (c) 2014 Marko Pintera. All rights reserved. ________________________________//
#pragma once

#include "BsD3D9Prerequisites.h"
#include "BsD3D9Driver.h"

namespace BansheeEngine 
{
	/**
	 * @brief	Holds a list of all drivers (adapters) and video modes.
	 */
	class BS_D3D9_EXPORT D3D9DriverList
	{
	public:
		D3D9DriverList();
		~D3D9DriverList();

		/**
		 * @brief	Returns the number of drivers (adapters) available.
		 */
		UINT32 count() const;

		/**
		 * @brief	Returns driver with the specified index.
		 */
		D3D9Driver* item(UINT32 index);

		/**
		 * @brief	Returns drivers with the specified name or null if it cannot be found.
		 */
		D3D9Driver* item(const String &name);

		/**
		 * @brief	Returns available video modes for all drivers and output devices.
		 */
		VideoModeInfoPtr getVideoModeInfo() const { return mVideoModeInfo; }

	private:
		Vector<D3D9Driver> mDriverList;
		VideoModeInfoPtr mVideoModeInfo;
	};
}