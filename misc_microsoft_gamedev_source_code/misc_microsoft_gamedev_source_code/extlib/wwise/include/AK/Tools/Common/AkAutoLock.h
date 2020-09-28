//////////////////////////////////////////////////////////////////////
//
// AkAutoLock.h
//
// AudioKinetic Lock base class.
//
// Copyright 2006-2008 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////
#ifndef _AUTO_LOCK_H_
#define _AUTO_LOCK_H_

#include <AK/SoundEngine/Common/AkTypes.h>

template< class TLock >
class AkAutoLock
{
public:
	/// Constructor
	AkForceInline AkAutoLock( TLock& in_rLock )
		: m_rLock( in_rLock )
	{
		m_rLock.Lock();
	}

	/// Destructor
	AkForceInline ~AkAutoLock()
	{
		m_rLock.Unlock();
	}

private:
	TLock& m_rLock;
};

#endif //_AUTO_LOCK_H_
