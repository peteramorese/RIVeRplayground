function [] = plot_estimate(t, ekf, ttl_str, animated, filename)
% plot_estimate
%
%   Function to plot the state estimates of marker position from the
%   Extended Kalman Filter
%
%   Inputs:
%       t - [s] Time vector
%       ekf - Structure containing the results from the EKF
%       ttl_str - Title String
%       animated - Flag to animate the plot
%
%   Outputs:
%
%   Created By: Jordan Abell
%   Created On: 20 January 2021
%   Last Modified On: 20 January 2021

k = 1; % Counter for the .gif file
fontsize = 18;

sig = zeros(3, length(t));
sig(1, :) = sqrt(ekf.P(1, 1, :));
sig(2, :) = sqrt(ekf.P(2, 2, :));
sig(3, :) = sqrt(ekf.P(3, 3, :));

fig = figure;
fig.WindowState = 'maximized';
hold on
sgtitle(sprintf('Estimation Errors for %s', ttl_str), 'FontSize', fontsize+4)

subplot(3, 1, 1)
hold on
grid on
ylabel({'Estimation Error', 'X Position [m]'})
xlim([0 t(end)])
p1 = animatedline('Color', 'k', 'LineWidth', 2, 'DisplayName', 'Estimation Error');
s1a = animatedline('Color', 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound');
s1b = animatedline('Color', 'r', 'LineWidth', 2);
legend([p1, s1a], 'location', 'NorthEast', 'FontSize', fontsize-8)
set(gca, 'FontSize', fontsize)

subplot(3, 1, 2)
hold on
grid on
ylabel({'Estimation Error','Y Position [m]'})
xlim([0 t(end)])
p2 = animatedline('Color', 'k', 'LineWidth', 2);
s2a = animatedline('Color', 'r', 'LineWidth', 2);
s2b = animatedline('Color', 'r', 'LineWidth', 2);
set(gca, 'FontSize', fontsize)

subplot(3, 1, 3)
hold on
grid on
ylabel({'Estimation Error','Z Position [m]'})
xlabel('Time [s]')
xlim([0 t(end)])
p3 = animatedline('Color', 'k', 'LineWidth', 2);
s3a = animatedline('Color', 'r', 'LineWidth', 2);
s3b = animatedline('Color', 'r', 'LineWidth', 2);
set(gca, 'FontSize', fontsize)

if animated == 1
    for i = 1:length(ekf.e_x)

        subplot(3, 1, 1)
        addpoints(p1, t(i), ekf.e_x(1, i));
        addpoints(s1a, t(i), 2*sig(1, i));
        addpoints(s1b, t(i), -2*sig(1, i));
        ylim([-10*sig(1, i) 10*sig(1, i)])


        subplot(3, 1, 2)
        addpoints(p2, t(i), ekf.e_x(2, i));
        addpoints(s2a, t(i), 2*sig(2, i));
        addpoints(s2b, t(i), -2*sig(2, i));
        ylim([-10*sig(2, i) 10*sig(2, i)])

        
        subplot(3, 1, 3)
        addpoints(p3, t(i), ekf.e_x(3, i));
        addpoints(s3a, t(i), 2*sig(3, i));
        addpoints(s3b, t(i), -2*sig(3, i));
        ylim([-10*sig(3, i) 10*sig(3, i)])
        
        if mod(i, 20) == 0
            drawnow
            % Capture the plot as an image 
            frame = getframe(fig); 
            im = frame2im(frame); 
            [imind,cm] = rgb2ind(im,256); 
            % Write to the GIF File 
            if k == 1 
                imwrite(imind,cm,filename,'gif', 'Loopcount',inf); 
                k = 0;
            else 
                imwrite(imind,cm,filename,'gif','WriteMode','append'); 
            end 
        end

    end
else
    
    subplot(3, 1, 1)
    p1 = plot(t, ekf.e_x(1, :), 'k', 'LineWidth', 2, 'DisplayName', 'Estimation Error');
    hold on
    p2 = plot(t, 2*sig(1, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound');
    plot(t, -2*sig(1, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound')
    grid on
    ylabel({'Estimation Error', 'X Position [m]'})
    ylim([-10*sig(1, end) 10*sig(1, end)])
    legend([p1, p2], 'location', 'NorthEast', 'FontSize', fontsize-8)
    set(gca, 'FontSize', fontsize)

    subplot(3, 1, 2)
    plot(t, ekf.e_x(2, :), 'k', 'LineWidth', 2, 'DisplayName', 'Estimation Error')
    hold on
    plot(t, 2*sig(2, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound')
    plot(t, -2*sig(2, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound')
    grid on
    ylabel({'Estimation Error','Y Position [m]'})
    ylim([-10*sig(2, end) 10*sig(2, end)])
    set(gca, 'FontSize', fontsize)

    subplot(3, 1, 3)
    plot(t, ekf.e_x(3, :), 'k', 'LineWidth', 2, 'DisplayName', 'Estimation Error')
    hold on
    plot(t, 2*sig(3, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound')
    plot(t, -2*sig(3, :), 'r', 'LineWidth', 2, 'DisplayName', '2\sigma Bound')
    grid on
    ylabel({'Estimation Error','Z Position [m]'})
    xlabel('Time [s]')
    ylim([-10*sig(3, end) 10*sig(3, end)])
    set(gca, 'FontSize', fontsize)
    
    saveas(fig, filename)
    
end

end
