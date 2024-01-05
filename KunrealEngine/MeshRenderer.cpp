#include "MeshRenderer.h"
#include "GameObject.h"
#include "Transform.h"

KunrealEngine::MeshRenderer::MeshRenderer()
	:_mesh(nullptr), _transform(nullptr), _reflect(0.f, 0.f, 0.f, 0.f)
{

}

KunrealEngine::MeshRenderer::~MeshRenderer()
{

}

void KunrealEngine::MeshRenderer::Initialize()
{
	this->_transform = this->GetOwner()->GetComponent<Transform>();
}

void KunrealEngine::MeshRenderer::Finalize()
{
	if (GetMeshStatus())
	{
		this->_mesh->Delete();
	}
}

void KunrealEngine::MeshRenderer::FixedUpdate()
{

}

void KunrealEngine::MeshRenderer::Update()
{
	if (GetMeshStatus())
	{
		this->_mesh->SetTransform(_transform->GetWorldTM());
	}
}

void KunrealEngine::MeshRenderer::LateUpdate()
{

}

void KunrealEngine::MeshRenderer::OnTriggerEnter()
{

}

void KunrealEngine::MeshRenderer::OnTriggerStay()
{

}

void KunrealEngine::MeshRenderer::OnTriggerExit()
{

}

void KunrealEngine::MeshRenderer::SetActive(bool active)
{
	if (_mesh != nullptr)
	{
		SetRenderingState(active);
	}
}

void KunrealEngine::MeshRenderer::SetMeshObject(const char* fileName, const char* textureName)
{
	this->_mesh = GRAPHICS->CreateRenderable(fileName, textureName);
	this->_meshFileName = fileName;

	if (textureName != nullptr)
	{
		this->_textureName = textureName;
	}
}

std::string KunrealEngine::MeshRenderer::GetMeshName()
{
	if (_mesh != nullptr)
	{
		return this->_meshFileName;
	}
}

void KunrealEngine::MeshRenderer::ChangeMeshObject(const char* fileName)
{
	this->_mesh->Delete();
	
	SetMeshObject(fileName);
}

void KunrealEngine::MeshRenderer::SetRenderingState(bool flag)
{
	this->_mesh->SetRenderingState(flag);
}

bool KunrealEngine::MeshRenderer::GetRenderingState()
{
	return this->_mesh->GetRenderingState();
}

void KunrealEngine::MeshRenderer::SetDiffuseTexture(const char* textureName)
{
	this->_mesh->SetDiffuseTexture(textureName);
	this->_textureName = textureName;
}

std::string KunrealEngine::MeshRenderer::GetTextureName()
{
	return this->_textureName;
}

void KunrealEngine::MeshRenderer::SetNormalTexture(const char* textureName)
{
	this->_mesh->SetNormalTexture(textureName);
	this->_normalName = _textureName;
}

std::string KunrealEngine::MeshRenderer::GetNormalName()
{
	if (_mesh != nullptr)
	{
		return this->_normalName;
	}
}

void KunrealEngine::MeshRenderer::SetMaterial(GInterface::Material material)
{
	this->_mesh->SetMaterial(material);
}

GInterface::Material KunrealEngine::MeshRenderer::GetMaterial()
{
	if (_mesh != nullptr)
	{
		return this->_mesh->GetMaterial();
	}
}

void KunrealEngine::MeshRenderer::SetReflect(float x, float y, float z, float w)
{
	KunrealMath::Float4 ref(x, y, z, w);

	this->_reflect = ref;
	this->_mesh->SetReflect(ref);
}

KunrealEngine::KunrealMath::Float4 KunrealEngine::MeshRenderer::GetReflect()
{
	if (_mesh != nullptr)
	{
		return this->_reflect;
	}
}

bool KunrealEngine::MeshRenderer::GetMeshStatus()
{
	if (_mesh == nullptr)
	{
		return false;
	}
	else
	{
		return true;
	}
}