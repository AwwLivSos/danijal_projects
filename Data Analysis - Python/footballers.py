import pandas as pd
import numpy as np
import mplcatppuccin
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
from ipyleaflet import Map
from google.colab import drive

drive.mount('/content/drive', force_remount=True)

df = pd.read_csv('/content/drive/MyDrive/fudbal_stats.csv')
##################################################################
df['Čiste mreže'] = pd.to_numeric(df['Čiste mreže'], errors='coerce')
#JAKO VAŽAN DIO, ovo bi trebali svaki put kucati ako ponovo učitavate set u novom poglavlju, jer pretvara kolonu u numeričku, a inače je kategorijska
#################################################################
df.head()
df.info()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
color = load_color("mocha", "peach")

############ # Ovaj dio koda je napisan čisto radi testiranja biblioteke ############

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
color = load_color("mocha", "peach")

numericke_varijable = ['Visina (m)', 'Najveća cijena na tržištu u milionima eura', 'Broj individualnih nagrada', 'Broj trofeja', 'Broj nastupa', 'Golovi', 'Asistencije', 'Broj kartona']
kategorijske_varijable = ['Liga', 'Konfederacija', 'Država', 'Kontinent', 'Pozicija', 'Preferirana noga', 'Aktivan igrač']

df[numericke_varijable].hist(bins=15, figsize=(15, 10))
plt.suptitle('Histogrami numeričkih varijabli')
plt.show()

for var in kategorijske_varijable:
    plt.figure(figsize=(10, 5))
    df[var].value_counts().plot(kind='bar')
    plt.title(f'Distribucija za {var}')
    plt.ylabel('Broj')
    plt.xlabel(var)
    plt.show()

import pandas as pd
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
from ipyleaflet import Map
from google.colab import drive

drive.mount('/content/drive', force_remount=True)

df = pd.read_csv('/content/drive/MyDrive/fudbal_stats.csv')
##################################################################
df['Čiste mreže'] = pd.to_numeric(df['Čiste mreže'], errors='coerce')

import matplotlib.pyplot as plt
import seaborn as sns
import matplotlib as mpl
mpl.style.use("mocha")
from mplcatppuccin.palette import load_color

numerical_columns = [
    'Visina (m)',
    'Najveća cijena na tržištu u milionima eura',
    'Broj individualnih nagrada',
    'Broj trofeja',
    'Broj nastupa',
    'Golovi',
    'Asistencije',
    'Čiste mreže',
    'Broj kartona'
]

num_columns = 3
num_rows = (len(numerical_columns) + num_columns - 1) // num_columns

fig, axes = plt.subplots(num_rows, num_columns, figsize=(20, 5 * num_rows))

for i, column in enumerate(numerical_columns):
    row = i // num_columns
    col = i % num_columns
    sns.boxplot(x=df[column].dropna(), ax=axes[row, col], color='white')
    axes[row, col].set_title(f'Box Plot za {column}', fontsize=13)
    axes[row, col].set_xlabel(column, fontsize=11)

for j in range(i + 1, num_rows * num_columns):
    fig.delaxes(axes.flatten()[j])

plt.tight_layout()
plt.show()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color

numerical_columns = [
    'Visina (m)',
    'Najveća cijena na tržištu u milionima eura',
    'Broj individualnih nagrada',
    'Broj trofeja',
    'Broj nastupa',
    'Golovi',
    'Asistencije',
    'Čiste mreže',
    'Broj kartona'
]

correlation_matrix = df[numerical_columns].corr()
print(correlation_matrix)

import matplotlib.pyplot as plt
import seaborn as sns

#sns.set(style="white")

mask = np.triu(np.ones_like(correlation_matrix, dtype=bool))

plt.figure(figsize=(14, 10))

cmap = sns.diverging_palette(220, 10, as_cmap=True)

sns.heatmap(correlation_matrix, mask=mask, cmap=cmap, vmax=.3, center=0,
            square=True, linewidths=.5, annot=True, cbar_kws={"shrink": .5})

plt.title('Korelacijska matrica za numeričke kolone', fontsize=16)
plt.show()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
sns.pairplot(df[numerical_columns], diag_kind='kde', plot_kws={'alpha':0.5})
plt.suptitle('Pair Plot za numeričke kolone', y=1.02)
plt.show()

categorical_columns = ['Klub', 'Liga', 'Konfederacija', 'Država', 'Kontinent', 'Pozicija', 'Preferirana noga', 'Aktivan igrač']

for column in categorical_columns:
    print(f'Broj vrijednosti za {column}:')
    print(df[column].value_counts())
    print('\n')

