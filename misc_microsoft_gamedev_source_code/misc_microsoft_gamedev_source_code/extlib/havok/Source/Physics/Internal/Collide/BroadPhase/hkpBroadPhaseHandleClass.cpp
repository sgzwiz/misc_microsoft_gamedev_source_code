/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2007 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Physics/Internal/Collide/BroadPhase/hkpBroadPhaseHandle.h'
#include <Physics/Internal/hkpInternal.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Physics/Internal/Collide/BroadPhase/hkpBroadPhaseHandle.h>



//
// Class hkpBroadPhaseHandle
//
HK_REFLECTION_DEFINE_SIMPLE(hkpBroadPhaseHandle);
static const hkInternalClassMember hkpBroadPhaseHandleClass_Members[] =
{
	{ "id", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT32, hkClassMember::TYPE_VOID, 0, 0|hkClassMember::SERIALIZE_IGNORED, HK_OFFSET_OF(hkpBroadPhaseHandle,m_id), HK_NULL }
};
extern const hkClass hkpBroadPhaseHandleClass;
const hkClass hkpBroadPhaseHandleClass(
	"hkpBroadPhaseHandle",
	HK_NULL, // parent
	sizeof(hkpBroadPhaseHandle),
	HK_NULL,
	0, // interfaces
	HK_NULL,
	0, // enums
	reinterpret_cast<const hkClassMember*>(hkpBroadPhaseHandleClass_Members),
	HK_COUNT_OF(hkpBroadPhaseHandleClass_Members),
	HK_NULL, // defaults
	HK_NULL, // attributes
	0
	);

/*
* Havok SDK - PUBLIC RELEASE, BUILD(#20070919)
*
* Confidential Information of Havok.  (C) Copyright 1999-2007 
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
*
* Use of this software for evaluation purposes is subject to and indicates 
* acceptance of the End User licence Agreement for this product. A copy of 
* the license is included with this software and is also available from salesteam@havok.com.
*
*/
