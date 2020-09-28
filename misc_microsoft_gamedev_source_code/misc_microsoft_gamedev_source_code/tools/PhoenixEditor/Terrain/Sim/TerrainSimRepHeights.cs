using System;
using System.IO;
using System.Diagnostics;
using System.Collections.Generic;
using Microsoft.DirectX;
using Microsoft.DirectX.Direct3D;
using System.Threading;

using EditorCore;
using Rendering;

namespace Terrain
{

   public class SimHeightRep : SpatialQuadTree
   {
      uint mWidth = 32;
      uint mHeight = 32;
      uint mNumXVertsPerCell = 0;
      uint mNumZVertsPerCell = 0;

      float mTileScale;

      float[] mHeights;
      public bool mRenderHeights = false;
      static float cVisualHeightOffset = 0.1f;

      //----------------------------------
      public SimHeightRep()
      {

      }
      ~SimHeightRep()
      {
         destroy();
      }

      public void init(int numXVerts, int numZVerts, float tileScale)
      {
         mWidth = (uint)numXVerts + 1;
         mHeight = (uint)numZVerts + 1;
         mHeights = new float[mWidth * mHeight];
         initHeightOverride();

         float terrainVisToSimMultipleX = (mWidth - 1) / (float)TerrainGlobals.getTerrain().getNumXVerts();
         float terrainVisToSimMultipleZ = (mHeight - 1) / (float)TerrainGlobals.getTerrain().getNumZVerts();
         mTileScale = TerrainGlobals.getTerrain().getTileScale() * (1.0f / terrainVisToSimMultipleX);

         createSimHeightRepQuadTree(mWidth, mHeight);
         recalculateBBs();
      }

      new public void destroy()
      {
         base.destroy();
         destroyHeightOverride();
         if (mHeights != null)
         { 
            mHeights = null;
         }
      }
      //----------------------------------

      class heightGenWorkerData
      {
         public heightGenWorkerData(uint minX, uint minZ, uint maxX, uint maxZ)
         {
            minXVert = minX;
            minZVert = minZ;
            maxXVert = maxX;
            maxZVert = maxZ;
         }
         public uint minXVert = 0;
         public uint minZVert = 0;
         public uint maxXVert = 0;
         public uint maxZVert = 0;
      }
      int mWorkInc = 4;
      AutoResetEvent mWorkCompleted = new AutoResetEvent(false);


     
      public void calcHeightRaycastWorker(object workerObjectDat)
      {
         Random rnd = new Random();

         heightGenWorkerData packet = workerObjectDat as heightGenWorkerData;

         bool doRacyast = true;
         for (int x = (int)packet.minXVert; x < (int)packet.maxXVert; x++)
         {
            for (int z = (int)packet.minZVert; z < (int)packet.maxZVert; z++)
            {

               if(!doRacyast)
               {
                  float u = (float)(x / ((float)mWidth - 1));
                  float v = (float)(z / ((float)mHeight - 1));
                  float h = TerrainGlobals.getTerrain().getHeightParametric(u, v);

                  if (h > mHeights[x * mWidth + z]) 
                  {
                     mHeights[x * mWidth + z] = h;
                  }
               }
               else if(doRacyast)
               {
                  //only if we're used in a tile that is passable
                  bool sharesPassable = false;

                  if (!TerrainGlobals.getEditor().getSimRep().getDataTiles().isLandObstructedComposite(x, z)) sharesPassable |= true;
                  if (!TerrainGlobals.getEditor().getSimRep().getDataTiles().isLandObstructedComposite(x - 1, z)) sharesPassable |= true;
                  if (!TerrainGlobals.getEditor().getSimRep().getDataTiles().isLandObstructedComposite(x - 1, z - 1)) sharesPassable |= true;
                  if (!TerrainGlobals.getEditor().getSimRep().getDataTiles().isLandObstructedComposite(x, z - 1)) sharesPassable |= true;



                  if (sharesPassable)
                  {
                     float xzoff = 0.01f;
                     Vector3 origin = new Vector3(x * mTileScale + xzoff, TerrainGlobals.getTerrain().getBBMax().Y, z * mTileScale + xzoff);
                     Vector3 dir = -BMathLib.unitY;
                     Vector3 intPt = new Vector3();
                     BTerrainQuadNode ownerNode = new BTerrainQuadNode();
                     if (TerrainGlobals.getTerrain().rayIntersects(ref origin, ref dir, ref intPt, ref ownerNode, false))
                     {
                        if (intPt.Y > mHeights[x * mWidth + z])
                        {
                           mHeights[x * mWidth + z] = intPt.Y + 0.001f;
                        }

                     }
                     else
                     {


                        // mHeights[x * mWidth + z] = 200.0f;

                        if (x != 0 && z != 0 && x < mWidth && z < mHeight)
                           break;

                        bool hit = false;

                        for (int i = 0; i < 4; i++)
                        {
                           dir = -BMathLib.unitY;
                           origin.X = (x * mTileScale + xzoff) + ((float)(rnd.NextDouble() * 0.2f) - 0.1f);
                           origin.Z = (z * mTileScale + xzoff) + ((float)(rnd.NextDouble() * 0.2f) - 0.1f);
                           //origin.Y = TerrainGlobals.getTerrain().getBBMax().Y;
                           if (TerrainGlobals.getTerrain().rayIntersects(ref origin, ref dir, ref intPt, ref ownerNode, false))
                           {
                              if (intPt.Y > mHeights[x * mWidth + z])
                              {
                                 mHeights[x * mWidth + z] = intPt.Y;
                                 hit = true;
                                 break;
                              }
                           }
                        }
                     }
                  }
               }
               

            }
         }
         Interlocked.Decrement(ref mWorkInc);
         if (mWorkInc <= 0)
            mWorkCompleted.Set();
      }

