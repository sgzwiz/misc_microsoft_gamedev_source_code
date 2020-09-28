/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok).  Not for disclosure or distribution without Havok's
 * prior written consent.This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2007 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// WARNING: THIS FILE IS GENERATED. EDITS WILL BE LOST.
// Generated from 'Common/Base/Types/Physics/ContactPoint/hkContactPointMaterial.h'
#include <Common/Base/hkBase.h>
#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/hkInternalClassMember.h>
#include <Common/Base/Reflection/hkTypeInfo.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPointMaterial.h>



//
// Enum hkContactPointMaterial::FlagEnum
//
static const hkInternalClassEnumItem hkContactPointMaterialFlagEnumEnumItems[] =
{
	{1, "CONTACT_IS_NEW_AND_POTENTIAL"},
	{2, "CONTACT_USES_SOLVER_PATH2"},
	{4, "CONTACT_BREAKOFF_OBJECT_ID"},
};
static const hkInternalClassEnum hkContactPointMaterialEnums[] = {
	{"FlagEnum", hkContactPointMaterialFlagEnumEnumItems, 3, HK_NULL, 0 }
};
extern const hkClassEnum* hkContactPointMaterialFlagEnumEnum = reinterpret_cast<const hkClassEnum*>(&hkContactPointMaterialEnums[0]);

//
// Class hkContactPointMaterial
//
HK_REFLECTION_DEFINE_SIMPLE(hkContactPointMaterial);
const hkInternalClassMember hkContactPointMaterial::Members[] =
{
	{ "userData", HK_NULL, HK_NULL, hkClassMember::TYPE_ULONG, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContactPointMaterial,m_userData), HK_NULL },
	{ "friction", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContactPointMaterial,m_friction), HK_NULL },
	{ "restitution", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContactPointMaterial,m_restitution), HK_NULL },
	{ "maxImpulse", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContactPointMaterial,m_maxImpulse), HK_NULL },
	{ "flags", HK_NULL, HK_NULL, hkClassMember::TYPE_UINT8, hkClassMember::TYPE_VOID, 0, 0, HK_OFFSET_OF(hkContactPointMaterial,m_flags), HK_NULL }
};
extern const hkClass hkContactPointMaterialClass;
const hkClass hkContactPointMaterialClass(
	"hkContactPointMaterial",
	HK_NULL, // parent
	sizeof(hkContactPointMaterial),
	HK_NULL,
	0, // interfaces
	reinterpret_cast<const hkClassEnum*>(hkContactPointMaterialEnums),
	1, // enums
	reinterpret_cast<const hkClassMember*>(hkContactPointMaterial::Members),
	HK_COUNT_OF(hkContactPointMaterial::Members),
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
