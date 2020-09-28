//-----------------------------------------------------------------------------
// File: indexed_tri.h
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

typedef int VertIndex;        // [0,numModelVerts)

typedef int EdgeIndex;        // [0,numModelEdges)
typedef int EdgeVertIndex; // [0,1]
typedef int EdgeSideIndex;  // [0,1]

typedef int TriVertIndex;  // [0,2]
typedef int TriEdgeIndex;  // [0,2]
typedef int TriIndex;          // [0,numModelTris)

typedef BDynamicArray<TriIndex> TriIndexVec;

enum 
{ 
   MaxTriVertIndex = 2,
   MaxTriEdgeIndex = 2,
   MaxEdgeVertIndex = 1,
   MaxEdgeSideIndex = 1,

   NumTriVerts = 3,
   NumTriEdges = 3,
   NumEdgeVerts = 2,
   NumEdgeSides = 2
};

template <class VertIndexT>
struct IndexedEdgeTemplate
{
   VertIndexT mVertIndex[NumEdgeVerts];

   IndexedEdgeTemplate()
   {
   }

   IndexedEdgeTemplate(VertIndexT l, VertIndexT m)
   {
      mVertIndex[0] = l;
      mVertIndex[1] = m;
   }

   VertIndexT operator[] (EdgeVertIndex i) const      { return mVertIndex[debugRangeCheck<int>(i, NumEdgeVerts)]; }
   VertIndexT& operator[] (EdgeVertIndex i)           { return mVertIndex[debugRangeCheck<int>(i, NumEdgeVerts)]; }

   VertIndexT  at(EdgeVertIndex i) const              { return (*this)[i]; }
   VertIndexT& at(EdgeVertIndex i)                    { return (*this)[i]; }
   
   int find(VertIndexT vertIndex) const
   {
      if (vertIndex == mVertIndex[0])     
         return 0;
      else if (vertIndex == mVertIndex[1]) 
         return 1;
      return cInvalidIndex;
   }
   
   bool equalEitherSide(const IndexedEdgeTemplate& b) const
   {
      return ((*this == b) || (side(1) == b));
   }

   IndexedEdgeTemplate side(EdgeSideIndex s) const
   {
      return IndexedEdgeTemplate(at(s), at(1 - s));
   }

   bool operator< (const IndexedEdgeTemplate& b) const
   {
      for (int i = 0; i < NumEdgeVerts; i++)
      {
         if (mVertIndex[i] < b.mVertIndex[i])
            return true;
         else if (mVertIndex[i] != b.mVertIndex[i])
            return false;
      }
      return false;
   }

   bool operator== (const IndexedEdgeTemplate& b) const
   {
      return (mVertIndex[0] == b.mVertIndex[0]) && (mVertIndex[1] == b.mVertIndex[1]);
   }

   operator size_t() const
   {
      return hashFast(this, sizeof(*this));
   }
};

typedef IndexedEdgeTemplate<uint> IndexedEdge;

template <class VertIndexT>
struct IndexedTriTemplate
{
   VertIndexT mVertIndex[NumTriVerts];

   IndexedTriTemplate()
   {
   }

   IndexedTriTemplate(VertIndexT l, VertIndexT m, VertIndexT n)
   {
      set(l, m, n);
   }
   
   void set(VertIndexT l, VertIndexT m, VertIndexT n)
   {
      mVertIndex[0] = l;
      mVertIndex[1] = m;
      mVertIndex[2] = n;
   }

   VertIndexT  operator[] (TriVertIndex i) const       { return mVertIndex[debugRangeCheck<int>(i, NumTriVerts)]; }
   VertIndexT& operator[] (TriVertIndex i)            { return mVertIndex[debugRangeCheck<int>(i, NumTriVerts)]; }

   VertIndexT  at(TriVertIndex i) const      { return (*this)[i]; }
   VertIndexT& at(TriVertIndex i)            { return (*this)[i]; }

   VertIndexT  atWrap(TriVertIndex i) const     { return mVertIndex[Math::iPosMod(i, NumTriVerts)]; }
   VertIndexT& atWrap(TriVertIndex i)           { return mVertIndex[Math::iPosMod(i, NumTriVerts)]; }
               