      public void recalculateHeights(bool doExtraRaycastCalc)
      {
         recalculateHeights(true, doExtraRaycastCalc);
      }

      public void recalculateHeights(bool recalcVis, bool doExtraRaycastCalc)
      {
         //generate our heights representation from the terrain, 
         //then add our jagged differences.
         int xRes = 256;
         float[] gpuHeightRepArray = new float[xRes * xRes];
         for (uint i = 0; i < xRes * xRes; i++)
            gpuHeightRepArray[i] = float.MinValue;
         HeightsGen hg = new HeightsGen();
         hg.computeHeightFieldDirectFloat((uint)xRes, (uint)xRes, true, -BMathLib.unitX, true, ref gpuHeightRepArray);

         hg.destroy();
         hg = null;


         //resize our terrain input to our local input.

        // mHeights = null;
         //mHeights = ImageManipulation.resizeF32Img(gpuHeightRepArray, xRes, xRes, (int)mWidth, (int)mHeight, ImageManipulation.eFilterType.cFilter_Linear); //new float[mWidth * mHeight];

         float[] tmpHeightRes = ImageManipulation.resizeF32Img(gpuHeightRepArray, xRes, xRes, (int)mWidth - 1, (int)mHeight - 1, ImageManipulation.eFilterType.cFilter_Linear);

         for (int x = 0; x < mWidth - 1; x++)
         {
            for (int z = 0; z < mHeight - 1; z++)
            {
               mHeights[x * mWidth + z] = tmpHeightRes[x * (mWidth - 1) + z];
            }
         }


         //fill our edges with the previous height val.
         for (int q = 0; q < mWidth - 1; q++)
         {
            mHeights[(mWidth - 1) * mWidth + q] = mHeights[(mWidth - 2) * mWidth + q];
            mHeights[q * mWidth + (mWidth - 1)] = mHeights[q * mWidth + (mWidth - 2)];
         }

         //Let's be douchebagish about this..
         if (doExtraRaycastCalc)
         {
            

            // CLM [11.10.08] Disabled due to problems called by rays not being 100% accurate
            //{
            //   mWorkInc = 4;
            //   ThreadPool.QueueUserWorkItem(new WaitCallback(calcHeightRaycastWorker), new heightGenWorkerData(0, 0, mWidth >> 1, mHeight >> 1));
            //   ThreadPool.QueueUserWorkItem(new WaitCallback(calcHeightRaycastWorker), new heightGenWorkerData(1 + (mWidth >> 1), 0, mWidth, mHeight >> 1));
            //   ThreadPool.QueueUserWorkItem(new WaitCallback(calcHeightRaycastWorker), new heightGenWorkerData(0, 1 + (mHeight >> 1), mWidth >> 1, mHeight));
            //   ThreadPool.QueueUserWorkItem(new WaitCallback(calcHeightRaycastWorker), new heightGenWorkerData(1 + (mWidth >> 1), 1 + (mHeight >> 1), mWidth, mHeight));
            //}
          
            {
               mWorkInc = 1;
               ThreadPool.QueueUserWorkItem(new WaitCallback(calcHeightRaycastWorker), new heightGenWorkerData(0, 0, mWidth, mHeight));
            }

            
            mWorkCompleted.WaitOne();
         }
         

         tmpHeightRes = null;
         gpuHeightRepArray = null;
         recalculateBBs();
         if (recalcVis)
            recalculateVisuals();
      }
      //----------------------------------
      public float getHeight(int x, int z)
      {
         if (x < 0 || z < 0)
            return cJaggedEmptyValue;

         if (x >= mWidth - 1) x = (int)(mWidth - 1u);
         if (z >= mHeight - 1) z = (int)(mHeight - 1u);


         return mHeights[x * mWidth + z];
      }
      public float getCompositeHeight(int x, int z)
      {
         if (x < 0 || z < 0)
            return cJaggedEmptyValue;

         if (x >= mWidth - 1) x = (int)(mWidth - 1u);
         if (z >= mHeight - 1) z = (int)(mHeight - 1u);


         float h = getHeight(x, z);

         float jh = getJaggedHeight(x, z);
         if (jh == cJaggedEmptyValue)
            return h;

         return jh;
      }
      public float3 getWorldspacePoint(int x, int z)
      {
         if (x < 0 || z < 0)
            return float3.Empty;

         if (x >= mWidth - 1) x = (int)(mWidth - 1);
         if (z >= mHeight - 1) z = (int)(mHeight - 1);


         return new float3(x * mTileScale, getCompositeHeight(x, z), z * mTileScale);
      }

