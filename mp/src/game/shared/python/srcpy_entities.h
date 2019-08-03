//====== Copyright � Sandern Corporation, All rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef SRCPY_ENTITES_H
#define SRCPY_ENTITES_H
#ifdef _WIN32
#pragma once
#endif

#include "convar.h"
#include "ehandle.h"
#ifndef CLIENT_DLL
	#include "physics_bone_follower.h"
	#include "srcpy_physics.h"

	class CBaseAnimating;
	class CRagdollProp;
#endif // CLIENT_DLL

#include "srcpy_boostpython.h"

extern boost::python::object _entities;

//-----------------------------------------------------------------------------
// Purpose: Python entity handle
//-----------------------------------------------------------------------------
template< class T >
class CEPyHandle : public CHandle<T>
{
public:
	CEPyHandle() : CHandle<T>() {}
	CEPyHandle( T *pVal ) : CHandle<T>(pVal) {}
	CEPyHandle( int iEntry, int iSerialNumber ) : CHandle<T>(iEntry, iSerialNumber) {}

	boost::python::object GetAttr( const char *name );

	bool	operator==( boost::python::object val ) const;
	bool	operator!=( boost::python::object val ) const;

#if PY_VERSION_HEX < 0x03000000
	int Cmp( boost::python::object other );
	bool NonZero();
#else
	bool Bool();
#endif // PY_VERSION_HEX < 0x03000000
	Py_hash_t Hash();
};

template< class T >
inline boost::python::object CEPyHandle<T>::GetAttr( const char *name )
{
	return boost::python::object(boost::python::ptr(this->Get())).attr(name);
}

template< class T >
inline bool CEPyHandle<T>::operator==( boost::python::object other ) const
{
	// Other value is None, this handle should be None too
	PyObject *pPyObject = other.ptr();
	if( pPyObject == Py_None )
	{
		return this->Get() == NULL;
	}

	// Other value is not None
	if( this->Get() == NULL )
	{
		return false; // Can't be same
	}

	// Check if it is directly a pointer to an entity
#ifdef CLIENT_DLL
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("C_BaseEntity")).ptr()) )
#else
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("CBaseEntity")).ptr()) )
#endif // CLIENT_DLL
	{
		CBaseEntity *pSelf = this->Get();
		CBaseEntity *pOther = boost::python::extract<CBaseEntity *>( other );
		return pSelf == pOther;
	}

	// Maybe it's a CBaseHandle?
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("CBaseHandle")).ptr()) )
	{
		return this == boost::python::extract< const CBaseHandle *>( other );
	}

	return false;
}

template< class T >
inline bool CEPyHandle<T>::operator!=( boost::python::object other ) const
{
	// Other value is None, this handle should not be None
	PyObject *pPyObject = other.ptr();
	if( pPyObject == Py_None )
	{
		return this->Get() != NULL;
	}

	// Other value is not None
	if( this->Get() == NULL )
	{
		return true; // Can't be same
	}

	// Check if it is directly a pointer to an entity
#ifdef CLIENT_DLL
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("C_BaseEntity")).ptr()) )
#else
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("CBaseEntity")).ptr()) )
#endif // CLIENT_DLL
	{
		CBaseEntity *pSelf = this->Get();
		CBaseEntity *pOther = boost::python::extract<CBaseEntity *>(other);
		return pSelf != pOther;
	}

	// Maybe it's a CBaseHandle?
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("CBaseHandle")).ptr()) )
	{
		return this != boost::python::extract< const CBaseHandle *>( other );
	}

	return true;
}

#if PY_VERSION_HEX < 0x03000000
template< class T >
int CEPyHandle<T>::Cmp( boost::python::object other )
{
	// The thing to which we compare is NULL
	PyObject *pPyObject = other.ptr();
	if( pPyObject == Py_None ) {
		return this->Get() != NULL;
	}

	// We are NULL
	if( this->Get() == NULL )
	{
		return pPyObject != NULL;
	}

	// Check if it is directly a pointer to an entity
#ifdef CLIENT_DLL
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("C_BaseEntity")).ptr()) )
#else
	if( PyObject_IsInstance(pPyObject, boost::python::object(_entities.attr("CBaseEntity")).ptr()) )
