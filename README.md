# aoc-auto-game
An API for programmatically starting Age of Empires II: The Conquerors matches with specified settings.

## Usage
1. Compile the DLL with dependencies listed at the bottom of this page
2. Load the DLL into age2_x1.exe process
3. Connect with a msgpack-RPC client on address 127.0.0.1:64720
4. Issue commands to the game to automate starting games

## Example client (Python)
Using the ```msgpack-rpc-python``` package.
```
import msgpackrpc
import time

autogame = msgpackrpc.Client(msgpackrpc.Address("127.0.0.1", 64720))
autogame.call('ResetGameSettings')                    # usually reset the settings to make sure everything is valid
autogame.call('SetGameRevealMap', 2)                  # set map to "All Visible"
autogame.call('SetGameMapType', 24)                   # choose a random map

autogame.call('SetPlayerComputer', 1, "Barbarian")    # put the Barbarian AI into player slot 1
autogame.call('SetPlayerComputer', 2, "AT_Empire")    # and AT_Empire into slot 2

autogame.call('StartGame')                            # start the match

while autogame.call('GetGameInProgress'):             # wait until the game has finished
  print("Game is running...")
  time.sleep(0.5)

print("Game finished")
autogame.call('QuitGame')                             # go back to the main menu
```

## API
|  **Main Functions** | **Param 1** | **Param 2** | **Description** |
| :--- | :--- | :--- | :--- |
|  **ResetGameSettings** |  |  | Reset all game settings and players to their valid default state. |
|  **StartGame** |  |  | Start the game with previously specified settings. |
|  **GetGameInProgress** |  |  | Whether a game is ongoing. Returns a boolean value. |
|  **QuitGame** |  |  | Exits the match. |
|  **GetApiVersion** |  |  | Returns a string with the aoc-auto-game API version, for example "1.1" |
|   |  |  |  |
|  **Game Settings** | **Param 1** | **Param2** | **Description** |
|  **SetGameType** | **int Type** |  |  |
|   | 0 - Random Map<br/>1 - Regicide<br/>2 - Death Match<br/>3 - Scenario<br/>5 - King of the Hill<br/>6 - Wonder Race<br/>7 - Defend the Wonder<br/>8 - Turbo Random Map |  |  |
|  **SetGameScenarioName** | **string ScenarioName** |  | Scenario map name. Only used if GameType is set to 3. |
|   | "test_scenario" |  |  |
|  **SetGameMapType** | **int MapType** |  |  |
|   | 9 - Arabia<br/>13 - Coastal<br/>14 - Continental<br/>24 - Random |  |  |
|  **SetGameMapSize** | **int MapSize** |  |  |
|   | 0 - Tiny<br/>1 - Small<br/>2 - Medium<br/>3 - Normal<br/>4 - Large<br/>5 - Giant |  |  |
|  **SetGameDifficulty** | **int Difficulty** |  |  |
|   | 0 - Hardest<br/>1 - Hard<br/>2 - Moderate<br/>3 - Standard<br/>4 - Easiest |  |  |
|  **SetGameStartingResources** | **int StartingResources** |  |  |
|   | 0 - Standard<br/>1 - Low<br/>2 - Medium<br/>3 - High |  |  |
|  **SetGamePopulationLimit** | **int PopulationLimit** |  |  |
|   | In increments of 25 |  |  |
|  **SetGameRevealMap** | **int RevealMap** |  |  |
|   | 0 - Normal<br/>1 - Explored<br/>2 - All Visible |  |  |
|  **SetGameStartingAge** | **int StartingAge** |  | UserPatch 1.5 implements some new options in this category, not certain of those IDs. |
|   | 0 - Standard<br/>1 - Dark<br/>2 - Feudal<br/>3 - Castle<br/>4 - Imperial<br/>5 - Post-Imperial |  |  |
|  **SetGameVictoryType** | **int VictoryType** | **int VictoryValue** |  |
|   | 0 - Standard<br/>1 - Conquest<br/>4 - Relics<br/>7 - Time Limit<br/>8 - Score | 0<br/>1<br/>0<br/>300 - 1500 years<br/>4000 - 14000 score |  |
|  ~**SetGameTeamsTogether**~ | ~**TODO (always enabled right now)**~ |  |  |
|  **SetGameLockTeams** | **bool LockTeams** |  |  |
|  **SetGameAllTechs** | **bool AllTechs** |  |  |
|  **SetGameRecord** | **bool Record** |  |  |
|   |  |  |  |
|  **Player Settings** | **Param 1** | **Param 2** | **Description** |
|  **SetPlayerHuman** | **int PlayerNumber** |  |  |
|   | 1-8 |  |  |
|  **SetPlayerComputer** | **int PlayerNumber** | **string AIFile** |  |
|   | 1-8 | "Barbarian" |  |
|  **SetPlayerClosed** | **int PlayerNumber** |  |  |
|   | 1-8 |  |  |
|  **SetPlayerCivilization** | **int PlayerNumber** | **int Civilization** |  |
|   | 1-8 | 1 - Britons<br/>30 - Random |  |
|  **SetPlayerColor** | **int PlayerNumber** | **int Color** |  |
|   | 1-8 | 1-8 |  |
|  **SetPlayerTeam** | **int PlayerNumber** | **int Team** |  |
|   | 1-8 | 0 - No team<br/>1-4 Teams |  |

## Known issues
* Calling "QuitGame" from post match stats screen results in looping background music.

## In the future
* Implement the rest of the less important game settings
* Maybe make the winner and players' scores available after the end of match. Any further analysis would not be in the scope of this project.

## Dependencies
* Microsoft Detours - https://github.com/Microsoft/Detours
* rpclib (with a minor change) - https://github.com/FLWL/rpclib
