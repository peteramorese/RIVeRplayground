function [] = run_NIS_test(epsilon_y, Nsim_runs, num_meas)
% Performs NEES test on KF filter outputs.
% inputs:
%       epsilon_y  - vector of NEES statistics (e_xk'*inv(Pk_plus)*e_xk)
%                    for all times k
%       Nsim_runs  - number of measurements 
%       num_meas   - number rows in H matrix 

Nny = Nsim_runs*num_meas;
epsilon_bar = mean(epsilon_y,1); % expected value
alpha = 0.05; % significance level

% compute intervals
r1y = chi2inv(alpha/2,Nny)./Nsim_runs;
r2y = chi2inv(1-alpha/2,Nny)./Nsim_runs;

figure();
plot(epsilon_bar,'ro','MarkerSize',6,'LineWidth',2);
hold on;
plot(r1y*ones(size(epsilon_bar)),'b--','LineWidth',2);
plot(r2y*ones(size(epsilon_bar)),'b--','LineWidth',2);
ylabel('NIS Statistic, $\bar{\epsilon}_y$', 'FontSize', 14, 'Interpreter', 'latex');
xlabel('Time Step, k', 'FontSize', 14);
title('NIS Estimation Results','FontSize',14);
legend('NIS @ time k', 'r_1 bound', 'r_2 bound');
end