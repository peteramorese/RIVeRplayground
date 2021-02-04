function [] = plot_meas_innov(t_vec, meas_innov, i, fig)

figure(fig);
fig.WindowState = 'maximized';
hold on
sgtitle('Measurement Innovations vs Time')
subplot(3, 1, 1)
hold on
grid on
ylabel('Range Innovation $\rho$', 'Interpreter', 'latex')
plot(t_vec, meas_innov(1, :))
set(gca, 'FontSize', 14)

subplot(3, 1, 2)
hold on
grid on
ylabel({'Range Rate', 'Innovation $\dot{\rho}$'}, 'Interpreter', 'latex')
plot(t_vec, meas_innov(2, :))
set(gca, 'FontSize', 14)


for j = 1:6
    if mod(j, 2) == 1
        k = 1;
    elseif mod(j, 2) == 0
        k = 2;
    end
    subplot(3, 1, k)
    hold on
    name = sprintf('MC Run %d', i);
    plot(t_vec, meas_innov(j, :), 'DisplayName', name)
end

