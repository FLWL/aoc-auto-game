#pragma once
#include <list>
#include <sstream>

const float API_VERSION = 1.15f;

bool RunUnfocused = false;
bool UseInGameResolution = false;
bool RunMultipleInstances = false;

namespace GameStructs
{
	struct Game;
	struct World;
	struct Player;
	struct VictoryConditions;
	struct CommunicationsHandler;

	struct Game
	{
		char unk1[48];
		char unk2[432];
		int prog_mode;
		char unk3[576];
		World* world;
		char unk4[3189];
		char resigned[9];
	};
	struct World
	{
		char unk1[16];
		unsigned int world_time;
		char unk2[56];
		Player** players;
	};
	struct Player
	{
		char unk1[132];
		VictoryConditions* victory_conditions;
	};
	struct VictoryConditions
	{
		char unk1[28];
		int total_victory_points;
	};
	struct CommunicationsHandler;

	enum PlayerHumanity
	{
		ME_ABSENT = 0x0,
		ME_CLOSED = 0x1,
		ME_HUMAN = 0x2,
		ME_COMPUTER = 0x4
	};

	/* Game speedup offset by abductedPlatypus */
	bool *DoFixedUpdate = (bool*)0x791208;

	HWND *GameWindow = (HWND*)0x7911D8;
	
	Game* GetGamePointer() { return *(Game**)0x7912A0; };
	CommunicationsHandler* GetCommPointer() { return *(CommunicationsHandler**)0x791200; };
}

namespace GameFuncs
{
	namespace CommHandler
	{
		void(__thiscall* _SetPlayerHumanity)(GameStructs::CommunicationsHandler* This, int PlayerIndex, int Humanity) = (void(__thiscall*)(GameStructs::CommunicationsHandler*, int, int))0x5D9680;
		
		GameStructs::PlayerHumanity(__thiscall* _GetPlayerHumanity)(GameStructs::CommunicationsHandler* This, int PlayerIndex) = (GameStructs::PlayerHumanity(__thiscall*)(GameStructs::CommunicationsHandler*, int))0x5D9650;
	}

	namespace TribeMPSetupScreen
	{
		int(__thiscall* _LaunchGame)(void* This) = (int(__thiscall*)(void*))0x4FF390;
	}

	namespace BaseGame
	{
		// General
		bool(__thiscall* _CheckMultiCopies)(GameStructs::Game* This) = (bool(__thiscall*)(GameStructs::Game*))0x5E70A0;

		void(__thiscall* _SetResolutionToInGameResolution)(GameStructs::Game* This) = (void(__thiscall*)(GameStructs::Game*))0x5EB5F0;

