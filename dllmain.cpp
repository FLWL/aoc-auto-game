// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <iostream>
#include <detours.h>
#include "rpc/server.h"

// Vars
FILE *DebugConsoleRedirect;
HMODULE CachedHModule;
rpc::server* srv;

// Func defs
void EnableDebugConsole();
void DisableDebugConsole();
void LibraryThread();
int GetRpcServerPort();
void CreateRpcServer();
void CheckCommandLineParameters();
void HookGame();
void UnhookGame();
#include "funcs.h"

int __fastcall GameThreadHook(GameStructs::Game* This, void* Unused);
int __fastcall GameFocusHook(GameStructs::Game* This, void* Unused, void* wnd, unsigned int msg, unsigned int wparam, int lparam);
bool __fastcall CheckMultiCopiesHook(GameStructs::Game* This, void* Unused);
bool __fastcall StartRecordingHook(GameStructs::Game* This, void* Unused);
void __fastcall InGameResolutionHook(GameStructs::Game* This, void* Unused);

// Function that gets executed in a thread once our library has been loaded into the game
void LibraryThread()
{
	//EnableDebugConsole();
	CheckCommandLineParameters();
	CreateRpcServer();
	HookGame();
}

// Set up a server that listens for RPCs
void CreateRpcServer()
{
	int RpcServerPort = GetRpcServerPort();
	std::cout << "Initializing RPC server at port: " << RpcServerPort << std::endl;
	srv = new rpc::server("127.0.0.1", RpcServerPort);

	srv->bind("ResetGameSettings", &RpcFuncs::ResetGameSettings);
	srv->bind("StartGame", &RpcFuncs::StartGame);
	srv->bind("GetGameInProgress", &RpcFuncs::GetGameInProgress);
	srv->bind("GetGameTime", &RpcFuncs::GetGameTime);
	srv->bind("GetWinningPlayer", &RpcFuncs::GetWinningPlayer);
	srv->bind("GetWinningPlayers", &RpcFuncs::GetWinningPlayers);
	srv->bind("RestartGame", &RpcFuncs::RestartGame);
	srv->bind("QuitGame", &RpcFuncs::QuitGame);
	srv->bind("GetApiVersion", &RpcFuncs::GetApiVersion);
	srv->bind("SetRunUnfocused", &RpcFuncs::SetRunUnfocused);
	srv->bind("SetWindowMinimized", &RpcFuncs::SetWindowMinimized);
	srv->bind("SetUseInGameResolution", &RpcFuncs::SetUseInGameResolution);
	srv->bind("SetRunFullSpeed", &RpcFuncs::SetRunFullSpeed);

	srv->bind("SetGameType", &RpcFuncs::SetGameType);
	srv->bind("SetGameScenarioName", &RpcFuncs::SetGameScenarioName);
	srv->bind("SetGameMapType", &RpcFuncs::SetGameMapType);
	srv->bind("SetGameMapSize", &RpcFuncs::SetGameMapSize);
	srv->bind("SetGameMapSizeDirectly", &RpcFuncs::SetGameMapSizeDirectly);
	srv->bind("SetGameDifficulty", &RpcFuncs::SetGameDifficulty);
	srv->bind("SetGameStartingResources", &RpcFuncs::SetGameStartingResources);
	srv->bind("SetGamePopulationLimit", &RpcFuncs::SetGamePopulationLimit);
	srv->bind("SetGameRevealMap", &RpcFuncs::SetGameRevealMap);
	srv->bind("SetGameStartingAge", &RpcFuncs::SetGameStartingAge);
	srv->bind("SetGameVictoryType", &RpcFuncs::SetGameVictoryType);
	srv->bind("SetGameTeamsTogether", &RpcFuncs::SetGameTeamsTogether);
	srv->bind("SetGameTeamsLocked", &RpcFuncs::SetGameTeamsLocked);
	srv->bind("SetGameAllTechs", &RpcFuncs::SetGameAllTechs);
	srv->bind("SetGameRecorded", &RpcFuncs::SetGameRecorded);

	srv->bind("SetPlayerHuman", &RpcFuncs::SetPlayerHuman);
	srv->bind("SetPlayerComputer", &RpcFuncs::SetPlayerComputer);
	srv->bind("SetPlayerClosed", &RpcFuncs::SetPlayerClosed);
	srv->bind("SetPlayerCivilization", &RpcFuncs::SetPlayerCivilization);
	srv->bind("SetPlayerColor", &RpcFuncs::SetPlayerColor);
	srv->bind("SetPlayerTeam", &RpcFuncs::SetPlayerTeam);
	srv->bind("GetPlayerExists", &RpcFuncs::GetPlayerExists);
	srv->bind("GetPlayerAlive", &RpcFuncs::GetPlayerAlive);
	srv->bind("GetPlayerScore", &RpcFuncs::GetPlayerScore);
}

