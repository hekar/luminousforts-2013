//====== Copyright � Sandern Corporation, All rights reserved. ===========//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "srcpy.h"
#include "srcpy_class_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef SRCPY_MOD_ENTITIES
boost::python::object CreatePyHandleHelper( const CBaseEntity *pEnt, const char *handlename )
{	
	boost::python::object clshandle;
	if( pEnt->GetPyInstance().ptr() != Py_None )	
	{							
		try 
		{																
			clshandle = _entities.attr("PyHandle");							
		} 
		catch(boost::python::error_already_set &) 
		{
#ifdef CLIENT_DLL
			Warning("CLIENT: ");
#else
			Warning("SERVER: ");
#endif // CLIENT_DLL
			Warning("Failed to create a PyHandle\n");				
			PyErr_Print();																
			return boost::python::object();														
		}																						
		return clshandle(pEnt->GetPyInstance());														
	}																							
	try 
	{																												
		clshandle = _entities.attr(handlename);		
		return clshandle(boost::python::ptr(pEnt));
	} 
	catch(boost::python::error_already_set &) 
	{				
#ifdef CLIENT_DLL
		Warning("CLIENT: ");
#else
		Warning("SERVER: ");
#endif // CLIENT_DLL
		Warning("Failed to create handle %s\n", handlename);								
		PyErr_Print();																		
		return boost::python::object();															
	}																							
	return boost::python::object();											
}
#endif // SRCPY_MOD_ENTITIES