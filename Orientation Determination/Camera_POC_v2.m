clear; close all; clc;
%% Define Camera Parameters
% FOV
Ncam = 3;
tol = .05;
zfovV = 40*ones(1,Ncam); % degrees (vertical)
yfovV = 60*ones(1,Ncam); % degrees (horizontal)
zpixV = 8*ones(1,Ncam); % resolution in pixels (vertical)
ypixV = 12*ones(1,Ncam); % resolution in pixels (horizontal)
griddisc = 2; % grid spacing on 2D plot
rollV = [0 0 0];
pitchV = [0 -15 15];
yawV = [0 -45 45];
rV(:,1) = [-8; 0; 0];
rV(:,2) = [-5; -10; 4];
rV(:,3) = [-5; 10; -4];
% rV = [rV [0; -4; 4]];

%% Create the Cameras
veye = eye(3); 
rollV = deg2rad(rollV);
pitchV = deg2rad(pitchV);
yawV = deg2rad(yawV);
zfovV = deg2rad(zfovV);
yfovV = deg2rad(yfovV);
Rx = @(t) [1 0 0; 0 cos(t) -sin(t); 0 sin(t) cos(t)]';
Ry = @(t) [cos(t) 0 sin(t); 0 1 0; -sin(t) 0 cos(t)]';
Rz = @(t) [cos(t) -sin(t) 0; sin(t) cos(t) 0; 0 0 1]';
for i = 1:Ncam
    vcV(:,:,i) = Rz(yawV(i))*Ry(pitchV(i))*Rx(rollV(i))*veye;
end



%% Define Workspace

colorMap = ['r','g','b','y','c','m'];
RGBcolorMap = [1 0 0; 0 1 0; 0 0 1; 1 1 0; 0 1 1; 1 0 1];

% Define bag:
width = 2*2;
height = 2*.8;
depth = 2*1.2;
rbag = [5, 0, 0];
rollbag = -5;
pitchbag = 0;
yawbag = 5;


bag(1,:) = [depth/2, 0, 0, 1];
bag(2,:) = [0, width/2, 0, 2];
bag(3,:) = [0, 0, height/2, 3];
bag(4,:) = [-depth/2, 0, 0, 4];
bag(5,:) = [0, -width/2, 0, 5];
bag(6,:) = [0, 0, -height/2, 6];
% bag(1,:) = [depth/2, 0, 0, 4];
% bag(2,:) = [0, width/2, 0, 2];
% bag(3,:) = [0, 0, height/2, 1];
% bag(4,:) = [-depth/2, 0, 0, 4];
% bag(5,:) = [0, -width/2, 0, 2];
% bag(6,:) = [0, 0, -height/2, 1];
% bag(7,:) = [depth/2, width/2, height/2, 3];
% bag(8,:) = [depth/2, width/2, -height/2, 3];
% bag(9,:) = [depth/2, -width/2, height/2, 3];
% bag(10,:) = [depth/2, -width/2, -height/2, 3];
% bag(11,:) = [-depth/2, width/2, height/2, 3];
% bag(12,:) = [-depth/2, width/2, -height/2, 3];
% bag(13,:) = [-depth/2, -width/2, height/2, 3];
% bag(14,:) = [-depth/2, -width/2, -height/2, 3];

% Bag rotation and translation

for i = 1:length(bag(:,1))
    Markers(i,1:3) = Rz(yawbag)*Ry(pitchbag)*Rx(rollbag)*bag(i,1:3)';
    Markers(i,4) = bag(i,4);
end
rbag = ones(length(Markers),1)*[rbag 0];
Markers = Markers + rbag;


bounds = [-10 10 -10 10 -10 10];

%% 3D --> 2D
% pause
figure(1)
% figure('units','normalized','outerposition',[0 0 1 1])
clf('reset')
ax1 = subplot(3,1,1);

hold on
axis(bounds)
grid on
title('3D Workspace DEFINED')
for i = 1:length(Markers(:,1))
    cell = Markers(i,:);
    x = cell(1);
    y = cell(2);
    z = cell(3);
    color = colorMap(cell(4));
    scatter3(x,y,z,30,color,'filled');
    
