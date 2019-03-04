#pragma once
#include <sstream>

namespace GameStructs
{
	struct Game
	{
		char unk[480];
		int prog_mode;
	};
	struct CommunicationsHandler;

	enum PlayerHumanity
	{
		ME_ABSENT = 0x0,
		ME_CLOSED = 0x1,
		ME_HUMAN = 0x2,
		ME_ELIMINATED = 0x3,
		ME_COMPUTER = 0x4,
		ME_CYBORG = 0x5,
		ME_VIEWONLY = 0x6,
	};

	/*enum GameOutcome
	{
		LOST = 0x0,
		WON = 0x1,
		RESIGNED = 0x2,
		DROPPED_PLAYER = 0x3,
		SAVE_AND_EXIT = 0x4,
		UNDETERMINED = 0x5,
	};*/

	Game* GamePointer = *(Game**)0x7912A0;
	CommunicationsHandler* CommPointer = *(CommunicationsHandler**)0x791200;
}

namespace GameFuncs
{
	namespace CommHandler
	{
		void(__thiscall* SetPlayerHumanity)(GameStructs::CommunicationsHandler* This, int PlayerIndex, int Humanity) = (void(__thiscall*)(GameStructs::CommunicationsHandler*, int, int))0x5D9680;
		
		GameStructs::PlayerHumanity(__thiscall* GetPlayerHumanity)(GameStructs::CommunicationsHandler* This, int PlayerIndex) = (GameStructs::PlayerHumanity(__thiscall*)(GameStructs::CommunicationsHandler*, int))0x5D9650;
		//GameStructs::GameOutcome(__thiscall* GetGameOutcomeForPlayer)(GameStructs::CommunicationsHandler* This, int PlayerIndex) = (GameStructs::GameOutcome(__thiscall*)(GameStructs::CommunicationsHandler*, int))0x5DF840;
	}

	namespace TribeMPSetupScreen
	{
		int(__thiscall* LaunchGame)(void* This) = (int(__thiscall*)(void*))0x4FF390;
	}

	namespace BaseGame
	{
		// Game funcs
		void(__thiscall* SetMultiplayerGame)(GameStructs::Game* This, bool bMultiplayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1C0;
		
		void(__thiscall* SetSinglePlayerGame)(GameStructs::Game* This, bool bSinglePlayer) = (void(__thiscall*)(GameStructs::Game*, bool))0x5EB1A0;
		
		void(__thiscall* SetScenarioName)(GameStructs::Game* This, const char* ScenarioName) = (void(__thiscall*)(GameStructs::Game*, const char*))0x5EB160;
		
		void(__thiscall* SetDifficulty)(GameStructs::Game* This, int Difficulty) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB2F0;
		
		void(__thiscall* GameTeamsLocked)(GameStructs::Game* This, char Locked) = (void(__thiscall*)(GameStructs::Game*, char))0x5EB460;
		
		void(__thiscall* SetRecordGame)(GameStructs::Game* This, int Record) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB230;
		
		void(__thiscall* SetNumberPlayers)(GameStructs::Game* This, int NumberPlayers) = (void(__thiscall*)(GameStructs::Game*, int))0x5EB260;

		// Player funcs
		void(__thiscall* SetPlayerTeam)(GameStructs::Game* This, int PlayerNum, char Team) = (void(__thiscall*)(GameStructs::Game*, int, char))0x5EB300;
	}

	namespace TribeGame
	{
		// Game thread hook
		int(__thiscall* HandleIdle)(void* This) = (int(__thiscall*)(void*))0x445BC0;
		
		// General funcs
		int(__thiscall* StartGame)(GameStructs::Game* This, bool bSkipStartupScreens) = (int(__thiscall*)(GameStructs::Game*, bool))0x4420D0;
		
		int(__thiscall* QuitGame)(GameStructs::Game* This) = (int(__thiscall*)(GameStructs::Game*))0x43F460;
		
		// Game funcs
		void(__thiscall* SetGameType)(GameStructs::Game* This, int GameType) = (void(__thiscall*)(GameStructs::Game*, int))0x445AE0;
		
		void(__thiscall* SetMapType)(GameStructs::Game* This, int MapType) = (void(__thiscall*)(GameStructs::Game*, int))0x445650;
		
		void(__thiscall* SetMapSize)(GameStructs::Game* This, int MapSize) = (void(__thiscall*)(GameStructs::Game*, int))0x4454F0;
		
