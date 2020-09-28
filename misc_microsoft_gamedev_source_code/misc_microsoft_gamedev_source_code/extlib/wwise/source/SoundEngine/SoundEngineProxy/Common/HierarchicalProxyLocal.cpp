/***********************************************************************
  The content of this file includes source code for the sound engine
  portion of the AUDIOKINETIC Wwise Technology and constitutes "Level
  Two Source Code" as defined in the Source Code Addendum attached
  with this file.  Any use of the Level Two Source Code shall be
  subject to the terms and conditions outlined in the Source Code
  Addendum and the End User License Agreement for Wwise(R).

  Version: v2008.2.1  Build: 2821
  Copyright (c) 2006-2008 Audiokinetic Inc.
 ***********************************************************************/


#include "stdafx.h"

#include "HierarchicalProxyLocal.h"
#include "AkParentNode.h"
#include "AkCritical.h"


void HierarchicalProxyLocal::AddChild( AkUniqueID in_id )
{
	CAkAudioNode* pIndexable = static_cast<CAkAudioNode*>( GetIndexable() );
	if( pIndexable )
	{
		CAkFunctionCritical SpaceSetAsCritical;

		pIndexable->AddChild( in_id );
	}
}

void HierarchicalProxyLocal::RemoveChild( AkUniqueID in_id )
{
	CAkAudioNode* pIndexable = static_cast<CAkAudioNode*>( GetIndexable() );
	if( pIndexable )
	{	
		CAkFunctionCritical SpaceSetAsCritical;

		pIndexable->RemoveChild( in_id );
	}
}

void HierarchicalProxyLocal::RemoveAllChildren()
{
	CAkAudioNode* pIndexable = static_cast<CAkAudioNode*>( GetIndexable() );
	if( pIndexable )
	{
		CAkFunctionCritical SpaceSetAsCritical;

		pIndexable->RemoveAllChildren();
	}
}
