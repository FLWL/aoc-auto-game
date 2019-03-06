# aoc-auto-game
An API for programmatically starting Age of Empires II: The Conquerors matches with specified settings.

## Features
* msgpack-rpc based API that is callable from many languages, including Python, Java and C++
* Change any game setting available in the in-game lobby programmatically
* Allow a single-player match to run even after minimizing the game
* Launch games with desired settings
* Determine when a match ends
* Determine the winning players
* Exit from a match

## Usage
1. Compile the DLL with dependencies listed at the bottom of this page
2. Load the DLL into age2_x1 process
3. Connect with a msgpack-RPC client on address 127.0.0.1:64720
4. Issue commands to the game to automate starting games

## Python example
Using the ```msgpack-rpc-python``` package. Let's start a game on an all-visible random map. We'll have custom AIs "Barbarian" and "AT_Empire" facing off against each other in player slots 1 and 2 respectively.
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
  time.sleep(1.0)

winner = autogame.call('GetWinningPlayer')
print("Game finished, winner: " + str(winner))
autogame.call('QuitGame')                             # go back to the main menu
```
## Java example
Using the ```msgpack-rpc``` from Maven. We're starting an all-visible default map with 2 default AIs playing against each other.
```
import org.msgpack.rpc.Client;

public class Main
{
    public static void main(String[] args) throws Exception
    {
        Client autogame = new Client("127.0.0.1", 64720);
        autogame.callApply("ResetGameSettings", new Object[]{});
        autogame.callApply("SetGameRevealMap", new Object[]{2});

        autogame.callApply("SetPlayerComputer", new Object[]{1, ""});
        autogame.callApply("SetPlayerComputer", new Object[]{2, ""});

        autogame.callApply("StartGame", new Object[]{});

        while (autogame.callApply("GetGameInProgress", new Object[]{}).asBooleanValue().getBoolean())
        {
            System.out.println("Game is running...");
            Thread.sleep(1000);
        }

        int winner = autogame.callApply("GetWinningPlayer", new Object[]{}).asIntegerValue().getInt();
        System.out.println("Game finished, winner: " + winner);
    }
}
```

## Notes

* If including a human player, it is recommended to put them into player slot 1.
* When including many players, it is preferred to use the player slots consecutively and avoid holes.
* The default settings is a standard 1v1 matchup between a human player in slot 1 and a default AI player in slot 2, on Arabia.

## API
|  **Function** | **Param 1** | **Param 2** | **Description** |
| :--- | :--- | :--- | :--- |
|  **ResetGameSettings** |  |  | Reset all game settings and players to their valid default state. |
|  **StartGame** |  |  | Start the game with previously specified settings. |
|  **GetGameInProgress** |  |  | Whether a game is ongoing. Returns a boolean value. |
|  **GetWinningPlayer** |  |  | Returns the first winning player number as an integer. |
|  **GetWinningPlayers** |  |  | Returns all winning players as a list. |
|  **QuitGame** |  |  | Exits the match. |
|  **GetApiVersion** |  |  | Returns the aoc-auto-game API version as a float. |
|  **SetGameType** | **int Type** |  | Sets the type of the game. |
|   | 0 - Random Map<br/>1 - Regicide<br/>2 - Death Match<br/>3 - Scenario<br/>5 - King of the Hill<br/>6 - Wonder Race<br/>7 - Defend the Wonder<br/>8 - Turbo Random Map |  |  |
|  **SetGameScenarioName** | **string ScenarioName** |  | Scenario map name. Only used if GameType is set to 3. |
|   | "test_scenario" |  |  |
|  **SetGameMapType** | **int MapType** |  | Sets the location of the random map. |
|   | 9 - Arabia<br/>13 - Coastal<br/>14 - Continental<br/>24 - Random |  |  |
|  **SetGameMapSize** | **int MapSize** |  | Random map size. |
|   | 0 - Tiny<br/>1 - Small<br/>2 - Medium<br/>3 - Normal<br/>4 - Large<br/>5 - Giant |  |  |
|  **SetGameDifficulty** | **int Difficulty** |  | Difficulty of the game. |
|   | 0 - Hardest<br/>1 - Hard<br/>2 - Moderate<br/>3 - Standard<br/>4 - Easiest |  |  |
|  **SetGameStartingResources** | **int StartingResources** |  | The amount of resources players start the game with. |
|   | 0 - Standard<br/>1 - Low<br/>2 - Medium<br/>3 - High |  |  |
|  **SetGamePopulationLimit** | **int PopulationLimit** |  | The maximum number of units players can concurrently have. |
|   | In increments of 25 |  |  |
|  **SetGameRevealMap** | **int RevealMap** |  | How much of the map the players can see. |
|   | 0 - Normal<br/>1 - Explored<br/>2 - All Visible |  |  |
|  **SetGameStartingAge** | **int StartingAge** |  | UserPatch 1.5 implements some new options in this category, not certain of those IDs. |
|   | 0 - Standard<br/>2 - Dark Age<br/>3 - Feudal Age<br/>4 - Castle Age<br/>5 - Imperial Age<br/>6 - Post-Imperial Age |  |  |
|  **SetGameVictoryType** | **int VictoryType** | **int VictoryValue** | Additional game victory condition. |
|   | 0 - Standard<br/>1 - Conquest<br/>4 - Relics<br/>7 - Time Limit<br/>8 - Score | 0<br/>1<br/>0<br/>300 - 1500 years<br/>4000 - 14000 score |  |
|  **SetGameTeamsTogether** | **bool TeamsTogether** |  | Cannot be disabled in scenario maps. |
|  **SetGameLockTeams** | **bool LockTeams** |  | Turn off diplomacy. |
|  **SetGameAllTechs** | **bool AllTechs** |  | Lose unique properties of each civilization and allow all techs to be researched. |
|  **SetGameRecorded** | **bool Recorded** |  | Whether to record the game into a replay file. |
|  **SetGameRunUnfocused** | **bool RunUnfocused** |  | Sets whether the game is allowed to run while not in focus. Enabled by default. |
|  **SetPlayerHuman** | **int PlayerNumber** |  | Specifies that a human will be playing in this slot. Recommended to use slot 1. |
|   | 1-8 |  |  |
|  **SetPlayerComputer** | **int PlayerNumber** | **string AIFile** | Adds a specific computer player to the specified slot. |
|   | 1-8 | "Barbarian" |  |
|  **SetPlayerClosed** | **int PlayerNumber** |  | Removes a player from a specific slot and marks it as closed. |
|   | 1-8 |  |  |
|  **SetPlayerCivilization** | **int PlayerNumber** | **int Civilization** | Set the player's civilization. |
|   | 1-8 | 1 - Britons<br/>30 - Random |  |
|  **SetPlayerColor** | **int PlayerNumber** | **int Color** | Set color of the player's units and buildings. |
|   | 1-8 | 1-8 |  |
|  **SetPlayerTeam** | **int PlayerNumber** | **int Team** | Set which team the player belongs to. |
|   | 1-8 | 0 - No team<br/>1-4 Teams |  |
|  **GetPlayerExists** | **int PlayerNumber** |  | Returns whether the player slot is taken. |
|   | 1-8 |  |  |
|  **GetPlayerAlive** | **int PlayerNumber** |  | Returns whether the player is alive in the game. |
|   | 1-8 |  |  |

## Known issues
* Calling "QuitGame" from post match stats screen results in looping background music.

## In the future
* Implement the rest of the less important game settings

## Dependencies
* Microsoft Detours - https://github.com/Microsoft/Detours
* rpclib (with a minor change) - https://github.com/FLWL/rpclib
