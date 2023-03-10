#include "stdafx.h"

#include "Render/RenderPipeline.h"
#include "Render/ShaderProgram.h"
#include "Render/Renderer.h"
#include "GlobalVariables.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "System/OperatingSystem.h"

#include "Application.h"
#include "Animation/Skeleton.h"

WHITEBOX_BEGIN

extern CSkeletonPtr skel;
extern CMeshPtr third;


#ifdef __GEAR_VR
struct SceneMatrices
{
	Matrix44	eyeViewMatrices[2];
	Matrix44	projectionMatrix;
};
#endif

CShaderUniformsValues&		CRenderPipeline::AddRenderProxyToQueue( CMesh* pMesh, size_t part, TRenderProxies& renderProxies, const Transform& meshTransform, CShaderProgram* pShaderProgram, bool bCullBackFace )
{
	renderProxies.push_back(SRenderProxy());
	SRenderProxy& proxy = renderProxies.back();

	CMesh::CPart* pPart = pMesh->GetPart( part );

	proxy.pIndexBuffer = pPart->GetIndexBuffer();
	proxy.pMaterial = pPart->GetMaterial().get();
	proxy.pShaderProgram = pShaderProgram;
	proxy.transformMatrix.FromTransform( meshTransform );

	proxy.bCullBackFace = bCullBackFace;
	
	return proxy.uniformValues;
}