		// Game funcs
		void(__thiscall* _SetMultiplayerGame)(GameStructs::Game* This, bool bMultiplayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1C0;
		
		void(__thiscall* _SetSinglePlayerGame)(GameStructs::Game* This, bool bSinglePlayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1A0;
		
		void(__thiscall* _SetScenarioName)(GameStructs::Game* This, const char* ScenarioName) = (void(__thiscall*)(GameStructs::Game*, const char*))0x5EB160;
		
		void(__thiscall* _SetDifficulty)(GameStructs::Game* This, int Difficulty) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB2F0;
		
		void(__thiscall* _GameTeamsLocked)(GameStructs::Game* This, char Locked) = (void(__thiscall*)(GameStructs::Game*, char))0x5EB460;
		
		void(__thiscall* _SetRecordGame)(GameStructs::Game* This, int Record) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB230;
		bool(__thiscall* _GetRecordGame)(GameStructs::Game* This) = (bool(__thiscall*)(GameStructs::Game*))0x5EAEF0;
		
		void(__thiscall* _SetNumberPlayers)(GameStructs::Game* This, int NumberPlayers) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB260;
		
		int(__thiscall* _HandleActivate)(GameStructs::Game* This, void* wnd, unsigned int msg, unsigned int wparam, int lparam) = (int(__thiscall*)(GameStructs::Game*, void*, unsigned int, unsigned int, int))0x5E8830;

		void(__thiscall* _SetMapSize)(GameStructs::Game* This, char x, char y, char z) = (void(__thiscall*)(GameStructs::Game*, char, char, char))0x5EB1E0;
		
		// Player funcs
		void(__thiscall* _SetPlayerTeam)(GameStructs::Game* This, int PlayerNum, char Team) = (void(__thiscall*)(GameStructs::Game*, int, char))0x5EB300;
	}

	namespace TribeGame
	{
		// Game thread hook
		int(__thiscall* _HandleIdle)(GameStructs::Game* This) = (int(__thiscall*)(GameStructs::Game*))0x445BC0;
		
		// Record game hook
		bool(__thiscall* _StartCommandLogs)(GameStructs::Game* This) = (bool(__thiscall*)(GameStructs::Game*))0x443960;

		// General funcs
		int(__thiscall* _StartGame)(GameStructs::Game* This, bool bSkipStartupScreens) = (int(__thiscall*)(GameStructs::Game*, bool))0x4420D0;
		
		void(__thiscall* _RestartGame)(GameStructs::Game* This) = (void(__thiscall*)(GameStructs::Game*))0x43F660;
		
		int(__thiscall* _QuitGame)(GameStructs::Game* This) = (int(__thiscall*)(GameStructs::Game*))0x43F460;
		
		// Game funcs
		void(__thiscall* _SetGameType)(GameStructs::Game* This, int GameType) = (void(__thiscall*)(GameStructs::Game*, int))0x445AE0;
		bool(__thiscall* _ScenarioGame)(GameStructs::Game* This) = (bool(__thiscall*)(GameStructs::Game*))0x4484A0;

		void(__thiscall* _SetMapType)(GameStructs::Game* This, int MapType) = (void(__thiscall*)(GameStructs::Game*, int))0x445650;
		
		void(__thiscall* _SetMapSize)(GameStructs::Game* This, int MapSize) = (void(__thiscall*)(GameStructs::Game*, int))0x4454F0;
		
		void(__thiscall* _SetResourceLevel)(GameStructs::Game* This, int ResourceLevel) = (void(__thiscall*)(GameStructs::Game*, int))0x445AB0;
		
		void(__thiscall* _SetPopLimit)(GameStructs::Game* This, int PopLimit) = (void(__thiscall*)(GameStructs::Game*, int))0x445AF0;
		
		void(__thiscall* _SetVisibilityType)(GameStructs::Game* This, int VisibilityType) = (void(__thiscall*)(GameStructs::Game*, int))0x4454D0;
		
		void(__thiscall* _SetStartingAge)(GameStructs::Game* This, int StartingAge) = (void(__thiscall*)(GameStructs::Game*, int))0x445AC0;
		
		void(__thiscall* _SetVictoryType)(GameStructs::Game* This, int VictoryTypeIn, int AmountIn) = (void(__thiscall*)(GameStructs::Game*, int, int))0x445860;
		
		void(__thiscall* _SetRandomizePositions)(GameStructs::Game* This, char RandomizePositions) = (void(__thiscall*)(GameStructs::Game*, char))0x445A90;
		int(__thiscall* _GetRandomizePositions)(GameStructs::Game* This) = (int(__thiscall*)(GameStructs::Game*))0x4459E0;
		
		void(__thiscall* _SetFullTechTree)(GameStructs::Game* This, int FullTechTree) = (void(__thiscall*)(GameStructs::Game*, int))0x445AA0;
		
		// Player funcs
		void(__thiscall* _SetComputerAIFile)(GameStructs::Game* This, int PlayerNum, const char* AIFile) = (void(__thiscall*)(GameStructs::Game*, int, const char*))0x445970;
		
		void(__thiscall* _SetComputerName)(GameStructs::Game* This, int PlayerNum, const char* Name) = (void(__thiscall*)(GameStructs::Game*, int, const char*))0x445930;
		
		void(__thiscall* _SetCivilization)(GameStructs::Game* This, int PlayerNum, int Civilization) = (void(__thiscall*)(GameStructs::Game*, int, int))0x445890;
		
		void(__thiscall* _SetPlayerColor)(GameStructs::Game* This, int PlayerNum, int Color) = (void(__thiscall*)(GameStructs::Game*, int, int))0x4458F0;
	}
}

namespace RpcFuncs
{
	void SetGameMultiPlayer(bool MultiPlayer)
	{
		GameFuncs::BaseGame::_SetMultiplayerGame(GameStructs::GetGamePointer(), MultiPlayer);
	}

	void SetGameTeamsTogether(bool TeamsTogether)
	{
		// Teams must be together in scenario maps right now, because the particular
		// scenario team randomization function requires an UI element
		if (GameFuncs::TribeGame::_ScenarioGame(GameStructs::GetGamePointer()))
		{
			TeamsTogether = true;
		}

		GameFuncs::TribeGame::_SetRandomizePositions(GameStructs::GetGamePointer(), !TeamsTogether);
	}
	
	void SetGameType(int GameType)
	{
		GameFuncs::TribeGame::_SetGameType(GameStructs::GetGamePointer(), GameType);

		// No support for randomizing positions in scenario maps right now
		if (GameFuncs::TribeGame::_ScenarioGame(GameStructs::GetGamePointer()))
		{
			SetGameTeamsTogether(true);
		}
	}

	void SetGameScenarioName(const std::string& ScenarioName)
	{
		GameFuncs::BaseGame::_SetScenarioName(GameStructs::GetGamePointer(), ScenarioName.c_str());
	}

