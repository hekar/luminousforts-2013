//====== Copyright � Sandern Corporation, All rights reserved. ===========//
//
// Purpose: Controls the available core Python modules
//
// TOOD: Remove unneeded modules or maybe don't use this at all.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"

#include "srcpy_boostpython.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/* Module configuration */

/* This file contains the table of built-in modules.
   See init_builtin() in import.c. */

#ifdef WIN32
extern "C"
{

PyMODINIT_FUNC PyInit_array(void);
#ifndef MS_WINI64
PyMODINIT_FUNC PyInit_audioop(void);
#endif
PyMODINIT_FUNC PyInit_binascii(void);
PyMODINIT_FUNC PyInit_cmath(void);
PyMODINIT_FUNC PyInit_errno(void);
PyMODINIT_FUNC PyInit_faulthandler(void);
PyMODINIT_FUNC PyInit__tracemalloc(void);
PyMODINIT_FUNC PyInit_gc(void);
PyMODINIT_FUNC PyInit_math(void);
PyMODINIT_FUNC PyInit__md5(void);
PyMODINIT_FUNC PyInit_nt(void);
PyMODINIT_FUNC PyInit__operator(void);
PyMODINIT_FUNC PyInit_signal(void);
PyMODINIT_FUNC PyInit__sha1(void);
PyMODINIT_FUNC PyInit__sha256(void);
PyMODINIT_FUNC PyInit__sha512(void);
PyMODINIT_FUNC PyInit_time(void);
PyMODINIT_FUNC PyInit__thread(void);
#ifdef WIN32
PyMODINIT_FUNC PyInit_msvcrt(void);
PyMODINIT_FUNC PyInit__locale(void);
#endif
PyMODINIT_FUNC PyInit__codecs(void);
PyMODINIT_FUNC PyInit__weakref(void);
PyMODINIT_FUNC PyInit_xxsubtype(void);
PyMODINIT_FUNC PyInit_zipimport(void);
PyMODINIT_FUNC PyInit__random(void);
PyMODINIT_FUNC PyInit_itertools(void);
PyMODINIT_FUNC PyInit__collections(void);
PyMODINIT_FUNC PyInit__heapq(void);
PyMODINIT_FUNC PyInit__bisect(void);
PyMODINIT_FUNC PyInit__symtable(void);
PyMODINIT_FUNC PyInit_mmap(void);
PyMODINIT_FUNC PyInit__csv(void);
PyMODINIT_FUNC PyInit__sre(void);
PyMODINIT_FUNC PyInit_parser(void);
PyMODINIT_FUNC PyInit_winreg(void);
PyMODINIT_FUNC PyInit__struct(void);
PyMODINIT_FUNC PyInit__datetime(void);
PyMODINIT_FUNC PyInit__functools(void);
PyMODINIT_FUNC PyInit__json(void);
PyMODINIT_FUNC PyInit_zlib(void);

PyMODINIT_FUNC PyInit__multibytecodec(void);
PyMODINIT_FUNC PyInit__codecs_cn(void);
PyMODINIT_FUNC PyInit__codecs_hk(void);
PyMODINIT_FUNC PyInit__codecs_iso2022(void);
PyMODINIT_FUNC PyInit__codecs_jp(void);
PyMODINIT_FUNC PyInit__codecs_kr(void);
PyMODINIT_FUNC PyInit__codecs_tw(void);
PyMODINIT_FUNC PyInit__winapi(void);
PyMODINIT_FUNC PyInit__lsprof(void);
PyMODINIT_FUNC PyInit__ast(void);
PyMODINIT_FUNC PyInit__io(void);
PyMODINIT_FUNC PyInit__pickle(void);
PyMODINIT_FUNC PyInit_atexit(void);
PyMODINIT_FUNC _PyWarnings_Init(void);
PyMODINIT_FUNC PyInit__string(void);
PyMODINIT_FUNC PyInit__stat(void);
PyMODINIT_FUNC PyInit__opcode(void);

/* tools/freeze/makeconfig.py marker for additional "extern" */
/* -- ADDMODULE MARKER 1 -- */

PyMODINIT_FUNC PyMarshal_Init(void);
PyMODINIT_FUNC PyInit_imp(void);

}

