//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006-2008 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

/// \file 
/// Audiokinetic's implementation-specific definitions and factory of 
/// overridable Stream Manager module.
/// Xbox 360 implementation.
#ifndef _AK_STREAM_MGR_MODULE_H_
#define _AK_STREAM_MGR_MODULE_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/IAkStreamMgr.h>

// Forward definitions.
struct AkThreadProperties;

namespace AK
{
    class IAkStreamMgr;
    class IAkLowLevelIO;
}

/// \name Audiokinetic Stream Manager's implementation-specific definitions.
//@{
/// Stream Manager initialization settings.
/// \sa 
/// - AK::IAkStreamMgr
/// - \ref streamingmanager_highlevel_defmanagerinit
struct AkStreamMgrSettings
{
    AK::IAkLowLevelIO * pLowLevelIO;    ///< Low-level IO hook.
    AkUInt32        uMemorySize;        ///< Size of memory pool for small objects of Stream Manager.
};

/// High-level IO devices initialization settings.
/// \sa 
/// - AK::IAkStreamMgr
/// - AK::IAkLowLevelIO
/// - \ref streamingmanager_highlevel_defmanagerinit
struct AkDeviceSettings
{
    AkUInt32        uIOMemorySize;       ///< Size of memory pool for I/O (for automatic streams).
    AkUInt32        uGranularity;        ///< I/O requests granularity (typical bytes/request).
    AkUInt32        uSchedulerTypeFlags; ///< Scheduler type flags.
    AkThreadProperties * pThreadProperties; ///< Scheduler thread properties.
    AkReal32        fTargetAutoStmBufferLength;	///< Targetted automatic stream buffer length (ms). When a stream reaches that buffering, it stops being scheduled for I/O except if the scheduler is idle.
    DWORD           dwIdleWaitTime;      ///< I/O thread maximum wait time when scheduler is idle. Can be INFINITE.
                                         ///< It is considered idle when running automatic streams have more data than their targetted buffering, and no standard stream is waiting for I/O.
};

/// \name Scheduler type flags.

/// Requests to Low-Level IO are synchronous.
#define AK_SCHEDULER_BLOCKING          (0x01)
/// Requests to Low-Level IO are asynchronous, and posted one after the other.
#define AK_SCHEDULER_DEFERRED_LINED_UP (0x02)

/// Platform-specific structure for Low-Level IO interface handshaking.
/// Xbox 360 definition.
/// \remarks
/// - If a file descriptor specifies a blocking scheduler, pOverlapped->hEvent is always NULL.
///   Otherwise, pOverlapped->hEvent is always a valid event.
/// - pOverlapped always contains the byte offset from the beginning of the Win32 file handle.
/// - bIsSequential indicates whether the client of the stream manager forced a new position between 2 IO operations.
/// \sa AK::IAkLowLevelIO
struct AkIOTransferInfo
{
    OVERLAPPED *    pOverlapped;        ///< Win32 OVERLAPPED structure. Contains event and file offset.
    AkUInt32        uTransferSize;      ///< Size of data to be tranfered.      
    AkUInt32        uSizeTransferred;   ///< Size of data actually transferred.
    bool            bIsSequential;      ///< If false, file position was changed since previous operation.
};
//@}

namespace AK
{
    /// Audiokinetic Stream Manager's implementation-specific interface of the Low-Level IO submodule.
    /// This submodule needs to be implemented by the game. All IO requests generated by the Stream Manager end up here.
    /// File location: Implements a map between file ID/names and file
    /// descriptors (AkFileDesc). The latter is used for file identification
    /// across all IAkLowLevelIO functions.
    class IAkLowLevelIO
    {
    public:

        /// Returns a file descriptor for a given file name (string).
        /// Performs the operations needed to make the file descriptor usable by
        /// the other methods of the interface (e.g. ask the OS for a valid file handle).
        /// \return 
        ///     - AK_Success:       A valid file descriptor is returned
        ///     - AK_FileNotFound:  File was not found.
        ///     - AK_Fail:          File could not be open for any other reason.
        /// \return A file descriptor, that contains 
        ///         - an unique identifier to be used with functions of the low-level IO 
        ///           interface.
        ///         - the total stream size in bytes.
        ///         - the offset from the beginning of the file (in blocks).
        ///         - a device ID, that was obtained through AK::IAkStreamMgr::CreateDevice().
        /// \sa GetBlockSize(), Read(), Write().
        virtual AKRESULT Open( 
            AkLpCtstr       in_pszFileName,     ///< File name.
            AkOpenMode      in_eOpenMode,       ///< Open mode.
            AkFileSystemFlags * in_pFlags,      ///< Special flags. Can pass NULL.
            AkFileDesc &    out_fileDesc        ///< Returned file descriptor.
            ) = 0;

