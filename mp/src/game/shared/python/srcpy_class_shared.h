//====== Copyright © Sandern Corporation, All rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SRCPY_CLASS_SHARED_H
#define SRCPY_CLASS_SHARED_H
#ifdef _WIN32
#pragma once
#endif

#ifdef SRCPY_MOD_ENTITIES

#include "srcpy_boostpython.h"

boost::python::object CreatePyHandleHelper( const CBaseEntity *pEnt, const char *handlename );

//-----------------------------------------------------------------------------
// Purpose: Implement a python class. For python/c++ handle conversion
//-----------------------------------------------------------------------------
#define DECLARE_PYCLASS_HELPER( name )																\
	public:																							\
	virtual boost::python::object CreatePyHandle( void ) const										\
{																									\
	return CreatePyHandleHelper(this, #name "HANDLE");												\
}

#define DECLARE_PYCLASS( name ) DECLARE_PYCLASS_HELPER( name )

#endif // SRCPY_MOD_ENTITIES

#endif // SRCPY_CLASS_SHARED_H