		void(__thiscall* SetResourceLevel)(GameStructs::Game* This, int ResourceLevel) = (void(__thiscall*)(GameStructs::Game*, int))0x445AB0;
		
		void(__thiscall* SetPopLimit)(GameStructs::Game* This, int PopLimit) = (void(__thiscall*)(GameStructs::Game*, int))0x445AF0;
		
		void(__thiscall* SetVisibilityType)(GameStructs::Game* This, int VisibilityType) = (void(__thiscall*)(GameStructs::Game*, int))0x4454D0;
		
		void(__thiscall* SetStartingAge)(GameStructs::Game* This, int StartingAge) = (void(__thiscall*)(GameStructs::Game*, int))0x445AC0;
		
		void(__thiscall* SetVictoryType)(GameStructs::Game* This, int VictoryTypeIn, int AmountIn) = (void(__thiscall*)(GameStructs::Game*, int, int))0x445860;
		
		void(__thiscall* SetRandomizePositions)(GameStructs::Game* This, char RandomizePositions) = (void(__thiscall*)(GameStructs::Game*, char))0x445A90;
		
		void(__thiscall* SetFullTechTree)(GameStructs::Game* This, int FullTechTree) = (void(__thiscall*)(GameStructs::Game*, int))0x445AA0;
		
		// Player funcs
		void(__thiscall* SetComputerAIFile)(GameStructs::Game* This, int PlayerNum, const char* AIFile) = (void(__thiscall*)(GameStructs::Game*, int, const char*))0x445970;
		
		void(__thiscall* SetCivilization)(GameStructs::Game* This, int PlayerNum, int Civilization) = (void(__thiscall*)(GameStructs::Game*, int, int))0x445890;
		
		void(__thiscall* SetPlayerColor)(GameStructs::Game* This, int PlayerNum, int Color) = (void(__thiscall*)(GameStructs::Game*, int, int))0x4458F0;
	}
}

namespace RpcFuncs
{
	// Game funcs
	void SetGameMultiPlayer(bool MultiPlayer)
	{
		GameFuncs::BaseGame::SetMultiplayerGame(GameStructs::GamePointer, MultiPlayer);
	}

	void SetGameType(int GameType)
	{
		GameFuncs::TribeGame::SetGameType(GameStructs::GamePointer, GameType);
	}

	void SetGameScenarioName(const std::string& ScenarioName)
	{
		GameFuncs::BaseGame::SetScenarioName(GameStructs::GamePointer, ScenarioName.c_str());
	}

	void SetGameMapType(int MapType)
	{
		GameFuncs::TribeGame::SetMapType(GameStructs::GamePointer, MapType);
	}

	void SetGameMapSize(int MapSize)
	{
		GameFuncs::TribeGame::SetMapSize(GameStructs::GamePointer, MapSize);
	}

	void SetGameDifficulty(int Difficulty)
	{
		GameFuncs::BaseGame::SetDifficulty(GameStructs::GamePointer, Difficulty);
	}

	void SetGameStartingResources(int StartingResources)
	{
		GameFuncs::TribeGame::SetResourceLevel(GameStructs::GamePointer, StartingResources);
	}

	void SetGamePopulationLimit(int PopulationLimit)
	{
		GameFuncs::TribeGame::SetPopLimit(GameStructs::GamePointer, PopulationLimit / 25);
	}

	void SetGameRevealMap(int RevealMap)
	{
		GameFuncs::TribeGame::SetVisibilityType(GameStructs::GamePointer, RevealMap);
	}

	void SetGameStartingAge(int StartingAge)
	{
		GameFuncs::TribeGame::SetStartingAge(GameStructs::GamePointer, StartingAge);
	}

	void SetGameVictoryType(int VictoryType, int VictoryValue)
	{
		GameFuncs::TribeGame::SetVictoryType(GameStructs::GamePointer, VictoryType, VictoryValue);
	}

	void SetGameTeamsTogether(bool TeamsTogether)
	{
		GameFuncs::TribeGame::SetRandomizePositions(GameStructs::GamePointer, !TeamsTogether);
	}

	void SetGameLockTeams(bool LockTeams)
	{
		GameFuncs::BaseGame::GameTeamsLocked(GameStructs::GamePointer, LockTeams);
	}