league_to_country = {
    "MLS": "SAD",
    "Saudi Pro League": "Saudijska Arabija",
    "LaLiga": "Španija",
    "Premier League": "Engleska",
    "Serie A": "Italija",
    "Super Lig": "Turska",
    "Ligue 1": "Francuska",
    "Brasileirão Série A": "Brazil",
    "Bundesliga": "Njemačka",
    "WWin Liga": "Bosna i Hercegovina",
    "Eredevisie": "Nizozemska",
    "Liga MX": "Meksiko",
    "Liga Portugal": "Portugal",
}


player_country = []
matching_count = 0
non_matching_count = 0

for i in range(len(df)):
  player_country.append(df["Država"][i])
  league = df["Liga"][i]
  if league in league_to_country:
    if league_to_country[league] == player_country[i]:
      matching_count += 1
    else:
      non_matching_count += 1
total_players = matching_count + non_matching_count
probability = matching_count / total_players

print("Vjerovatnoća da će igrač igrati za klub svoje države:", probability)

bayern_english_count = 0

for i in range(len(df)):
  country = df["Država"][i]
  club = df["Klub"][i]
  if country == "Engleska" and club == "Bayern Munich":
    bayern_english_count += 1

print("Broj Engleza u Bayernu:", bayern_english_count)


attackers_df = df[df["Pozicija"] == "Napad"]

left_footed = attackers_df["Preferirana noga"].value_counts().get("Lijeva", 0)
right_footed = attackers_df["Preferirana noga"].value_counts().get("Desna", 0)
ambipedal = attackers_df["Preferirana noga"].value_counts().get("Obje", 0)

total_attackers = len(attackers_df)

left_footed_prob = left_footed / total_attackers
right_footed_prob = right_footed / total_attackers
ambipedal_prob = ambipedal / total_attackers

print("Šansa za ljevaka:", left_footed_prob)
print("Šansa za dešnjaka:", right_footed_prob)
print("Šansa za ambipedalca:", ambipedal_prob)

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
average_goals = df['Golovi'].mean()
average_assists = df['Asistencije'].mean()

filtered_df = df[df['Pozicija'] != "Odbrana"]
filtered_df = filtered_df[filtered_df['Pozicija'] != "Golman"]

cmap = plt.cm.coolwarm

filtered_df['color_value'] = abs(filtered_df['Golovi'] - average_goals) + abs(filtered_df['Asistencije'] - average_assists)

plt.figure(figsize=(10, 6))

plt.scatter(filtered_df['Golovi'], filtered_df['Asistencije'], c=filtered_df['color_value'], cmap=cmap)

plt.axhline(y=average_assists, color='gray', linestyle='--', linewidth=1)
plt.axvline(x=average_goals, color='gray', linestyle='--', linewidth=1)

plt.xlabel('Golovi')
plt.ylabel('Asistencije')
plt.title('Golovi vs. Asistencije (Prosječne vrijednosti označene)')
plt.colorbar(label='Udaljenost od prosjeka')

plt.show()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
average_goals = df['Golovi'].mean()
average_assists = df['Asistencije'].mean()

filtered_df = df[df['Pozicija'] != "Odbrana"]
filtered_df = filtered_df[filtered_df['Pozicija'] != "Golman"]

cmap = plt.cm.coolwarm

filtered_df['color_value'] = abs(filtered_df['Golovi'] - average_goals) + abs(filtered_df['Asistencije'] - average_assists)

plt.figure(figsize=(10, 6))

plt.scatter(filtered_df['Golovi'], filtered_df['Asistencije'], c=filtered_df['color_value'], cmap=cmap)

plt.axhline(y=average_assists, color='gray', linestyle='--', linewidth=1)
plt.axvline(x=average_goals, color='gray', linestyle='--', linewidth=1)

for i, row in filtered_df.iterrows():
  plt.annotate(row['Igrač'], (row['Golovi'] + 0.1, row['Asistencije'] + 0.1), fontsize=8)

plt.xlabel('Golovi')
plt.ylabel('Asistencije')
plt.title('Golovi vs. Asistencije (Prosječne vrijednosti označene)')
plt.colorbar(label='Udaljenost od prosjeka')

plt.show()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
average_goals = df['Golovi'].mean()
average_assists = df['Asistencije'].mean()

filtered_df = df[df['Pozicija'] != "Odbrana"]
filtered_df = filtered_df[filtered_df['Pozicija'] != "Golman"]
filtered_df = filtered_df[~filtered_df['Igrač'].isin(["Lionel Messi", "Cristiano Ronaldo", "Pele"])]

