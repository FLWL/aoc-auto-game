// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <detours.h>
#include "rpc/server.h"

// Vars
HMODULE CachedHModule;
rpc::server* srv;

// Func defs
void LibraryThread();
void CreateRpcServer();
void HookGameThread();
void UnhookGameThread();
int __fastcall GameThreadHook(void* This, void* Unused);
#include "funcs.h"

// Function that gets executed in a thread once our library has been loaded into the game
void LibraryThread()
{
	CreateRpcServer();
	HookGameThread();
}

// Set up a server that listens for RPCs
void CreateRpcServer()
{
	srv = new rpc::server("127.0.0.1", 64720);

	srv->bind("ResetGameSettings", &RpcFuncs::ResetGameSettings);
	srv->bind("StartGame", &RpcFuncs::StartGame);
	srv->bind("GetGameInProgress", &RpcFuncs::GetGameInProgress);
	srv->bind("QuitGame", &RpcFuncs::QuitGame);
	srv->bind("GetApiVersion", &RpcFuncs::GetApiVersion);

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
	//srv->bind("SetGameTeamsTogether", &RpcFuncs::SetGameTeamsTogether);
	srv->bind("SetGameLockTeams", &RpcFuncs::SetGameLockTeams);
	srv->bind("SetGameAllTechs", &RpcFuncs::SetGameAllTechs);
	srv->bind("SetGameRecord", &RpcFuncs::SetGameRecord);

	srv->bind("SetPlayerHuman", &RpcFuncs::SetPlayerHuman);
	srv->bind("SetPlayerComputer", &RpcFuncs::SetPlayerComputer);
	srv->bind("SetPlayerClosed", &RpcFuncs::SetPlayerClosed);
	srv->bind("SetPlayerCivilization", &RpcFuncs::SetPlayerCivilization);
	srv->bind("SetPlayerColor", &RpcFuncs::SetPlayerColor);
	srv->bind("SetPlayerTeam", &RpcFuncs::SetPlayerTeam);
	//srv->bind("GetPlayerGameOutcome", &RpcFuncs::GetPlayerGameOutcome);
}

// Hook into the game thread
void HookGameThread()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)GameFuncs::TribeGame::HandleIdle, GameThreadHook);
	DetourTransactionCommit();
}

// Remove the game thread hook
void UnhookGameThread()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)GameFuncs::TribeGame::HandleIdle, GameThreadHook);
	DetourTransactionCommit();
}

int __fastcall GameThreadHook(void* This, void* Unused)
{
	int ReturnValue = GameFuncs::TribeGame::HandleIdle(This);

	// Poll the RPC server for work, from the game thread
	if (srv)
	{
		srv->poll();
	}
	
	return ReturnValue;
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
			UnhookGameThread();
			if (srv) srv->stop();
			break;
		}
	}

	return TRUE;
}