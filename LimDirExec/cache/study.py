import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")
print(df["iterations"])

fig = plt.figure()
plt.plot(df["stride"], df["timeSeconds"])
plt.savefig(fig)
