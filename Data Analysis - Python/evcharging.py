import pandas as pd
import numpy as np
from google.colab import drive

drive.mount('/content/drive', force_remount=True)

#KAD SE RELOADA COLAB, OVAJ DIO SE MORA PONOVO POKRENUTI DA BI SE UČITALI PODACI, INAČE NA SVAKOM SLJEDEĆEM CELLU ĆE BITI PRIJAVLJENA GREŠKA
#alternativno, ovaj dio se može pisati za svaki naredni segment
df = pd.read_csv('/content/drive/MyDrive/ev_charging.csv')
df.head()

#preimenovanje i pregled varijabli i kolona
weekday_mapping = {
    'Mon': 'ponedjeljak',
    'Tue': 'utorak',
    'Wed': 'srijeda',
    'Thu': 'četvrtak',
    'Fri': 'petak',
    'Sat': 'subota',
    'Sun': 'nedjelja'
}
df['weekday'] = df['weekday'].replace(weekday_mapping)
df = df.rename(columns={'userId': 'Korisnici', 'stationId': 'Stanice', 'locationId': 'Lokacije'})
print (df.columns)
df.info()

# osnovne informacije
number_of_rows = len(df)
print("Ovaj dataset ima", number_of_rows, "redova, tj sesija punjenja električnih vozila.")
print(f"Broj korisnika koji je analiziran je {df['Korisnici'].nunique()}, broj stanica {df['Stanice'].nunique()}, na {df['Lokacije'].nunique()} različitih lokacija.")

# moda, medijan, standardna devijacija i varijansa za punjenje
stats = [df['chargeTimeHrs'].mean(), df['chargeTimeHrs'].median(), df['chargeTimeHrs'].std(), df['chargeTimeHrs'].var()]

print("\nOsnovne inofrmacije o vremenu punjenja:")
print(f"Prosječno (mean): {stats[0]:.2f} sati")
print(f"Medijan: {stats[1]:.2f} sati")
print(f"Standardna devijacija: {stats[2]:.2f} sati")
print(f"Varijansa: {stats[3]:.2f} sati^2\n")
print(f"Vrijeme kada se punjnje najviše započinjalo je {df['startTime'].mode()[0]}h, dok je {df['endTime'].mode()[0]}h vrijeme kada je najčešće završavalo.")

# trazimo broj platformi i koje su
platform_counts = df['platform'].value_counts()
print("\nPunjenje se vršilo na sljedećim platformama:")
for platform, count in platform_counts.items():
  print(f"- {platform}: {count} puta")


#informacije o danima
weekday_counts = df['weekday'].value_counts()
print(f"\nPunjenje se vršilo na {df['weekday'].nunique()} različitih dana, od kojih je najčešći {df['weekday'].mode()[0]}, a najrjeđi {weekday_counts.idxmin()}.\n")

#statistike za kwh
stats_kwh = df['kwhTotal'].describe()
print("Osnovne informacije o utrošenoj energiji (kWh):")
print(f"Prosjek (mean): {stats_kwh['mean']:.2f} kWh")
print(f"Medijan: {stats_kwh['50%']:.2f} kWh")
print(f"Standardna devijacija: {stats_kwh['std']:.2f} kWh")
print(f"Varijansa: {df['kwhTotal'].var():.2f} (kWh)^2\n")

#suma jer su jedinice ako jest vozilo menadžera, a 0 ako nije
print(f"Punjenje vozila direktora: {df['managerVehicle'].sum()}")

import matplotlib.pyplot as plt

platform_consumption = df.groupby('platform')['kwhTotal'].sum()

plt.figure(figsize=(8, 6))
plt.bar(platform_consumption.index, platform_consumption.values)
plt.xlabel('Platforma')
plt.ylabel('Ukupna utrošena energija (kWh)')
plt.title('Utrošena energija po platformama')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

plt.show()

import matplotlib.pyplot as plt
platform_consumption = df.groupby('platform')['kwhTotal'].mean()

plt.figure(figsize=(8, 6))
plt.bar(platform_consumption.index, platform_consumption.values)
plt.xlabel('Platforma')
plt.ylabel('Prosječna utrošena energija (kWh)')
plt.title('Prosječna utrošena energija po platformama')
plt.xticks(rotation=45, ha='right')
plt.tight_layout()

plt.show()

import matplotlib.pyplot as plt

plt.figure(figsize=(8, 6))
plt.hist(df['chargeTimeHrs'], bins=20, edgecolor='black')
plt.xlabel('Vrijeme punjenja (sati)')
plt.ylabel('Broj sesija punjenja')
plt.title('Raspodjela vremena punjenja')
plt.grid(True)
plt.tight_layout()

plt.show()

import matplotlib.pyplot as plt

plt.figure(figsize=(8, 6))

desired_percentile = 0.999
percentile_value = df['chargeTimeHrs'].quantile(desired_percentile)

plt.xlim(0, percentile_value * 1.1)

plt.hist(df['chargeTimeHrs'], bins=20, edgecolor='black')
plt.xlabel('Vrijeme punjenja (sati)')
plt.ylabel('Broj sesija punjenja')
plt.title('Raspodjela vremena punjenja')
plt.grid(True)
plt.tight_layout()

