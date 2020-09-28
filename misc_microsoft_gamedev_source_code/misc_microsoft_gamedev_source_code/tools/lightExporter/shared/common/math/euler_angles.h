//-----------------------------------------------------------------------------
// File: euler_angles.cpp - Convert Euler angles to/from matrix or quat
// Originally by Ken Shoemake, 1993 
// Converted to C++ by Rich Geldreich, July 2003
// FIXME: CAN I USE THIS CODE?
// The original code appeared in Graphics Gems, which I believe is public domain.
//-----------------------------------------------------------------------------

#pragma once
#ifndef EULER_ANGLES_H
#define EULER_ANGLES_H

#include "common/math/quat.h"

/*** Order type constants, constructors, extractors ***/
    /* There are 24 possible conventions, designated by:    */
    /*	  o EulAxI = axis used initially		    */
    /*	  o EulPar = parity of axis permutation		    */
    /*	  o EulRep = repetition of initial axis as last	    */
    /*	  o EulFrm = frame from which axes are taken	    */
    /* Axes I,J,K will be a permutation of X,Y,Z.	    */
    /* Axis H will be either I or K, depending on EulRep.   */
    /* Frame S takes axes from initial static frame.	    */
    /* If ord = (AxI=X, Par=Even, Rep=No, Frm=S), then	    */
    /* {a,b,c,ord} means Rz(c)Ry(b)Rx(a), where Rz(c)v	    */
    /* rotates v around Z by c radians.			    */
#define EulFrmS	     0
#define EulFrmR	     1
#define EulFrm(ord)  ((unsigned)(ord)&1)
#define EulRepNo     0
#define EulRepYes    1
#define EulRep(ord)  (((unsigned)(ord)>>1)&1)
#define EulParEven   0
#define EulParOdd    1
#define EulPar(ord)  (((unsigned)(ord)>>2)&1)
#define EulSafe	     "\000\001\002\000"
#define EulNext	     "\001\002\000\001"
#define EulAxI(ord)  ((int)(EulSafe[(((unsigned)(ord)>>3)&3)]))
#define EulAxJ(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)==EulParOdd)]))
#define EulAxK(ord)  ((int)(EulNext[EulAxI(ord)+(EulPar(ord)!=EulParOdd)]))
#define EulAxH(ord)  ((EulRep(ord)==EulRepNo)?EulAxK(ord):EulAxI(ord))
    /* EulGetOrd unpacks all useful information about order simultaneously. */
#define EulGetOrd(ord,i,j,k,h,n,s,f) {unsigned o=ord;f=o&1;o>>=1;s=o&1;o>>=1;\
    n=o&1;o>>=1;i=EulSafe[o&3];j=EulNext[i+n];k=EulNext[i+1-n];h=s?k:i;}
    /* EulOrd creates an order value between 0 and 23 from 4-tuple choices. */
#define EulOrd(i,p,r,f)	   (((((((i)<<1)+(p))<<1)+(r))<<1)+(f))
    /* Static axes */
#define EulOrdXYZs    EulOrd(X,EulParEven,EulRepNo,EulFrmS)
#define EulOrdXYXs    EulOrd(X,EulParEven,EulRepYes,EulFrmS)
#define EulOrdXZYs    EulOrd(X,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdXZXs    EulOrd(X,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdYZXs    EulOrd(Y,EulParEven,EulRepNo,EulFrmS)
#define EulOrdYZYs    EulOrd(Y,EulParEven,EulRepYes,EulFrmS)
#define EulOrdYXZs    EulOrd(Y,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdYXYs    EulOrd(Y,EulParOdd,EulRepYes,EulFrmS)
#define EulOrdZXYs    EulOrd(Z,EulParEven,EulRepNo,EulFrmS)
#define EulOrdZXZs    EulOrd(Z,EulParEven,EulRepYes,EulFrmS)
#define EulOrdZYXs    EulOrd(Z,EulParOdd,EulRepNo,EulFrmS)
#define EulOrdZYZs    EulOrd(Z,EulParOdd,EulRepYes,EulFrmS)
    /* Rotating axes */
#define EulOrdZYXr    EulOrd(X,EulParEven,EulRepNo,EulFrmR)
#define EulOrdXYXr    EulOrd(X,EulParEven,EulRepYes,EulFrmR)
#define EulOrdYZXr    EulOrd(X,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdXZXr    EulOrd(X,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdXZYr    EulOrd(Y,EulParEven,EulRepNo,EulFrmR)
#define EulOrdYZYr    EulOrd(Y,EulParEven,EulRepYes,EulFrmR)
#define EulOrdZXYr    EulOrd(Y,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdYXYr    EulOrd(Y,EulParOdd,EulRepYes,EulFrmR)
#define EulOrdYXZr    EulOrd(Z,EulParEven,EulRepNo,EulFrmR)
#define EulOrdZXZr    EulOrd(Z,EulParEven,EulRepYes,EulFrmR)
#define EulOrdXYZr    EulOrd(Z,EulParOdd,EulRepNo,EulFrmR)
#define EulOrdZYZr    EulOrd(Z,EulParOdd,EulRepYes,EulFrmR)

namespace gr
{
	struct EulerAngles
  {
		float x;
		float y;
		float z;
    int order;

		// Define default rotation order
		enum 
		{ 
			DefaultOrder = EulOrdXYZr 
		};

    EulerAngles(int ord = DefaultOrder) :
      x(0), y(0), z(0), order(ord)
    {
    }

    EulerAngles(float xx, float yy, float zz, int ord = DefaultOrder) :
      x(xx), y(yy), z(zz), order(ord)
    {
    }
  
    EulerAngles(const Vec4& v, int ord = DefaultOrder) :
      x(v[0]), y(v[1]), z(v[2]), order(ord)
    {
    }
    
    EulerAngles(const Vec3& v, int ord = DefaultOrder) :
      x(v[0]), y(v[1]), z(v[2]), order(ord)
    {
    }

		EulerAngles(const Matrix44& m, int ord = DefaultOrder)
		{
			setFromMatrix(m, ord);
		}

		EulerAngles(const Quat& q, int ord = DefaultOrder)
		{
			setFromQuat(q, ord);
		}
     
    Quat& toQuat(Quat& qu) const;
    Matrix44& toMatrix(Matrix44& M) const;

    EulerAngles& setFromMatrix(const Matrix44& m, int newOrder = -1);
    EulerAngles& setFromQuat(const Quat& q, int newOrder = -1);

		static EulerAngles makeFromMatrix(const Matrix44& m, int order = DefaultOrder)
		{
			EulerAngles ret(order);
			return ret.setFromMatrix(m);
		}

    static EulerAngles makeFromQuat(const Quat& q, int order = DefaultOrder)
		{
			EulerAngles ret(order);
			return ret.setFromQuat(q);
		}

		static Quat createQuat(const EulerAngles& eu);
		static Matrix44 createMatrix(const EulerAngles& eu);
  };
} // namespace gr

#endif // EULER_ANGLES_H




