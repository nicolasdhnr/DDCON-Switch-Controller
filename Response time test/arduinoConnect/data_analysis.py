import pandas as pd

data = pd.read_csv("response_time_data.csv")
# print(data)

average_time = data["Response time"].mean()

print(average_time)
print(data["Response time"].std())