plt.show()

import matplotlib.pyplot as plt

plt.figure(figsize=(8, 6))
plt.boxplot(df['chargeTimeHrs'])
plt.xlabel('Vrijeme punjenja (sati)')
plt.ylabel('Vrijednost')
plt.title('Raspodjela vremena punjenja (sa označenim outlierima)')
plt.grid(True)
plt.tight_layout()

plt.show()

import matplotlib.pyplot as plt

day_counts = df['weekday'].value_counts()

plt.figure(figsize=(8, 6))
plt.scatter(day_counts.index, day_counts.values)
plt.xlabel('Dan u sedmici')
plt.ylabel('Broj sesija punjenja')
plt.title('Broj sesija punjenja po danu u sedmici')
plt.xticks(rotation=45, ha='right')

plt.show()


import matplotlib.pyplot as plt
day_avg_time = df.groupby('weekday')['chargeTimeHrs'].mean()
day_order = ['ponedjeljak', 'utorak', 'srijeda', 'četvrtak', 'petak', 'subota', 'nedjelja']
plt.figure(figsize=(8, 6))
plt.bar(day_order, day_avg_time.loc[day_order])
plt.xlabel('Dan u sedmici')
plt.ylabel('Prosječno vrijeme punjenja (sati)')
plt.title('Prosječno vrijeme punjenja po danu u sedmici')
plt.xticks(rotation=45, ha='right')

plt.show()

import pandas as pd
import matplotlib.pyplot as plt

hourly_kwh_totals = df.groupby('startTime')['kwhTotal'].sum()

peak_hour = hourly_kwh_totals.idxmax()
peak_kwh = hourly_kwh_totals.max()

hourly_kwh_percentages = hourly_kwh_totals / hourly_kwh_totals.sum() * 100

normalized = True

data_to_plot = (
    hourly_kwh_percentages.values.reshape(1, -1) if normalized else hourly_kwh_totals.values.reshape(1, -1)
)

plt.figure(figsize=(10, 6))
plt.imshow(data_to_plot, cmap='YlGnBu', aspect='auto')  #
plt.colorbar(
    label='Ukupni kWh potrošen' if not normalized else 'Procenat ukupnog kWh'
)
plt.xticks(np.arange(24), range(24))
plt.xlabel('Sat')
plt.ylabel('Ukupni kWh (ili Procenat)' if normalized else 'Ukupni kWh')
plt.title('Ukupni kWh potrošeni po satima tokom dana')

if not normalized:
    plt.text(peak_hour, peak_kwh + 50, f"Peak: {peak_hour} ({peak_kwh:.2f} kWh)", ha='center', va='bottom', fontsize=10)

plt.tight_layout()
plt.show()

#za svaki novi naslov, najbolje je ponovno učitati podatke sa drive-a, pa onda sa njima manipulirati

from google.colab import drive

drive.mount('/content/drive', force_remount=True)

df = pd.read_csv('/content/drive/MyDrive/ev_charging.csv')

import pandas as pd

high_consumption_threshold = 5.81 #Promijeniti po želji, ali ovo je izabrano radi prosjeka

high_consumption_sessions = df[df['kwhTotal'] > high_consumption_threshold]

total_sessions = len(df)

probability_high_consumption = len(high_consumption_sessions) / total_sessions

percentage_high_consumption = probability_high_consumption * 100
percentage_formatted = f"{percentage_high_consumption:.2f}%"
probability_formatted = f"{probability_high_consumption:.4f}"

print(f"Vjerovatnoća da sesija prebaci {high_consumption_threshold} kWh: {probability_formatted} ({percentage_formatted})")

import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt

sns.kdeplot(df['kwhTotal'], fill=True)
plt.xlabel('kWh potrošeno')
plt.ylabel('Udio')
plt.title('Distribucija sesija punjenja po kwh')
plt.grid(True)
plt.show()

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

manager_vehicles = df[df['managerVehicle'] == 1]
non_manager_vehicles = df[df['managerVehicle'] == 0]

average_kwh_manager = manager_vehicles['kwhTotal'].mean()
average_kwh_non_manager = non_manager_vehicles['kwhTotal'].mean()

average_kwh = df['kwhTotal'].mean()
print(f"Prosjek utrošenih kwh za standardna vozila: {average_kwh_non_manager:.2f}")

average_kwh_manager = manager_vehicles['kwhTotal'].mean()
print(f"Prosjek utrošenih kwh za vozila direktora: {average_kwh_manager:.2f}")

plt.figure(figsize=(10, 5))
plt.hist(manager_vehicles['kwhTotal'], bins=20, alpha=0.5, label='Vozila direktora')
plt.hist(non_manager_vehicles['kwhTotal'], bins=20, alpha=0.5, label='Standardna vozila')
plt.xlabel('kWh korišteno')
plt.ylabel('Broj punjenja')
plt.title('Distribucija trošnje kwh (direktori vs. ne-direktori)')
plt.grid(True)
plt.legend()
plt.show()

