function [] = run_NEES_test(NEESsamps, Nsims, Nstates)
% Performs NEES test on KF filter outputs.
% inputs:
%       epsilon_x  - vector of NEES statistics (e_xk'*inv(Pk_plus)*e_xk)
%                    for all times k
%       Nsim_runs - number of truth trajectory points
%       num_vars - number of states in the system

Nnx = Nsims*Nstates;
eps_bar = mean(NEESsamps, 1); % expected value
alpha = 0.05; % significance level

% compute intervals
r1x = chi2inv(alpha/2,Nnx)./Nsims;
r2x = chi2inv(1-alpha/2,Nnx)./Nsims;

figure();
plot(eps_bar,'ro','MarkerSize',6,'LineWidth',2);
hold on;
plot(r1x*ones(size(eps_bar)),'b--','LineWidth',2);
plot(r2x*ones(size(eps_bar)),'b--','LineWidth',2);
ylabel('NEES Statistic, $\bar{\epsilon}_x$', 'FontSize', 14, 'Interpreter', 'latex');
xlabel('Time Step, k', 'FontSize', 14);
title('NEES Estimation Results','FontSize',14);
legend('NEES @ time k', 'r_1 bound', 'r_2 bound');
end