end
ax1.Color = .8*[1 1 1];
for ic = 1:Ncam
    vc = vcV(:,:,ic);
    r = rV(:,ic);
    zfov = zfovV(ic);
    yfov = yfovV(ic);
    zpix = zpixV(ic);
    ypix = ypixV(ic);
    figure(1)
    quiver3(r(1),r(2),r(3),vc(1,1),vc(2,1),vc(3,1),2,'r') % vector representing the camera
    quiver3(r(1),r(2),r(3),vc(1,2),vc(2,2),vc(3,2),2,'g') % vector representing the camera
    view(-60,10)
    xlabel('x')
    ylabel('y')
    zlabel('z')
    
    % Determine the 2D views
    figure(2)
    coords = display2D(r, Markers, vc, zfov, yfov, zpix, ypix);
    coords_2Ddata{ic} = coords;
    handle2d = subplot(Ncam,1,ic);
    handle2d.XTick = 0:griddisc:ypix;
    handle2d.YTick = 0:griddisc:zpix;
    handle2d.Color = .8*[1 1 1];
    hold on
    if ~isempty(coords)
        for i = 1:length(coords(1,:))
            scatter(ypix-coords(1,i), coords(2,i), colorMap(coords(3,i)),'filled')
        end
    end
    axis([0 ypix 0 zpix])
    grid on
    s = sprintf('2D Display for Camera %d',ic);
    title(s)
end


%% 2D --> 3D
coords_3Ddata = triangulate(coords_2Ddata,vcV,rV,yfovV,zfovV,ypixV,zpixV,Ncam,tol);
figure(1)
ax2 = subplot(3,1,2);
ax2.Color = .8*[1 1 1];
hold on
grid on
view(-60,10)
axis(bounds)
title('3D Workspace DERIVED')
if ~isempty(coords_3Ddata)
for i = 1:length(coords_3Ddata(1,:))
    scatter3(coords_3Ddata(1,i), coords_3Ddata(2,i), coords_3Ddata(3,i),30, colorMap(coords_3Ddata(4,i)),'filled')
end
end
% end
%% Accuracy Analysis
if ~isempty(coords_3Ddata)
dist = [];
for i = 1:length(coords_3Ddata(1,:))
    pti = coords_3Ddata(1:3,i);
    %     dist = zeros(1,length(Markers(:,1)));
    for ii = 1:length(Markers(:,1))
        ptj = Markers(ii,1:3)';
        distT = norm(pti-ptj);
        if distT<tol
            dist = [dist distT];
        end
    end
end
fprintf('Average distance error: %.3f\nNumber of points: %d\n',mean(dist), length(dist))
else
fprintf('Average distance error: N/A\nNumber of points: 0\n')
end


%% Orientation Determination
Nsamples = 30;
alpha = .04;
debug = false;
% for i = 1:6
%     alpha = i*.04;
tic
Q = determineOrientation(bag, coords_3Ddata,Nsamples,alpha,debug,RGBcolorMap);
toc
% end
% figure(98)
% legend('\alpha=.04','\alpha=.08','\alpha=.12','\alpha=.16','\alpha=.20','\alpha=.24')
for i = 1:length(bag(:,1))
    bagel(i,1:3) = Q*bag(i,1:3)';
    bagel(i,4) = bag(i,4);
end

bagel = bagel + rbag;
figure(1)
ax3 = subplot(3,1,3);
ax3.Color = .8*[1 1 1];
hold on
grid on
view(-60,10)
axis(bounds)
title('Orientation Result')
for i = 1:length(bagel(:,1))
    scatter3(bagel(i,1),bagel(i,2),bagel(i,3),colorMap(bagel(i,4)),'filled')
end


%% Functions

function coords_3Ddata = triangulate(coords_2Ddata,vcV,rV,yfovV,zfovV,ypixV,zpixV,Ncam,tol)
if Ncam<2
    error('Number of Cameras must be greater than one')
end
% figure(99)
% grid on
% hold on
% view(-60,10)
% bounds = [-5 10 -5 10 -5 10];
% axis(bounds)

