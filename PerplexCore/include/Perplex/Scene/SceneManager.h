#pragma once

#include <Perplex/Core/Core.h>

namespace Perplex
{
	class Scene;
	class Asset;

	class SceneManager
	{
	public:
		SceneManager();

		static SceneManager& Get()
		{
			static SceneManager instance{};
			return instance;
		}

		bool CanLoadScene(Asset asset);
		void LoadScene(Asset asset);
		void SaveScene();
		Ref<Scene> ActiveScene() { return m_ActiveScene; }

		void Play();
		void Stop();
		void Reset();

		void OnUpdateEnd();
		bool JustLoaded() const { return m_JustLoaded; }

		const std::filesystem::path& GetActiveScenePath() const { return m_ActiveScenePath; }
	private:
		std::filesystem::path m_ActiveScenePath{};

		Ref<Scene> m_ActiveScene{};
		Ref<Scene> m_SavedScene{};
		Ref<Scene> m_NextScene{};

		bool m_JustLoaded{ false };
	};
}