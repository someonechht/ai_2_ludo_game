clc; clear; close all;

files = ["build/weights_var_1.csv", ...
    "build/weights_var_2.csv", ...
    "build/weights_var_3.csv", ...
    "build/weights_var_4.csv", ...
    "build/weights_var_5.csv"];
tit = ["\sigma = 1", "\sigma = 2", "\sigma = 3", "\sigma = 4", "\sigma = 5"];

i = 1;

for file = files
    % extract and generate data
    data = readmatrix(file);
    data = data(:, :);
    x = 1:1:size(data, 2);
    y = 1:1:size(data, 1);
    [X, Y] = meshgrid(x, y);

    % plot data
    w = waterfall(X', Y', data');
%     view(2)
    
    title(strcat("Weight change for ", tit(i)))
    xlabel("Synapse")
    ylabel("Itteration")
    zlabel("Weight")

    % save plot
    file_name = erase(file,["build/", ".csv"]);
    file_name = strcat(file_name, '.pdf');
    saveas(gcf,file_name);
    system(strcat("pdfcrop ", file_name, " ", file_name));
    i = i + 1;
    
end
