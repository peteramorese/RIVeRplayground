function Q = determineOrientation(bag, coords_3Ddata,Niterations,alpha,debug,RGBcolorMap)
% determineOrientation
%
%   Function that determines the closest rotation Q matrix of a bag given
%   3D data.
%
%   Inputs:
%       double bag - [n1 x 4] n1 rows for each marker on the bag, columns 1:3
%           represent the x, y, z of the vector that points from the center of
%           the bag to the marker, column 4 stores the marker identifier, a
%           unique integer for each marker color
%       double coords_3Ddata - [4 x n2] n2 columns for each data point, rows 1:3
%           represent the x, y, z of the data point position vector, row 4
%           is the marker identifier. Same form as 'bag' but transposed.
%           Don't ask me why it is transposed because I don't have a good
%           answer sry
%       int Nsamples - number of iterations run on the gradient descent.
%           Accuracy and runtime improve by increasing this parameter
%       double alpha - gradient descent scaling factor. Determines how large each
%           gradient descent step is. Increasing this might improve the
%           number of iterations it takes to converge, but might also make
%           the gradient descent unstable (may not converge nicely)
%       bool debug - boolean that turns the cool moving scatter plots on (true) or
%           off (false) to visually determine how well the grad desc is perfoming
%
%   Outputs:
%       double Q - 3D rotation matrix that rotates the bag from the
%           orientation defined in 'bag' to the orientation that closely
%           matches the data

datause = coords_3Ddata(:,1);
Ndata = 1;
% this averages the position of all points of the same marker.
% if there is only one point of the same marker (averaging has 
% been done beforehand), it should not disturb the data.
for i = 2:length(coords_3Ddata(1,:)) 
    color = coords_3Ddata(4,i);
    I = find(datause(4,:)==color,1);
    if isempty(I)
        datause = [datause coords_3Ddata(:,i)];
        Ndata = [Ndata 1];
    else
        datause(1:3,I) = datause(1:3,I) + coords_3Ddata(1:3,i);
        Ndata(I) = Ndata(I) + 1;
    end
end
for i = 1:length(Ndata)
    datause(1:3,i) = 1/Ndata(i)*datause(1:3,i);
end

Q = [];


if debug
     hello = figure(99);
     view(-60,10)
%      hello.Color = .8*[1 1 1];
     Tdata = zeros(length(datause(1,:)),Niterations);
%      pause
end
for i = 1:length(datause(1,:))
    Qi = eye(3);
    color = datause(4,i); % find the color of that data point
    rcolor = datause(1:3,i); % store the rvec to that data point
    
    % Move the data to the origin about the current marker
    colormatd = [];
    dataorigin = datause;
    colormatd(1,:) = RGBcolorMap(dataorigin(4,i),:);
    dataorigin(:,i) = [];
    dataorigin(1:3,:) = dataorigin(1:3,:) - rcolor*ones(1,length(dataorigin(1,:))); 
    colormatd = [colormatd; RGBcolorMap(dataorigin(4,:),:)];
    
    % Move the bag definition so that the current marker is at the origin
    bagorigin = bag;
    colormatb = [];
    Icurmark = find(bag(:,4)==color);
    rcolorbag = bag(Icurmark, 1:3);
    colormatb(1,:) = RGBcolorMap(bag(Icurmark,4),:);
    bagorigin(Icurmark,:) = [];
    bagorigin(:,1:3) = bagorigin(:,1:3) - ones(length(bagorigin(:,1)),1)*rcolorbag;
    colormatb = [colormatb; RGBcolorMap(bagorigin(:,4),:)];
        
    
    for ii = 1:Niterations
        T = zeros(1,3);
        for iii = 1:length(dataorigin(1,:))
            coloriii = dataorigin(4,iii);
            I = find(bagorigin(:,4)==coloriii,1);
            f = dataorigin(1:3,iii)' - bagorigin(I,1:3); % ficticious "attractive force"
            r = bagorigin(I,1:3)/norm(bagorigin(I,1:3)); % ficticious "moment arm" (unit)
            T = T + cross(r,f);
        end
        % Create the quaternion and dcm
        v = T/norm(T);
        theta = norm(T)*alpha; % scale the rotation by alpha, rotation also porportional to the mag of cross product
        R = q2R(v,theta);
        for iii = 1:length(bagorigin(:,1))
            bagorigin(iii,1:3) = R*bagorigin(iii,1:3)';
        end
        Qi = R*Qi;
        if debug
            pause(.05)
%             pause
            scatter3([0; bagorigin(:,1)],[0; bagorigin(:,2)],[0; bagorigin(:,3)],100,colormatb,'filled');
            hold on
            scatter3([0, dataorigin(1,:)],[0, dataorigin(2,:)],[0, dataorigin(3,:)],50,colormatb,'s','filled');
            hold off
            axis([-5 5 -5 5 -5 5])
            xlabel('X')
            ylabel('Y')
            zlabel('Z')
            Tdata(i,ii) = norm(T);
        end
    end
    Q = cat(3,Q,Qi);
end
Q = mean(Q,3);
if debug
    figure(98)
    hold on
    Tdata = mean(Tdata,1);
    plot(Tdata)
    xlabel('Iteration')
    ylabel('Total Discrepancy')
%     s = sprintf('Accuracy vs. Iterations for \\alpha=%.2f, N_{iterations}=%d',alpha,Niterations);
    s = sprintf('Accuracy vs. Iterations vs. \\alpha \\epsilon [.04,.24]');
    title(s)
    figure(99)
end
end