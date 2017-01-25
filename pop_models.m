%%% D(:,1) = income
%%% D(:,2) = jackpot
%%% D(:,3) = population
%%% D(:,4) = sales

D = load('MASTER_DATA_ALL.csv');
income = D(:,1);
jackpot = D(:,2);
pop = D(:,3);
sales = D(:,4);
L = max(sales) + 1; 

%linear model
A = [income, jackpot, pop, ones(size(D,1),1)];
b = sales;
c_linear = A\b;
projected_linear = A*c_linear;
r = projected_linear - b;
RMSE_linear = sqrt((r'*r)/size(b,1));

graph_scatter(income, jackpot, pop, sales, projected_linear, 'income', 'jackpot', 'population');
title('linear model');

%logistic model
A = [income, jackpot, pop];
b = -log(1./sales*L - 1);
c_logistic = A\b;
projected_logistic = L./(exp(-A*c_logistic)+1);
r = sales - projected_logistic;
RMSE_logistic = sqrt((r'*r)/size(b,1));

graph_scatter(income, jackpot, pop, sales, projected_logistic, 'income', 'jackpot', 'population');
title('logistic model');

%tanh model
A = [income, jackpot, pop, ones(size(D,1),1)];
b = atanh(sales./L);
c_tanh = A\b;
projected_tanh = L*tanh(A*c_tanh);
r = sales - projected_tanh;
RMSE_tanh = sqrt((r'*r)/size(b,1));

graph_scatter(income, jackpot, pop, sales, projected_tanh, 'income', 'jackpot', 'population');
title('tanh model');

