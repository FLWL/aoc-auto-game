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

## Dependencies
* Microsoft Detours - https://github.com/Microsoft/Detours
* rpclib (with a minor change) - https://github.com/FLWL/rpclib
