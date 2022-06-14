import matplotlib.pyplot as plt

with open('Untitled Document 1') as f:
    lines = f.readlines()
    Channel_names = []
    Channel_scores = []
    for elem in lines:
        tally = elem.count("X")
        Channel_scores.append(tally)
        print(len(Channel_scores))

xAxis = range(1, 128)
plt.bar(xAxis, Channel_scores)
plt.title("Graph of scan results of available channels on 2.4GHz frequency band")
plt.xlabel('Channel number')
plt.ylabel("Number of signals detected")
plt.show()

print(Channel_scores)