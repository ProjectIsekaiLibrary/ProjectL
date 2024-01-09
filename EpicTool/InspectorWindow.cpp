#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "FileSave.h"
#include "KunrealAPI.h"
#include "SceneManager.h"
#include "InspectorWindow.h"

EpicTool::InspectorWindow::InspectorWindow()
	:_gameObjectlist(NULL), _selectedObjectIndex(0), _tranform(nullptr), _meshState(false), _cameraFix(false)//, _selectedMesh(0)
	, _selectedNormal(-1), _comboMeshSelect(-1), _comboNormalSelect(-1), _comboDiffuseSelect(-1), _selectObjectNumber(-1), _isObjectActive(true), _comboLightSelect(-1)
	, _isMeshRenderActive(true), _isLightActive(true), _isCameraActive(true), _ambient{ 0 }, _diffuse{ 0 }, _specular{ 0 }, _direction{ 0 }, _lightGet(true), _pointDiffuse{ 0 }, _pointRange(0),
	_pointAmbient{0}, _pointSpecular{0}
{																			
																			
}																			

EpicTool::InspectorWindow::~InspectorWindow()
{

}



template<typename PieceType>
void EpicTool::InspectorWindow::DrawComponentPiece(PieceType& data, const std::string name)
{
    const int numberDataType = sizeof(PieceType) / sizeof(int);

    const char* cName = name.c_str();

    // int

    if constexpr (std::is_same_v<PieceType, int>)
    {
        ImGui::DragInt(cName, data);
    }
    else if constexpr (std::is_same_v<PieceType, int[numberDataType]>)
    {
        switch (numberDataType)
        {
            case 1:
                ImGui::DragInt(cName, data);
                break;
            case 2:
                ImGui::DragInt2(cName, data);
                break;
            case 3:
                ImGui::DragInt3(cName, data);
                break;
            default:
                break;
        }
    }
    else if constexpr (std::is_same_v< PieceType, float>)
    {
        ImGui::DragFloat(cName, data);
    }
    // float 배열까지 모두 적용 가능
    else if constexpr (std::is_same_v<PieceType, float[numberDataType]>)
    {
        switch (numberDataType)
        {
            case 1:
                ImGui::DragFloat(cName, data);
                break;
            case 2:
                ImGui::DragFloat2(cName, data);
                break;
            case 3:
                ImGui::DragFloat3(cName, data);
				break;
			case 4:
				ImGui::DragFloat4(cName, data);
                break;
            default:
                break;
        }
    }
    // double
    else if constexpr (std::is_same_v<PieceType, double>)
    {
        ImGui::DragFloat(cName, data);
    }
    else if constexpr (std::is_same_v<PieceType, double[numberDataType]>)
    {
        switch (numberDataType)
        {
            case 1:
                ImGui::DragFloat(cName, data);
                break;
            case 2:
                ImGui::DragFloat2(cName, data);
                break;
            case 3:
                ImGui::DragFloat3(cName, data);
                break;
            default:
                break;
        }
    }

    else if constexpr (std::is_same_v<PieceType, std::vector>)
    {
        ImGui::InputDouble(cName, &data.x, 0.001f, 0.0f, "%.3f");
        ImGui::SameLine();
        ImGui::InputDouble(cName, &data.y, 0.001f, 0.0f, "%.3f");
        ImGui::SameLine();
        ImGui::InputDouble(cName, &data.z, 0.001f, 0.0f, "%.3f");
        ImGui::SameLine();
    }

}


template<>
void EpicTool::InspectorWindow::DrawComponentPiece<bool>(bool& instance, const std::string name)
{
    //이름을 가져오는 부분
    ImGui::Spacing();

    ImGui::Text("%s", name.c_str());

	ImGui::SameLine();

    ImGui::Checkbox(" ", &instance);

	ImGui::Spacing();
}

template<typename ComponentType>
void EpicTool::InspectorWindow::DrawComponentInfo(ComponentType* instance)
{

}

void EpicTool::InspectorWindow::DeleteComponent(KunrealEngine::Component* instance)
{
    if (ImGui::Button("DeleteCompoent"))
    {
        _gameObjectlist[_selectedObjectIndex]->DeleteComponent(instance);
        _compoenetList = _gameObjectlist[_selectedObjectIndex]->GetComponentList();
	}
	//if (ImGui::Button("Cube"))
	//{
		//KunrealEngine::GraphicsSystem::GetInstance().CreateCubeMap("내가 관리하고 싶은 이름", "경로");
		//KunrealEngine::GraphicsSystem::GetInstance().DeleteCubeMap("내가 관리하고 싶은 이름");
		//KunrealEngine::GraphicsSystem::GetInstance().SetMainCubeMap("내가 관리하고 싶은 이름");
		//KunrealEngine::GraphicsSystem::GetInstance().GetCubeMapList();		// std::vector<std::string> 이걸로 반환
	//}
}

