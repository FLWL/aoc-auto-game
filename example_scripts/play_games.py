import msgpackrpc
import time

# this script repeatedly plays games and marks down the winners
autogame = msgpackrpc.Client(msgpackrpc.Address("127.0.0.1", 64720))

def launch_game():
    autogame.call('ResetGameSettings')                    # usually reset the settings to make sure everything is valid
    autogame.call('SetGameRevealMap', 2)                  # set map to "All Visible"
    autogame.call('SetGameMapType', 12)                   # set map location to Black Forest
    autogame.call('SetGameMapSize', 0)
    autogame.call('SetGameDifficulty', 1)
    autogame.call('SetGameStartingAge', 0)
    autogame.call('SetGameType', 0)
    autogame.call('SetGameTeamsLocked', True)

    autogame.call('SetPlayerComputer', 1, "Barbarian")
    autogame.call('SetPlayerComputer', 2, "AT_Empire")

    autogame.call('SetRunFullSpeed', True)                # run the game logic as fast as possible
    autogame.call('SetRunUnfocused', True)                # allow the game to run while minimized
    return autogame.call('StartGame')                     # start the match


num_games = 0
win_one = 0
win_two = 0
draws = 0
while True:
    if not launch_game():
        break

    seconds_passed = 0
    draw = False
    while autogame.call('GetGameInProgress'):  # wait until the game has finished
        time.sleep(1.0)
        seconds_passed += 1
        if seconds_passed > 600:	# if a game has been simulated for more than 10 minutes, declare it a draw
            autogame.call('QuitGame')
            num_games += 1
            draws += 1
            draw = True
            print("Finished game " + str(num_games) + ": draw.")
            break

    if not draw:
        winner = autogame.call('GetWinningPlayer')
        num_games += 1
        if winner == 1: win_one += 1
        if winner == 2: win_two += 1

        print("Finished game " + str(num_games) + ", winner: " + str(winner) + " and score: " + str(win_one) + " / " + str(win_two) + " (p1 / p2)" + " draws: " + str(draws))