	void SetGameAllTechs(bool AllTechs)
	{
		GameFuncs::TribeGame::SetFullTechTree(GameStructs::GamePointer, AllTechs);
	}

	void SetGameRecord(bool Record)
	{
		GameFuncs::BaseGame::SetRecordGame(GameStructs::GamePointer, Record);
	}

	// Player funcs
	void SetPlayerHuman(int PlayerNumber)
	{
		GameFuncs::CommHandler::SetPlayerHumanity(GameStructs::CommPointer, PlayerNumber, GameStructs::PlayerHumanity::ME_HUMAN);
	}

	void SetPlayerComputer(int PlayerNumber, const std::string& AIFile)
	{
		GameFuncs::CommHandler::SetPlayerHumanity(GameStructs::CommPointer, PlayerNumber, GameStructs::PlayerHumanity::ME_COMPUTER);
		GameFuncs::TribeGame::SetComputerAIFile(GameStructs::GamePointer, PlayerNumber - 1, AIFile.c_str());
	}

	void SetPlayerClosed(int PlayerNumber)
	{
		GameFuncs::CommHandler::SetPlayerHumanity(GameStructs::CommPointer, PlayerNumber, GameStructs::PlayerHumanity::ME_CLOSED);
	}

	void SetPlayerCivilization(int PlayerNumber, int Civilization)
	{
		GameFuncs::TribeGame::SetCivilization(GameStructs::GamePointer, PlayerNumber - 1, Civilization);
	}

	void SetPlayerColor(int PlayerNumber, int Color)
	{
		GameFuncs::TribeGame::SetPlayerColor(GameStructs::GamePointer, PlayerNumber - 1, Color);
	}

	void SetPlayerTeam(int PlayerNumber, int Team)
	{
		GameFuncs::BaseGame::SetPlayerTeam(GameStructs::GamePointer, PlayerNumber - 1, Team + 1);
	}

	/*int GetPlayerGameOutcome(int PlayerNumber)
	{
		return GameFuncs::CommHandler::GetGameOutcomeForPlayer(GameStructs::CommPointer, PlayerNumber);
	}*/

	// General funcs
	void ResetGameSettings()
	{
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
		SetGameStartingAge(0);
		SetGameVictoryType(0, 0);
		SetGameTeamsTogether(true);
		SetGameLockTeams(false);
		SetGameAllTechs(false);
		SetGameRecord(false);

		// Reset players' properties
		SetPlayerHuman(1);
		SetPlayerComputer(2, "");
		for (int i = 1; i <= 8; i++)
		{
			if (i >= 3) SetPlayerClosed(i);
			SetPlayerCivilization(i, 30);
			SetPlayerColor(i, i);
			SetPlayerTeam(i, 0);
		}
	}

	bool GetGameInProgress()
	{
		int ProgMode = GameStructs::GamePointer->prog_mode;

		// 4 = active game
		// 5 = game over
		// 0 = stats screen
		// 2 = main menu
		return ProgMode == 4 || ProgMode == 6 || ProgMode == 7;
	}

	bool GetGameOver()
	{
		int ProgMode = GameStructs::GamePointer->prog_mode;

		return ProgMode == 5;
	}

	bool GetGameStatsScreen()
	{
		int ProgMode = GameStructs::GamePointer->prog_mode;

		return ProgMode == 0;
	}

	void QuitGame()
	{
		GameFuncs::TribeGame::QuitGame(GameStructs::GamePointer);
	}

	bool StartGame()
	{
		if (GetGameInProgress() || GetGameOver() || GetGameStatsScreen())
			QuitGame();

		int NumPlayers = 0;
		for (int i = 1; i <= 8; i++)
		{
			GameStructs::PlayerHumanity Humanity = GameFuncs::CommHandler::GetPlayerHumanity(GameStructs::CommPointer, i);
			
			if (Humanity == GameStructs::PlayerHumanity::ME_HUMAN || Humanity == GameStructs::PlayerHumanity::ME_COMPUTER)
				NumPlayers = i;
		}
		GameFuncs::BaseGame::SetNumberPlayers(GameStructs::GamePointer, NumPlayers);

		void* DummyMPSetupScreen = operator new(0xDD0u);
		bool LaunchResult = GameFuncs::TribeMPSetupScreen::LaunchGame(DummyMPSetupScreen);
		delete DummyMPSetupScreen;
		return LaunchResult;
	}
}