        /// Returns a file descriptor for a given file ID.
        /// Performs the operations needed to make the file descriptor usable by
        /// the other methods of the interface (e.g. ask the OS for a valid file handle).
        /// \return 
        ///     - AK_Success:       A valid file descriptor is returned
        ///     - AK_FileNotFound:  File was not found.
        ///     - AK_Fail:          File could not be open for any other reason.
        /// \return A file descriptor, that contains 
        ///         - an unique identifier to be used with functions of the low-level IO 
        ///           interface.
        ///         - the total stream size in bytes.
        ///         - the offset of the beginning of the file (in blocks). 
        ///         - a device ID, that was obtained through AK::IAkStreamMgr::CreateDevice().
        /// \sa GetBlockSize(), Read(), Write().
        virtual AKRESULT Open( 
            AkFileID        in_fileID,          ///< File ID.
            AkOpenMode      in_eOpenMode,       ///< Open mode.
            AkFileSystemFlags * in_pFlags,      ///< Special flags. Can pass NULL.
            AkFileDesc &    out_fileDesc        ///< Returned file descriptor.
            ) = 0;

        /// Cleans up a file.
        /// \return AK_Success if the file was properly cleaned-up.
        virtual AKRESULT Close(
            const AkFileDesc & in_fileDesc      ///< File descriptor.
            ) = 0;

        /// Reads data from a file.
        /// \remarks AkIOTransferInfo is a platform-specific structure that may be 
        /// used to pass file position and synchronization objects. Users will 
        /// pass the same address when calling AK::IAkLowLevelIO::GetAsyncResult().
        /// \remarks If the high-level device is non-blocking, 
        /// io_transferInfo.pOverlapped->hEvent will contain a valid event.
        /// The Low-Level IO module is responsible for signaling that event.
        /// Then AK::IAkLowLevelIO::GetAsyncResult() is called by the Stream Manager.
        /// If the high-level device is blocking, io_transferInfo.pOverlapped->hEvent
        /// wil be NULL, and this method must return only when I/O request is complete 
        /// (whether it failed or not). It is thus not allowed to return AK_NoDataReady.
        /// \remarks File position is computed by the high-level device as 
        /// io_fileDesc.uSector * Block_Size + Stream_Position. Block size is obtained via GetBlockSize(). 
        /// \return 
        ///     - AK_DataReady:   out_pBuffer is filled with data (amount = out_uSizeRead).
        ///     - AK_NoMoreData:  out_pBuffer is filled with data (can be 0 bytes), but reached EOF.
        ///     - AK_NoDataReady: IO is pending: the Stream Manager will call GetAsyncResult() after 
        ///						  Low-Level IO signaled io_transferInfo.pOverlapped->hEvent
        ///						  (applies to non-blocking devices only).
        ///     - AK_Fail:        an error occured.
        virtual AKRESULT Read(
            AkFileDesc &    io_fileDesc,        ///< File descriptor.
            void *          out_pBuffer,        ///< Buffer to be filled with data.
            AkIOTransferInfo & io_transferInfo  ///< Platform-specific IO data transfer info. 
            ) = 0;