void CheckCommandLineParameters()
{
	std::wstring CmdLine(GetCommandLine());

	if (CmdLine.find(L"-multipleinstances") != -1)
		RunMultipleInstances = true;
}

// Hook into the game thread
void HookGame()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)GameFuncs::TribeGame::_HandleIdle, GameThreadHook);
	DetourAttach(&(PVOID&)GameFuncs::BaseGame::_HandleActivate, GameFocusHook);
	DetourAttach(&(PVOID&)GameFuncs::BaseGame::_CheckMultiCopies, CheckMultiCopiesHook);
	DetourAttach(&(PVOID&)GameFuncs::TribeGame::_StartCommandLogs, StartRecordingHook);
	DetourAttach(&(PVOID&)GameFuncs::BaseGame::_SetResolutionToInGameResolution, InGameResolutionHook);
	DetourTransactionCommit();
}

// Remove the game thread hook
void UnhookGame()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)GameFuncs::TribeGame::_HandleIdle, GameThreadHook);
	DetourDetach(&(PVOID&)GameFuncs::BaseGame::_HandleActivate, GameFocusHook);
	DetourDetach(&(PVOID&)GameFuncs::BaseGame::_CheckMultiCopies, CheckMultiCopiesHook);
	DetourDetach(&(PVOID&)GameFuncs::TribeGame::_StartCommandLogs, StartRecordingHook);
	DetourDetach(&(PVOID&)GameFuncs::BaseGame::_SetResolutionToInGameResolution, InGameResolutionHook);
	DetourTransactionCommit();
}

void EnableDebugConsole()
{
	AllocConsole();
	freopen_s(&DebugConsoleRedirect, "CONOUT$", "w", stdout);
	std::cout << "Debug console initialized." << std::endl;
}

void DisableDebugConsole()
{
	fclose(DebugConsoleRedirect);
	FreeConsole();
}

// Get RPC server port
// Just something that gets the job done right now
// This doesn't account for all cases, such as quotation marks around the parameter value
int GetRpcServerPort()
{
	std::wstring CmdLine(GetCommandLine());
	std::wstring Keyword = L"-autogameport ";

	unsigned int Pos = CmdLine.find(Keyword);
	if (Pos != -1)
	{
		Pos += Keyword.length();
		std::wstring PortString = L"    ";
		while (Pos < CmdLine.length() &&
			((CmdLine[Pos] >= '0' && CmdLine[Pos] <= '9') || CmdLine[Pos] == ' '))
		{
			PortString += CmdLine[Pos++];
		}

		return _wtoi(PortString.c_str());
	}

	return 64720;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		{
			DisableThreadLibraryCalls(hModule);
			CachedHModule = hModule;

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)LibraryThread, NULL, NULL, NULL);
			break;
		}
		case DLL_PROCESS_DETACH:
		{
			UnhookGame();
			if (srv) srv->stop();
			break;
		}
	}

	return TRUE;
}

int __fastcall GameThreadHook(GameStructs::Game* This, void* Unused)
{
	int ReturnValue = GameFuncs::TribeGame::_HandleIdle(This);

	// Poll the RPC server for work, from the game thread
	if (srv)
	{
		srv->poll();
	}

	return ReturnValue;
}

int __fastcall GameFocusHook(GameStructs::Game* This, void* Unused, void* wnd, unsigned int msg, unsigned int wparam, int lparam)
{
	if (RunUnfocused && wparam == 0)
		wparam = 1;

	return GameFuncs::BaseGame::_HandleActivate(This, wnd, msg, wparam, lparam);
}

bool __fastcall CheckMultiCopiesHook(GameStructs::Game* This, void* Unused)
{
	if (!RunMultipleInstances)
		return GameFuncs::BaseGame::_CheckMultiCopies(This);

	return true; // make the multi copy check pass
}

bool __fastcall StartRecordingHook(GameStructs::Game* This, void* Unused)
{
	if (!RunMultipleInstances || RpcFuncs::GetGameRecorded()) // only start recording if the game should actually be recorded
		return GameFuncs::TribeGame::_StartCommandLogs(This);

	return true;
}

void __fastcall InGameResolutionHook(GameStructs::Game* This, void* Unused)
{
	if (UseInGameResolution)
		GameFuncs::BaseGame::_SetResolutionToInGameResolution(This);
}
