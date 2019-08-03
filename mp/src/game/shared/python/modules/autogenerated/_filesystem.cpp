// This file has been generated by Py++.

#include "cbase.h"



#include "filesystem.h"

#include "srcpy_filesystem.h"

#include "srcpy.h"

#include "tier0/memdbgon.h"

namespace bp = boost::python;

BOOST_PYTHON_MODULE(_filesystem){
    bp::docstring_options doc_options( true, true, false );

    { //::PyFS_CreateDirHierarchy
    
        typedef void ( *CreateDirHierarchy_function_type )( char const *,char const * );
        
        bp::def( 
            "CreateDirHierarchy"
            , CreateDirHierarchy_function_type( &::PyFS_CreateDirHierarchy )
            , ( bp::arg("path"), bp::arg("pathID")=bp::object() ) );
    
    }

    { //::PyFS_FileExists
    
        typedef bool ( *FileExists_function_type )( char const *,char const * );
        
        bp::def( 
            "FileExists"
            , FileExists_function_type( &::PyFS_FileExists )
            , ( bp::arg("pFileName"), bp::arg("pPathID")=bp::object() ) );
    
    }

    { //::PyFS_FindOrAddFileName
    
        typedef int ( *FindOrAddFileName_function_type )( char const * );
        
        bp::def( 
            "FindOrAddFileName"
            , FindOrAddFileName_function_type( &::PyFS_FindOrAddFileName )
            , ( bp::arg("pFileName") ) );
    
    }

    { //::PyFS_FullPathToRelativePath
    
        typedef ::boost::python::api::object ( *FullPathToRelativePath_function_type )( char const *,char const *,::boost::python::api::object );
        
        bp::def( 
            "FullPathToRelativePath"
            , FullPathToRelativePath_function_type( &::PyFS_FullPathToRelativePath )
            , ( bp::arg("path"), bp::arg("pathid")=bp::object(), bp::arg("defaultvalue")=boost::python::object() ) );
    
    }

    { //::PyFS_GetFileTime
    
        typedef long int ( *GetFileTime_function_type )( char const *,char const * );
        
        bp::def( 
            "GetFileTime"
            , GetFileTime_function_type( &::PyFS_GetFileTime )
            , ( bp::arg("pFileName"), bp::arg("pPathID")=bp::object() ) );
    
    }

    { //::PyFS_IsAbsolutePath
    
        typedef bool ( *IsAbsolutePath_function_type )( char const * );
        
        bp::def( 
            "IsAbsolutePath"
            , IsAbsolutePath_function_type( &::PyFS_IsAbsolutePath )
            , ( bp::arg("path") ) );
    
    }

    { //::PyFS_IsDirectory
    
        typedef bool ( *IsDirectory_function_type )( char const *,char const * );
        
        bp::def( 
            "IsDirectory"
            , IsDirectory_function_type( &::PyFS_IsDirectory )
            , ( bp::arg("pFileName"), bp::arg("pathID")=bp::object() ) );
    
    }

    { //::PyFS_ListDir
    
        typedef ::boost::python::list ( *ListDir_function_type )( char const *,char const *,char const *,bool );
        
        bp::def( 
            "ListDir"
            , ListDir_function_type( &::PyFS_ListDir )
            , ( bp::arg("path"), bp::arg("pathid")=bp::object(), bp::arg("wildcard")="*", bp::arg("appendslashdir")=(bool)(true) ) );
    
    }

    { //::PyFS_ReadFile
    
        typedef ::boost::python::api::object ( *ReadFile_function_type )( char const *,char const *,bool,int,int,bool );
        
        bp::def( 
            "ReadFile"
            , ReadFile_function_type( &::PyFS_ReadFile )
            , ( bp::arg("filepath"), bp::arg("pathid"), bp::arg("optimalalloc")=(bool)(false), bp::arg("maxbytes")=(int)(0), bp::arg("startingbyte")=(int)(0), bp::arg("textmode")=(bool)(false) ) );
    
    }

    { //::PyFS_RelativePathToFullPath
    
        typedef ::boost::python::api::object ( *RelativePathToFullPath_function_type )( char const *,char const *,::boost::python::api::object );
        
        bp::def( 
            "RelativePathToFullPath"
            , RelativePathToFullPath_function_type( &::PyFS_RelativePathToFullPath )
            , ( bp::arg("path"), bp::arg("pathid")=bp::object(), bp::arg("defaultvalue")=boost::python::object() ) );
    
    }

    { //::PyFS_Size
    
        typedef unsigned int ( *Size_function_type )( char const *,char const * );
        
        bp::def( 
            "Size"
            , Size_function_type( &::PyFS_Size )
            , ( bp::arg("pFileName"), bp::arg("pPathID")=bp::object() ) );
    
    }

    { //::PyFS_String
    
        typedef ::boost::python::tuple ( *String_function_type )( int );
        
        bp::def( 
            "String"
            , String_function_type( &::PyFS_String )
            , ( bp::arg("handle") ) );
    
    }

    { //::PyFS_WriteFile
    
        typedef void ( *WriteFile_function_type )( char const *,char const *,char const * );
        
        bp::def( 
            "WriteFile"
            , WriteFile_function_type( &::PyFS_WriteFile )
            , ( bp::arg("filepath"), bp::arg("pathid"), bp::arg("content") ) );
    
    }
}
