#pragma once

namespace GameStructs
{
	struct Game;
	struct CommunicationsHandler;

	enum PLAYERHUMANITY
	{
		ME_ABSENT = 0x0,
		ME_CLOSED = 0x1,
		ME_HUMAN = 0x2,
		ME_ELIMINATED = 0x3,
		ME_COMPUTER = 0x4,
		ME_CYBORG = 0x5,
		ME_VIEWONLY = 0x6,
	};

	Game* GamePointer = *(Game**)0x7912A0;
	CommunicationsHandler* CommPointer = *(CommunicationsHandler**)0x791200;
}

namespace GameFuncs
{
	namespace CommHandler
	{
		void(__thiscall* SetPlayerHumanity)(GameStructs::CommunicationsHandler*, int, int) = (void(__thiscall*)(GameStructs::CommunicationsHandler*, int, int))0x5D9680;
	}

	namespace TribeMPSetupScreen
	{
		int(__thiscall* LaunchGame)(void* This) = (int(__thiscall*)(void*))0x4FF390;
	}

	namespace BaseGame
	{
		void(__thiscall* SetMultiplayerGame)(GameStructs::Game* This, bool bMultiplayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1C0;
		
		void(__thiscall* SetSinglePlayerGame)(GameStructs::Game* This, bool bSinglePlayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1A0;
		
		void(__thiscall* SetScenarioName)(GameStructs::Game* This, const char* ScenarioName) = (void(__thiscall*)(GameStructs::Game*, const char*))0x5EB160;
		
		void(__thiscall* SetPlayerTeam)(GameStructs::Game* This, int PlayerNum, int Team) = (void(__thiscall*)(GameStructs::Game*, int, int))0x5EB300;
	
		void(__thiscall* LockGameTeams)(void* This) = (void(__thiscall*)(void*))0x5EB3D0;
	}

	namespace TribeGame
	{
		int(__thiscall* HandleIdle)(void* This) = (int(__thiscall*)(void*))0x445BC0;
		
		void(__thiscall* SetGameType)(GameStructs::Game* This, int GameType) = (void(__thiscall*)(GameStructs::Game*, int))0x445AE0;
		
		void(__thiscall* SetMapType)(GameStructs::Game* This, int MapType) = (void(__thiscall*)(GameStructs::Game*, int))0x445650;
		
		void(__thiscall* SetMapSize)(GameStructs::Game* This, int MapSize) = (void(__thiscall*)(GameStructs::Game*, int))0x4454F0;
		
		bool(__thiscall* ScenarioGame)(GameStructs::Game* This) = (bool(__thiscall*)(GameStructs::Game*))0x4484A0;
		
		int(__thiscall* StartGame)(GameStructs::Game* This, bool SkipStartupScreens) = (int(__thiscall*)(GameStructs::Game*, bool))0x4420D0;
		
		void(__thiscall* SetComputerAIFile)(GameStructs::Game* This, int PlayerNum, const char* AIFile) = (void(__thiscall*)(GameStructs::Game*, int, const char*))0x445970;
	}
}

namespace RpcFuncs
{
	void SetPlayerHuman(int PlayerIndex)
	{
		GameFuncs::CommHandler::SetPlayerHumanity(GameStructs::CommPointer, PlayerIndex, GameStructs::PLAYERHUMANITY::ME_HUMAN);
	}

	void SetPlayerComputer(int PlayerIndex, const std::string& AIFile)
	{
		GameFuncs::CommHandler::SetPlayerHumanity(GameStructs::CommPointer, PlayerIndex, GameStructs::PLAYERHUMANITY::ME_COMPUTER);
		GameFuncs::TribeGame::SetComputerAIFile(GameStructs::GamePointer, PlayerIndex - 1, AIFile.c_str());
	}

	void LockTeams()
	{
		GameFuncs::BaseGame::LockGameTeams(GameStructs::GamePointer);
	}

	void SetPlayerTeam(int PlayerIndex, int Team)
	{
		GameFuncs::BaseGame::SetPlayerTeam(GameStructs::GamePointer, PlayerIndex - 1, Team);
	}

	void SetMultiplayerGame(bool bMultiplayer)
	{
		GameFuncs::BaseGame::SetMultiplayerGame(GameStructs::GamePointer, bMultiplayer);
	}

	void SetSinglePlayerGame(bool bSinglePlayer)
	{
		GameFuncs::BaseGame::SetSinglePlayerGame(GameStructs::GamePointer, bSinglePlayer);
	}

	void SetGameType(int GameType)
	{
		GameFuncs::TribeGame::SetGameType(GameStructs::GamePointer, GameType);
	}

	void SetMapType(int MapType)
	{
		GameFuncs::TribeGame::SetMapType(GameStructs::GamePointer, MapType);
	}

	void SetMapSize(int MapSize)
	{
		GameFuncs::TribeGame::SetMapSize(GameStructs::GamePointer, MapSize);
	}

	bool IsScenarioGame()
	{
		return GameFuncs::TribeGame::ScenarioGame(GameStructs::GamePointer);
	}

	void SetScenarioName(const std::string& ScenarioName)
	{
		GameFuncs::BaseGame::SetScenarioName(GameStructs::GamePointer, ScenarioName.c_str());
	}

	void LaunchGame()
	{
		if (IsScenarioGame())
		{
			GameFuncs::TribeGame::StartGame(GameStructs::GamePointer, true);
		}
		else
		{
			void* DummyMPSetupScreen = operator new(0xDD0u);
			GameFuncs::TribeMPSetupScreen::LaunchGame(DummyMPSetupScreen);
			delete DummyMPSetupScreen;
		}
	}
}














