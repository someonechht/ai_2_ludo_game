clc; clear; close all;
file = 'build/evolution_data.csv';
opt = detectImportOptions(file);
data = readmatrix(file, opt);
generation = data(:,1);
best = data(:,2);
average = data(:,3);
worst = data(:,4);


hold on
plot(generation, best)
plot(generation, average)
plot(generation, worst)
hold off