for i = 1:Ncam
    yfov = yfovV(i);
    zfov = zfovV(i);
    ypix = ypixV(i);
    zpix = zpixV(i);
    vc = vcV(:,:,i);
    h = 2*tan(zfov/2);
    w = 2*tan(yfov/2);
    coords = coords_2Ddata{i};
    if ~isempty(coords)
        vcell = zeros(3,length(coords(1,:)));
        colorIDcell = zeros(1,length(coords(1,:)));
        for ii = 1:length(coords(1,:))
            y = -w/2 + w/ypix*(coords(1,ii));
            z = -h/2 + h/zpix*coords(2,ii);
            v = 1*vc(:,1) + y*vc(:,2) + z*vc(:,3);
            vcell(:,ii) = v/norm(v);
            colorIDcell(ii) = coords(3,ii);
        end
        linecell{1,i} = vcell;
        linecell{2,i} = rV(:,i);
        linecell{3,i} = colorIDcell;
    else
        linecell{1,i} = [];
        linecell{2,i} = rV(:,i);
        linecell{3,i} = [];
    end
    %     for ii = 1:length(colorIDcell)
    %         quiver3(rV(1,i),rV(2,i),rV(3,i),vcell(1,ii),vcell(2,ii),vcell(3,ii),10);
    %     end
end

location = [];
b = 0;
for i = 1:(Ncam-1)
    vmat = linecell{1,i};
    r1 = linecell{2,i};
    colorID = linecell{3,i};
    for ii = 1:length(colorID)
        matchcolor = colorID(ii);
        v1 = vmat(:,ii);
        for iii = (i+1):Ncam % matching camera
            matchcolorID = linecell{3,iii};
            r2 = linecell{2,iii};
            matchvmat = linecell{1,iii};
            a = 0;
            meanptvec = [];
            Dvec = [];
            for iv = 1:length(matchcolorID)
                if matchcolorID(iv)==matchcolor
                    v2 = matchvmat(:,iv);
                    [meanpt, D] = lineIntersection3D(r1,r2,v1,v2);
                    a = a+1;
                    meanptvec(:,a) = meanpt;
                    Dvec(a) = D;
                end
            end
            [minD, i_D] = min(Dvec);
%                         disp(minD)
            if (~isempty(meanptvec))&&(minD<tol)
                b = b+1;
                location(:,b) = meanptvec(:,i_D);
                colorIDout(b) = matchcolor;
            end
        end
    end
end
if ~isempty(location)
    coords_3Ddata = [location; colorIDout];
else
    coords_3Ddata = [];
end
end

function [meanpt, D] = lineIntersection3D(r1,r2,v1,v2)
n = cross(v1,v2);
N1 = cross(v1,n);
N2 = cross(v2,n);
d1 = -dot(N1,r1);
d2 = -dot(N2,r2);
c1 = -(d2 + dot(N2,r1))/(dot(N2,v1));
c2 = -(d1 + dot(N1,r2))/(dot(N1,v2));
pt1 = r1 + c1*v1;
pt2 = r2 + c2*v2;
meanpt = (pt1 + pt2)/2;

D1 = -dot(n,r1);
D2 = -dot(n,r2);
D = abs(D1-D2)/norm(n); % shortest distance between the lines

end


function coords = display2D(r, Markers, vc, zfov, yfov, zpix, ypix)
coords = [];
for i = 1:length(Markers(:,1))
    a = Markers(i,1:3)';
    ra = a - r;
    lx = dot(ra,vc(:,1));
    ly = dot(ra,vc(:,2));
    lz = dot(ra,vc(:,3));
    %     boolplanet = zeros(zpix,ypix);
    if lx>0 && abs(tan(zfov/2))>abs((lz/lx)) && abs(tan(yfov/2))>abs((ly/lx))% FOV check
        pcz = lz/(lx*tan(zfov/2));
        pcy = ly/(lx*tan(yfov/2));
        z = floor(zpix/2*(pcz+1));
        y = floor(ypix/2*(pcy +1));
        %         boolplanet(z,y) = Markers(i,4);
        %         boolplane = boolplane + boolplanet;
        coords = [coords [y;z;Markers(i,4)]];
    end
end
end







