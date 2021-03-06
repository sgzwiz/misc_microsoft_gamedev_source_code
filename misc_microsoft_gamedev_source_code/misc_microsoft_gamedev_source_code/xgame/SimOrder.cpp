//==============================================================================
// SimOrder.cpp
// Copyright (c) 2007 Ensemble Studios
//==============================================================================

#include "common.h"
#include "SimOrder.h"
#include "SimOrderManager.h"
#include "SquadAI.h"
#include "unit.h"
#include "squad.h"
#include "world.h"


//==============================================================================
//==============================================================================
IMPLEMENT_FREELIST(BSimOrder, 5, &gSimHeap);

GFIMPLEMENTVERSION(BSimOrder, 2);
GFIMPLEMENTVERSION(BSimOrderEntry, 2);

//==============================================================================
//==============================================================================
bool BSimOrder::init()
{
   mOwnerID.invalidate();
   mID = 0;
   mTarget.reset();
   mWaypoints.clear();
   mRefCount = 0;
   mMode = -1;
   mAngle = 0.0f;
   mPriority = cPrioritySim;
   mAttackMove = false;
   mAutoGeneratedAttackMove = false;
   mFlagOverridePosition = false;
   mFlagOverrideRange = false;
   return (true);
}

//==============================================================================
//==============================================================================
const char* BSimOrder::getTypeName(BSimOrderType type) const
{
   switch (type)
   {
      case cTypeNone: return("None"); 
	   case cTypeMove: return("Move"); 
	   case cTypeAttack: return("Attack"); 
	   case cTypeGather: return("Gather"); 
	   case cTypeRepair: return("Repair"); 
	   case cTypeCapture: return("Capture"); 
      case cTypeJoin: return("Join"); 
	   case cTypeGarrison: return("Garrison"); 
      case cTypeUngarrison: return ("Ungarrison");
	   case cTypeDetonate: return("Detonate"); 
      case cTypeCloak: return("Cloak");
	   case cTypeHonk: return("Honk"); 
	   case cTypeChangeMode: return("ChangeMode"); 
	   case cTypeMines: return("Mines"); 
	   case cTypePlayBlockingAnimation: return("PlayBlockingAnimation"); 
	   case cTypeRallyPoint: return("RallyPoint"); 
	   case cTypeIdle: return("Idle"); 
      case cTypeUnpack: return("Unpack");
	   case cTypeRequestRepair: return("RequestRepair"); 
	   case cTypeBuild: return("Build"); 
	   case cTypeGuard: return("Guard"); 
      case cTypeHitch: return ("Hitch");
      case cTypeUnhitch: return ("Unhitch");
      case cTypeTransport: return ("Transport");
      case cTypeWander: return ("Wander");
      case cTypeJump: return ("Jump");
      case cTypeJumpGather: return ("JumpGather");
      case cTypeJumpGarrison: return ("JumpGarrison");
      case cTypeJumpAttack: return ("JumpAttack");
      case cTypePointBlankAttack: return ("PointBlankAttack");
      case cTypeEnergyShield: return ("EnergyShield");
      case cTypeJumpPull: return ("JumpPull");
      case cTypeInfantryEnergyShield: return ("InfantryEnergyShield");
   }
   return("Unknown");
}

//==============================================================================
//==============================================================================
bool BSimOrder::setWaypoints(const BDynamicVectorArray& waypoints)
{
   mWaypoints.setNumber(waypoints.getSize());
   memcpy(mWaypoints.getPtr(), waypoints.getPtr(), sizeof(BVector)*mWaypoints.getSize());
   return(true);
}

//==============================================================================
//==============================================================================
bool BSimOrder::setWaypoints(const BDynamicSimVectorArray& waypoints)
{
   mWaypoints.setNumber(waypoints.getSize());
   memcpy(mWaypoints.getPtr(), waypoints.getPtr(), sizeof(BVector)*mWaypoints.getSize());
   return(true);
}