template<> // 이후 리스트가 생긴하면 해당 부분을 통해서 사용하게 될것이다  // 컴포넌트 리스트
void EpicTool::InspectorWindow::DrawComponentInfo<KunrealEngine::Transform>(KunrealEngine::Transform* instance)
{
	float _positionArray[3];
	float _rotationArray[3];
	float _scaleArray[3];

	_positionArray[0] = instance->GetPosition().x;
	_positionArray[1] = instance->GetPosition().y;
	_positionArray[2] = instance->GetPosition().z;

	DrawComponentPiece(_positionArray, "Position");
	instance->SetPosition(_positionArray[0], _positionArray[1], _positionArray[2]);


	_rotationArray[0] = instance->GetRotation().x;
	_rotationArray[1] = instance->GetRotation().y;
	_rotationArray[2] = instance->GetRotation().z;

	DrawComponentPiece(_rotationArray, "Rotation");
    instance->SetRotation(_rotationArray[0], _rotationArray[1], _rotationArray[2]);

	_scaleArray[0] = instance->GetScale().x;
	_scaleArray[1] = instance->GetScale().y;
	_scaleArray[2] = instance->GetScale().z;

	DrawComponentPiece(_scaleArray, "Scale");
    instance->SetScale(_scaleArray[0], _scaleArray[1], _scaleArray[2]);

}

template<>
void EpicTool::InspectorWindow::DrawComponentInfo<KunrealEngine::Camera>(KunrealEngine::Camera* instance)
{
    bool isComponent = false;

	for (auto componentName : _compoenetList)
	{
		std::string checkName = componentName->GetComponentName();
		if (checkName == "Camera")
		{
			isComponent = true;
		}
	}

	if (isComponent == false) // 깡통 추가
	{
		_gameObjectlist[_selectedObjectIndex]->AddComponent<KunrealEngine::Camera>();
	}


	_isCameraActive = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>()->GetActivated();

	ImGui::Checkbox("SetActive", &_isCameraActive);

	_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>()->SetActive(_isCameraActive);
	


	DrawComponentPiece<bool>(_cameraFix, "FixCamera");

   
	if (_cameraFix == true)
	{
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>()->FixCamera();
	}
	else if (_cameraFix == false)
	{
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>()->UnfixCamera();
	}

    DeleteComponent(instance);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

}

template<>
void EpicTool::InspectorWindow::DrawComponentInfo<KunrealEngine::MeshRenderer>(KunrealEngine::MeshRenderer* instance)
{
    bool meshState = false;
    bool isComponent = false;
        
    _compoenetList = _gameObjectlist[_selectedObjectIndex]->GetComponentList();

	for (auto componentName : _compoenetList) 
	{
		std::string checkName = componentName->GetComponentName();
		if (checkName == "MeshRenderer")
		{
			isComponent = true;
		}
	}
       
	if (isComponent == false) // 깡통 추가
	{
	    _gameObjectlist[_selectedObjectIndex]->AddComponent<KunrealEngine::MeshRenderer>();
	}

	if (_selectObjectName != _gameObjectlist[_selectedObjectIndex]->GetObjectName())
	{
		_comboMeshSelect = -1;

		_comboNormalSelect = -1;

		_comboDiffuseSelect = -1;
	}

	_isMeshRenderActive = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetActivated();

	if (ImGui::Checkbox("SetActive", &_isMeshRenderActive))
	{
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->SetActive(_isMeshRenderActive);
	}


    ComboMeshControl(_meshList, _meshListEditor, _comboMeshSelect, "MeshList");

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    ComboMeshControl(_textureList, _TextureListEditor, _comboDiffuseSelect, "Diffuse");
    
    ComboMeshControl(_textureList, _TextureListEditor, _comboNormalSelect, "Normal");


	//if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshStatus()) // 임시 예외처리임
	//{
	//	meshState = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetRenderingState();

	//	DrawComponentPiece<bool>(meshState, "MeshState");

 //      

	//	instance->SetActive(meshState);  // 만약 메쉬가 없는 오브젝트를 선택 후 컴포넌트를 추가 하면 터짐 
 //   }

    DeleteComponent(instance);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::Button("ResetList"))
	{
		_meshList = KunrealEngine::GraphicsSystem::GetInstance().GetRenderableList();

		_textureList = KunrealEngine::GraphicsSystem::GetInstance().GetTextureList();

		_textureList.insert(_textureList.begin(), "None");

		_comboMeshSelect = -1;

		_comboNormalSelect = -1;

		_comboDiffuseSelect = -1;

		ListToRemove(_meshList, _meshListEditor, _meshStringToRemove);

		ListToRemove(_textureList, _TextureListEditor, _textureStringToRemove);
	}

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();
}

