#ifndef _AK_WWISE_MOTION_PLUGIN
#define _AK_WWISE_MOTION_PLUGIN

#include <AK\Wwise\AudioPlugin.h>
#include <AK\SoundEngine\Common\AkTypes.h>

struct AkAudioFormat;

namespace AK
{
	namespace Wwise
	{
		/// MotionDataType
		enum MotionDataType {TypePositionSamples, TypeSpeedSamples, TypeForceSamples, TypeCommands};

		/// Motion device bus plugin interface.  This represent a device bus in Wwise.
		/// \warning The functions in this interface are not thread-safe, unless stated otherwise.
		/// \warning For internal use only.
		class IMotionBusPlugin : public IAudioPlugin
		{
		public:
			/// Returns if the data type is compatible with the device
			/// \return True if supported, False otherwise
			virtual bool	IsTypeCompatible(MotionDataType in_eType) const = 0;
		};

		///Interfaces for motion device source plugins.  Currently used only to discriminate with audio plugins.
		class IMotionSourcePlugin : public IAudioPlugin
		{
		};

		class IMotionFileSourcePlugin : public IMotionSourcePlugin
		{
		public:
			virtual void GetFormat(AkAudioFormat &out_rFormat, GUID in_guidPlatform) const = 0;
			virtual void FileChangedNotification() = 0;
		};

		const WCHAR c_szPropMotionSourceFile[] = L"FileName"; ///< FileName
	}
}

#endif