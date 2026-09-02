import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmark.csv")
print(df["iterations"])

fig, axs = plt.subplots(2,3, figsize=(18,8))
axes = axs.flatten()

def plotXstride(yname, idx):
    axes[idx].plot(df["stride"], df[yname], label=f'{yname}(stride)')
    axes[idx].set_xlabel("stride")
    axes[idx].set_ylabel(yname)
    axes[idx].set_title("evolution of {yname} for stride variable")
    axes[idx].legend()

for i, yname in enumerate(df.columns):
    if yname != "stride":
        plotXstride(yname, i)

plt.tight_layout()
fig.savefig("figure")