struct _inittab _PySourceImport_Inittab[] = {

    {"array", PyInit_array},
    {"_ast", PyInit__ast},
#ifdef MS_WINDOWS
#ifndef MS_WINI64
    {"audioop", PyInit_audioop},
#endif
#endif
    {"binascii", PyInit_binascii},
    {"cmath", PyInit_cmath},
    {"errno", PyInit_errno},
    {"faulthandler", PyInit_faulthandler},
    {"gc", PyInit_gc},
    {"math", PyInit_math},
    {"nt", PyInit_nt}, /* Use the NT os functions, not posix */
    {"_operator", PyInit__operator},
    {"signal", PyInit_signal},
    {"_md5", PyInit__md5},
    {"_sha1", PyInit__sha1},
    {"_sha256", PyInit__sha256},
    {"_sha512", PyInit__sha512},
    {"time", PyInit_time},
#ifdef WITH_THREAD
    {"_thread", PyInit__thread},
#endif
#ifdef WIN32
    {"msvcrt", PyInit_msvcrt},
    {"_locale", PyInit__locale},
#endif
    {"_tracemalloc", PyInit__tracemalloc},
    /* XXX Should _winapi go in a WIN32 block?  not WIN64? */
    {"_winapi", PyInit__winapi},

    {"_codecs", PyInit__codecs},
    {"_weakref", PyInit__weakref},
    {"_random", PyInit__random},
    {"_bisect", PyInit__bisect},
    {"_heapq", PyInit__heapq},
    {"_lsprof", PyInit__lsprof},
    {"itertools", PyInit_itertools},
    {"_collections", PyInit__collections},
    {"_symtable", PyInit__symtable},
    {"mmap", PyInit_mmap},
    {"_csv", PyInit__csv},
    {"_sre", PyInit__sre},
    {"parser", PyInit_parser},
    {"winreg", PyInit_winreg},
    {"_struct", PyInit__struct},
    {"_datetime", PyInit__datetime},
    {"_functools", PyInit__functools},
    {"_json", PyInit__json},

    {"xxsubtype", PyInit_xxsubtype},
    {"zipimport", PyInit_zipimport},
    {"zlib", PyInit_zlib},

    /* CJK codecs */
    {"_multibytecodec", PyInit__multibytecodec},
    {"_codecs_cn", PyInit__codecs_cn},
    {"_codecs_hk", PyInit__codecs_hk},
    {"_codecs_iso2022", PyInit__codecs_iso2022},
    {"_codecs_jp", PyInit__codecs_jp},
    {"_codecs_kr", PyInit__codecs_kr},
    {"_codecs_tw", PyInit__codecs_tw},

/* tools/freeze/makeconfig.py marker for additional "_inittab" entries */
/* -- ADDMODULE MARKER 2 -- */

    /* This module "lives in" with marshal.c */
    {"marshal", PyMarshal_Init},

    /* This lives it with import.c */
    {"_imp", PyInit_imp},

    /* These entries are here for sys.builtin_module_names */
    {"builtins", NULL},
    {"sys", NULL},
    {"_warnings", _PyWarnings_Init},
    {"_string", PyInit__string},

    {"_io", PyInit__io},
    {"_pickle", PyInit__pickle},
    {"atexit", PyInit_atexit},
    {"_stat", PyInit__stat},
    {"_opcode", PyInit__opcode},

    /* Sentinel */
    {0, 0}
};

#endif // WIN32

#ifdef OSX