cmap = plt.cm.coolwarm

filtered_df['color_value'] = abs(filtered_df['Golovi'] - average_goals) + abs(filtered_df['Asistencije'] - average_assists)

plt.figure(figsize=(10, 6))

plt.scatter(filtered_df['Golovi'], filtered_df['Asistencije'], c=filtered_df['color_value'], cmap=cmap)

plt.axhline(y=average_assists, color='gray', linestyle='--', linewidth=1)
plt.axvline(x=average_goals, color='gray', linestyle='--', linewidth=1)

for i, row in filtered_df.iterrows():
  plt.annotate(row['Igrač'], (row['Golovi'] + 0.1, row['Asistencije'] + 0.1), fontsize=8)

plt.xlabel('Golovi')
plt.ylabel('Asistencije')
plt.title('Golovi vs. Asistencije (Prosječne vrijednosti označene)')
plt.colorbar(label='Udaljenost od prosjeka')

plt.show()

mpl.style.use("mocha")
from mplcatppuccin.palette import load_color
average_goals = df['Golovi'].mean()
average_assists = df['Asistencije'].mean()

filtered_df = df[df['Pozicija'] != "Napad"]
filtered_df = filtered_df[filtered_df['Pozicija'] != "Sredina"]

cmap = plt.cm.coolwarm

filtered_df['color_value'] = abs(filtered_df['Golovi'] - average_goals) + abs(filtered_df['Asistencije'] - average_assists)

plt.figure(figsize=(10, 6))

plt.scatter(filtered_df['Golovi'], filtered_df['Asistencije'], c=filtered_df['color_value'], cmap=cmap)

plt.axhline(y=average_assists, color='gray', linestyle='--', linewidth=1)
plt.axvline(x=average_goals, color='gray', linestyle='--', linewidth=1)

for i, row in filtered_df.iterrows():
  plt.annotate(row['Igrač'], (row['Golovi'] + 0.1, row['Asistencije'] + 0.1), fontsize=8)

plt.xlabel('Golovi')
plt.ylabel('Asistencije')
plt.title('Golovi vs. Asistencije (Prosječne vrijednosti označene)')
plt.colorbar(label='Udaljenost od prosjeka')

plt.show()

import pandas as pd
import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import seaborn as sns
from ipyleaflet import Map
from google.colab import drive


drive.mount('/content/drive', force_remount=True)

df = pd.read_csv('/content/drive/MyDrive/fudbal_stats.csv')

df['Čiste mreže'] = pd.to_numeric(df['Čiste mreže'], errors='coerce')

import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import seaborn as sns
mpl.style.use("mocha")
from mplcatppuccin.palette import load_color

# List of numerical columns
numerical_columns = [
    'Visina (m)',
    'Najveća cijena na tržištu u milionima eura',
    'Broj individualnih nagrada',
    'Broj trofeja',
    'Broj nastupa',
    'Golovi',
    'Asistencije',
    'Čiste mreže',
    'Broj kartona'
]

filtered_df = df[numerical_columns]

def check_normality(data, column):
    ks_stat, ks_p_value = stats.kstest(data, 'norm', args=(np.mean(data), np.std(data)))
    sw_stat, sw_p_value = stats.shapiro(data)
    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    stats.probplot(data, dist="norm", plot=plt)
    plt.title(f'QQ-Graf za {column}')

    plt.subplot(1, 2, 2)
    sns.histplot(data, kde=True)
    plt.title(f'Histogram za {column}')

    plt.figtext(0.5, -0.1, f'Kolmogorov-Smirnov statistika: {ks_stat:.5f}\n'
                           f'Kolmogorov-Smirnov p-vrijednost: {ks_p_value:.5f}\n'
                           f'Shapiro-Wilk statistika: {sw_stat:.5f}\n'
                           f'Shapiro-Wilk p-vrijednost: {sw_p_value:.5f}',
                wrap=True, horizontalalignment='center', fontsize=12)

    plt.tight_layout()
    plt.show()

    return {
        'Kolmogorov-Smirnov statistika': f"{ks_stat:.5f}",
        'Kolmogorov-Smirnov p-vrijednost': f"{ks_p_value:.5f}",
        'Shapiro-Wilk statistika': f"{sw_stat:.5f}",
        'Shapiro-Wilk p-vrijednost': f"{sw_p_value:.5f}",
    }

results = {}
for column in numerical_columns:
    data = filtered_df[column].dropna()
    results[column] = check_normality(data, column)

