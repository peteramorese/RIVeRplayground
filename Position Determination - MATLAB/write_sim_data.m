function [] = write_sim_data(filename, Y)

for i = 1:length(Y)
        
    for j = 1:size(Y{i}, 2)

        dlmwrite(filename, Y{i}(:, j)', '-append');

    end

end

end