      public float getInterpolatedHeightParametric(float x, float z)
      {
         float mTotalWidth = mWidth * mTileScale;
         float mTotalheight = mHeight * mTileScale;

         float u = x / mTotalWidth;
         float v = z / mTotalheight;

         return getCompositeHeightParametric(u, v);
      }

      public float getCompositeHeightParametric(float u, float v)
      {
         float x = u * mWidth;
         float z = v * mHeight;
         int iX = (int)x;
         int iZ = (int)z;

         float y0 = getCompositeHeight(iX, iZ);
         float y1 = getCompositeHeight(iX, iZ + 1);
         float y2 = getCompositeHeight(iX + 1, iZ + 1);
         float y3 = getCompositeHeight(iX + 1, iZ);

         float uFrac = x - iX;
         float vFrac = z - iZ;

         float topX = BMathLib.LERP(y1, y2, uFrac);
         float botX = BMathLib.LERP(y0, y3, uFrac);

         float retHeight = BMathLib.LERP(botX, topX, vFrac);

         return retHeight;
      }
      public uint getNumXPoints() { return mWidth; }
      public uint getNumZPoints() { return mHeight; }
      public float getTileScale() { return mTileScale; }
      public Vector3 getLastIntersectionPoint() { return mLastMoustIntPt.toVec3(); }
      //----------------------------------
      public static float cJaggedEmptyValue = -9999;
      private JaggedContainer<float> mHeightOverride = null;
      public void initHeightOverride()
      {
         mHeightOverride = new JaggedContainer<float>((int)(mWidth * mHeight));
         mHeightOverride.SetEmptyValue(cJaggedEmptyValue);
      }
      public void destroyHeightOverride()
      {
         if (mHeightOverride != null)
         {
            clearHeightOverride();
            mHeightOverride = null;
         }
      }
      public void clearHeightOverride()
      {
         mHeightOverride.Clear();
      }
      public void createJaggedFrom(JaggedContainer<float> v)
      {
         destroyHeightOverride();
         initHeightOverride();

         long id;
         float maskValue;
         v.ResetIterator();
         while (v.MoveNext(out id, out maskValue))
         {
            if (maskValue == cJaggedEmptyValue)
               continue;

            mHeightOverride.SetValue(id, maskValue);
         }
      }
      public void setJaggedHeight(int x, int z, float val)
      {
         if (x < 0 || x >= mWidth || z < 0 || z >= mHeight)
            return;

         int idx = (int)(x * mWidth + z);
         mHeightOverride.SetValue(idx, val);
      }
      public float getJaggedHeight(int x, int z)
      {
         if (x < 0 || x >= mWidth || z < 0 || z >= mHeight)
            return cJaggedEmptyValue;

         return getJaggedHeight((int)(x * mWidth + z));
      }
      public float getJaggedHeight(int idx)
      {
         return mHeightOverride.GetValue(idx);
      }
      public JaggedContainer<float> getJaggedHeight()
      {
         return mHeightOverride;
      }