	void SetGameMapType(int MapType)
	{
		GameFuncs::TribeGame::_SetMapType(GameStructs::GetGamePointer(), MapType);
	}

	void SetGameMapSize(int MapSize)
	{
		GameFuncs::TribeGame::_SetMapSize(GameStructs::GetGamePointer(), MapSize);
	}

	void SetGameMapSizeDirectly(int x, int y, int z)
	{
		GameFuncs::BaseGame::_SetMapSize(GameStructs::GetGamePointer(), x, y, z);
	}

	void SetGameDifficulty(int Difficulty)
	{
		GameFuncs::BaseGame::_SetDifficulty(GameStructs::GetGamePointer(), Difficulty);
	}

	void SetGameStartingResources(int StartingResources)
	{
		GameFuncs::TribeGame::_SetResourceLevel(GameStructs::GetGamePointer(), StartingResources);
	}

	void SetGamePopulationLimit(int PopulationLimit)
	{
		GameFuncs::TribeGame::_SetPopLimit(GameStructs::GetGamePointer(), PopulationLimit / 25);
	}

	void SetGameRevealMap(int RevealMap)
	{
		GameFuncs::TribeGame::_SetVisibilityType(GameStructs::GetGamePointer(), RevealMap);
	}

	void SetGameStartingAge(int StartingAge)
	{
		GameFuncs::TribeGame::_SetStartingAge(GameStructs::GetGamePointer(), StartingAge);
	}

	void SetGameVictoryType(int VictoryType, int VictoryValue)
	{
		GameFuncs::TribeGame::_SetVictoryType(GameStructs::GetGamePointer(), VictoryType, VictoryValue);
	}

	void SetGameTeamsLocked(bool Locked)
	{
		GameFuncs::BaseGame::_GameTeamsLocked(GameStructs::GetGamePointer(), Locked);
	}

	void SetGameAllTechs(bool AllTechs)
	{
		GameFuncs::TribeGame::_SetFullTechTree(GameStructs::GetGamePointer(), AllTechs);
	}

	void SetGameRecorded(bool Record)
	{
		GameFuncs::BaseGame::_SetRecordGame(GameStructs::GetGamePointer(), Record);
	}

	bool GetGameRecorded()
	{
		return GameFuncs::BaseGame::_GetRecordGame(GameStructs::GetGamePointer());
	}
	
	void SetPlayerHuman(int PlayerNumber)
	{
		GameFuncs::CommHandler::_SetPlayerHumanity(GameStructs::GetCommPointer(), PlayerNumber, GameStructs::PlayerHumanity::ME_HUMAN);
	}

	void SetPlayerComputer(int PlayerNumber, const std::string& AIFile)
	{
		GameFuncs::CommHandler::_SetPlayerHumanity(GameStructs::GetCommPointer(), PlayerNumber, GameStructs::PlayerHumanity::ME_COMPUTER);
		GameFuncs::TribeGame::_SetComputerAIFile(GameStructs::GetGamePointer(), PlayerNumber - 1, AIFile.c_str());
	}

	void SetPlayerComputerName(int PlayerNumber, const std::string& Name)
	{
		GameFuncs::TribeGame::_SetComputerName(GameStructs::GetGamePointer(), PlayerNumber, Name.c_str());
	}

	void SetPlayerClosed(int PlayerNumber)
	{
		GameFuncs::CommHandler::_SetPlayerHumanity(GameStructs::GetCommPointer(), PlayerNumber, GameStructs::PlayerHumanity::ME_CLOSED);
	}

	void SetPlayerCivilization(int PlayerNumber, int Civilization)
	{
		GameFuncs::TribeGame::_SetCivilization(GameStructs::GetGamePointer(), PlayerNumber - 1, Civilization);
	}

	void SetPlayerColor(int PlayerNumber, int Color)
	{
		GameFuncs::TribeGame::_SetPlayerColor(GameStructs::GetGamePointer(), PlayerNumber - 1, Color);
	}

	void SetPlayerTeam(int PlayerNumber, int Team)
	{
		GameFuncs::BaseGame::_SetPlayerTeam(GameStructs::GetGamePointer(), PlayerNumber - 1, Team + 1);
	}

	bool GetPlayerExists(int PlayerNum)
	{
		GameStructs::PlayerHumanity Humanity = GameFuncs::CommHandler::_GetPlayerHumanity(GameStructs::GetCommPointer(), PlayerNum);
		
		return Humanity > GameStructs::PlayerHumanity::ME_CLOSED;
	}

	bool GetPlayerAlive(int PlayerNum)
	{
		return GetPlayerExists(PlayerNum) && !GameStructs::GetGamePointer()->resigned[PlayerNum];
	}

