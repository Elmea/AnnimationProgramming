#include "stdafx.h"
#include "ResourceManager.h"

#include "DataStream.h"
#include "FileSystem.h"
#include "GlobalVariables.h"
#include "ScriptSerializer.h"

#include "Render/MeshSerializer.h"
#include "Render/MaterialSerializer.h"
#include "Render/TextureSerializer.h"
#include "Render/ShaderSerializer.h"
#include "Render/ShaderProgramSerializer.h"
#include "Render/FontSerializer.h"
#include "System/OperatingSystem.h"
#include "LogSystem/LogSystem.h"
#include "Animation/SkeletonSerializer.h"
#include "Animation/Skeleton.h"
#include "Animation/AnimationSerializer.h"
#include "Animation/Animation.h"

WHITEBOX_BEGIN

CResourceManager::CResourceManager()
	: m_bCheckForDependencyMode(false)
{
	AddResourceType< CSkeleton, CSkeletonSerializer >( "skel" );
	AddResourceType< CAnimation, CAnimationSerializer >( "anim" );
	AddResourceType< CMesh, CMeshSerializer >( "msh" );
	AddResourceType< CMaterial, CMaterialSerializer >( "mat" );
	AddResourceType< CTexture, CTextureSerializer >( "dds" );
 	AddResourceExtension< CTexture >( "png" );

	AddResourceType< CShader, CShaderSerializer >( "vs" );
	AddResourceExtension< CShader >( "ps" );

	AddResourceType< CShaderProgram, CShaderProgramSerializer >( "program" );

	//AddResourceType< CFont, CFontSerializer >( "ttf" );
}

void CResourceManager::AddResourceType( const String& name, const String& extension, IResourceSerializer* pSerializer )
{
	CResourceType* pResourceType = new CResourceType( name, m_resourceTypes.size(), pSerializer, extension );
	m_resourceTypes.push_back( pResourceType );
	m_resourceTypesByName[ name ] = pResourceType;
	m_resourceTypesByExtension[ extension ] = pResourceType;
}

void CResourceManager::AddResourceExtension( const String& typeName, const String& extension )
{
	CResourceType* pResourceType = m_resourceTypesByName[ typeName ];
	pResourceType->AddExtension( extension );
	m_resourceTypesByExtension[ extension ] = pResourceType;
}

void CResourceManager::AddLoadResourceQuery( CResourceDescriptor& descriptor )
{
	if ( !descriptor.IsProcedural() )
	{
		m_resourceLoadQueries.push_back( &descriptor );
	}
}

void CResourceManager::AddUnloadResourceQuery( CResourceDescriptor& descriptor )
{
	m_resourceUnloadQueries.push_back( &descriptor );
}

void CResourceManager::UpdateResourceLoading()
{
	std::vector< CResourceDescriptor* > dependenciesToLoad; dependenciesToLoad.reserve(100);

	if ( !m_bCheckForDependencyMode )
	{
		// 1. Force acquire dependencies that we're going to load (in order to avoid them being destroyed and then reloaded again)
		for ( size_t iRes = 0; iRes < m_resourceLoadQueries.size(); ++iRes )
		{
			CResourceDescriptor* pDescriptor = m_resourceLoadQueries[ iRes ];
			if ( pDescriptor->GetRefCount() == 0 )
			{
				continue; // Can happen if resource queried for load and unload the same frame
			}
			GetRecursiveDependencies( pDescriptor, dependenciesToLoad );
		}
		for ( size_t i = 0; i < dependenciesToLoad.size(); ++i )
		{
			dependenciesToLoad[i]->Acquire();
		}

		// 2. Destroy unused resources
		for ( size_t iRes = 0; iRes < m_resourceUnloadQueries.size(); ++iRes )
		{
			CResourceDescriptor* pDescriptor = m_resourceUnloadQueries[ iRes ];
			pDescriptor->DestroyIfNoRef();
		}
	}
	
	// 3. Load resources (and dependencies)
	for( size_t iRes = 0 ; iRes < ((m_bCheckForDependencyMode)? 1 : m_resourceLoadQueries.size()) ; ++iRes )
	{
		// Load the resource
		CResourceDescriptor* pDescriptor = m_resourceLoadQueries[ iRes ];
		if ( pDescriptor->GetRefCount() == 0 )
		{
			continue; // Can happen if resource queried for load and unload the same frame
		}

		//WbLog( "Default",  "Loading resource %s of type %s\n...", pDescriptor->GetName().c_str(), pDescriptor->GetResourceType()->GetName().c_str() );
		
		CDataStream dataStream;
		gVars->pOperatingSystem->GetDataStream( m_resourceRootDir + pDescriptor->GetPath(), dataStream, pDescriptor->GetSize() );
		
		IResource* pResource = pDescriptor->GetResourceType()->GetSerializer()->Load( dataStream, *pDescriptor );
		if ( pResource == NULL )
		{
			//WbLog( "Default",  "FAILED loading resource %s\n", pDescriptor->GetName().c_str() );
		}
		else
		{
		//	WbLog( "Default",  "Succeeded loading resource %s\n", pDescriptor->GetName().c_str() );
		}
		
		pDescriptor->SetResource( pResource );
		
		dataStream.Destroy();
	}

	// Resource post load
	for( CResourceDescriptor* pDescriptor : m_resourceLoadQueries )
	{
		if ( pDescriptor->GetResource() != nullptr )
		{
			pDescriptor->GetResource()->OnPostLoad();
		}
	}

	if ( !m_bCheckForDependencyMode )
	{
		// 4. Reset Release of step 1
		for ( size_t i = 0; i < dependenciesToLoad.size(); ++i )
		{
			dependenciesToLoad[ i ]->Release();
		}

		m_resourceLoadQueries.clear();
	}
}