#endif // CLIENT_DLL
	{
		CBaseEntity *pSelf = this->Get();
		CBaseEntity *pOther = boost::python::extract<CBaseEntity *>(other);
		if( pOther == pSelf )
		{
			return 0;
		}
		else if( pOther->entindex() > pSelf->entindex() )
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	try
	{
		// Must be a handle
		CBaseHandle *pHandle = boost::python::extract<CBaseHandle *>( other );
		if( pHandle )
		{
			if( pHandle->ToInt() == this->ToInt() )
				return 0;
			else if( pHandle->GetEntryIndex() > this->GetEntryIndex() )
				return 1;
			else
				return -1;
		}
	}
	catch( boost::python::error_already_set & )
	{
		// Not a handle, just clear error and return -1
		PyErr_Clear();	
	}

	return -1;
}

template< class T >
inline bool CEPyHandle<T>::NonZero()
{
	return this->Get() != NULL;
}

#else

template< class T >
inline bool CEPyHandle<T>::Bool()
{
	return this->Get() != NULL;
}

template< class T >
inline Py_hash_t CEPyHandle<T>::Hash()
{
	return this->Get() ? (Py_hash_t)this->Get() : PyObject_Hash( Py_None );
}

#endif // PY_VERSION_HEX < 0x03000000

//----------------------------------------------------------------------------
// Purpose: Python entity handle, for python entities only
//-----------------------------------------------------------------------------
class PyHandle : public CBaseHandle
{
public:
	PyHandle(boost::python::object ent);
	PyHandle( int iEntry, int iSerialNumber ) : CBaseHandle(iEntry, iSerialNumber) {}

public:
	CBaseEntity *Get() const;
	boost::python::object PyGet() const;
	void Set( boost::python::object ent );

	bool	operator==( boost::python::object val ) const;
	bool	operator!=( boost::python::object val ) const;
	bool	operator==( const PyHandle &val ) const;
	bool	operator!=( const PyHandle &val ) const;
	const PyHandle& operator=( boost::python::object val );

	boost::python::object GetAttr( const char *name );
	boost::python::object GetAttribute( const char *name );
	void SetAttr( const char *name, boost::python::object v );

	Py_hash_t Hash();
#if PY_VERSION_HEX < 0x03000000
	int Cmp( boost::python::object other );
	bool NonZero() { return PyGet().ptr() != Py_None; }
#else
	bool Bool() { return PyGet().ptr() != Py_None; }
#endif // PY_VERSION_HEX < 0x03000000

	virtual PyObject *GetPySelf() { return NULL; }

	boost::python::object Str();
};

inline Py_hash_t PyHandle::Hash()
{
	return PyGet().ptr() ? PyObject_Hash( PyGet().ptr() ) : PyObject_Hash( Py_None );
}

boost::python::object CreatePyHandle( int iEntry, int iSerialNumber );

#ifndef CLIENT_DLL
boost::python::object PyGetWorldEntity();
#endif // CLIENT_DLL

//-----------------------------------------------------------------------------
// Purpose: Dead python entity. The __class__ object of a removed entity gets 
//			rebinded to this. This way you can't accidently access most of the
//			(potential) dangerous methods.
//-----------------------------------------------------------------------------
class DeadEntity 
{
public:
#if PY_VERSION_HEX < 0x03000000
	static bool NonZero() { return false; }
#else
	static bool Bool() { return false; }
#endif // PY_VERSION_HEX < 0x03000000
};

#ifdef CLIENT_DLL
// ----------- Linking python classes to entities
class PyEntityFactory 
{
public:
	PyEntityFactory( const char *pClassName, boost::python::object PyClass );
	~PyEntityFactory();

	C_BaseEntity *Create();

	boost::python::object GetClass() { return m_PyClass; }

private:
	char m_ClassName[128];
	boost::python::object m_PyClass;
};