        /// Writes data to a file.
        /// \remarks AkIOTransferInfo is a platform-specific structure that may be 
        /// used to pass file position and synchronization objects. Users will 
        /// pass the same address when calling AK::IAkLowLevelIO::GetAsyncResult().
        /// \remarks If the high-level device is non-blocking, 
        /// io_transferInfo.pOverlapped->hEvent will contain a valid event.
        /// The Low-Level IO module is responsible for signaling that event.
        /// Then AK::IAkLowLevelIO::GetAsyncResult() is called by the Stream Manager.
        /// If the high-level device is blocking, io_transferInfo.pOverlapped->hEvent
        /// wil be NULL, and this method must return only when I/O request is complete 
        /// (whether it failed or not). It is thus not allowed to return AK_NoDataReady.
        /// \remarks File position is computed by the high-level device as 
        /// io_fileDesc.uSector * Block_Size + Stream_Position. Block size is obtained via GetBlockSize().
        /// \return 
        ///     - AK_DataReady:   out_pBuffer is filled with data (amount = out_uSizeRead).
        ///     - AK_NoDataReady: IO is pending: the Stream Manager will call GetAsyncResult() after 
        ///						  Low-Level IO signaled io_transferInfo.pOverlapped->hEvent
        ///						  (applies to non-blocking devices only).
        ///     - AK_Fail:        an error occured.
        virtual AKRESULT Write(
            AkFileDesc &    io_fileDesc,        ///< File descriptor.
            void *          in_pData,           ///< Data to be written.
            AkIOTransferInfo & io_transferInfo  ///< Platform-specific IO operation info. 
            ) = 0;

        /// Polling method for pending I/O operations.
        /// Used by non-blocking high-level devices. 
        /// \remarks io_transferInfo.pOverlapped->hEvent always contains a valid event.
        /// The Low-Level IO module is responsible for signaling that event.
        /// Then this method is called by the Stream Manager.
        /// \return 
        ///     - AK_DataReady:     buffer passed to Read() or Write() is ready.
        ///     - AK_NoMoreData:    buffer passed to Read() is filled with data 
        ///                         (can be 0 bytes), but reached EOF.
        ///     - AK_Fail:          an error occured.
        virtual AKRESULT GetAsyncResult(
            AkFileDesc &    io_fileDesc,        ///< File descriptor.
            AkIOTransferInfo & io_transferInfo  ///< Platform-specific IO operation info. 
            ) = 0;

        /// Returns the block size for the file or its storage device. 
        /// The block size is a constraint for clients
        /// of the Stream Manager: All reads, writes and position changes need to be a multiple of
        /// that size.
        /// \return The block size for a specific file or storage device.
        /// \remarks Some files might be open with flags that require IO transfers to be a multiple 
        ///         of this size. The stream manager will query this function to resolve calls 
        ///         to IAk(Auto)Stream::GetBlockSize( ).
        ///         Also, AkFileDesc::uSector specifies a number of sectors in multiples
        ///         of this value.
        ///         Files/IO devices that do not require byte alignment should return 1.
        /// \warning Returning 0 is not allowed and will likely make the Stream Manager crash.
        /// \sa Open(), Read(), Write().
        virtual AkUInt32 GetBlockSize(
            const AkFileDesc &  in_fileDesc     ///< File descriptor.
            ) = 0;

        /// Returns a device description for a given device, specified by its ID.
        /// \remarks For profiling purposes only. Compiled out when AK_OPTIMIZED is defined.
        /// \return AK_Success if the device exists, AK_Fail otherwise.
#ifndef AK_OPTIMIZED
        virtual AKRESULT GetDeviceDesc(
            AkDeviceID      in_deviceID,        ///< High-level device ID.
            AkDeviceDesc &  out_deviceDesc      ///< Description of associated low-level I/O device.
            ) = 0;
#endif
    };


    /// \name Audiokinetic implementation-specific Stream Manager factory.
    //@{
    /// Stream Manager factory.
    /// \sa AK::IAkStreamMgr
    IAkStreamMgr * CreateStreamMgr( 
        AkStreamMgrSettings * in_pSettings  ///< Stream manager initialization settings.
        );
    //@}

    /// \name Stream Manager: High-level I/O devices management.
    //@{
    /// Streaming device creation.
    /// Creates a high-level device, with specific settings. 
    /// \return The device ID. AK_INVALID_DEVICE_ID if there was an error and it could not be created.
    /// \warning This function is not thread-safe.
    /// \remarks The device ID should be kept by the Low-Level IO, to assign it to file descriptors in AK::IAkLowLevelIO::Open().
    AkDeviceID CreateDevice(
        const AkDeviceSettings * in_pDeviceSettings  ///< Device settings (implementation specific). \sa AkDeviceSettings.
        );
    /// Streaming device destruction.
    /// \return AK_Success if the device was successfully destroyed.
    /// \warning This function is not thread-safe. No stream should exist for that device when it is destroyed.
    AKRESULT   DestroyDevice(
        AkDeviceID               in_deviceID         ///< Device ID.
        );
    //@}

}

#endif //_AK_STREAM_MGR_MODULE_H_