high_consumption_manager = manager_vehicles['kwhTotal'] > average_kwh
probability_high_consumption_manager = high_consumption_manager.mean()
print(f"\nVjerovatnoća da punjenje vozila direktora prebaci prosjek: {probability_high_consumption_manager:.4f}")

difference_kwh = average_kwh_manager - average_kwh_non_manager
print(f"\nProsjek razlike u kwh (direktori vs. ne-direktori): {difference_kwh:.2f}")

percentage_difference = (difference_kwh / average_kwh_non_manager) * 100
print(f"Procentualna razlika u kWh: {percentage_difference:.2f}%")

import pandas as pd

def is_weekend(day):
  weekends = ['Sat', 'Sun']
  return day in weekends

manager_vehicles = df[df['managerVehicle'] == 1]

weekend_sessions = manager_vehicles[manager_vehicles['weekday'].apply(is_weekend)]
weekday_sessions = manager_vehicles[~manager_vehicles['weekday'].apply(is_weekend)]

total_managers = len(manager_vehicles)
weekend_charges = len(weekend_sessions)
weekday_charges = len(weekday_sessions)

# Option 1: Probabilities with 4 decimals
probability_weekend_charge = weekend_charges / total_managers
probability_weekday_charge = weekday_charges / total_managers

print("Vjerovatnoća da će direktor puniti vozilo za vikend:", f"{probability_weekend_charge:.4f}")
print("Vjerovatnoća da će direktor puniti vozilo radnim danom:", f"{probability_weekday_charge:.4f}")

# Option 2: Percentages with 2 decimals
percentage_weekend_charge = (weekend_charges / total_managers) * 100
percentage_weekday_charge = (weekday_charges / total_managers) * 100

print("\nVjerovatnoća da će direktor puniti vozilo za vikend (kao postotak):", f"{percentage_weekend_charge:.2f}%")
print("Vjerovatnoća da će direktor puniti vozilo radnim danom (kao postotak):", f"{percentage_weekday_charge:.2f}%")

import pandas as pd

manager_vehicles = df[df['managerVehicle'] == 1]
non_manager_vehicles = df[df['managerVehicle'] == 0]

total_managers = len(manager_vehicles)
managers_reported_zip = len(manager_vehicles[manager_vehicles['reportedZip'] == 1])

total_non_managers = len(non_manager_vehicles)
non_managers_reported_zip = len(non_manager_vehicles[non_manager_vehicles['reportedZip'] == 1])

probability_manager_reported = managers_reported_zip / total_managers
probability_non_manager_reported = non_managers_reported_zip / total_non_managers

print("Vjerovatnoća da direktor prijavi ZIP kod:", f"{probability_manager_reported:.4f}")
print("Vjerovatnoća da ne-direktor prijavi ZIP kod:", f"{probability_non_manager_reported:.4f}")

#za svaki novi naslov, najbolje je ponovno učitati podatke sa drive-a, pa onda sa njima manipulirati
import pandas as pd
from google.colab import drive

drive.mount('/content/drive', force_remount=True)

df = pd.read_csv('/content/drive/MyDrive/ev_charging.csv')

import pandas as pd

def chebyshev_inequality(data, k):
  mean = data.mean()
  std = data.std()
  bound = 1 - (1 / (k**2))
  return bound


columns_to_analyze = ['kwhTotal', 'chargeTimeHrs']

for col in columns_to_analyze:
  data = df[col]

  std = data.std()
  print(f"\nStandard Deviation for {col}: {std:.2f}")

  k = 3

  chebyshev_bound = chebyshev_inequality(data, k)

  print(f"Chebyshev's Inequality Bound (k = {k} standard deviations) for {col}:")
  print(f"At least {chebyshev_bound:.4f} proportion of data points will fall within {k} standard deviations of the mean.")

plt.hist(df['dollars'], bins='auto')
plt.xlabel('Potrošeni dolari')
plt.ylabel('Broj sesija')
plt.title('Distribucija cijene')
plt.grid(True)
plt.show()

selected_columns = ['kwhTotal', 'dollars', 'chargeTimeHrs']
data = df[selected_columns]

covariance_matrix = data.cov()
print("Matrica kovarijanse:")
print(covariance_matrix)

correlation_matrix = data.corr(method='pearson')
print("\nMatrica korelacije:")
print(correlation_matrix)

import numpy as np
import matplotlib.pyplot as plt

def calculate_cdf(data):
  sorted_data = data.sort_values()
  unique_data, counts = np.unique(sorted_data, return_counts=True)
  cdf = np.cumsum(counts) / len(data)
  return pd.Series(cdf, index=unique_data)

columns_to_analyze = ['kwhTotal', 'dollars', 'chargeTimeHrs']

plt.figure(figsize=(12, 6))

for i, col in enumerate(columns_to_analyze):
  data = df[col]
  cdf = calculate_cdf(data)

  plt.subplot(1, 3, i + 1)
  plt.plot(cdf.index, cdf.values)
  plt.xlabel(col)
  plt.ylabel('Kumulativna vjerovatnoća')
  plt.title(f'FKD za "{col}"')
  plt.grid(True)

plt.tight_layout()
plt.show()