#else
// ----------- Linking python classes to entities
class PyEntityFactory : public IEntityFactory
{
public:
	PyEntityFactory( const char *pClassName, boost::python::object PyClass );
	~PyEntityFactory();

	IServerNetworkable *Create( const char *pClassName );

	void Destroy( IServerNetworkable *pNetworkable );

	virtual size_t GetEntitySize();

	void InitPyClass();
	virtual bool IsPyFactory() { return true; }

	boost::python::object GetClass() { return m_PyClass; }
	const char *GetClassname() { return m_ClassName; }

private:
	void CheckEntities();

public:
	PyEntityFactory *m_pPyNext;

private:
	char m_ClassName[128];
	boost::python::object m_PyClass;
};
void InitAllPythonEntities();
#endif

boost::python::object PyGetClassByClassname( const char *class_name );
boost::python::list PyGetAllClassnames();
#ifndef CLIENT_DLL
boost::python::dict PyReadDataDesc( CBaseEntity *entity );
#endif // CLIENT_DLL

#ifndef CLIENT_DLL
//-----------------------------------------------------------------------------
// Purpose: Sending events to client
//-----------------------------------------------------------------------------
void PySendEvent( IRecipientFilter &filter, EHANDLE ent, int event, int data);

//-----------------------------------------------------------------------------
// Purpose: (Re)spawn a player using the specified class
//-----------------------------------------------------------------------------
boost::python::object PyRespawnPlayer( CBasePlayer *pPlayer, const char *classname );

//-----------------------------------------------------------------------------
// Purpose: PyOutputEvent
//-----------------------------------------------------------------------------
class PyOutputEvent : public CBaseEntityOutput
{
public:
	PyOutputEvent();

	void Set( variant_t value );

	// void Firing, no parameter
	void FireOutput( CBaseEntity *pActivator, CBaseEntity *pCaller, float fDelay = 0 );
	void FireOutput( variant_t Value, CBaseEntity *pActivator, CBaseEntity *pCaller, float fDelay = 0 ) 
	{ 
		CBaseEntityOutput::FireOutput( Value, pActivator, pCaller, fDelay ); 
	}
};

//-----------------------------------------------------------------------------
// Purpose: Bone followers
//-----------------------------------------------------------------------------
Vector GetAttachmentPositionInSpaceOfBone( CStudioHdr *pStudioHdr, const char *pAttachmentName, int outputBoneIndex );

typedef struct pyphysfollower_t
{
	int boneindex;
	boost::python::object follower;
} pyphysfollower_t;

class PyBoneFollowerManager : public CBoneFollowerManager
{
public:
	// Use either of these to create the bone followers in your entity's CreateVPhysics()
	void InitBoneFollowers( CBaseAnimating *pParentEntity, boost::python::list followerbonenames );
	void AddBoneFollower( CBaseAnimating *pParentEntity, const char *pFollowerBoneName, solid_t *pSolid = NULL );	// Adds a single bone follower

	// Call this after you move your bones
	void UpdateBoneFollowers( CBaseAnimating *pParentEntity );

	// Call this when your entity's removed
	void DestroyBoneFollowers( void );

	pyphysfollower_t GetBoneFollower( int iFollowerIndex );
	int				GetBoneFollowerIndex( CBoneFollower *pFollower );
	int				GetNumBoneFollowers( void ) const { return CBoneFollowerManager::GetNumBoneFollowers(); }
};

Vector GetAttachmentPositionInSpaceOfBone( CStudioHdr *pStudioHdr, const char *pAttachmentName, int outputBoneIndex );

CRagdollProp *PyCreateServerRagdollAttached( CBaseAnimating *pAnimating, const Vector &vecForce, int forceBone, int collisionGroup, PyPhysicsObject &pyAttached, CBaseAnimating *pParentEntity, int boneAttach, const Vector &originAttached, int parentBoneAttach, const Vector &boneOrigin );

#endif // CLIENT_DLL

#endif // SRCPY_ENTITES_H
