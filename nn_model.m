%%% D(:,1) = income
%%% D(:,2) = jackpot
%%% D(:,3) = population
%%% D(:,4) = sales

D = load('MASTER_DATA_ALL.csv');
income = D(:,1);
jackpot = D(:,2);
pop = D(:,3);
sales = D(:,4);
N = load('NN_best.log');
projected = N(:,1);

r = projected - sales;
RMSE_NN = sqrt((r'*r)/size(sales,1));

graph_scatter(income, jackpot, pop, sales, projected, 'income', 'jackpot', 'population');
title('Neural Network model');
caxis([0,260000]);
