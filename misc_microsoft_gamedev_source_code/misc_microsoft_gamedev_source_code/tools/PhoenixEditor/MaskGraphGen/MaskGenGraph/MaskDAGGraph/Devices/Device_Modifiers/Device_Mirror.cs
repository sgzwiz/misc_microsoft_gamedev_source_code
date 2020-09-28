using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Drawing2D;
//using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;

namespace graphapp
{
   public class Device_Mirror : MaskDevice
   {
      MaskParam mConstraintMask = new MaskParam();
      [XmlIgnore]
      [ConnectionType("Constraint", "Primary Constraint")]
      public DAGMask ConstraintMask
      {
         get { return mConstraintMask.Value; }
         set { mConstraintMask.Value = value; }
      }

      MaskParam mInputMask = new MaskParam();
      [XmlIgnore]
      [ConnectionType("Input", "Primary Input", true)]
      public DAGMask InputMask
      {
         get { return mInputMask.Value; }
         set { mInputMask.Value = value; }
      }

      MaskParam mOutputMask = new MaskParam();
      [XmlIgnore]
      [ConnectionType("Output", "Primary Output")]
      public DAGMask OutputMask
      {
         get { return mOutputMask.Value; }
         set { mOutputMask.Value = value; }
      }

      enum eMethod
      {
         eLeftToRight = 0,
         eRightToLeft,
         eTopToBottom,
         eBottomToTop,
         
         eCount
      }
      IntParam mMethod = new IntParam(0, 0, (int)eMethod.eCount);
      [ConnectionType("Param", "Method")]
      public int Method
      {
         get { return mMethod.Value; }
         set { mMethod.Value = value; }
      }
     

      public Device_Mirror()
      { }
      public Device_Mirror(GraphCanvas owningCanvas)
         :
          base(owningCanvas)
      {
         base.Text = "Mirror";
         mColorTop = Color.White;
         mColorBottom = Color.CornflowerBlue;
         mBorderSize = 1;

         mSize.Width = 60;
         mSize.Height = 20;

         generateConnectionPoints();
         resizeFromConnections();
      }
      public override bool load(MaskDAGGraphNode fromNode)
      {
         Device_Mirror dc = fromNode as Device_Mirror;
         mGUID = dc.mGUID;
         draggedByMouse(Location, dc.Location);

         Method = dc.Method;

         return true;
      } 
      override public bool computeOutput(ConnectionPoint connPoint, OutputGenerationParams parms)
      {
         if (!verifyInputConnections())
            return false;

         if (!gatherInputAndParameters(parms))
            return false;

         MaskParam mp = ((MaskParam)(connPoint.ParamType));
         mp.Value = InputMask.Clone();
         mp.Value.mConstraintMask = ConstraintMask;

         int workWidth = parms.Width - 1;
         int workHeight = parms.Height - 1;

         int halfWid = parms.Width >> 1;
         int halfHei = parms.Height >> 1;

         if(Method == (int) eMethod.eLeftToRight)
         {
            for (int x = 0; x < halfWid; x++)
            {
               for (int y = 0; y < parms.Height; y++)
               {
                  mp.Value[workWidth-x, y] = InputMask[x, y];
               }
            }
         }
         else if (Method == (int)eMethod.eRightToLeft)
         {
            for (int x = 0; x < halfWid; x++)
            {
               for (int y = 0; y < parms.Height; y++)
               {
                  mp.Value[x, y] = InputMask[workWidth - x, y];
               }
            }
         }
         else if (Method == (int)eMethod.eTopToBottom)
         {
            for (int x = 0; x < parms.Width; x++)
            {
               for (int y = 0; y < halfHei; y++)
               {
                  mp.Value[x, y] = InputMask[x, workHeight-y];
               }
            }
         }
         else if (Method == (int)eMethod.eBottomToTop)
         {
            for (int x = 0; x < parms.Width; x++)
            {
               for (int y = 0; y < halfHei; y++)
               {
                  mp.Value[x, workHeight - y] = InputMask[x, y];
               }
            }
         }

         

         return true;
      }

   }
}