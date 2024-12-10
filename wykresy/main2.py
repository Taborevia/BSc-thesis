import numpy as np
import matplotlib.pyplot as plt
plt.rcParams.update({'font.family': 'Times New Roman'})

# Wczytanie danych z pliku
with open('wyniki_4_new.txt', 'r') as file:
    lines = file.readlines()
print(lines[0])
start = 1
end = 1001
data = []
# pokolenie = []
# for line in lines[start:end]:
#     genotype = list(map(float, line.strip().split()))
#     pokolenie.append(genotype)
# data.append(pokolenie)
# start = 102
# end = 212
while True:
    pokolenie = []
    for line in lines[start:end]:
        genotype = list(map(float, line.strip().split()))
        pokolenie.append(genotype)
    data.append(pokolenie)
    start += 1001
    end += 1001
    if end > 714714:
        break
# Obliczenie średnich, maksymalnych wartości i odchylenia standardowego

std_devs = []
for i in range(0,len(data)):
    temp = np.std(data[i], ddof=0, axis=0)
    std_devs.append(sum(temp)/len(temp))


print(std_devs)

# Generowanie wykresów
plt.figure(figsize=(12, 6))

# # Średnia i maksymalna wartość funkcji oceny
# plt.subplot(1, 2, 1)
# plt.plot(means, label='Średnia wartość', marker='o')
# plt.plot(max_values, label='Maksymalna wartość', marker='x', linestyle='--')
# plt.title('Średnia i maksymalna wartość funkcji oceny w pokoleniach')
# plt.xlabel('Pokolenie')
# plt.ylabel('Wartość funkcji oceny')
# plt.legend()
# plt.grid(True)

# Odchylenie standardowe
plt.plot(std_devs)
plt.title('')
plt.xlabel('Pokolenie',fontsize=20)
plt.ylabel('Średnie odchylenie standardowe wag',fontsize=20)
plt.grid(True)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.tight_layout()
plt.savefig("wykres_zbieznosci_eksp_4.svg",dpi=300, bbox_inches='tight')
plt.show()