	int GetWinningPlayer()
	{
		for (int i = 1; i <= 8; i++)
		{
			if (GetPlayerAlive(i))
				return i;
		}

		return 0;
	}

	std::list<int> GetWinningPlayers()
	{
		std::list<int> WinningPlayers;

		for (int i = 1; i <= 8; i++)
		{
			if (GetPlayerAlive(i))
				WinningPlayers.push_back(i);
		}

		return WinningPlayers;
	}

	bool GetGameInProgress()
	{
		int ProgMode = GameStructs::GetGamePointer()->prog_mode;

		// 4 = active game
		// 5 = game over
		// 0 = stats screen
		// 2 = main menu
		return ProgMode == 4 || ProgMode == 6 || ProgMode == 7;
	}

	bool GetGameOver()
	{
		int ProgMode = GameStructs::GetGamePointer()->prog_mode;

		return ProgMode == 5;
	}

	bool GetGameStatsScreen()
	{
		int ProgMode = GameStructs::GetGamePointer()->prog_mode;

		return ProgMode == 0;
	}

	void QuitGame()
	{
		GameFuncs::TribeGame::_QuitGame(GameStructs::GetGamePointer());
	}
	
	bool GetWorldLoaded()
	{
		return GetGameInProgress() || GetGameOver() || GetGameStatsScreen();
	}

	bool StartGame()
	{
		if (GetWorldLoaded())
			QuitGame();
		
		int NumPlayers = 0;
		for (int i = 1; i <= 8; i++)
		{
			GameStructs::PlayerHumanity Humanity = GameFuncs::CommHandler::_GetPlayerHumanity(GameStructs::GetCommPointer(), i);
			
			if (Humanity == GameStructs::PlayerHumanity::ME_HUMAN || Humanity == GameStructs::PlayerHumanity::ME_COMPUTER)
				NumPlayers = i;
		}
		GameFuncs::BaseGame::_SetNumberPlayers(GameStructs::GetGamePointer(), NumPlayers);

		void* DummyMPSetupScreen = operator new(0xDD0u);
		bool LaunchResult = GameFuncs::TribeMPSetupScreen::_LaunchGame(DummyMPSetupScreen);
		delete DummyMPSetupScreen;
		return LaunchResult;
	}

	void RestartGame()
	{
		if (GetWorldLoaded())
		{
			GameFuncs::TribeGame::_RestartGame(GameStructs::GetGamePointer());
		}
	}

	void ResetGameSettings()
	{
		// Quiting the game changes some of the settings, so in order to reset them
		// we need to go to the main menu
		if (GetWorldLoaded())
			QuitGame();

		// Reset game settings
		SetGameMultiPlayer(false);
		SetGameType(0);
		SetGameScenarioName("");
		SetGameMapType(9);
		SetGameMapSize(1);
		SetGameDifficulty(3);
		SetGameStartingResources(0);
		SetGamePopulationLimit(200);
		SetGameRevealMap(0);
		SetGameStartingAge(2);
		SetGameVictoryType(0, 0);
		SetGameTeamsTogether(true);
		SetGameTeamsLocked(false);
		SetGameAllTechs(false);
		SetGameRecorded(false);

		// Reset players' properties
		for (int i = 1; i <= 8; i++)
		{
			if (i == 1)
			{
				SetPlayerHuman(i);
			}
			else
			{
				SetPlayerComputer(i, "");
				if (i >= 3)
					SetPlayerClosed(i);
			}

			SetPlayerComputerName(i, "");
			SetPlayerCivilization(i, 30);
			SetPlayerColor(i, i);
			SetPlayerTeam(i, 0);
		}
	}

	const float GetApiVersion()
	{
		return API_VERSION;
	}

	void SetRunUnfocused(bool Run)
	{
		RunUnfocused = Run;
	}

	void SetUseInGameResolution(bool InGameResolution)
	{
		UseInGameResolution = InGameResolution;
	}

	void SetWindowMinimized(bool Minimized)
	{
		ShowWindow(*GameStructs::GameWindow, Minimized ? SW_MINIMIZE : SW_SHOW);
	}

	void SetRunFullSpeed(bool FullSpeed)
	{
		*GameStructs::DoFixedUpdate = FullSpeed;
	}

	int GetGameTime()
	{
		if (!GetWorldLoaded())
			return 0;

		return GameStructs::GetGamePointer()->world->world_time / 1000;
	}

	int GetPlayerScore(int PlayerNumber)
	{
		if (!GetWorldLoaded() || !GetPlayerExists(PlayerNumber))
			return 0;

		return GameStructs::GetGamePointer()->world->players[PlayerNumber]->victory_conditions->total_victory_points;
	}
}
