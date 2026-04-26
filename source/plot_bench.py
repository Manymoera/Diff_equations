import matplotlib.pyplot as plt
import pandas as pd

try:
    df = pd.read_csv('data/bench_results.csv') 
    versions = df.columns[1:]
    times = df.iloc[0, 1:].values
except:
    versions = ['No Opt', 'O3', 'O3+native_march', 'O3+native_march+fast-math', 'OpenMP', 'OpenMP_xorshift']
    times = [175.19, 52.26, 52.09, 47.99, 29.46, 6.35]

plt.figure(figsize=(10, 6))
colors = ['#888888', '#3498db', '#2980b9', '#1f618d', '#27ae60']
bars = plt.bar(versions, times, color=colors, edgecolor='black', alpha=0.8)

for bar in bars:
    yval = bar.get_height()
    plt.text(bar.get_x() + bar.get_width()/2, yval + 1, round(yval, 2), 
             ha='center', va='bottom', fontweight='bold')

plt.title('Performance Comparison (N=100,000)', fontsize=14)
plt.ylabel('Mean Time (seconds)', fontsize=12)
plt.xticks(rotation=15)
plt.grid(axis='y', linestyle='--', alpha=0.6)

plt.savefig('data/benchmark_plot.png')
print("График сохранен в data/benchmark_plot.png")
plt.show()