//====== Copyright © Sandern Corporation, All rights reserved. ===========//
//
// Purpose: 
//
// $Workfile:     $
// $Date:         $
//
//-----------------------------------------------------------------------------
// $Log: $
//
// $NoKeywords: $
//=============================================================================//
#if !defined( SRCPY_CLIENT_CLASS_H )
#define SRCPY_CLIENT_CLASS_H
#ifdef _WIN32
#pragma once
#endif

#if !defined(ENABLE_PYTHON) || !defined(SRCPY_MOD_ENTITIES)
	#define IMPLEMENT_PYCLIENTCLASS_SYSTEM( name, network_name )
	#define DECLARE_PYCLIENTCLASS( name )
	#define DECLARE_PYCLASS( name )
#else

#include "client_class.h"
#include "srcpy_class_shared.h"

class NetworkedClass;

// Forward declaration
class PyClientClassBase;

// Class head 
extern PyClientClassBase *g_pPyClientClassHead;

namespace DT_BaseEntity
{
	extern RecvTable g_RecvTable;
}

// Use as base for client networked entities
class PyClientClassBase : public ClientClass
{
public:
	PyClientClassBase( char *pNetworkName, RecvTable *pRecvTable ) : ClientClass(pNetworkName, NULL, NULL, pRecvTable ), m_pNetworkedClass(NULL)
	{
		m_pPyNext				= g_pPyClientClassHead;
		g_pPyClientClassHead	= this;
		m_bFree = true;
	}

	virtual void SetPyClass( boost::python::object cls_type ) {}
	virtual void InitPyClass() {}

public:
	PyClientClassBase *m_pPyNext;
	bool m_bFree;
	NetworkedClass *m_pNetworkedClass;
	char m_strPyNetworkedClassName[512];
	boost::python::object m_PyClass;
};

C_BaseEntity *ClientClassFactory( boost::python::object cls_type, int entnum, int serialNum);
void InitAllPythonEntities();
void CheckEntities(PyClientClassBase *pCC, boost::python::object pyClass );

// For each available free client class we need a unique class because of the static data
// No way around this since m_pCreateFn is called from the engine (must always be a static function pointer?)
#define IMPLEMENT_PYCLIENTCLASS_SYSTEM( name, network_name, table, fallback_class_name )			\
	class name : public PyClientClassBase															\
	{																								\
	public:																							\
		name() : PyClientClassBase( #network_name, table )											\
		{																							\
			m_pCreateFn = PyClientClass_CreateObject;												\
		}																							\
		static IClientNetworkable* PyClientClass_CreateObject( int entnum, int serialNum );			\
		virtual void SetPyClass( boost::python::object cls_type );									\
		virtual void InitPyClass();																	\
	public:																							\
		static boost::python::object m_PyClass;														\
	};																								\
	boost::python::object name::m_PyClass;															\
	IClientNetworkable* name::PyClientClass_CreateObject( int entnum, int serialNum )				\
	{																								\
		C_BaseEntity *pRet = ClientClassFactory(m_PyClass, entnum, serialNum);						\
		if( !pRet )																					\
		{																							\
			pRet = new fallback_class_name;															\
			pRet->Init( entnum, serialNum );														\
		}																							\
		return pRet;																				\
	}																								\
	void name::SetPyClass( boost::python::object cls_type )											\
	{																								\
		m_PyClass = cls_type;																		\
		InitPyClass();																				\
		CheckEntities(this, m_PyClass);																\
	}																								\
	void name::InitPyClass()																		\
	{																								\
		boost::python::object meth = SrcPySystem()->Get("InitEntityClass", m_PyClass, false);		\
		if( meth.ptr() == Py_None )																	\
			return;																					\
		SrcPySystem()->Run( meth );																	\
	}																								\
	name name##_object;

// NetworkedClass is exposed to python and deals with getting a server/client class and cleaning up
class NetworkedClass
{
public:
	NetworkedClass( const char *pNetworkName, boost::python::object cls_type, const char *pClientModuleName=0 );
	~NetworkedClass();

	void AttachClientClass( PyClientClassBase *pClientClass );

public:
	const char *m_pNetworkName;
	PyClientClassBase *m_pClientClass;
	boost::python::object m_pyClass;
};

// Implement a networkable python class. Used to determine the right recv/send tables
#define DECLARE_PYCLIENTCLASS( name )																\
	DECLARE_PYCLASS( name )

#endif // ENABLE_PYTHON

#endif // SRCPY_CLIENT_CLASS_H