//==============================================================================
//==============================================================================
bool BSimOrder::operator==(const BSimOrder& order) const
{
   if ((mOwnerID != order.mOwnerID) ||
      (mTarget != order.mTarget) ||
      (mMode != order.mMode) ||
      (mAngle != order.mAngle) ||
      (mPriority != order.mPriority) ||
      (mAttackMove != order.mAttackMove) ||
      (mAutoGeneratedAttackMove != order.mAutoGeneratedAttackMove) ||
      (mFlagOverridePosition != order.mFlagOverridePosition) ||
      (mFlagOverrideRange != order.mFlagOverrideRange) ||
      (mWaypoints.getSize() != order.mWaypoints.getSize()))
      return (false);
   for (uint i=0; i < mWaypoints.getSize(); i++)
   {
      if (mWaypoints[i] != order.mWaypoints[i])
         return(false);
   }
   return (true);
}

//==============================================================================
//==============================================================================
bool BSimOrder::operator!=(const BSimOrder& order) const
{
   return (!(*this == order));
}

//==============================================================================
//==============================================================================
bool BSimOrder::isSimilarOrder(BSimOrder* pOrder)
{  
   //-- We need to do a squad to squad entityID compare
   BEntityID ourSquadID = mTarget.getID();
   BEntityID theirSquadID = pOrder->mTarget.getID();
   
   if(theirSquadID.getType() == BEntity::cClassTypeUnit)
   {
      BUnit* pUnit = gWorld->getUnit(theirSquadID);
      if(pUnit)
      {
         BSquad *pSquad = pUnit->getParentSquad();
         if(pSquad)
            theirSquadID = pSquad->getID();
      }
   }

   if(ourSquadID.getType() == BEntity::cClassTypeUnit)
   {
      BUnit* pUnit = gWorld->getUnit(ourSquadID);
      if(pUnit)
      {
         BSquad *pSquad = pUnit->getParentSquad();
         if(pSquad)
            ourSquadID = pSquad->getID();
      }
   }

   if(theirSquadID.getType() != BEntity::cClassTypeSquad || ourSquadID.getType() != BEntity::cClassTypeSquad)
      return false;

   bool sameTargetID = theirSquadID == ourSquadID;

   // Only check positions if target IDs not specified for both squads
   bool samePosition = sameTargetID;
   if ((theirSquadID == cInvalidObjectID) && (ourSquadID == cInvalidObjectID))
      samePosition = (mTarget.getPosition() == pOrder->mTarget.getPosition()) != 0;

   //-- Is the command same enough to not have to execute it?
   if ( (!sameTargetID && !samePosition) ||
      (mMode != pOrder->mMode) ||
      (mAngle != pOrder->mAngle) ||
      (mAttackMove != pOrder->mAttackMove) ||
      (mAutoGeneratedAttackMove != pOrder->mAutoGeneratedAttackMove) ||
      (mFlagOverridePosition != pOrder->mFlagOverridePosition) ||
      (mFlagOverrideRange != pOrder->mFlagOverrideRange) ||
      (mTarget.getAbilityID() != pOrder->mTarget.getAbilityID()))      
      return (false);

   //-- Check the waypoints if the targetID's are not the same
   if(!sameTargetID)
   {
      for (uint i=0; i < mWaypoints.getSize(); i++)
      {
         if (mWaypoints[i] != pOrder->mWaypoints[i])
            return(false);
      }
   }

   return (true);
}

//==============================================================================
//==============================================================================
void BSimOrder::copyFromTemplate(const BSimOrder& ref)
{
   // copy everything but the mID and mRefCount
   mOwnerID=ref.mOwnerID;
   mTarget=ref.mTarget;
   uint count=ref.mWaypoints.getSize();
   if (count > 0)
   {
      mWaypoints.setNumber(count);
      for (uint i=0; i < count; i++)
         mWaypoints[i]=ref.mWaypoints[i];
   }
   mMode=ref.mMode;
   mAngle=ref.mAngle;
   mPriority=ref.mPriority;
   mAttackMove=ref.mAttackMove;
   mAutoGeneratedAttackMove = ref.mAutoGeneratedAttackMove;
   mFlagOverridePosition = ref.mFlagOverridePosition;
   mFlagOverrideRange = ref.mFlagOverrideRange;
}

