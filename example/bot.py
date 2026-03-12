import random

print("bot.py", flush=True)

board_size, turns = map(int, input().split())

players = int(input())

for _ in range(players):
    row, col = map(int, input().split())

for _ in range(turns):
    agents = int(input())

    for _ in range(agents):
        row, col, object, dist, rot, has_gold = input().split()

    for _ in range(agents):
        actions = ["FIRE", "GO", "MINE", "LEFT", "RIGHT", "BACK"]
        a = random.choice(actions)
        print(a, flush=True)

