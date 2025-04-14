import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("benchmarking/results.csv")

df['array_size'] = df['array_size'].astype(int)
df['time'] = df['time'].astype(float)

methods = df['execution_method'].unique()

plt.figure(figsize=(10, 6))

for method in methods:
    method_df = df[df['execution_method'] == method]
    method_df = method_df.sort_values('array_size')
    plt.plot(method_df['array_size'], method_df['time'], marker='o', label=method)

plt.xlabel("Array Size")
plt.ylabel("Elapsed Time (seconds)")
plt.title("Execution Time vs. Array Size")
plt.xscale('log')
plt.yscale('linear')
plt.legend()
plt.tight_layout()
# plt.savefig("execution_time_plot_clean.png") # if you have no GUI component and need to save as png
plt.show()