   IndexedTriTemplate& flip(void) 
   {
      const VertIndexT temp = mVertIndex[0];
      mVertIndex[0] = mVertIndex[2];
      mVertIndex[2] = temp;
      return *this;
   }

   IndexedTriTemplate& shift(void)
   {
      const VertIndexT temp = mVertIndex[0];
      mVertIndex[0] = mVertIndex[1];
      mVertIndex[1] = mVertIndex[2];
      mVertIndex[2] = temp;
   }

   IndexedTriTemplate flipped(void) const
   {
      return IndexedTriTemplate(mVertIndex[2], mVertIndex[1], mVertIndex[0]);
   }

   IndexedTriTemplate permuted(TriVertIndex p) const
   {
      return IndexedTriTemplate(mVertIndex[debugRangeCheck<int>(p, NumTriVerts)],  mVertIndex[Math::PosWrap<int>(1 + p, NumTriVerts)], mVertIndex[Math::PosWrap<int>(2 + p, NumTriVerts)]);
   }
   
   IndexedTriTemplate offsetted(int vertOfs) const
   {
      return IndexedTriTemplate(mVertIndex[0] + vertOfs, mVertIndex[1] + vertOfs, mVertIndex[2] + vertOfs);
   }
   
   IndexedTriTemplate& offset(int vertOfs) const
   {
      mVertIndex[0] += vertOfs;
      mVertIndex[1] += vertOfs;
      mVertIndex[2] += vertOfs;
      return *this;
   }

   TriEdgeIndex find(VertIndexT vertIndex) const
   {
      for (int i = 0; i < NumTriVerts; i++)
         if (vertIndex == mVertIndex[i])
            return i;
      return cInvalidIndex;
   }

   IndexedEdgeTemplate<VertIndexT> edge(TriEdgeIndex edgeIndex) const
   {
      debugRangeCheck<int>(edgeIndex, NumTriEdges);
      return IndexedEdgeTemplate<VertIndexT>(mVertIndex[edgeIndex], mVertIndex[Math::PosWrap(1 + edgeIndex, 3)]);
   }

   VertIndexT minVertIndex(void) const
   {
      return Math::Min3(mVertIndex[0], mVertIndex[1], mVertIndex[2]);
   }

   VertIndexT maxVertIndex(void) const
   {
      return Math::Max3(mVertIndex[0], mVertIndex[1], mVertIndex[2]);
   }

   TriVertIndex minTriVertIndex(void) const
   {
      return Math::Min3Index(mVertIndex[0], mVertIndex[1], mVertIndex[2]);
   }

   TriVertIndex maxTriVertIndex(void) const
   {
      return Math::Max3Index(mVertIndex[0], mVertIndex[1], mVertIndex[2]);
   }

   IndexedTriTemplate canonicalize(void) const
   {
      return permuted(minTriVertIndex());
   }

   bool isDegenerate(void) const
   {
      return (mVertIndex[0] == mVertIndex[1]) || (mVertIndex[0] == mVertIndex[2]) || (mVertIndex[1] == mVertIndex[2]);
   }

   bool operator< (const IndexedTriTemplate& b) const
   {
      for (int i = 0; i < NumTriVerts; i++)
      {
         if (mVertIndex[i] < b.mVertIndex[i])
            return true;
         else if (mVertIndex[i] != b.mVertIndex[i])
            return false;
      }
      return false;
   }

   bool operator== (const IndexedTriTemplate& b) const
   {
      return (mVertIndex[0] == b.mVertIndex[0]) && (mVertIndex[1] == b.mVertIndex[1]) && (mVertIndex[2] == b.mVertIndex[2]);
   }

   operator size_t() const
   {
      return ComputeHashFast(this, sizeof(*this));
   }
};

typedef IndexedTriTemplate<ushort> IndexedTri16;
typedef IndexedTriTemplate<uint> IndexedTri32;
typedef IndexedTri32 IndexedTri;

typedef BDynamicArray<IndexedTri16> IndexedTri16Vec;
typedef BDynamicArray<IndexedTri32> IndexedTri32Vec;
typedef IndexedTri32Vec IndexedTriVec;

