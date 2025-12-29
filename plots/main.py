import matplotlib.pyplot as plt


players = list(input().split())
# num_players = len(players)

iter = int(input())

m = {}
a = {}
g = {}

for p in players:
    m[p] = []
    a[p] = []
    g[p] = []
    

for _ in iter:
    for p in players:
        micros, agents, gold = map(int, input().split())
        m[p].append(micros)
        a[p].append(agents)
        g[p].append(gold)

plt.style.use('dark_background');
time = list(range(iter))

plt.xlabel('iterations')
plt.ylabal('exec time (us)')
for p in players:
    plt.plot(time, m[p], color=p, label=p)
plt.legend()

plt.savefig('micros.pdf', format='pdf', dpi=300, bbox_inches='tight')


plt.xlabel('iterations')
plt.ylabal('alive agents')
for p in players:
    plt.plot(time, a[p], color=p, label=p)
plt.legend()

plt.savefig('agents.pdf', format='pdf', dpi=300, bbox_inches='tight')


plt.xlabel('iterations')
plt.ylabal('gold')
for p in players:
    plt.plot(time, g[p], color=p, label=p)
plt.legend()

plt.savefig('gold.pdf', format='pdf', dpi=300, bbox_inches='tight')