void	CRenderPipeline::RenderQueue( TRenderProxies& renderProxies, IRenderTargetPtr pRenderTarget, const Matrix34& invCamMatrix, const Matrix44& projectionMatrix, size_t& drawCalls, size_t& polyCount )
{
	drawCalls = renderProxies.size();
	polyCount = 0;

	Matrix44	invCamMatrixFormated;
	
#ifdef __GEAR_VR
 	SceneMatrices* matrices = (SceneMatrices*)gVars->pRenderer->LockUniformBuffer(m_sceneMatUniformBufferId, 0, sizeof(SceneMatrices) );
 	
	matrices->projectionMatrix = projectionMatrix;
	matrices->eyeViewMatrices[ 0 ] = gVars->pOperatingSystem->GetInverseEyeMatrices()[ 0 ];
	matrices->eyeViewMatrices[ 1 ] = gVars->pOperatingSystem->GetInverseEyeMatrices()[ 1 ];

	Matrix44 formatInCamMatrix;
	gVars->pRenderer->FormatTransformMatrix(invCamMatrix, formatInCamMatrix);
	matrices->eyeViewMatrices[0] = formatInCamMatrix * matrices->eyeViewMatrices[0];
	matrices->eyeViewMatrices[1] = formatInCamMatrix * matrices->eyeViewMatrices[1];

 	gVars->pRenderer->UnlockUniformBuffer();
#else
	Matrix44* matrices = (Matrix44*)gVars->pRenderer->LockUniformBuffer( m_sceneMatUniformBufferId, 0, sizeof(Matrix44) );
	matrices[0] = projectionMatrix;
	gVars->pRenderer->UnlockUniformBuffer();
#endif

	Vec3 lightDir = Vec3(-1.0f, 0.0f, -1.0f);
	lightDir.Normalize();


	//Vec3* pLightDir = (Vec3*)gVars->pRenderer->LockUniformBuffer( m_lightUniformBufferId, 0, sizeof(Vec3) );
	//*pLightDir = lightDir;
	//gVars->pRenderer->UnlockUniformBuffer();

	SRenderProxy* pLastRenderProxy = nullptr;
	for ( size_t i = 0; i < renderProxies.size(); ++i )
	{
		SRenderProxy& renderProxy = renderProxies[ i ];


		gVars->pRenderer->BindIndexBuffer( renderProxy.pIndexBuffer->GetBufferId(), renderProxy.pIndexBuffer->GetIndexCount() );

		renderProxy.pShaderProgram->Bind(true);

		// Uniforms
 		Matrix44 transformMatrix;
#ifndef __GEAR_VR
		gVars->pRenderer->FormatTransformMatrix(invCamMatrix * renderProxy.transformMatrix, transformMatrix);

		renderProxy.uniformValues.SetUniformValue< Matrix44 >( renderProxy.pShaderProgram, "modelViewMatrix", transformMatrix );
#else
		gVars->pRenderer->FormatTransformMatrix(renderProxy.transformMatrix, transformMatrix);

		Matrix44 r;
		renderProxy.uniformValues.SetUniformValue< Matrix44 >(renderProxy.pShaderProgram, "modelMatrix", renderProxy.transformMatrix);
#endif
 		renderProxy.uniformValues.SetUniformValue< void* >(renderProxy.pShaderProgram, "SceneMatrices", m_sceneMatUniformBufferId);
 		//renderProxy.uniformValues.SetUniformValue< void* >(renderProxy.pShaderProgram, "Lighting", m_lightUniformBufferId);

	//	renderProxy.uniformValues.SetUniformValue< void* >(renderProxy.pShaderProgram, "SkinningMatrices", skinMatId);

		renderProxy.uniformValues.SetUniformValue< Vec3 >(renderProxy.pShaderProgram, "lightDirection", lightDir);
 		
 		renderProxy.uniformValues.ApplyValues( renderProxy.pShaderProgram );

		gVars->pRenderer->CullFace( renderProxy.bCullBackFace );

		if ( renderProxy.pMaterial != nullptr )
		{
			for (size_t iTex = 0; iTex < CMaterial::MAX_TEXTURE_LAYER; ++iTex)
			{
				CTexturePtr pTexture = renderProxy.pMaterial->m_textureLayers[iTex].m_pTexture;
				if (!pTexture)
				{
					break;
				}

				gVars->pRenderer->BindTexture( pTexture->GetTextureId(), iTex );

// 				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
// 				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
// 				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

// 
//   			glEnable(GL_BLEND);
// 				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			}
		}

		gVars->pRenderer->RenderBoundTriangles( renderProxy.pIndexBuffer->GetIndexCount() );

		pLastRenderProxy = &renderProxy;

		polyCount += renderProxy.pIndexBuffer->GetIndexCount() / 3;
	}
}


void* pFontId;
void			CRenderPipeline::Init( uint width, uint height )
{
	mainCamera.pRenderTarget = gVars->pRenderer->CreateRenderWindow( 0, 0, width, height );


#ifdef __GEAR_VR
	m_sceneMatUniformBufferId = gVars->pRenderer->CreateUniformBuffer( sizeof(SceneMatrices) );
	m_lightUniformBufferId = gVars->pRenderer->CreateUniformBuffer( sizeof(Vec3)  );
#else
	m_sceneMatUniformBufferId = gVars->pRenderer->CreateUniformBuffer( sizeof(Matrix44) );
	m_lightUniformBufferId = gVars->pRenderer->CreateUniformBuffer( sizeof(Vec3)  );
#endif



}



void			CRenderPipeline::Render()
{




#ifndef __GEAR_VR
 gVars->pRenderer->StartRenderFrame();
	gVars->pRenderer->BindRenderWindow( static_cast<CRenderWindow*>( mainCamera.pRenderTarget.get() ) );

 	mainCamera.ComputeProjectionMatrix();
#endif

	
	mainCamera.ComputeInverseTransformMatrix();

//  	CRenderPipeline::RenderQueue( mainRenderQueue, mainCamera.pRenderTarget, mainCamera.projectionMatrix, m_drawCalls, m_polyCount );
//  	mainRenderQueue.clear();

	RenderQueue(proxies, mainCamera.pRenderTarget, mainCamera.inverseTransformMatrix, mainCamera.projectionMatrix, m_drawCalls, m_polyCount);
 	proxies.clear();


	RenderQueue(m_proxies, mainCamera.pRenderTarget, mainCamera.inverseTransformMatrix, mainCamera.projectionMatrix, m_drawCalls, m_polyCount);

	Transform invCam = mainCamera.transform.GetInverse();
	for (const SDrawLine& drawLine : m_drawLines)
	{
		gVars->pRenderer->DrawLine(invCam * drawLine.p0, invCam * drawLine.p1, drawLine.color, mainCamera.projectionMatrix);
	}
	m_drawLines.clear();


 	gVars->pRenderer->UnbindIndexBuffer();
 	gVars->pRenderer->BindProgram( nullptr );
 	gVars->pRenderer->UnbindTexture( 0 );


}


CRenderPipeline::~CRenderPipeline()
{
	gVars->pRenderer->DestroyUniformBuffer( m_sceneMatUniformBufferId );

	gVars->pRenderer->DestroyUniformBuffer(m_lightUniformBufferId);
}


size_t			CRenderPipeline::GetDrawCalls() const
{
	return m_drawCalls;
}

size_t			CRenderPipeline::GetPolyCount() const
{
	return m_polyCount;
}


SRenderProxy&			CRenderPipeline::AddRenderProxy()
{
	size_t proxyIndex = m_proxies.size();

	m_proxies.resize( proxyIndex + 1 );
	m_proxies.back().index = proxyIndex;
	return m_proxies.back();
}

void			CRenderPipeline::RemoveRenderProxy( const SRenderProxy& proxy )
{
	size_t proxyIndex = proxy.index;
	if ( proxyIndex < m_proxies.size() - 1 )
	{
		m_proxies[ proxyIndex ] = m_proxies[ m_proxies.size() - 1 ];
	}
	m_proxies.resize( m_proxies.size() - 1 );
}

WHITEBOX_END