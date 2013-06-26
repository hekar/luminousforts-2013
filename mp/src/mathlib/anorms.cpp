//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
//=============================================================================//
#if !defined(_STATIC_LINKED) || defined(_SHARED_LIB)


#include "mathlib/vector.h"
#include "mathlib/anorms.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

Vector g_anorms[NUMVERTEXNORMALS] =
{
	Vector(-0.525731, 0.000000, 0.850651), 
	Vector(-0.442863, 0.238856, 0.864188), 
	Vector(-0.295242, 0.000000, 0.955423), 
	Vector(-0.309017, 0.500000, 0.809017), 
	Vector(-0.162460, 0.262866, 0.951056), 
	Vector(0.000000, 0.000000, 1.000000), 
	Vector(0.000000, 0.850651, 0.525731), 
	Vector(-0.147621, 0.716567, 0.681718), 
	Vector(0.147621, 0.716567, 0.681718), 
	Vector(0.000000, 0.525731, 0.850651), 
	Vector(0.309017, 0.500000, 0.809017), 
	Vector(0.525731, 0.000000, 0.850651), 
	Vector(0.295242, 0.000000, 0.955423), 
	Vector(0.442863, 0.238856, 0.864188), 
	Vector(0.162460, 0.262866, 0.951056), 
	Vector(-0.681718, 0.147621, 0.716567), 
	Vector(-0.809017, 0.309017, 0.500000), 
	Vector(-0.587785, 0.425325, 0.688191), 
	Vector(-0.850651, 0.525731, 0.000000), 
	Vector(-0.864188, 0.442863, 0.238856), 
	Vector(-0.716567, 0.681718, 0.147621), 
	Vector(-0.688191, 0.587785, 0.425325), 
	Vector(-0.500000, 0.809017, 0.309017), 
	Vector(-0.238856, 0.864188, 0.442863), 
	Vector(-0.425325, 0.688191, 0.587785), 
	Vector(-0.716567, 0.681718, -0.147621), 
	Vector(-0.500000, 0.809017, -0.309017), 
	Vector(-0.525731, 0.850651, 0.000000), 
	Vector(0.000000, 0.850651, -0.525731), 
	Vector(-0.238856, 0.864188, -0.442863), 
	Vector(0.000000, 0.955423, -0.295242), 
	Vector(-0.262866, 0.951056, -0.162460), 
	Vector(0.000000, 1.000000, 0.000000), 
	Vector(0.000000, 0.955423, 0.295242), 
	Vector(-0.262866, 0.951056, 0.162460), 
	Vector(0.238856, 0.864188, 0.442863), 
	Vector(0.262866, 0.951056, 0.162460), 
	Vector(0.500000, 0.809017, 0.309017), 
	Vector(0.238856, 0.864188, -0.442863), 
	Vector(0.262866, 0.951056, -0.162460), 
	Vector(0.500000, 0.809017, -0.309017), 
	Vector(0.850651, 0.525731, 0.000000), 
	Vector(0.716567, 0.681718, 0.147621), 
	Vector(0.716567, 0.681718, -0.147621), 
	Vector(0.525731, 0.850651, 0.000000), 
	Vector(0.425325, 0.688191, 0.587785), 
	Vector(0.864188, 0.442863, 0.238856), 
	Vector(0.688191, 0.587785, 0.425325), 
	Vector(0.809017, 0.309017, 0.500000), 
	Vector(0.681718, 0.147621, 0.716567), 
	Vector(0.587785, 0.425325, 0.688191), 
	Vector(0.955423, 0.295242, 0.000000), 
	Vector(1.000000, 0.000000, 0.000000), 
	Vector(0.951056, 0.162460, 0.262866), 
	Vector(0.850651, -0.525731, 0.000000), 
	Vector(0.955423, -0.295242, 0.000000), 
	Vector(0.864188, -0.442863, 0.238856), 
	Vector(0.951056, -0.162460, 0.262866), 
	Vector(0.809017, -0.309017, 0.500000), 
	Vector(0.681718, -0.147621, 0.716567), 
	Vector(0.850651, 0.000000, 0.525731), 
	Vector(0.864188, 0.442863, -0.238856), 
	Vector(0.809017, 0.309017, -0.500000), 
	Vector(0.951056, 0.162460, -0.262866), 
	Vector(0.525731, 0.000000, -0.850651), 
	Vector(0.681718, 0.147621, -0.716567), 
	Vector(0.681718, -0.147621, -0.716567), 
	Vector(0.850651, 0.000000, -0.525731), 
	Vector(0.809017, -0.309017, -0.500000), 
	Vector(0.864188, -0.442863, -0.238856), 
	Vector(0.951056, -0.162460, -0.262866), 
	Vector(0.147621, 0.716567, -0.681718), 
	Vector(0.309017, 0.500000, -0.809017), 
	Vector(0.425325, 0.688191, -0.587785), 
	Vector(0.442863, 0.238856, -0.864188), 
	Vector(0.587785, 0.425325, -0.688191), 
	Vector(0.688191, 0.587785, -0.425325), 
	Vector(-0.147621, 0.716567, -0.681718), 
	Vector(-0.309017, 0.500000, -0.809017), 
	Vector(0.000000, 0.525731, -0.850651), 
	Vector(-0.525731, 0.000000, -0.850651), 
	Vector(-0.442863, 0.238856, -0.864188), 
	Vector(-0.295242, 0.000000, -0.955423), 
	Vector(-0.162460, 0.262866, -0.951056), 
	Vector(0.000000, 0.000000, -1.000000), 
	Vector(0.295242, 0.000000, -0.955423), 
	Vector(0.162460, 0.262866, -0.951056), 
	Vector(-0.442863, -0.238856, -0.864188), 
	Vector(-0.309017, -0.500000, -0.809017), 
	Vector(-0.162460, -0.262866, -0.951056), 
	Vector(0.000000, -0.850651, -0.525731), 
	Vector(-0.147621, -0.716567, -0.681718), 
	Vector(0.147621, -0.716567, -0.681718), 
	Vector(0.000000, -0.525731, -0.850651), 
	Vector(0.309017, -0.500000, -0.809017), 
	Vector(0.442863, -0.238856, -0.864188), 
	Vector(0.162460, -0.262866, -0.951056), 
	Vector(0.238856, -0.864188, -0.442863), 
	Vector(0.500000, -0.809017, -0.309017), 
	Vector(0.425325, -0.688191, -0.587785), 
	Vector(0.716567, -0.681718, -0.147621), 
	Vector(0.688191, -0.587785, -0.425325), 
	Vector(0.587785, -0.425325, -0.688191), 
	Vector(0.000000, -0.955423, -0.295242), 
	Vector(0.000000, -1.000000, 0.000000), 
	Vector(0.262866, -0.951056, -0.162460), 
	Vector(0.000000, -0.850651, 0.525731), 
	Vector(0.000000, -0.955423, 0.295242), 
	Vector(0.238856, -0.864188, 0.442863), 
	Vector(0.262866, -0.951056, 0.162460), 
	Vector(0.500000, -0.809017, 0.309017), 
	Vector(0.716567, -0.681718, 0.147621), 
	Vector(0.525731, -0.850651, 0.000000), 
	Vector(-0.238856, -0.864188, -0.442863), 
	Vector(-0.500000, -0.809017, -0.309017), 
	Vector(-0.262866, -0.951056, -0.162460), 
	Vector(-0.850651, -0.525731, 0.000000), 
	Vector(-0.716567, -0.681718, -0.147621), 
	Vector(-0.716567, -0.681718, 0.147621), 
	Vector(-0.525731, -0.850651, 0.000000), 
	Vector(-0.500000, -0.809017, 0.309017), 
	Vector(-0.238856, -0.864188, 0.442863), 
	Vector(-0.262866, -0.951056, 0.162460), 
	Vector(-0.864188, -0.442863, 0.238856), 
	Vector(-0.809017, -0.309017, 0.500000), 
	Vector(-0.688191, -0.587785, 0.425325), 
	Vector(-0.681718, -0.147621, 0.716567), 
	Vector(-0.442863, -0.238856, 0.864188), 
	Vector(-0.587785, -0.425325, 0.688191), 
	Vector(-0.309017, -0.500000, 0.809017), 
	Vector(-0.147621, -0.716567, 0.681718), 
	Vector(-0.425325, -0.688191, 0.587785), 
	Vector(-0.162460, -0.262866, 0.951056), 
	Vector(0.442863, -0.238856, 0.864188), 
	Vector(0.162460, -0.262866, 0.951056), 
	Vector(0.309017, -0.500000, 0.809017), 
	Vector(0.147621, -0.716567, 0.681718), 
	Vector(0.000000, -0.525731, 0.850651), 
	Vector(0.425325, -0.688191, 0.587785), 
	Vector(0.587785, -0.425325, 0.688191), 
	Vector(0.688191, -0.587785, 0.425325), 
	Vector(-0.955423, 0.295242, 0.000000), 
	Vector(-0.951056, 0.162460, 0.262866), 
	Vector(-1.000000, 0.000000, 0.000000), 
	Vector(-0.850651, 0.000000, 0.525731), 
	Vector(-0.955423, -0.295242, 0.000000), 
	Vector(-0.951056, -0.162460, 0.262866), 
	Vector(-0.864188, 0.442863, -0.238856), 
	Vector(-0.951056, 0.162460, -0.262866), 
	Vector(-0.809017, 0.309017, -0.500000), 
	Vector(-0.864188, -0.442863, -0.238856), 
	Vector(-0.951056, -0.162460, -0.262866), 
	Vector(-0.809017, -0.309017, -0.500000), 
	Vector(-0.681718, 0.147621, -0.716567), 
	Vector(-0.681718, -0.147621, -0.716567), 
	Vector(-0.850651, 0.000000, -0.525731), 
	Vector(-0.688191, 0.587785, -0.425325), 
	Vector(-0.587785, 0.425325, -0.688191), 
	Vector(-0.425325, 0.688191, -0.587785), 
	Vector(-0.425325, -0.688191, -0.587785), 
	Vector(-0.587785, -0.425325, -0.688191), 
	Vector(-0.688191, -0.587785, -0.425325)
};

#endif // !_STATIC_LINKED || _SHARED_LIB