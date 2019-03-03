# aoc-auto-game
An API for programmatically starting Age of Empires II: The Conquerors matches with specified settings.

## Example client (Python)
```
import msgpackrpc

autogame = msgpackrpc.Client(msgpackrpc.Address("127.0.0.1", 64720))
autogame.call_async('SetMultiplayerGame', False)
autogame.call_async('SetGameType', 0)
autogame.call_async('SetPlayerComputer', 1, "AT_Empire")
autogame.call_async('SetPlayerComputer', 2, "Barbarian")
autogame.call('LaunchGame')
```

## Dependencies
* Microsoft Detours - https://github.com/Microsoft/Detours
* rpclib (with a minor change) - https://github.com/FLWL/rpclib
