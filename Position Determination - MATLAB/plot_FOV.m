function [] = plot_FOV(s, Q, gamma, theta, vis_axes)

R1 = @(th) [1 0 0;
    0 cosd(th) sind(th);
    0 -sind(th) cosd(th)];

R2 = @(th) [cosd(th) 0 -sind(th);
    0 1 0;
    sind(th) 0 cosd(th)];

FOV_vec1 = R1(-theta) * [0; 0; 1];
FOV_vec2 = R1(theta) * [0; 0; 1];
FOV_vec3 = R2(-gamma) * [0; 0; 1];
FOV_vec4 = R2(gamma) * [0; 0; 1];

n = 2;

vec1 = [FOV_vec3(1); FOV_vec1(2); FOV_vec1(3)] .* ones(3, n);
vec2 = [FOV_vec4(1); FOV_vec1(2); FOV_vec1(3)] .* ones(3, n);
vec3 = [FOV_vec3(1); FOV_vec2(2); FOV_vec2(3)] .* ones(3, n);
vec4 = [FOV_vec4(1); FOV_vec2(2); FOV_vec2(3)] .* ones(3, n);

vec1(1, :) = vec1(1, 1) * linspace(-1, 1, n);
vec2(2, :) = vec2(2, 1) * linspace(-1, 1, n);
vec3(2, :) = vec3(2, 1) * linspace(-1, 1, n);
vec4(1, :) = vec4(1, 1) * linspace(-1, 1, n);

vec1 = Q * vec1;
vec2 = Q * vec2;
vec3 = Q * vec3;
vec4 = Q * vec4;

for i = 1:n
    if i == n
        plot_vec(s, vec1(:, i), 'k', 'FOV', vis_axes, 0);
    else
        plot_vec(s, vec1(:, i), 'k', '', vis_axes, 0);
        plot_vec(s, vec2(:, i), 'k', '', vis_axes, 0);
        plot_vec(s, vec3(:, i), 'k', '', vis_axes, 0);
        plot_vec(s, vec4(:, i), 'k', '', vis_axes, 0);
    end
end

