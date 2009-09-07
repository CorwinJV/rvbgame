//-----------------------------------------------------------------------------
// This file is part of the b-reality project:
// http://sourceforge.net/projects/b-reality
//
// (C) Francis Page  2003
//-----------------------------------------------------------------------------

#ifndef _VECTOR3_H_
#define _VECTOR3_H_
 
#if _MSC_VER == 1200
#pragma warning(disable:4786)
#endif
 
#include <iostream>
 
class   CMatrix3x3;
class   CVector4;

class CVector3
{
public:

CVector3() { x = y = z = 0.0f; }

CVector3( float x_, float y_, float z_ ) { x = x_; y = y_; z = z_; }

CVector3( const CVector4& v );

CVector3        operator + ( const CVector3 &v ) const;

CVector3        operator - ( const CVector3 &v ) const;

float           operator * ( const CVector3 &v ) const;

CVector3        operator ^ ( const CVector3 &v ) const;

CVector3        operator * ( const float &a ) const;

CVector3        operator / ( const float &a ) const;

CVector3&       operator ^= ( const CVector3 &v );

CVector3&       operator *= ( const float &a );

CVector3&       operator /= ( const float &a );

CVector3&       operator += ( const CVector3 &v );

CVector3&       operator -= ( const CVector3 &v );

float&          operator [] ( const int i )                     { return *(&x + i);     }

float           operator [] ( const int i )     const   { return *(&x + i);     }

CVector3        operator - () const;

bool            operator == ( const CVector3& v ) const;

bool            operator != ( const CVector3& v ) const;

friend          CVector3 operator * ( const float &a, const CVector3 &v );

friend          CVector3        operator / ( const float &a, const CVector3 &v );

friend          std::ostream& operator << (std::ostream& os, const CVector3& v );

float           Norm() const;

CVector3        Normalize();

CMatrix3x3      Star() const;

float           Max() const;
float           Min() const;

static const CVector3   ZERO;

float
x,
y,
z;
};
#endif
