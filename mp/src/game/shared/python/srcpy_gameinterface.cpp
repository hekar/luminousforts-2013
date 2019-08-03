//====== Copyright � Sandern Corporation, All rights reserved. ===========//
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "srcpy_gameinterface.h"
#include "srcpy.h"

#include "utldict.h"

#include "cdll_int.h"
#include <filesystem.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace boost::python;

//-----------------------------------------------------------------------------
// Purpose: Python function for getting the mod path
//-----------------------------------------------------------------------------
bp::object PyGetModPath()
{
	char buf[_MAX_PATH];
	filesystem->GetSearchPath( "MOD", true, buf, sizeof( buf ) );
	V_StripTrailingSlash( buf );
	return bp::object( buf );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void PyAsyncFinishAllWrites( void )
{
	filesystem->AsyncFinishAllWrites();
}

//-----------------------------------------------------------------------------
// Purpose: Shutdown a Python ConVar
//-----------------------------------------------------------------------------
bool PyShutdownConVar( const char *pName )
{
	if( !cvar )
		return false;

	PyConVar *pConVar = dynamic_cast<PyConVar *>( cvar->FindVar( pName ) );
	if( pConVar != NULL )
	{
		pConVar->Shutdown();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
// Purpose: Shutdown a Python ConCommand
//-----------------------------------------------------------------------------
bool PyShutdownConCommand( const char *pName )
{
	if( !cvar )
		return false;

	PyConCommand *pConCommand = dynamic_cast<PyConCommand *>( cvar->FindCommand( pName ) );
	if( pConCommand != NULL )
	{
		pConCommand->Shutdown();
		return true;
	}

	return false;
}

#define VERIFY_CONVAR_HASNAME( pName ) \
	if( !pName || V_strlen( pName ) == 0 ) { \
		PyErr_SetString(PyExc_Exception, "ConVar must have a name and string must not be empty" );	\
		throw boost::python::error_already_set(); \
	}

// -------------------------------------------------------------------------------
void PyDummyCallback( const CCommand &args ) {}

PyConCommand::PyConCommand( const char *pName, bp::object method, const char *pHelpString, int flags, 
						   bp::object completionfunc, bool useweakref ) :
#ifdef CLIENT_DLL
ConCommand( pName ? CopyString(pName) : NULL, PyDummyCallback, pHelpString ? CopyString(pHelpString) : NULL, flags|FCVAR_CLIENTDLL, 0 )
#else
ConCommand( pName ? CopyString(pName) : NULL, PyDummyCallback, pHelpString ? CopyString(pHelpString) : NULL, flags|FCVAR_GAMEDLL, 0 )
#endif
{
	VERIFY_CONVAR_HASNAME( pName );

	// Save call back
	m_bUsesWeakRef = useweakref;
	if( m_bUsesWeakRef )
		m_pyCommandCallback = SrcPySystem()->CreateWeakRef( method );
	else
		m_pyCommandCallback = method;
	m_pyCompletionFunc = completionfunc;
}

PyConCommand::~PyConCommand()
{
	m_pyCommandCallback = bp::object();
	Shutdown();
}

// Invoke the function
void PyConCommand::Dispatch( const CCommand &command )
{
	ConCommand::Dispatch(command);

	try 
	{
		if( m_bUsesWeakRef )
			m_pyCommandCallback()( command );
		else
			m_pyCommandCallback( command );
	} 
	catch( error_already_set & ) 
	{
		PyErr_Print();
	}
}

// Auto completion
int PyConCommand::AutoCompleteSuggest( const char *partial, CUtlVector< CUtlString > &commands )
{
	if( m_pyCompletionFunc.ptr() == Py_None )
		return 0;

	bp::list suggests;
	try 
	{
		suggests = bp::list( m_pyCompletionFunc( partial ) );
	}
	catch(boost::python::error_already_set &) 
	{
		PyErr_Print();
		return 0;
	}

	for( int i=0; i < bp::len(suggests); i++ )
	{
		try 
		{
			const char *pCommand = bp::extract<const char *>(suggests[i]);
			commands.AddToTail( CUtlString( pCommand ) );
		}
		catch(boost::python::error_already_set &) 
		{
			PyErr_Print();
		}
	}

	return commands.Count();
}

bool PyConCommand::CanAutoComplete( void )
{
	return m_pyCommandCallback.ptr() != Py_None;
}

void PyConCommand::Shutdown()
{
	BaseClass::Shutdown();
}

//-----------------------------------------------------------------------------
// Purpose: Constructors. Saves python callback method and ensures 
//			unique copies of the strings
//-----------------------------------------------------------------------------
PyConVar::PyConVar( const char *name, const char *default_value, int flags )
: ConVar((name ? CopyString(name) : NULL), (default_value ? CopyString(default_value) : NULL), flags)
{
	VERIFY_CONVAR_HASNAME( name );
	m_pyChangeCallback = bp::object();
}

PyConVar::PyConVar( const char *name, const char *default_value, int flags, 
				   const char *help_string )
				   : ConVar((name ? CopyString(name) : NULL), (default_value ? CopyString(default_value) : NULL), 
				   flags, (help_string ? CopyString(help_string) : NULL))
{
	VERIFY_CONVAR_HASNAME( name );
	m_pyChangeCallback = bp::object();
}

PyConVar::PyConVar( const char *name, const char *default_value, int flags, 
				   const char *help_string, bool bMin, float fMin, bool bMax, float fMax )
				   : ConVar((name ? CopyString(name) : NULL), (default_value ? CopyString(default_value) : NULL), flags, 
				   (help_string ? CopyString(help_string) : NULL), bMin, fMin, bMax, fMax)
{
	VERIFY_CONVAR_HASNAME( name );
	m_pyChangeCallback = bp::object();
}

PyConVar::PyConVar( const char *name, const char *default_value, int flags, 
				   const char *help_string, bp::object callback )
				   : ConVar((name ? CopyString(name) : NULL), (default_value ? CopyString(default_value) : NULL), flags, 
				   (help_string ? CopyString(help_string) : NULL))
{
	VERIFY_CONVAR_HASNAME( name );
	m_pyChangeCallback = callback;
}

PyConVar::PyConVar( const char *name, const char *default_value, int flags, 
				   const char *help_string, bool bMin, float fMin, bool bMax, float fMax,
				   bp::object callback )
				   : ConVar((name ? CopyString(name) : NULL), (default_value ? CopyString(default_value) : NULL), flags, 
				   (help_string ? CopyString(help_string) : NULL), bMin, fMin, bMax, fMax)
{
	VERIFY_CONVAR_HASNAME( name );
	m_pyChangeCallback = callback;
}

//-----------------------------------------------------------------------------
// Purpose: Need to unregister
//-----------------------------------------------------------------------------
PyConVar::~PyConVar()
{
	// See gameinterface.py. Always kills the ConVar before creating a new one with the same command
	// So always do Shutdown here.
#if 0
	// Might have been replaced already, so check if this cvar name is still pointing to this instance
	ConVar *pConVar = cvar->FindVar( GetName() );
	if( pConVar && pConVar == this )
	{
		Shutdown();
	}
	else
	{
		Warning("ConVar %s replaced by new ConVar\n", GetName());
	}
#else
	Shutdown();
#endif // 0 
}

void PyConVar::Shutdown()
{
	BaseClass::Shutdown();
}

#if 0
//-----------------------------------------------------------------------------
// Purpose: Bit ugly, but need to call the call the python callback.
//-----------------------------------------------------------------------------
void PyConVar::SetValue( const char *value )
{
	float flOldValue = GetFloat();
 	char* pszOldValue = (char*)stackalloc( m_Value.m_StringLength );
	memcpy( pszOldValue, m_Value.m_pszString, m_Value.m_StringLength );

	BaseClass::SetValue(value);

	// Invoke any necessary callback function
	if ( m_pyChangeCallback.ptr() != Py_None )
	{
		// Invoke any necessary callback function
		SrcPySystem()->Run<pointer_wrapper<PyConVar *>, const char *, float>(m_pyChangeCallback, ptr(this), pszOldValue, flOldValue );
	}

	stackfree( pszOldValue );
}

void PyConVar::SetValue( float value )
{
	float flOldValue = m_Value.m_fValue;
 	char* pszOldValue = (char*)stackalloc( m_Value.m_StringLength );
	memcpy( pszOldValue, m_Value.m_pszString, m_Value.m_StringLength );

	BaseClass::SetValue(value);

	// Invoke any necessary callback function
	if ( m_pyChangeCallback.ptr() != Py_None )
	{
		// Invoke any necessary callback function
		SrcPySystem()->Run<pointer_wrapper<PyConVar *>, const char *, float>(m_pyChangeCallback, ptr(this), pszOldValue, flOldValue );
	}

	stackfree( pszOldValue );
}

void PyConVar::SetValue( int value )
{
	float flOldValue = m_Value.m_fValue;
 	char* pszOldValue = (char*)stackalloc( m_Value.m_StringLength );
	memcpy( pszOldValue, m_Value.m_pszString, m_Value.m_StringLength );

	BaseClass::SetValue(value);

	// Invoke any necessary callback function
	if ( m_pyChangeCallback.ptr() != Py_None )
	{
		// Invoke any necessary callback function
		SrcPySystem()->Run<pointer_wrapper<PyConVar *>, const char *, float>(m_pyChangeCallback, ptr(this), pszOldValue, flOldValue );
	}

	stackfree( pszOldValue );
}
#endif // 0

// -------------------------------------------------------------------------------
PyGameEvent::PyGameEvent() 
{
	Init(NULL);
}
PyGameEvent::PyGameEvent( const char *event_name, bool force )
{
	Init( gameeventmanager->CreateEvent(event_name, force) );
}
void PyGameEvent::Init( IGameEvent *pEvent )
{
	m_pEvent = pEvent;
	if( m_pEvent )
		m_bValid = true;
	else
		m_bValid = false;
}
const char *PyGameEvent::GetName() const
{
	if( !m_bValid )
		return NULL;
	return m_pEvent->GetName();
}
bool PyGameEvent::IsReliable() const
{
	if( !m_bValid )
		return 0;
	return m_pEvent->IsReliable();
}
bool PyGameEvent::IsLocal() const
{
	if( !m_bValid )
		return 0;
	return m_pEvent->IsReliable();
}
bool PyGameEvent::IsEmpty(const char *keyName )
{
	if( !m_bValid )
		return 0;
	return m_pEvent->IsEmpty(keyName);
}
// Data access
bool PyGameEvent::GetBool( const char *keyName, bool defaultValue )
{
	if( !m_bValid )
		return 0;
	return m_pEvent->GetBool(keyName, defaultValue);
}
int PyGameEvent::GetInt( const char *keyName, int defaultValue )
{
	if( !m_bValid )
		return 0;
	return m_pEvent->GetInt(keyName, defaultValue);
}
float PyGameEvent::GetFloat( const char *keyName, float defaultValue )
{
	if( !m_bValid )
		return 0;
	return m_pEvent->GetFloat(keyName, defaultValue);
}
const char *PyGameEvent::GetString( const char *keyName, const char *defaultValue )
{
	if( !m_bValid )
		return 0;
	return m_pEvent->GetString(keyName, defaultValue);
}
void PyGameEvent::SetBool( const char *keyName, bool value )
{
	if( !m_bValid )
		return;
	m_pEvent->SetBool(keyName, value);
}
void PyGameEvent::SetInt( const char *keyName, int value )
{
	if( !m_bValid )
		return;
	m_pEvent->SetInt(keyName, value);
}
void PyGameEvent::SetFloat( const char *keyName, float value )
{
	if( !m_bValid )
		return;
	m_pEvent->SetFloat(keyName, value);
}
void PyGameEvent::SetString( const char *keyName, const char *value )
{
	if( !m_bValid )
		return;
	m_pEvent->SetString(keyName, value);
}

void PyFireGameEvent( PyGameEvent *pEvent, bool bDontBroadcast )
{
	if( pEvent )
	{
		gameeventmanager->FireEvent( pEvent->GetEvent(), bDontBroadcast );

		// After firing the game event I assume it is destroyed
		// I don't know if this is actually true, but for safety I clear the event
		pEvent->Init(NULL);
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "GameEvent is None" );
		throw boost::python::error_already_set(); 
	}
}

void PyFireGameEventClientSide( PyGameEvent *pEvent )
{
	if( pEvent )
	{
		gameeventmanager->FireEventClientSide( pEvent->GetEvent() );

		// After firing the game event I assume it is destroyed
		// I don't know if this is actually true, but for safety I clear the event
		pEvent->Init(NULL);
	}
	else
	{
		PyErr_SetString(PyExc_Exception, "GameEvent is None" );
		throw boost::python::error_already_set(); 
	}
}

void PyGameEventListener::FireGameEvent( IGameEvent *event ) 
{
	try {
		boost::python::object ref = SrcPySystem()->Import("_gameinterface").attr("IGameEvent")();

		PyGameEvent *pPyEvent = boost::python::extract<PyGameEvent *>(ref);
		pPyEvent->Init(event);
		PyFireGameEvent(ref);
		pPyEvent->Init(NULL);
	} catch(boost::python::error_already_set &) {
		PyErr_Print();
	}
}

//-----------------------------------------------------------------------------
// Purpose: Commands
//-----------------------------------------------------------------------------
void PyAddSearchPath( const char *pPath, const char *pathID, SearchPathAdd_t addType )
{
	filesystem->AddSearchPath(pPath, pathID, addType);
}

bool PyRemoveSearchPath( const char *pPath, const char *pathID)
{
	return filesystem->RemoveSearchPath(pPath, pathID);
}

boost::python::object PyGetSearchPath( const char *pPathID, bool bGetPackFiles )
{
	char maxpath[2048];
	filesystem->GetSearchPath( pPathID, bGetPackFiles, maxpath, 2048 );
	return boost::python::object(maxpath);
}

dheader_t PyGetMapHeader( const char *mapname )
{
	FileHandle_t f = filesystem->Open(mapname, "rb");
	if ( f == FILESYSTEM_INVALID_HANDLE ) {
		PyErr_SetString(PyExc_Exception, "No such map" );
		throw boost::python::error_already_set(); 
		return dheader_t();
	}
	unsigned int fileSize = filesystem->Size( f );
	if( fileSize < sizeof(dheader_t) )
	{
		filesystem->Close(f);
		PyErr_SetString(PyExc_Exception, "Invalid map" );
		throw boost::python::error_already_set(); 
		return dheader_t();
	}
	dheader_t header;
	filesystem->Read(&header, sizeof(dheader_t), f);
	filesystem->Close(f);
	if( header.version != BSPVERSION)
	{
		PyErr_SetString(PyExc_Exception, "Invalid bsp version" );
		throw boost::python::error_already_set(); 
		return header;
	}
	
	return header;
}



//-----------------------------------------------------------------------------
// Purpose: Engine
//-----------------------------------------------------------------------------
#ifdef CLIENT_DLL
bool PyVEngineClient::GetPlayerInfo( int ent_num, py_player_info_t *ppyinfo )
#else
bool PyVEngineServer::GetPlayerInfo( int ent_num, py_player_info_t *ppyinfo )
#endif 
{
	if( !ppyinfo )
		return false;

	player_info_s info;
	bool rv = engine->GetPlayerInfo(ent_num, &info);

	boost::python::object name = boost::python::object(
		boost::python::handle<>( 
			PyUnicode_DecodeUTF8( info.name, sizeof( info.name ), "ignore" )
		)
	);

	ppyinfo->name = name;
	ppyinfo->userID = info.userID;
	ppyinfo->guid = boost::python::object(info.guid);
	ppyinfo->friendsID = info.friendsID;
	ppyinfo->friendsName = boost::python::object(info.friendsName);
	ppyinfo->fakeplayer = info.fakeplayer;
	ppyinfo->ishltv = info.ishltv;
	//ppyinfo->customFiles = info.customFiles;
	memcpy(ppyinfo->customFiles, info.customFiles, MAX_CUSTOM_FILES*sizeof(CRC32_t));
	ppyinfo->filesDownloaded = info.filesDownloaded;
	return rv;
}

#ifdef CLIENT_DLL
model_t *PyVEngineClient::LoadModel( const char *pName, bool bProp )
{
	if( pName == NULL || V_strlen(pName) == 0 )
	{
		PyErr_SetString(PyExc_Exception, "Name cannot be None" );
		throw boost::python::error_already_set(); 
		return NULL;
	}

	return (model_t *)engine->LoadModel(pName, bProp);
}

boost::python::object PyVEngineClient::GetChapterName()
{
	char buf[MAX_PATH];
	engine->GetChapterName(buf, MAX_PATH);
	return boost::python::object(buf);
}

boost::python::object PyVEngineClient::GetMainMenuBackgroundName()
{
	char buf[MAX_PATH];
	engine->GetMainMenuBackgroundName(buf, MAX_PATH);
	return boost::python::object(buf);
}

boost::python::object PyVEngineClient::GetUILanguage()
{
	char buf[MAX_PATH];
	engine->GetUILanguage(buf, MAX_PATH);
	return boost::python::object(buf);
}

#else
boost::python::object PyVEngineServer::GetGameDir()
{
	char buf[MAX_PATH];
	engine->GetGameDir(buf, MAX_PATH);
	return boost::python::object(buf);
}

boost::python::object PyVEngineServer::GetClientSteamID( CBasePlayer *player )
{
	if( !player )
		return boost::python::object();
	const CSteamID *temp = engine->GetClientSteamID(player->edict());
	if( !temp )
		return boost::python::object();
	return steam.attr("CSteamID")(temp->ConvertToUint64());
}

#endif // CLIENT_DLL

#ifdef CLIENT_DLL
static PyVEngineClient pypyengine;
PyVEngineClient *pyengine = &pypyengine;
#else
static PyVEngineServer pypyengine;
PyVEngineServer *pyengine = &pypyengine;
#endif // CLIENT_DLL

// Model_t/modelinfo
boost::python::tuple PyVModelInfo::GetModelBounds(model_t *pModel)
{
	Vector mins, maxs;
	if( modelinfo )
		modelinfo->GetModelBounds( pModel, mins, maxs );
	return boost::python::make_tuple( mins, maxs );
}

boost::python::object PyVModelInfo::GetModelName( model_t *model )
{
	if( !model )
		return bp::object();
	const char *pModelName = modelinfo->GetModelName( model );
	return bp::object( pModelName );
}

model_t *PyVModelInfo::FindOrLoadModel( const char *name )
{
	if( name == NULL || V_strlen(name) == 0 )
	{
		PyErr_SetString( PyExc_Exception, "Name cannot be None" );
		throw boost::python::error_already_set(); 
		return NULL;
	}

	return (model_t *)modelinfo->FindOrLoadModel( name );
}

boost::python::object PyVModelInfo::GetStudioModel( model_t *model )
{
	if( model == NULL )
	{
		PyErr_SetString(PyExc_Exception, "model cannot be None" );
		throw boost::python::error_already_set(); 
		return boost::python::object();
	}
	 
	const studiohdr_t *pStudioHDR_t = modelinfo->GetStudiomodel( model );
	if( !pStudioHDR_t )
	{
		return boost::python::object();
	}

	boost::python::object studiohdr = SrcPySystem()->Import("_animation").attr("CStudioHdr")();
	CStudioHdr *pStudioHDR = boost::python::extract< CStudioHdr * >( studiohdr );
	if( pStudioHDR )
	{
		pStudioHDR->Init( pStudioHDR_t );
	}
	return studiohdr;
}

static PyVModelInfo pypymodelinfo;
PyVModelInfo *pymodelinfo = &pypymodelinfo;

#ifndef CLIENT_DLL
void PyMapEntity_ParseAllEntities(const char *pMapData, IMapEntityFilter *pFilter, bool bActivateEntities)
{
	if( !pMapData )
	{
		PyErr_SetString(PyExc_Exception, "mapdata cannot be None" );
		throw boost::python::error_already_set(); 
		return;
	}

	MapEntity_ParseAllEntities( pMapData, pFilter, bActivateEntities );
}

CMapEntityRef PyGetMapEntityRef( int it )
{
	if( g_MapEntityRefs.IsValidIndex( it ) )
		return g_MapEntityRefs[it];
	CMapEntityRef invalid;
	invalid.m_iEdict = -1;
	invalid.m_iSerialNumber = -1;
	return invalid;
}

int PyGetMapEntityRefIteratorHead()
{
	return g_MapEntityRefs.Head();
}

int PyGetMapEntityRefIteratorNext( int it )
{
	if( g_MapEntityRefs.IsValidIndex( it ) == false )
		return -1;
	it = g_MapEntityRefs.Next( it );
	if( g_MapEntityRefs.IsValidIndex( it ) == false )
		return -1;
	return it;
}

#endif // CLIENT_DLL