extern "C" {
	
	
	extern PyObject* PyInit__thread(void);
	extern PyObject* PyInit_signal(void);
	extern PyObject* PyInit_posix(void);
	extern PyObject* PyInit_errno(void);
	extern PyObject* PyInit_pwd(void);
	extern PyObject* PyInit__sre(void);
	extern PyObject* PyInit__codecs(void);
	extern PyObject* PyInit__weakref(void);
	extern PyObject* PyInit__functools(void);
	extern PyObject* PyInit__operator(void);
	extern PyObject* PyInit__collections(void);
	extern PyObject* PyInit_itertools(void);
	extern PyObject* PyInit_atexit(void);
	extern PyObject* PyInit__stat(void);
	extern PyObject* PyInit__locale(void);
	extern PyObject* PyInit__io(void);
	extern PyObject* PyInit_zipimport(void);
	extern PyObject* PyInit_faulthandler(void);
	extern PyObject* PyInit__tracemalloc(void);
	extern PyObject* PyInit__symtable(void);
	extern PyObject* PyInit_array(void);
	extern PyObject* PyInit_cmath(void);
	extern PyObject* PyInit_math(void);
	extern PyObject* PyInit__struct(void);
	extern PyObject* PyInit_time(void);
	extern PyObject* PyInit__weakref(void);
	extern PyObject* PyInit__random(void);
	extern PyObject* PyInit__elementtree(void);
	extern PyObject* PyInit__pickle(void);
	extern PyObject* PyInit__datetime(void);
	extern PyObject* PyInit__bisect(void);
	extern PyObject* PyInit__heapq(void);
	extern PyObject* PyInit_unicodedata(void);
	extern PyObject* PyInit__socket(void);
	extern PyObject* PyInit__md5(void);
	extern PyObject* PyInit__sha1(void);
	extern PyObject* PyInit__sha256(void);
	extern PyObject* PyInit__sha512(void);
	extern PyObject* PyInit_binascii(void);
	extern PyObject* PyInit_parser(void);
	extern PyObject* PyInit_xxsubtype(void);
	/* -- ADDMODULE MARKER 1 -- */
	
	extern PyObject* PyMarshal_Init(void);
	extern PyObject* PyInit_imp(void);
	extern PyObject* PyInit_gc(void);
	extern PyObject* PyInit__ast(void);
	extern PyObject* _PyWarnings_Init(void);
	extern PyObject* PyInit__string(void);
	
	struct _inittab _PySourceImport_Inittab[] = {
		
		{"_thread", PyInit__thread},
		{"signal", PyInit_signal},
		{"posix", PyInit_posix},
		{"errno", PyInit_errno},
		{"pwd", PyInit_pwd},
		{"_sre", PyInit__sre},
		{"_codecs", PyInit__codecs},
		{"_weakref", PyInit__weakref},
		{"_functools", PyInit__functools},
		{"_operator", PyInit__operator},
		{"_collections", PyInit__collections},
		{"itertools", PyInit_itertools},
		{"atexit", PyInit_atexit},
		{"_stat", PyInit__stat},
		{"_locale", PyInit__locale},
		{"_io", PyInit__io},
		{"zipimport", PyInit_zipimport},
		{"faulthandler", PyInit_faulthandler},
		{"_tracemalloc", PyInit__tracemalloc},
		{"_symtable", PyInit__symtable},
		{"array", PyInit_array},
		{"cmath", PyInit_cmath},
		{"math", PyInit_math},
		{"_struct", PyInit__struct},
		{"time", PyInit_time},
		{"_weakref", PyInit__weakref},
		{"_random", PyInit__random},
		{"_elementtree", PyInit__elementtree},
		{"_pickle", PyInit__pickle},
		{"_datetime", PyInit__datetime},
		{"_bisect", PyInit__bisect},
		{"_heapq", PyInit__heapq},
		{"unicodedata", PyInit_unicodedata},
		{"_socket", PyInit__socket},
		{"_md5", PyInit__md5},
		{"_sha1", PyInit__sha1},
		{"_sha256", PyInit__sha256},
		{"_sha512", PyInit__sha512},
		{"binascii", PyInit_binascii},
		{"parser", PyInit_parser},
		{"xxsubtype", PyInit_xxsubtype},
		/* -- ADDMODULE MARKER 2 -- */
		
		/* This module lives in marshal.c */
		{"marshal", PyMarshal_Init},
		
		/* This lives in import.c */
		{"_imp", PyInit_imp},
		
		/* This lives in Python/Python-ast.c */
		{"_ast", PyInit__ast},
		
		/* These entries are here for sys.builtin_module_names */
		{"builtins", NULL},
		{"sys", NULL},
		
		/* This lives in gcmodule.c */
		{"gc", PyInit_gc},
		
		/* This lives in _warnings.c */
		{"_warnings", _PyWarnings_Init},
		
		/* This lives in Objects/unicodeobject.c */
		{"_string", PyInit__string},
		
		/* Sentinel */
		{0, 0}
	};
}

#endif // OSX