CResourceDescriptor* CResourceManager::AddResource( const String& path, size_t size /*=0*/ )
{
	size_t pointIndex = path.find( '.' );
	if ( pointIndex == String::npos || pointIndex == 0 )
	{
		return nullptr;
	}
	
	String extension = path.substr( pointIndex + 1 );
	TResourceTypeMap::FindRes resTypeIt = m_resourceTypesByExtension.FindElement( extension );
	if ( resTypeIt == nullptr )
	{
		return nullptr;
	}
	CResourceType* pResourceType = *resTypeIt;
	
	size_t nameStartIndex = pointIndex-1;
	while( nameStartIndex > 0 && (path.c_str()[ nameStartIndex-1 ] != '/') )
	{
		--nameStartIndex;
	}
	CString name = path.substr( nameStartIndex, pointIndex - nameStartIndex );
	
	if ( size == 0 )
	{
		TFileHandle file = gVars->pFileSystem->OpenFile( path.c_str(), true, false );
		size = gVars->pFileSystem->GetFileSize( file );
		gVars->pFileSystem->CloseFile( file );	

		if ( size == 0 )
		{
	//		WbLog( "Default",  "Error : resource file %s not found\n", path.c_str() );
		}
	}
	
//	WbLog( "Default",  "Adding resource %s of extension %s and type %s and size %d, located at %s\n", name.c_str(), extension.c_str(), pResourceType->GetName().c_str(), (int)size, path.c_str() );
	
	return pResourceType->GetResourceSpecificManager()->AddResource( name, path, extension, size, pResourceType, this );	
}

void CResourceManager::ParseResources( const String& resourceFolder )
{
	String resourceFolderFormat = resourceFolder;
	if ( !resourceFolderFormat.empty() && (resourceFolderFormat.c_str()[ resourceFolderFormat.length()-1 ] != '/') )
	{
		resourceFolderFormat = resourceFolderFormat + "/";
	}

	m_resourceRootDir = resourceFolderFormat + "Resources/";

	CDataStream dataStream;
	gVars->pOperatingSystem->GetDataStream( resourceFolderFormat + "Resources.list", dataStream );

	CScriptStreamReader scriptStreamReader;
	SScriptNodePtr pScriptNode = scriptStreamReader.Parse( dataStream );
	
	CScriptNodeReader scriptNodeReader( pScriptNode );
	
	if ( scriptNodeReader.BeginGroup( "Resources" ) )
	{
		while ( scriptNodeReader.BeginGroup( "Resource" ) )
		{		
			String path;
			int size = 0;
			
			scriptNodeReader.Value( "path", path );
			scriptNodeReader.Value( "size", size );
			
			CResourceDescriptor* pDescriptor = AddResource( path, size );
			if ( pDescriptor != nullptr )
			{
				std::vector< String >& dependencies = pDescriptor->GetDependencies();
				
				while ( scriptNodeReader.BeginGroup( "Dependency" ) )
				{
					String dependency;
					scriptNodeReader.Value( "path", dependency );

					dependencies.push_back( dependency );

					scriptNodeReader.EndGroup();
				}
			}
			scriptNodeReader.EndGroup();
		}
	
		scriptNodeReader.EndGroup();
	}
}

void CResourceManager::ComputeResourceDependencies(const String& resourceRootDir, const String& path, std::vector< String >& dependencies )
{
	CResourceManager tempResourceManager;
	tempResourceManager.m_bCheckForDependencyMode = true;
	tempResourceManager.m_resourceRootDir = resourceRootDir;

	CResourceManager* pOldResMan = gVars->pResourceManager;
	gVars->pResourceManager = &tempResourceManager;

	CResourceDescriptor* pDescriptor = tempResourceManager.GetResourceDescriptor( path );
	if ( pDescriptor != nullptr )
	{
		pDescriptor->Acquire();

		tempResourceManager.UpdateResourceLoading();

		// m_resourceLoadQueries contains now the current resource, followed by all its direct dependencies
		for ( size_t i = 1; i < tempResourceManager.m_resourceLoadQueries.size(); ++i )
		{
			dependencies.push_back( tempResourceManager.m_resourceLoadQueries[ i ]->GetPath() );
		}

		pDescriptor->Release();
	}

	gVars->pResourceManager = pOldResMan;
}

CResourceDescriptor*	CResourceManager::GetResourceDescriptor( const String& path )
{
	TResourceTypeMap::FindRes itResType = m_resourceTypesByExtension.FindElement( path.get_path_extension() );
	if ( itResType == nullptr )
	{
		return nullptr;
	}

	CResourceType* pResourceType = *itResType;
	CResourceDescriptor* pDescriptor = pResourceType->GetResourceSpecificManager()->GetResourceDescriptor( path );
	if (pDescriptor == NULL)
	{
		if ( m_bCheckForDependencyMode )
		{
			AddResource( path );
		}

		pDescriptor = pResourceType->GetResourceSpecificManager()->GetResourceDescriptor( path );

		if (pDescriptor == nullptr)
		{
			WbLog( "Default", "Resource %s not found\n", path.c_str());
		}
	}
	
	return pDescriptor;
}

void	CResourceManager::GetRecursiveDependencies( CResourceDescriptor* pDescriptor, std::vector< CResourceDescriptor* >& dependencies )
{
	for( const String& dependencyPath : pDescriptor->GetDependencies() )
	{
		CResourceDescriptor* pDependency = GetResourceDescriptor( dependencyPath );
		if ( pDependency == nullptr )
		{
			continue;
		}
		dependencies.push_back( pDependency );
		GetRecursiveDependencies( pDependency, dependencies );
	}
}

WHITEBOX_END