//==============================================================================
//==============================================================================
const char* BSimOrderEntry::getStateName() const
{
   switch (mState)
   {
      case cStateNone: return("None"); 
	   case cStateQueued: return("Queued"); 
	   case cStatePaused: return("Paused"); 
	   case cStateExecute: return("Execute");
   }
   return ("Unknown");
}

//==============================================================================
//==============================================================================
bool BSimOrderEntry::operator==(const BSimOrderEntry& v) const
{
   if ((mOrder != v.mOrder) ||
      (mType != v.mType) ||
      (mState != v.mState) || 
      (mFlagRemoveMe != v.mFlagRemoveMe) || 
      (mFlagRemoveMeWhenInterruptible != v.mFlagRemoveMeWhenInterruptible))
      return (false);
   return (true);
}

//==============================================================================
//==============================================================================
bool BSimOrderEntry::operator!=(const BSimOrderEntry& v) const
{
   return (!(*this == v));
}

//==============================================================================
//==============================================================================
bool BSimOrder::save(BStream* pStream, int saveType) const
{
   GFWRITECLASS(pStream, saveType, mTarget);
   GFWRITEVECTORARRAY(pStream, mWaypoints, uint8, 200);
   GFWRITEVAR(pStream, BEntityID, mOwnerID);
   GFWRITEVAR(pStream, uint, mID);
   GFWRITEVAR(pStream, uint, mRefCount);
   GFWRITEVAR(pStream, float, mAngle);
   GFWRITEVAR(pStream, int8, mMode);
   GFWRITEVAL(pStream, uint8, mPriority);
   GFWRITEBITBOOL(pStream, mAttackMove);
   GFWRITEBITBOOL(pStream, mFlagOverridePosition);
   GFWRITEBITBOOL(pStream, mFlagOverrideRange);
   GFWRITEBITBOOL(pStream, mAutoGeneratedAttackMove);
   return true;
}

//==============================================================================
//==============================================================================
bool BSimOrder::load(BStream* pStream, int saveType)
{
   GFREADCLASS(pStream, saveType, mTarget);
   GFREADVECTORARRAY(pStream, mWaypoints, uint8, 200);
   GFREADVAR(pStream, BEntityID, mOwnerID);
   GFREADVAR(pStream, uint, mID);
   GFREADVAR(pStream, uint, mRefCount);
   GFREADVAR(pStream, float, mAngle);
   GFREADVAR(pStream, int8, mMode);
   GFREADVAL(pStream, uint8, uint8, mPriority);
   GFREADBITBOOL(pStream, mAttackMove);
   GFREADBITBOOL(pStream, mFlagOverridePosition);
   GFREADBITBOOL(pStream, mFlagOverrideRange);
   if (BSimOrder::mGameFileVersion >= 2)
   {
      GFREADBITBOOL(pStream, mAutoGeneratedAttackMove);
   }
   else
   {
      mAutoGeneratedAttackMove = false;
   }
   return true;
}

//==============================================================================
//==============================================================================
bool BSimOrderEntry::save(BStream* pStream, int saveType) const
{
   GFWRITEFREELISTITEMPTR(pStream, BSimOrder, mOrder);
   GFWRITEVAR(pStream, BSimOrderType, mType);
   GFWRITEVAL(pStream, uint8, mState);
   GFWRITEBITBOOL(pStream, mFlagRemoveMe);
   GFWRITEBITBOOL(pStream, mFlagRemoveMeWhenInterruptible);
   return true;
}

//==============================================================================
//==============================================================================
bool BSimOrderEntry::load(BStream* pStream, int saveType)
{
   GFREADFREELISTITEMPTR(pStream, BSimOrder, mOrder);
   GFREADVAR(pStream, BSimOrderType, mType);
   GFREADVAL(pStream, uint8, uint, mState);
   GFREADBITBOOL(pStream, mFlagRemoveMe);
   if (BSimOrderEntry::mGameFileVersion >= 2)
   {
      GFREADBITBOOL(pStream, mFlagRemoveMeWhenInterruptible);
   }
  return true;
}