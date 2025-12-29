import matplotlib.pyplot as plt
import numpy as np
import sys

sys.stdin = open('./statistics.txt', 'r')

def ma(x, scale=1e-3, window_size=15):
    data = np.array(x) * scale
    window = np.ones(window_size) / window_size
    return np.convolve(data, window, mode='same')
    
players = list(input().split())
print("players", players)

iter = int(input())

stats = ['Gold', 'Alive agents', 'Execution time (ms)']
data = {stat: {p: [] for p in players} for stat in stats}

for _ in range(iter):
    for p in players:
        y = tuple(map(int, input().split()))
        for i, stat in enumerate(stats):
            data[stat][p].append(y[i])

i = stats[2]
data[i] = {p: ma(data[i][p]) for p in players}

plt.style.use('dark_background');
fig, axis = plt.subplots(3, 1, figsize=(20, 12))

x = list(range(iter))
for i, (stat, y) in enumerate(data.items()):
    # axis[i].set_xlabel('it')
    # axis[i].set_ylabel(stat)
    for p in players:
        axis[i].plot(x, y[p], color=p, label=p)
    axis[i].legend()
    axis[i].set_title(stat, fontweight='bold')
    

result = "stats.pdf"
# plt.tight_layout()
plt.tight_layout(pad=5.0)
plt.savefig(result, format='pdf', dpi=300, bbox_inches='tight')
print(f"Game statistics created in {result}")
plt.show()
