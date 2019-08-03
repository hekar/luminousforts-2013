//====== Copyright © Sandern Corporation, All rights reserved. ===========//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "srcpy_vgui.h"
#include "srcpy.h"
#include <vgui_controls/Panel.h>
#include <ienginevgui.h>
#include <vgui/ISystem.h>
#include <vgui/IInput.h>

#include "game_controls/basemodel_panel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

int g_PythonPanelCount = 0; // For debugging

static CWrapSurface gWrapSurface;
static CWrapIPanel gWrapIPanel;
static PyLocalize pylocalize;

CWrapSurface *wrapsurface() { return &gWrapSurface; }
CWrapIPanel *wrapipanel() { return &gWrapIPanel; }
PyLocalize *g_pylocalize = &pylocalize;

//-----------------------------------------------------------------------------
// Purpose: Clears all python panels
//-----------------------------------------------------------------------------
void DestroyPyPanels()
{
	DevMsg("Clearing python panels...\n");

	float fStartTime = Plat_FloatTime();

	// TODO:
	//for( int i = g_PyPanels.Count()-1; i >= 0; i-- )
	//	PyDeletePanel( g_PyPanels[i]->GetPanel(), g_PyPanels[i], i ); 

	vgui::ivgui()->RunFrame();

	DevMsg( "\tFinished clearing Python panels in %f seconds\n", Plat_FloatTime() - fStartTime );
}

//-----------------------------------------------------------------------------
// Purpose: Panel handle implementation
//			Returns a pointer to a valid panel, NULL if the panel has been deleted
//-----------------------------------------------------------------------------
Panel *PyBaseVGUIHandle::Get() const
{
	if (m_hPanelHandle != INVALID_PANEL)
	{
		VPANEL panel = ivgui()->HandleToPanel(m_hPanelHandle);
		if (panel)
		{
			return ipanel()->GetPanel(panel, GetControlsModuleName());
		}
	}
	return NULL;
}

const PyBaseVGUIHandle& PyBaseVGUIHandle::Set( IClientPanel *pPanel )
{
	if (pPanel)
	{
		m_hPanelHandle = ivgui()->PanelToHandle(pPanel->GetVPanel());
	}
	else
	{
		m_hPanelHandle = INVALID_PANEL;
	}

	return *this; 
}

//-----------------------------------------------------------------------------
// Purpose: Maybe not very efficient, but it's nice and safe for python usage
//-----------------------------------------------------------------------------
void CWrapSurface::DrawFilledRectArray( boost::python::list rects )
{
	int i, n;
	n = boost::python::len( rects );
	IntRect *pRects = new IntRect[n];
	for( i = 0; i < n; i++ )
	{
		pRects[i] = boost::python::extract<IntRect>(rects[i]);
	}
	surface()->DrawFilledRectArray(pRects, n);
	delete [] pRects;
}

boost::python::tuple CWrapSurface::GetTextSize( HFont font, boost::python::object unistr )
{
#if PY_VERSION_HEX < 0x03000000
	if (!PyString_Check(unistr.ptr()))
	{
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: First argument must be a string.");
		throw boost::python::error_already_set(); 
	}
#else
	if (!PyUnicode_Check(unistr.ptr()))
	{
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: First argument must be a string.");
		throw boost::python::error_already_set(); 
	}
#endif

#if PY_VERSION_HEX < 0x03000000
	const char* value = PyString_AsString(unistr.ptr());
	Py_ssize_t l = PyString_Size(unistr.ptr());
#else
	Py_ssize_t l;
	const char* value = PyUnicode_AsUTF8AndSize( unistr.ptr(), &l );
#endif
	if (value == 0) {
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: String allocation error."); 
		throw boost::python::error_already_set();
	}
	wchar_t* w = new wchar_t[l+1];
	for( int i = 0; i < l; i++ ){
		w[i] = value[i];
	}
	w[l] = '\0';

	int wide, tall;
	surface()->GetTextSize(font, w, wide, tall); 

	delete [] w;
	return boost::python::make_tuple(wide, tall);
}