      //----------------------------------
      public void recalculateBBs()
      {
         for (uint minx = 0; minx < mNumXLeafCells; minx++)
         {
            for (uint minz = 0; minz < mNumZLeafCells; minz++)
            {
               SpatialQuadTreeCell spatialCell = getLeafCellAtGridLoc(minx, minz);
               recalculateCellBB(spatialCell);
            }
         }
         mRootCell.updateBoundsFromChildren();
      }
      void recalculateCellBB(SpatialQuadTreeCell spatialCell)
      {
         BBoundingBox bb = new BBoundingBox();
         bb.empty();
         SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)spatialCell.mExternalData);
         for (uint i = (uint)cell.mMinXVert; i <= cell.mMinXVert + mNumXVertsPerCell; i++)
         {
            for (uint j = (uint)cell.mMinZVert; j <= cell.mMinZVert + mNumZVertsPerCell; j++)
            {
               if (i >= mWidth || j >= mHeight)
                  continue;

               float3 worldPos = getWorldspacePoint((int)i, (int)j);
               bb.addPoint(worldPos.toVec3());

            }
         }
         spatialCell.setBounds(new float3(bb.min), new float3(bb.max));
         bb = null;
      }
      //----------------------------------
      public void render(bool renderCursor)
      {
         if (!mRenderHeights)
            return;

         List<SpatialQuadTreeCell> nodes = new List<SpatialQuadTreeCell>();
         getVisibleNodes(nodes, TerrainGlobals.getTerrain().getFrustum());

         //update any visual handles that need it..
         for (int i = 0; i < nodes.Count; i++)
         {
            SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)nodes[i].mExternalData);
            Debug.Assert(cell != null);

            if (cell.mVisualHandle == null)
               cell.mVisualHandle = newVisualHandle((int)cell.mMinXVert, (int)cell.mMinZVert);

            renderCell(cell.mVisualHandle);
         }

         if (renderCursor)
            ((BTerrainSimBrush)TerrainGlobals.getEditor().getCurrentBrush()).render();
      }
      void renderCell(SimHeightVisualData handle)
      {
         BRenderDevice.getDevice().VertexDeclaration = VertexTypes.Pos_Color.vertDecl;
         BRenderDevice.getDevice().VertexFormat = VertexTypes.Pos_Color.FVF_Flags;

         BRenderDevice.getDevice().VertexShader = null;
         BRenderDevice.getDevice().PixelShader = null;
         BRenderDevice.getDevice().SetTexture(0, null);

         BRenderDevice.getDevice().SetStreamSource(0, handle.mVB, 0);
         BRenderDevice.getDevice().Indices = handle.mIB;

         BRenderDevice.getDevice().RenderState.CullMode = Cull.None;

         BRenderDevice.getDevice().SetRenderState(RenderStates.ZBufferWriteEnable, false);
         BRenderDevice.getDevice().SetRenderState(RenderStates.FillMode, (int)FillMode.WireFrame);
         BRenderDevice.getDevice().DrawPrimitives(PrimitiveType.TriangleList, 0, handle.mNumPrims);
         
         BRenderDevice.getDevice().SetRenderState(RenderStates.FillMode, (int)FillMode.Solid);

         BRenderDevice.getDevice().SetRenderState(RenderStates.AlphaTestEnable, false);

         BRenderDevice.getDevice().SetRenderState(RenderStates.SourceBlend, (int)Blend.SourceAlpha);
         BRenderDevice.getDevice().SetRenderState(RenderStates.DestinationBlend, (int)Blend.InvSourceAlpha);
         BRenderDevice.getDevice().SetRenderState(RenderStates.AlphaBlendEnable, true);

         BRenderDevice.getDevice().DrawPrimitives(PrimitiveType.TriangleList, 0, handle.mNumPrims);
         
         BRenderDevice.getDevice().SetRenderState(RenderStates.AlphaBlendEnable, false);
         BRenderDevice.getDevice().SetRenderState(RenderStates.ZBufferWriteEnable, true);
         BRenderDevice.getDevice().RenderState.CullMode = Cull.CounterClockwise;

      }
      unsafe SimHeightVisualData newVisualHandle(int minX, int minZ)
      {
         int width = (int)mNumXVertsPerCell;
         int vd = width + 1;
         int tw = width;
         int td = width;


         SimHeightVisualData svd = new SimHeightVisualData();
         int numVertsPerTile = 6;
         svd.mNumVerts = width * numVertsPerTile * width * numVertsPerTile;
         svd.mVB = new VertexBuffer(typeof(VertexTypes.Pos_Color), (int)svd.mNumVerts, BRenderDevice.getDevice(), Usage.None, VertexTypes.Pos_Color.FVF_Flags, Pool.Managed);

         //standard IB
         svd.mNumPrims = width * width * 2;

         //update and fill our vertex buffer
         updateVisualHandle(ref svd, minX, minZ);

         return svd;

      }
      unsafe void updateVisualHandle(ref SimHeightVisualData handle, int minX, int minZ)
      {
         if (handle == null)
            return;

         int width = (int)(mNumXVertsPerCell + 1);
         int numTiles = (int)(mNumXVertsPerCell);

         GraphicsStream stream = handle.mVB.Lock(0, handle.mNumVerts * sizeof(VertexTypes.Pos_Color), LockFlags.None);
         VertexTypes.Pos_Color* verts = (VertexTypes.Pos_Color*)stream.InternalDataPointer;
         BTerrainSimRep.eChannels channel = TerrainGlobals.getEditor().getSimRep().getChannel();
         //generate each tile as a seperate triList
         int counter = 0;
         for (int x = 0; x < numTiles; x++)
         {
            for (int z = 0; z < numTiles; z++)
            {
               int offX = (int)BMathLib.Clamp(minX + x, 0, mWidth - 1);
               int offZ = (int)BMathLib.Clamp(minZ + z, 0, mHeight - 1);

               //calculate our tile positions
               float3 wsp = getWorldspacePoint(offX, offZ);// new float3(offX * mTileScale, getCompositeHeight(offX, offZ), offZ * mTileScale);
               float3[] xyzVals = new float3[4];
               xyzVals[0] = getWorldspacePoint(minX + x, minZ + z);
               xyzVals[1] = getWorldspacePoint(minX + x, minZ + z + 1);
               xyzVals[2] = getWorldspacePoint(minX + x + 1, minZ + z + 1);
               xyzVals[3] = getWorldspacePoint(minX + x + 1, minZ + z);


               //Determine our tile COLOR & VISUALIZATION MODE
               int xT = minX + x;
               int zT = minZ + z;
               if (xT >= TerrainGlobals.getEditor().getSimRep().getNumXTiles()) xT = TerrainGlobals.getEditor().getSimRep().getNumXTiles() - 1;
               if (zT >= TerrainGlobals.getEditor().getSimRep().getNumXTiles()) zT = TerrainGlobals.getEditor().getSimRep().getNumXTiles() - 1;

               int obsCol = 0x7FFFFFFF;
               bool obstruction = false;
               if (channel == BTerrainSimRep.eChannels.cObstrtuctionChannel)
               {
                  if (TerrainGlobals.getEditor().getSimRep().getDataTiles().isTileLandObstructed(xT, zT) == true)
                     obstruction |= true;
               }
               else if (channel == BTerrainSimRep.eChannels.cBuildableChannel)
               {
                  
                  if (TerrainGlobals.getEditor().getSimRep().getDataTiles().isBuildable(xT, zT) == true)
                     obstruction |= true;
               }
               else if (channel == BTerrainSimRep.eChannels.cFloodObstructionChannel)
               {

                  if (TerrainGlobals.getEditor().getSimRep().getDataTiles().isFloodObstructed(xT, zT) == true)
                     obstruction |= true;
               }
               else if (channel == BTerrainSimRep.eChannels.cScarabObstructionChannel)
               {
                  if (TerrainGlobals.getEditor().getSimRep().getDataTiles().isScarabObstructed(xT, zT) == true)
                     obstruction |= true;
               }
               
               obsCol = obstruction ? TerrainGlobals.getEditor().getSimRep().getChannelNegColor(channel) : TerrainGlobals.getEditor().getSimRep().getChannelPosColor(channel);

               //tile type colors are defined by a text file, so we need to query for them
               if (channel == BTerrainSimRep.eChannels.cTileTypeChannel)
               {
                  int tileTypeOverride = TerrainGlobals.getEditor().getSimRep().getDataTiles().getJaggedTileType(xT, zT);
                  if (tileTypeOverride != 0)
                  {
                     obsCol = (int)TerrainGlobals.getEditor().getSimRep().getDataTiles().getTileTypeColor(tileTypeOverride);  
                  }
               }


               const float heightShift = 0.1f;
               const float xShift = 0;// -0.5f;
               const float zShift = 0;// -0.5f;
               //update our tiles
               verts[counter].x = xyzVals[0].X + xShift;
               verts[counter].y = xyzVals[0].Y + heightShift;
               verts[counter].z = xyzVals[0].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

               verts[counter].x = xyzVals[1].X + xShift;
               verts[counter].y = xyzVals[1].Y + heightShift;
               verts[counter].z = xyzVals[1].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

               verts[counter].x = xyzVals[2].X + xShift;
               verts[counter].y = xyzVals[2].Y + heightShift;
               verts[counter].z = xyzVals[2].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

               verts[counter].x = xyzVals[0].X + xShift;
               verts[counter].y = xyzVals[0].Y + heightShift;
               verts[counter].z = xyzVals[0].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

               verts[counter].x = xyzVals[2].X + xShift;
               verts[counter].y = xyzVals[2].Y + heightShift;
               verts[counter].z = xyzVals[2].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

               verts[counter].x = xyzVals[3].X + xShift;
               verts[counter].y = xyzVals[3].Y + heightShift;
               verts[counter].z = xyzVals[3].Z + zShift;
               verts[counter].color = obsCol;
               counter++;

            }
         }
         handle.mVB.Unlock();
      }
      public void recalculateVisuals()
      {
         for (uint minx = 0; minx < mNumXLeafCells; minx++)
         {
            for (uint minz = 0; minz < mNumZLeafCells; minz++)
            {
               SpatialQuadTreeCell spatialCell = getLeafCellAtGridLoc(minx, minz);
               SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)spatialCell.mExternalData);
               updateVisualHandle(ref cell.mVisualHandle, cell.mMinXVert, cell.mMinZVert);
            }
         }
      }
      public void updateAfterPainted(int minX, int minZ, int maxX, int maxZ)
      {
         //verts are in our local stride.
         //find any chunks intersecting these numbers and update them.
         uint minXCell = (uint)BMathLib.Clamp((minX / mNumXVertsPerCell) - 1, 0, mNumXLeafCells);
         uint minZCell = (uint)BMathLib.Clamp((minZ / mNumZVertsPerCell) - 1, 0, mNumXLeafCells);
         uint maxXCell = (uint)BMathLib.Clamp((maxX / mNumXVertsPerCell) + 1, 0, mNumXLeafCells);
         uint maxZCell = (uint)BMathLib.Clamp((maxZ / mNumZVertsPerCell) + 1, 0, mNumXLeafCells);


         for (uint minx = minXCell; minx < maxXCell; minx++)
         {
            for (uint minz = minZCell; minz < maxZCell; minz++)
            {
               SpatialQuadTreeCell spatialCell = getLeafCellAtGridLoc(minx, minz);
               SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)spatialCell.mExternalData);
               updateVisualHandle(ref cell.mVisualHandle, cell.mMinXVert, cell.mMinZVert);
               recalculateCellBB(spatialCell);
            }
         }


         mRootCell.updateBoundsFromChildren();
      }
      //----------------------------------
      public void createSimHeightRepQuadTree(uint numXVerts, uint numZVerts)
      {
         Vector3 min = new Vector3(0, 0, 0);
         Vector3 max = new Vector3(numXVerts * mTileScale, 10, numZVerts * mTileScale);

         uint numvertspercell = (uint)(BTerrainQuadNode.cMaxWidth * TerrainGlobals.getEditor().getSimRep().getVisToSimScale());
         uint numNodesTest = numXVerts / numvertspercell;
         if (numNodesTest * numvertspercell < mWidth)
            numNodesTest++;

         uint numXNodes = numNodesTest;// (uint)(TerrainGlobals.getTerrain().getNumXVerts() / BTerrainQuadNode.cMaxWidth);// +1;
         // while (numXNodes > 1) { numXNodes = numXNodes >> 1; numLevels++; }

         base.createSpatialQuadTree(new float3(min), new float3(max), numXNodes, numXNodes);//numLevels);

         mNumXVertsPerCell = numvertspercell;// mWidth / mNumXLeafCells;
         mNumZVertsPerCell = numvertspercell;//mHeight / mNumZLeafCells;

         //initalize our leaf nodes
         for (uint i = 0; i < mNumXLeafCells; i++)
         {
            for (uint j = 0; j < mNumZLeafCells; j++)
            {
               SpatialQuadTreeCell cell = getLeafCellAtGridLoc(i, j);
               if (cell != null)
               {
                  cell.mExternalData = new SimHeightRepQuadCell();
                  ((SimHeightRepQuadCell)cell.mExternalData).mMinXVert = (int)(i * mNumXVertsPerCell);
                  ((SimHeightRepQuadCell)cell.mExternalData).mMinZVert = (int)(j * mNumZVertsPerCell);
               }
            }
         }
      }
      //----------------------------------
      void getPointsIntersectingSphere(List<int> points, float3 sphereCenter, float sphereRadius)
      {
         List<SpatialQuadTreeCell> nodes = new List<SpatialQuadTreeCell>();
         getLeafCellsIntersectingSphere(nodes, sphereCenter, sphereRadius);
         if (nodes.Count == 0)
            return;

         Vector3 center = sphereCenter.toVec3();
         for (int q = 0; q < nodes.Count; q++)
         {
            SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)nodes[q].mExternalData);
            Debug.Assert(cell != null);
            for (uint i = (uint)cell.mMinXVert; i < (uint)cell.mMinXVert + mNumXVertsPerCell; i++)
            {
               for (uint j = (uint)cell.mMinZVert; j < (uint)cell.mMinZVert + mNumZVertsPerCell; j++)
               {
                  uint index = (uint)((i) + mWidth * (j));
                  Vector3 vert = getWorldspacePoint((int)(i), (int)(j)).toVec3();
                  if (BMathLib.pointSphereIntersect(ref center, sphereRadius, ref vert))
                  {
                     points.Add((int)index);
                  }
               }
            }

         }
      }
      public bool getClosestIntersectionPoint(float3 rayOrig, float3 rayDir, out float3 intPt)
      {
         intPt = float3.Empty;

         List<SpatialQuadTreeCell> nodes = new List<SpatialQuadTreeCell>();
         getLeafCellsIntersectingRay(nodes, rayOrig, rayDir);
         if (nodes.Count == 0)
            return false;


         //walk through each cell returned, do a per polygon intersection with each one..

         Vector3 origin = rayOrig.toVec3();
         Vector3 dir = rayDir.toVec3();

         Vector3 closestIntersect = Vector3.Empty;
         float closestDist = float.MaxValue;
         Vector3[] verts = new Vector3[3];
         bool hit = false;
         for (int q = 0; q < nodes.Count; q++)
         {
            SimHeightRepQuadCell cell = ((SimHeightRepQuadCell)nodes[q].mExternalData);
            Debug.Assert(cell != null);

            int minx = (int)(cell.mMinXVert);
            int minz = (int)(cell.mMinZVert);
            for (uint i = 0; i < mNumXVertsPerCell; i++)
            {
               for (uint j = 0; j < mNumZVertsPerCell; j++)
               {
                  int tileI = (int)(minx + i);
                  int tileJ = (int)(minz + j);
                  if (tileI >= mWidth - 1 || tileJ >= mHeight - 1)
                     continue;


                  bool tHit = false;
                  Vector3 pt = Vector3.Empty;

                  float[] h = new float[4];
                  h[0] = getCompositeHeight(tileI, tileJ);
                  h[1] = getCompositeHeight(tileI, tileJ + 1);
                  h[2] = getCompositeHeight(tileI + 1, tileJ);
                  h[3] = getCompositeHeight(tileI + 1, tileJ + 1);


                 // if (h[0] != cJaggedEmptyValue && h[3] != cJaggedEmptyValue && h[1] != cJaggedEmptyValue)
                  {
                     verts[0] = new Vector3(tileI * mTileScale, h[0], (tileJ) * mTileScale);
                     verts[1] = new Vector3((tileI + 1) * mTileScale, h[3], (tileJ + 1) * mTileScale);
                     verts[2] = new Vector3(tileI * mTileScale, h[1], (tileJ + 1) * mTileScale);

                     if (BMathLib.raySegmentIntersectionTriangle(verts, ref origin, ref dir, false, ref pt))
                     {
                        Vector3 vec = pt - origin;
                        //ensure this hit point is the closest to the origin
                        float len = vec.Length();
                        if (len < closestDist)
                        {
                           closestDist = len;
                           closestIntersect = pt;
                        }
                        hit = true;

                     }

                  }

                  //if (h[0] != cJaggedEmptyValue && h[3] != cJaggedEmptyValue && h[2] != cJaggedEmptyValue)
                  {
                     verts[0] = new Vector3(tileI * mTileScale, h[0], (tileJ) * mTileScale);
                     verts[1] = new Vector3((tileI + 1) * mTileScale, h[3], (tileJ + 1) * mTileScale);
                     verts[2] = new Vector3((tileI + 1) * mTileScale, h[2], (tileJ) * mTileScale);

                     if (BMathLib.raySegmentIntersectionTriangle(verts, ref origin, ref dir, false, ref pt))
                     {
                        Vector3 vec = pt - origin;
                        //ensure this hit point is the closest to the origin
                        float len = vec.Length();
                        if (len < closestDist)
                        {
                           closestDist = len;
                           closestIntersect = pt;
                        }
                        hit = true;

                     }
                  }
               }
            }

         }
         if (!hit)
            closestDist = 0;

         intPt.X = closestIntersect.X;
         intPt.Y = closestIntersect.Y;
         intPt.Z = closestIntersect.Z;
         return hit;
      }

      //----------------------------------
      float3 mLastMoustIntPt = float3.Empty;
      public void input()
      {

      }

      void getVertCursorWeights(float3 intPt)
      {

      }
      public Vector3 getIntersectPointFromScreenCursor()
      {
         float3 currIntersectionPt = float3.Empty;
         Vector3 orig = TerrainGlobals.getEditor().getRayPosFromMouseCoords(false);
         Vector3 dir = TerrainGlobals.getEditor().getRayPosFromMouseCoords(true) - orig;
         dir = BMathLib.Normalize(dir);

         //get our intersect point
         if (!getClosestIntersectionPoint(new float3(orig), new float3(dir), out currIntersectionPt))
            return Vector3.Empty;

         return currIntersectionPt.toVec3();
      }
      public void applySimRepBrush(bool alternate)
      {
         if (TerrainGlobals.getEditor().getStrokeInputType() == BTerrainEditor.eEditorStrokeInput.cStrokeInputMouse)
         {
            //raycast and get our intersection
            Vector3 orig = TerrainGlobals.getEditor().getRayPosFromMouseCoords(false);
            Vector3 dir = TerrainGlobals.getEditor().getRayPosFromMouseCoords(true) - orig;
            dir = BMathLib.Normalize(dir);

            //get our intersect point
            if (!getClosestIntersectionPoint(new float3(orig), new float3(dir), out mLastMoustIntPt))
               return;

            //If we're in tile mode, shift our center over to better match what our cursor is doing...
            if(TerrainGlobals.getEditor().getMode() == BTerrainEditor.eEditorMode.cModeSimBuildibility || 
               TerrainGlobals.getEditor().getMode() == BTerrainEditor.eEditorMode.cModeSimPassibility ||
               TerrainGlobals.getEditor().getMode() == BTerrainEditor.eEditorMode.cModeSimFloodPassibility ||
               TerrainGlobals.getEditor().getMode() == BTerrainEditor.eEditorMode.cModeSimScarabPassibility)
            {
               mLastMoustIntPt.X += mTileScale;
               mLastMoustIntPt.Z += mTileScale;
            }

            // Undo info
            // addVertexUndoNodes(nodes, false);

            // Find affected points
            List<int> points = new List<int>();
            getPointsIntersectingSphere(points, mLastMoustIntPt, TerrainGlobals.getEditor().mBrushInfo.mRadius);

            Vector3 intPoint = mLastMoustIntPt.toVec3();
            Vector3 intNormal = BMathLib.unitY;
            BrushInfo bi = TerrainGlobals.getEditor().getCurrentBrushInfo();
            ((BTerrainSimBrush)TerrainGlobals.getEditor().getCurrentBrush()).applyOnBrush(points, ref intPoint, ref intNormal,
               ref bi,
               alternate);

            // addVertexUndoNodes(nodes, true);
         }
         else if (TerrainGlobals.getEditor().getStrokeInputType() == BTerrainEditor.eEditorStrokeInput.cStrokeInputKeyboard)
         {
           
            BrushInfo bi = TerrainGlobals.getEditor().getCurrentBrushInfo();
            ((BTerrainSimBrush)TerrainGlobals.getEditor().getCurrentBrush()).applyOnSelection(Masking.getCurrSelectionMaskWeights(), bi.mIntensity, alternate);

         }

      }


      //----------------------------------
   };

   public class SimHeightVisualData
   {
      public SimHeightVisualData()
      {

      }

      ~SimHeightVisualData()
      {
         destroy();
      }
      public void destroy()
      {
         if (mVB != null)
         {
            mVB.Dispose();
            mVB = null;
         }
         if (mIB != null)
         {
            mIB.Dispose();
            mIB = null;
         }
      }
      public VertexBuffer mVB = null;
      public IndexBuffer mIB = null;
      public int mNumVerts = 0;
      public int mNumPrims = 0;
   }

   public class SimHeightRepQuadCell
   {
      public int mMinXVert = 0;
      public int mMinZVert = 0;
      public SimHeightVisualData mVisualHandle = null;

      public SimHeightRepQuadCell()
      {

      }
      ~SimHeightRepQuadCell()
      {
         destroy();
      }
      public void destroy()
      {
         if (mVisualHandle != null)
         {
            mVisualHandle.destroy();
            mVisualHandle = null;
         }
      }
   };

}