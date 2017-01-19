#make master data table
import pandas as pd

#populations
df = pd.read_csv('populations.csv', quotechar='"')
counties = df['County'].values
pops = df[' population '].values
mpcounty_pop = {}
for i in range(len(counties)):
	mpcounty_pop[counties[i].strip()] = int(pops[i].replace(',',''))

#incomes
df = pd.read_csv('incomes.csv', quotechar='"')
years = df['Year'].values
counties = df['County'].values
incomes = df['Income'].values
mpyearcounty_income = {}
for i in range(len(incomes)):
	mpyearcounty_income[str(years[i])+counties[i].strip()] = int(incomes[i].replace('$','').replace(',',''))

#jackpots
df = pd.read_csv('jackpots_ALL.csv')
dates = df['Date'].values
jackpots = df['Jackpot'].values
mpdate_jackpot = {}
for i in range(len(dates)):
	mpdate_jackpot[dates[i]] = int(jackpots[i].replace('$','').replace(',',''))

#county sales
df = pd.read_csv('county_sales_ALL.csv', quotechar='"')
weeks = df['Week'].values
counties = df['County'].values
sales = df['Sales'].values

#make master csv
SALES = []
POPULATION = []
INCOME = []
JACKPOT = []
for i in range(len(weeks)):
	#remove schenectady
	if(counties[i].strip() == 'SCHENECTADY' or counties[i].strip() == 'OUT OF STATE'):
		continue

	#SALES.append(float(sales[i].replace(',','')))
	SALES.append(float(sales[i]))
	POPULATION.append(mpcounty_pop[counties[i].strip()])
	#use most recent income data available
	if(weeks[i][-4:] == '2015' or weeks[i][-4:] == '2016'):
		INCOME.append(mpyearcounty_income['2014' + counties[i].strip()])
	else:
		INCOME.append(mpyearcounty_income[weeks[i][-4:] + counties[i].strip()])
	JACKPOT.append(mpdate_jackpot[weeks[i]])

df = pd.DataFrame({'Sales':SALES, 'Income':INCOME, 'Jackpot':JACKPOT, 'Population':POPULATION})
df.to_csv('MASTER_DATA_ALL.csv', encoding='utf8')
#print df.to_string()

