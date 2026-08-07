#include <Perplex/pch.h>
#include <Perplex/Scene/SceneManager.h>

#include <Perplex/Scene/Scene.h>
#include <Perplex/Core/Log.h>
#include <Perplex/Core/FileIO.h>
#include <Perplex/Scene/SceneSerializer.h>
#include <Perplex/Assets/Asset.h>
#include <Perplex/Assets/AssetType.h>

namespace Perplex
{
	SceneManager::SceneManager() : m_NextScene(CreateRef<Scene>()), m_SavedScene(CreateRef<Scene>()), m_ActiveScenePath(FileIO::GameRootDirectory() / "assets/scene.pxs")
	{ }

	bool SceneManager::CanLoadScene(Asset asset)
	{
		return asset && (asset.GetType() == AssetType::SceneAsset || asset.GetType() == AssetType::PrefabAsset);
	}

	void SceneManager::LoadScene(Asset asset)
	{
		if (!CanLoadScene(asset))
			HW_CORE_ERROR("Cannot load asset!");

		m_NextScene = asset.LoadData<Scene>();
		m_ActiveScenePath = asset.GetPath();
	}

	void SceneManager::SaveScene()
	{
		SceneSerializer::Serialize(m_ActiveScene, m_ActiveScenePath);
	}

	void SceneManager::OnUpdateEnd()
	{
		if (m_JustLoaded)
			m_JustLoaded = false;

		if (m_ActiveScene != m_NextScene)
		{
			bool playing = m_ActiveScene != nullptr && m_ActiveScene->IsPlaying();

			if (playing)
				Stop();

			m_ActiveScene = m_NextScene;

			if (playing)
				Play();

			m_JustLoaded = true;
		}
	}

	void SceneManager::Play()
	{
		m_SavedScene = CreateRef<Scene>(*m_ActiveScene.get());
		m_ActiveScene->Start();
	}

	void SceneManager::Stop()
	{
		m_ActiveScene->Stop();
		m_ActiveScene = CreateRef<Scene>(*m_SavedScene.get());
	}

	void SceneManager::Reset()
	{
		m_NextScene = m_ActiveScene;
	}
}