//-----------------------------------------------------------------------------
// Purpose: Maybe not very efficient, but it's nice and safe for python usage
//-----------------------------------------------------------------------------
void CWrapSurface::DrawTexturedPolyLine( boost::python::list vertices )
{
	int i, n;
	Vertex_t *pVertices;
	try 
	{
		n = boost::python::len( vertices );
		pVertices = new Vertex_t[n];
		for( i = 0; i < n; i++ )
			pVertices[i] = boost::python::extract<Vertex_t>(vertices[i]);
	}
	catch(...)
	{
		PyErr_Print();
		return;
	}
	vgui::surface()->DrawTexturedPolyLine(pVertices, n);

	delete [] pVertices;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWrapSurface::DrawTexturedPolygon( boost::python::list vertices )
{
	int i, n;
	Vertex_t *pVertices;
	try 
	{
		n = boost::python::len( vertices );
		pVertices = new Vertex_t[n];
		for( i = 0; i < n; i++ )
			pVertices[i] = boost::python::extract<Vertex_t>(vertices[i]);
	}
	catch(...)
	{
		PyErr_Print();
		return;
	}
	vgui::surface()->DrawTexturedPolygon(n, pVertices);

	delete [] pVertices;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CWrapSurface::DrawUnicodeString( boost::python::object unistr, FontDrawType_t drawType )
{
#if PY_VERSION_HEX < 0x03000000
	if (!PyString_Check(unistr.ptr()))
	{
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: First argument must be a string.");
		throw boost::python::error_already_set(); 
	}
#else
	if (!PyUnicode_Check(unistr.ptr()))
	{
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: First argument must be a string.");
		throw boost::python::error_already_set(); 
	}
#endif

#if PY_VERSION_HEX < 0x03000000
	const char* value = PyString_AsString(unistr.ptr());
	Py_ssize_t l = PyString_Size(unistr.ptr());
#else
	Py_ssize_t l;
	const char* value = PyUnicode_AsUTF8AndSize( unistr.ptr(), &l );
#endif
	if (value == 0) {
		PyErr_SetString(PyExc_ValueError, "DrawUnicodeString: String allocation error."); 
		throw boost::python::error_already_set();
	}
	wchar_t* w = new wchar_t[l+1];
	for( int i = 0; i < l; i++ ){
		w[i] = value[i];
	}
	w[l] = '\0';
	surface()->DrawUnicodeString(w, drawType);

	delete [] w;
}

//=============================================================================
// ILocalize
//=============================================================================
// converts an english string to unicode
// returns the number of wchar_t in resulting string, including null terminator
boost::python::tuple PyLocalize::ConvertANSIToUnicode(const char *ansi) 
{ 
	wchar_t unicode[1024];
	int n = g_pVGuiLocalize->ConvertANSIToUnicode(ansi, unicode, 1024); 
	return boost::python::make_tuple( boost::python::object(unicode), n );
}

// converts an unicode string to an english string
// unrepresentable characters are converted to system default
// returns the number of characters in resulting string, including null terminator
boost::python::tuple PyLocalize::ConvertUnicodeToANSI(const wchar_t *unicode)
{ 
	char ansi[1024];
	int n = g_pVGuiLocalize->ConvertUnicodeToANSI(unicode, ansi, 1024); 
	return boost::python::make_tuple( boost::python::object(ansi), n );
}

boost::python::object PyLocalize::ConstructString(const char *tokenName, KeyValues *localizationVariables)
{	
	wchar_t unicodeOutput[1024];
	g_pVGuiLocalize->ConstructString(unicodeOutput, 1024, tokenName, localizationVariables);
	return boost::python::object(unicodeOutput);
}
boost::python::object PyLocalize::ConstructString(StringIndex_t unlocalizedTextSymbol, KeyValues *localizationVariables)	
{
	wchar_t unicodeOutput[1024];
	g_pVGuiLocalize->ConstructString(unicodeOutput, 1024, unlocalizedTextSymbol, localizationVariables);
	return boost::python::object(unicodeOutput);
}


//=============================================================================
// Misc
//=============================================================================
bool PyIsGameUIVisible()
{
	return enginevgui->IsGameUIVisible();
}

VPANEL PyGetPanel( VGuiPanel_t type )
{
	return enginevgui->GetPanel( type );
}

CON_COMMAND(py_debug_panel_count, "Debug command for getting the number of python panels.")
{
	Msg("Python Panels: %d\n", g_PythonPanelCount );
}

boost::python::tuple PyInput_GetCursorPos()
{
	int x, y;
	vgui::input()->GetCursorPos(x, y);
	return boost::python::make_tuple(x, y);
}

boost::python::tuple PyInput_GetCursorPosition()
{
	int x, y;
	vgui::input()->GetCursorPosition(x, y);
	return boost::python::make_tuple(x, y);
}