for column, result in results.items():
    print(f'Results for {column}:\n{result}\n')

import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import seaborn as sns
mpl.style.use("mocha")
from mplcatppuccin.palette import load_color

numerical_columns = [
    'Najveća cijena na tržištu u milionima eura',
    'Golovi',
    'Asistencije',
    'Broj kartona'
]

def check_normality(data, column):
    ks_stat, ks_p_value = stats.kstest(data, 'norm', args=(np.mean(data), np.std(data)))
    sw_stat, sw_p_value = stats.shapiro(data)

    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    stats.probplot(data, dist="norm", plot=plt)
    plt.title(f'QQ-Graf za {column}')

    plt.subplot(1, 2, 2)
    sns.histplot(data, kde=True)
    plt.title(f'Histogram za {column}')

    plt.figtext(0.5, -0.1, f'Kolmogorov-Smirnov statistika: {ks_stat:.5f}\n'
                           f'Kolmogorov-Smirnov p-vrijednost: {ks_p_value:.5f}\n'
                           f'Shapiro-Wilk statistika: {sw_stat:.5f}\n'
                           f'Shapiro-Wilk p-vrijednost: {sw_p_value:.5f}',
                wrap=True, horizontalalignment='center', fontsize=12)

    plt.tight_layout()
    plt.show()

    return {
        'Kolmogorov-Smirnov statistika': f"{ks_stat:.5f}",
        'Kolmogorov-Smirnov p-vrijednost': f"{ks_p_value:.5f}",
        'Shapiro-Wilk statistika': f"{sw_stat:.5f}",
        'Shapiro-Wilk p-vrijednost': f"{sw_p_value:.5f}",
    }

filtered_results = {}

active_players = df[df['Aktivan igrač'] == 'da']
filtered_results['Najveća cijena na tržištu u milionima eura'] = check_normality(active_players['Najveća cijena na tržištu u milionima eura'].dropna(), 'Najveća cijena na tržištu u milionima eura')

forwards = df[df['Pozicija'] == 'Napad']
filtered_results['Golovi'] = check_normality(forwards['Golovi'].dropna(), 'Golovi')

forwards_midfielders = df[df['Pozicija'].isin(['Napad', 'Sredina'])]
filtered_results['Asistencije'] = check_normality(forwards_midfielders['Asistencije'].dropna(), 'Asistencije')

defenders = df[df['Pozicija'] == 'Odbrana']
filtered_results['Broj kartona'] = check_normality(defenders['Broj kartona'].dropna(), 'Broj kartona')

for column in numerical_columns:
    if column not in filtered_results:
        data = df[column].dropna()
        filtered_results[column] = check_normality(data, column)

for column, result in filtered_results.items():
    print(f'Results for {column}:\n{result}\n')

import numpy as np
import scipy.stats as stats
import matplotlib.pyplot as plt
import seaborn as sns
mpl.style.use("mocha")
from mplcatppuccin.palette import load_color

forwards['LogGolovi'] = np.log1p(forwards['Golovi'])

def check_normality(data, column):
    ks_stat, ks_p_value = stats.kstest(data, 'norm', args=(np.mean(data), np.std(data)))
    sw_stat, sw_p_value = stats.shapiro(data)

    plt.figure(figsize=(12, 6))
    plt.subplot(1, 2, 1)
    stats.probplot(data, dist="norm", plot=plt)
    plt.title(f'QQ-Plot za {column}')

    plt.subplot(1, 2, 2)
    sns.histplot(data, kde=True)
    plt.title(f'Histogram za {column}')

    plt.figtext(0.5, -0.1, f'Kolmogorov-Smirnov statistika: {ks_stat:.5f}\n'
                           f'Kolmogorov-Smirnov p-vrijednost: {ks_p_value:.5f}\n'
                           f'Shapiro-Wilk statistika: {sw_stat:.5f}\n'
                           f'Shapiro-Wilk p-vrijednost: {sw_p_value:.5f}',
                wrap=True, horizontalalignment='center', fontsize=12)

    plt.tight_layout()
    plt.show()

    return {
        'Kolmogorov-Smirnov statistika': f"{ks_stat:.5f}",
        'Kolmogorov-Smirnov p-vrijednost': f"{ks_p_value:.5f}",
        'Shapiro-Wilk statistika': f"{sw_stat:.5f}",
        'Shapiro-Wilk p-vrijednost': f"{sw_p_value:.5f}",
    }

log_golovi_data = forwards['LogGolovi'].dropna()
check_normality(log_golovi_data, 'LogGolovi')