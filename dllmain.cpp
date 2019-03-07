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
void CreateRpcServer();
void HookGame();
void UnhookGame();
#include "funcs.h"

int __fastcall GameThreadHook(GameStructs::Game* This, void* Unused);
int __fastcall GameFocusHook(GameStructs::Game* This, void* Unused, void* wnd, unsigned int msg, unsigned int wparam, int lparam);

// Function that gets executed in a thread once our library has been loaded into the game
void LibraryThread()
{
	CreateRpcServer();
	HookGame();
}

// Set up a server that listens for RPCs
void CreateRpcServer()
{
	srv = new rpc::server("127.0.0.1", 64720);

	srv->bind("ResetGameSettings", &RpcFuncs::ResetGameSettings);
	srv->bind("StartGame", &RpcFuncs::StartGame);
	srv->bind("GetGameInProgress", &RpcFuncs::GetGameInProgress);
	srv->bind("GetWinningPlayer", &RpcFuncs::GetWinningPlayer);
	srv->bind("GetWinningPlayers", &RpcFuncs::GetWinningPlayers);
	srv->bind("QuitGame", &RpcFuncs::QuitGame);
	srv->bind("GetApiVersion", &RpcFuncs::GetApiVersion);
	srv->bind("SetRunUnfocused", &RpcFuncs::SetRunUnfocused);
	srv->bind("SetRunFullSpeed", &RpcFuncs::SetRunFullSpeed);

	srv->bind("SetGameType", &RpcFuncs::SetGameType);
	srv->bind("SetGameScenarioName", &RpcFuncs::SetGameScenarioName);
	srv->bind("SetGameMapType", &RpcFuncs::SetGameMapType);
	srv->bind("SetGameMapSize", &RpcFuncs::SetGameMapSize);
	srv->bind("SetGameDifficulty", &RpcFuncs::SetGameDifficulty);
	srv->bind("SetGameStartingResources", &RpcFuncs::SetGameStartingResources);
	srv->bind("SetGamePopulationLimit", &RpcFuncs::SetGamePopulationLimit);
	srv->bind("SetGameRevealMap", &RpcFuncs::SetGameRevealMap);
	srv->bind("SetGameStartingAge", &RpcFuncs::SetGameStartingAge);
	srv->bind("SetGameVictoryType", &RpcFuncs::SetGameVictoryType);
	srv->bind("SetGameTeamsTogether", &RpcFuncs::SetGameTeamsTogether);
	srv->bind("SetGameLockTeams", &RpcFuncs::SetGameLockTeams);
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
}

// Hook into the game thread
void HookGame()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)GameFuncs::TribeGame::_HandleIdle, GameThreadHook);
	DetourAttach(&(PVOID&)GameFuncs::BaseGame::_HandleActivate, GameFocusHook);
	DetourTransactionCommit();
}

// Remove the game thread hook
void UnhookGame()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)GameFuncs::TribeGame::_HandleIdle, GameThreadHook);
	DetourDetach(&(PVOID&)GameFuncs::BaseGame::_HandleActivate, GameFocusHook);
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