template<>
void EpicTool::InspectorWindow::DrawComponentInfo<KunrealEngine::Light>(KunrealEngine::Light* instance)
{
	bool isComponent = false;
	_compoenetList = _gameObjectlist[_selectedObjectIndex]->GetComponentList();

	for (auto componentName : _compoenetList)
	{
		std::string checkName = componentName->GetComponentName();
		if (checkName == "Light")
		{
			isComponent = true;
			
		}
	}

	if (isComponent == false) // 깡통 추가
	{
		_gameObjectlist[_selectedObjectIndex]->AddComponent<KunrealEngine::Light>();
	}

	_isLightActive = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetActivated();

	ComboLightControl(_lightList, _lightList, _comboLightSelect);

	if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightStatus() && _selectedObjectIndex2 != _selectedObjectIndex) // 예쁜 모양은 아님
	{																																					// 해당 코드는 임시로, 엔진초기화 함수에서 예시코드를 작성했을때 사용되는 부분이다
		_ambient[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().x;
		_ambient[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().y;
		_ambient[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().z;
		_ambient[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().w;
		
		_diffuse[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().x;
		_diffuse[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().y;
		_diffuse[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().z;
		_diffuse[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().w;
		
		_specular[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().x;
		_specular[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().y;
		_specular[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().z;
		_specular[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().w;

		_direction[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDirection().x;
		_direction[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDirection().y;
		_direction[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDirection().z;	

		_pointAmbient[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().x;
		_pointAmbient[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().y;
		_pointAmbient[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().z;
		_pointAmbient[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetAmbient().w;

		_pointDiffuse[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().x;
		_pointDiffuse[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().y;
		_pointDiffuse[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().z;
		_pointDiffuse[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetDiffuse().w;

		_pointSpecular[0] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().x;
		_pointSpecular[1] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().y;
		_pointSpecular[2] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().z;
		_pointSpecular[3] = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetSpecular().w;

		_selectedObjectIndex2 = _selectedObjectIndex;
	}



	if (_lightSelect == "DirectionalLight" || _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightType() == KunrealEngine::DirectionalLight)
	{		
		ImGui::DragFloat4("Ambient" , _ambient, 0.0f, 0.0f , 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetAmbient(_ambient[0], _ambient[1], _ambient[2], _ambient[3]);
		
		ImGui::DragFloat4("Diffuse", _diffuse,  0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetDiffuse(_diffuse[0], _diffuse[1], _diffuse[2], _diffuse[3]);
		
		ImGui::DragFloat4( "Specular", _specular, 0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetSpecular(_specular[0], _specular[1], _specular[2], _specular[3]);
		
		ImGui::DragFloat3("Direction", _direction, 0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetDirection(_direction[0], _direction[1], _direction[2]);
	}

	if (_lightSelect == "PointLight" || _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightType() == KunrealEngine::PointLight)
	{
		ImGui::DragFloat4("Ambient", _pointAmbient, 0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetAmbient(_pointAmbient[0], _pointAmbient[1], _pointAmbient[2], _pointAmbient[3]);

		ImGui::DragFloat4("Diffuse", _pointDiffuse, 0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetDiffuse(_pointDiffuse[0], _pointDiffuse[1], _pointDiffuse[2], _pointDiffuse[3]);

		ImGui::DragFloat4("Specular", _pointSpecular, 0.0f, 0.0f, 1.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetSpecular(_pointSpecular[0], _pointSpecular[1], _pointSpecular[2], _pointSpecular[3]);

		// 여기에 기존 range값 받아오는 코드 필요

		_pointRange = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetPointRange();

		ImGui::DragFloat("Range", &_pointRange, 1.0f, 0.0f);
		_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->SetPointRange(_pointRange);
	}


	DeleteComponent(instance);
}

void EpicTool::InspectorWindow::Initialize() // 필요없어보이는데
{
	_gameObjectlist = KunrealEngine::GetCurrentScene()->
		GetObjectList();
	_tranform = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Transform>();
	
    _meshRenderer = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>();

    _camera = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>();

    _meshList = KunrealEngine::GraphicsSystem::GetInstance().GetRenderableList();

    _textureList = KunrealEngine::GraphicsSystem::GetInstance().GetTextureList();

	_textureList.insert(_textureList.begin(), "None");

   // _selectedMesh = -1; // 선택하지 않은 상태

    _lightList.push_back("DirectionalLight");
    _lightList.push_back("PointLight");
    _lightList.push_back("SpotLight");

	//_ambient[4] = {};

    ListToRemove(_meshList, _meshListEditor, _meshStringToRemove);

    ListToRemove(_textureList, _TextureListEditor, _textureStringToRemove);

}

void EpicTool::InspectorWindow::ShowWindow(int& selectedObjectIndex)
{

	_gameObjectlist = KunrealEngine::GetCurrentScene()->
		GetObjectList();
	
	static bool selectedComponentIndex = false;
	bool transformOpen = false;
	bool meshStateOpen = false;
	bool CameraOpen = false;
    bool LightOpen = false;

    _selectedObjectIndex = selectedObjectIndex;

    if (_selectedObjectIndex != -1)
    {
        _compoenetList = _gameObjectlist[_selectedObjectIndex]->GetComponentList();

        SetSelectObject();      

        if (selectedComponentIndex)
        {
            const char* items[] = {"MeshRenderer" , "Camera" , "Light"};
            int selectedItem = -1;

            if (ImGui::Combo("Component", &selectedItem, items, 3)) {
                // 사용자가 새로운 아이템을 선택했을 때 실행할 코드
                // 임시 테스트 용이며 삭제할것임
                switch (selectedItem)
                {
                    case 0:                        
                        IsCheckItem(meshStateOpen);
                         selectedComponentIndex = !selectedComponentIndex;
                        break;
                    case 1:
                        IsCheckItem(CameraOpen);
                        selectedComponentIndex = !selectedComponentIndex;
						break;
					case 2:
						IsCheckItem(LightOpen);
						selectedComponentIndex = !selectedComponentIndex;
						break;
                    default:
                        break;
                }
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
        }


		for (auto componentlist : _compoenetList)
		{
			std::string checkComponentName = componentlist->GetComponentName();

			if (checkComponentName == "MeshRenderer" && checkComponentName != "Light")
			{
				meshStateOpen = true; // 아직 추가한 컴포넌트를 제거하는 기능은 없음
			}
			if (checkComponentName == "Camera" && checkComponentName != "Light")
			{
				CameraOpen = true;
			}
			if (checkComponentName == "Light")
			{
				LightOpen = true;
			}
		}

		if (meshStateOpen == true)
		{
			DrawComponentInfo(_meshRenderer);
		}

		if (CameraOpen == true)
		{
			DrawComponentInfo(_camera);
		}

        if (LightOpen == true)
        {
            DrawComponentInfo(_light);
        }

		if (ImGui::Button("Add Component"))
		{
			selectedComponentIndex = !selectedComponentIndex;
		}

    }    
}



void EpicTool::InspectorWindow::ShowWindow()
{

}

void EpicTool::InspectorWindow::SetSelectObject()
{
    _tranform = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Transform>();
    _meshRenderer = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>();
    _camera = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Camera>();
	_light = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>();

    std::string ObjectName = _gameObjectlist[_selectedObjectIndex]->GetObjectName();
    char nameBuffer[256];
    

    std::strncpy(nameBuffer, ObjectName.c_str(), sizeof(nameBuffer));

    if (ImGui::InputText("Name", nameBuffer, sizeof(ObjectName)))
    {
        ObjectName = nameBuffer;
        _gameObjectlist[_selectedObjectIndex]->SetObjectName(ObjectName);
    }


    _isObjectActive = _gameObjectlist[_selectedObjectIndex]->GetActivated();

	if (ImGui::Checkbox("SetActive", &_isObjectActive))
	{
		_gameObjectlist[_selectedObjectIndex]->SetActive(_isObjectActive);
	}
    

    DrawComponentInfo(_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Transform>());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();
}

void EpicTool::InspectorWindow::IsCheckItem(bool& Item)
{
	if (Item != true)
	{
        Item = true;
	}
	else
	{
        Item = false;
	}
}

void EpicTool::InspectorWindow::SetMeshObjectEditor(std::vector<std::string>& meshList, int selectedItem)
{
    if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshStatus())
    {
        _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->ChangeMeshObject(meshList[selectedItem].c_str());
    }
    else
    {
        _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->SetMeshObject(meshList[selectedItem].c_str(), nullptr);
    }
}

void EpicTool::InspectorWindow::ComboMeshControl(std::vector<std::string> list, std::vector<std::string> listEditor, int selected, const char* name)
{

        if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshStatus())
		{
			std::string listStr;
			if (list == _meshList)  // 여기서 판단해서 분기할것
			{
				listStr = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshName();
				ListStrToRemove(listStr, _meshStringToRemove);
			}
			if (list == _textureList && name == "Normal")
			{
				listStr = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetNormalName();
				ListStrToRemove(listStr, _textureStringToRemove);
			}
			if (list == _textureList && name == "Diffuse")
			{
				listStr = _gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetTextureName();
				ListStrToRemove(listStr, _textureStringToRemove);
			}
		}
    
    
		if (ImGui::Combo(name, &selected, [](void* data, int idx, const char** out_text)
			{
				auto& items = *static_cast<std::vector<std::string>*>(data);
				if (idx < 0 || idx >= static_cast<int>(items.size()))
					return false;
				*out_text = items[idx].c_str();
				return true;
			},
			static_cast<void*>(&listEditor), static_cast<int>(listEditor.size())))
		{
			if (list == _meshList)  // 여기서 판단해서 분기할것
			{
				SetMeshObjectEditor(list, selected);
				_comboMeshSelect = selected;
			}
			if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->GetMeshStatus())
			{
				if (list == _textureList && name == "Normal")
				{
					_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->
						SetNormalTexture(list[selected].c_str());
					_comboNormalSelect = selected;
				}

				if (list == _textureList && name == "Diffuse")
				{
					_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::MeshRenderer>()->
						SetDiffuseTexture(list[selected].c_str());
					_comboDiffuseSelect = selected;
				}
			}
		}
       
}

void EpicTool::InspectorWindow::ComboLightControl(std::vector<std::string> list, std::vector<std::string> listEditor, int selected)
{

	if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightStatus())
	{
		if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightType() == EpicTool::LightType::DirectionalLight)
		{
			ImGui::Text("%s", list[EpicTool::LightType::DirectionalLight].c_str());
		}
		else if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightType() == EpicTool::LightType::PointLight)
		{
			ImGui::Text("%s", list[EpicTool::LightType::PointLight].c_str());
		}
		else if (_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightType() == EpicTool::LightType::SpotLight)
		{
			ImGui::Text("%s", list[EpicTool::LightType::SpotLight].c_str());
		}
		
	}
	
	
	if (ImGui::Combo("Light", &selected, [](void* data, int idx, const char** out_text)
		{
			auto& items = *static_cast<std::vector<std::string>*>(data);
			if (idx < 0 || idx >= static_cast<int>(items.size()))
				return false;
			*out_text = items[idx].c_str();
			return true;
		},
		static_cast<void*>(&listEditor), static_cast<int>(listEditor.size())))
	{
		if (list[selected] == _directionalLight && !(_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->GetLightStatus()))
		{
			_gameObjectlist[_selectedObjectIndex]->GetComponent<KunrealEngine::Light>()->CreateDirectionalLight();
			_lightSelect = "DirectionalLight";
		}

		_selectObjectName = _gameObjectlist[_selectedObjectIndex]->GetObjectName();
	}
}

void EpicTool::InspectorWindow::ListToRemove(std::vector<std::string>& list, std::vector<std::string>& listEditor, const std::string stringToRemove)
{
	for (const auto& listString : list)
	{
		std::string ListStr = listString;

		size_t pos = ListStr.find(stringToRemove);

		if (pos != std::string::npos)
		{
			ListStr.erase(pos, stringToRemove.length());
		}

        listEditor.push_back(ListStr);
	}
}

void EpicTool::InspectorWindow::ListStrToRemove(std::string& listStr, const std::string removeString)
{
	size_t pos = listStr.find(removeString);

	if (pos != std::string::npos)
	{
        listStr.erase(pos, removeString.length());
		ImGui::Text("%s", listStr.c_str());
	}
	else
	{
		ImGui::Text("%s", listStr.c_str());
	}
}

void EpicTool::InspectorWindow::ShowWindow(bool* p_open, std::vector<Object>& object)
{

}
