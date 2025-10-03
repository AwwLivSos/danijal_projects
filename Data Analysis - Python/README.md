# Data Analysis - Python 🐍📊

Welcome to my **Data Analysis - Python** portfolio!  
This repository showcases a collection of my Python projects, where I explore datasets, perform statistical analysis, visualize data, and extract meaningful insights. The projects demonstrate proficiency in **Pandas, NumPy, Matplotlib, Seaborn**, and other data analysis tools.  

---

## 📂 Projects Included

### 1. EV Charging Analysis ⚡
- **Dataset:** Electric vehicle charging sessions
- **Focus:** 
  - Exploratory Data Analysis (EDA) of charging sessions  
  - Statistical analysis: mean, median, standard deviation, Chebyshev bounds  
  - Visualization: histograms, bar charts, scatter plots, heatmaps  
  - Probability analysis: likelihood of high consumption, director vs non-director vehicles, weekend vs weekday trends
- **Technologies:** Pandas, NumPy, Matplotlib, Seaborn, Google Colab
- **Highlights:**  
  - Cumulative distribution functions (CDF) of energy consumption  
  - Insights on charging patterns by user type and day of week  

### 2. Football Players Statistics Analysis ⚽
- **Dataset:** Player stats from various leagues
- **Focus:** 
  - EDA of numerical and categorical features  
  - Visualization: box plots, histograms, scatter plots, correlation matrices, pair plots  
  - Probability analysis and normality tests (Kolmogorov-Smirnov, Shapiro-Wilk)  
  - Log transformation and filtered analysis by positions  
- **Technologies:** Pandas, NumPy, Matplotlib, Seaborn, mplcatppuccin, Google Colab
- **Highlights:**  
  - Comparing player performance metrics across positions  
  - Analyzing goal/assist patterns and deviations from averages  
  - Filtering for active players, position-based insights, and outlier handling  

---

## 📊 Visualizations
Some examples of the visualizations generated in these projects:  

- **EV Charging:**  
  - ![Bar chart of energy consumption by platform](images/ev_platform_bar.png)  
  - ![Histogram of charge time](images/ev_charge_hist.png)  

- **Football Stats:**  
  - ![Boxplots of player metrics](images/football_boxplots.png)  
  - ![Correlation heatmap of numerical features](images/football_corr_heatmap.png)  

> ⚠️ *Note:* For full interactive visualizations, run the notebooks in Google Colab.

---

## 🛠️ Tools & Libraries
- **Python:** Pandas, NumPy, Matplotlib, Seaborn, Scipy, mplcatppuccin  
- **Environment:** Google Colab  

---

# Data Analysis - Python Projects

## 📚 Projects Included

1. **EV Charging Analysis**

   * Analysis of electric vehicle charging sessions.
   * Includes session durations, energy consumption, platform usage, and manager vs. non-manager statistics.

2. **Football Player Stats Analysis**

   * Analysis of football player statistics across various leagues.
   * Includes numerical and categorical analysis, correlations, visualizations, and normality checks.

## 🚀 How to Run

1. Clone this repository:
2. Open the desired notebook in **Google Colab**.
3. If the datasets are stored on Google Drive, mount it:

   ```python
   from google.colab import drive
   drive.mount('/content/drive')
   ```
4. Run all cells to reproduce the analysis.

📌 **Notes**

* Each project includes its own notebook and dataset references.
* Visualizations are automatically generated from the datasets; some preprocessing may be required if you use new datasets.

## 🖼️ Visualizations

* Histograms, box plots, scatter plots, heatmaps, pair plots, and KDE plots for the included datasets.
* Color themes applied using `mplcatppuccin`.

## ✨ Author
Danijal Alibegović


