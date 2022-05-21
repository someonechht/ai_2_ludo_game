clc; clear; close all;

files = ["build/evolution_data_var_1.csv", ...
    "build/evolution_data_var_2.csv", ...
    "build/evolution_data_var_3.csv", ...
    "build/evolution_data_var_4.csv", ...
    "build/evolution_data_var_5.csv"];
tit = ["\sigma = 1", "\sigma = 2", "\sigma = 3", "\sigma = 4", "\sigma = 5"];
i = 1;

avg_lst = [];
for file = files
    % read data
    data = readmatrix(file);
    generation = data(:,1);
    best = data(:,2);
    average = data(:,3);
    worst = data(:,4);
    
    avg_lst = [avg_lst, average];

    % plot data
    f = figure(i);
    f.Position = [200, 200, 200, 200];

    hold on
    plot(generation, best)
    plot(generation, average)
    plot(generation, worst)
    axis([0, 500, 0.15, 0.45])
    title(tit(i))
%     legend(["Best", "Average", "Worst"],'Location','northwest')
    xlabel("Iteration")
    ylabel("Win-rate")
    hold off
    
    % save plot
    file_name = erase(file,["build/", ".csv"]);
    file_name = strcat(file_name, '.pdf');
    saveas(gcf,file_name);
    system(strcat("pdfcrop ", file_name, " ", file_name));
    i = i + 1;
end
%%
files = ["build/evolution_data_var_1.csv", ...
    "build/evolution_data_var_2.csv", ...
    "build/evolution_data_var_3.csv", ...
    "build/evolution_data_var_4.csv", ...
    "build/evolution_data_var_5.csv"];

avg_lst = [];
for file = files
    % read data
    data = readmatrix(file);
    generation = data(:,1);
    best = data(:,2);
    average = data(:,3);
    worst = data(:,4);
    
    avg_lst = [avg_lst, average];
end
% get average from all
figure(i)
hold on
plot(avg_lst)
plot(ones(1,length(avg_lst))*mean(mean(avg_lst)),'linewidth',2)
hold off
axis([0, 500, 0.28, 0.35])
title("Average winrate with different \sigma")
legend({'\sigma = 1', '\sigma = 2', '\sigma = 3', '\sigma = 4', '\sigma = 5'},'Location','northwest')
xlabel("Iteration")
ylabel("Win-rate")
% save plot
file_name = "averages.pdf";
saveas(gcf,file_name);
system(strcat("pdfcrop ", file_name, " ", file_name));
%%
clc;clear;close all;
files = ["build/evolution_data_var_1.csv", ...
    "build/evolution_data_var_2.csv", ...
    "build/evolution_data_var_3.csv", ...
    "build/evolution_data_var_4.csv", ...
    "build/evolution_data_var_5.csv"];
avg_lst = [];
% X = randn(1, 1000);
% histogram(X)
i = 1;

for file = files
    % read data
    data = readmatrix(file);
    generation = data(:,1);
    best = data(:,2);
    average = data(:,3);
    worst = data(:,4);
    norm_avg = normalize(average);
    h = kstest(norm_avg);
    
    f = figure(i);
    f.Position = [200, 200, 180, 180];
    cdfplot(norm_avg)
    hold on
    x_values = linspace(min(norm_avg),max(norm_avg));
    plot(x_values,normcdf(x_values,0,1),'r-')
    title(strcat("\sigma=", string(i), " kstest"))
    hold off
    file_name = strcat("sigma_", string(i), "_kstest", '.pdf');
    saveas(gcf,file_name);
    system(strcat("pdfcrop ", file_name, " ", file_name));
    avg_lst = [avg_lst, average];
    v = var(average)
    i = i+1
end
vartest2(avg_lst(:,3), avg_lst(:,4))
close all;
avg_lst = avg_lst(:,1:4);
[p, t, stats] = anova1(avg_lst);
multcompare(stats);


% figure(6)
% boxplot(avg_lst,'Labels',{'sigma = 1','sigma = 2','sigma = 3','sigma = 4','sigma = 5'})
% title("Boxplot of systems with different standard variances")
% file_name = strcat("boxplot", '.pdf');
% saveas(gcf,file_name);
% system(strcat("pdfcrop ", file_name, " ", file_name));


