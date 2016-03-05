% coords.m
close all;
clear all;

mcleod = [
-123.249461, 49.261442;		% 270,420
-123.248812, 49.261701;		% 400,420
-123.248938, 49.261859;		% 400,400
-123.249367, 49.261710;		% 270,400
-123.249686, 49.262083;		% 270,250
-123.249899, 49.262008];	% 250,250

cheese = [
-123.249475, 49.262268;		% 300,200
-123.249218, 49.262354;		% 375,200
-123.249006, 49.262084;		% 375,250
-123.249272, 49.261996];	% 300,250


% image extents
extents = [
-123.250812, 49.262161;		% (0,0)
-123.248334, 49.263114;		% (800,0)
-123.247403, 49.261765;		% (800,480)
-123.249801, 49.260998];	% (0,480)


CEME = [
-123.249667, 49.262356;		% 350,75
-123.249788, 49.262519;		% 350,0
-123.248495, 49.263007;		% 700,0
-123.248085, 49.262515;		% 700,200
-123.248353, 49.262454;		% 600,200
-123.248533, 49.262661;		% 600,75
-123.248898, 49.262552;		% 500,75
-123.248590, 49.262144;		% 500,300
-123.248858, 49.262020;		% 400,300
-123.249255, 49.262473];	% 400,75

BC = [
270, - 420;
400, - 420;
400, - 400;
270, - 400;
270, - 250;
250, - 250;
300, - 200;
375, - 200;
375, - 250;
300, - 250;
0,	 -  0;
800, -  0;
800, -  480;
0,	 -  480;
350, - 75;
350, - 0;
700, - 0;
700, - 200;
600, - 200;
600, - 75;
500, - 75;
500, - 300;
400, - 300;
400, - 75];

% Concatenate all sampled points
all_points = [mcleod; cheese; extents; CEME];

T = delaunay(all_points(:,1),all_points(:,2));
TRI = delaunayTriangulation(extents);

test_ = [ -123.249718, 49.261797 ];



figure(7); % GPS Coordinate Space Simple Test

hold on;
ct2 = tri_centroid(extents,TRI);
plot(ct2(:,1),ct2(:,2),'r*')
triplot(TRI);



a = figure(8); % GPS Coordinate Space Full Mesh
hold on;
TRI_ALL = delaunayTriangulation(all_points);
ct = tri_centroid(all_points,T);
[cdist,nn] = nearestneighbour(test_,ct);
nn % index to nearest triangle
B = cartesianToBarycentric(TRI_ALL,nn,test_)

triplot(TRI_ALL);
plot(ct(:,1),ct(:,2),'r*')
plot(test_(:,1),test_(:,2),'g*')
title('GPS Coordinate Space')
xlabel('Longitude')
ylabel('Latitude')



b = figure(9); % Screen Space
hold on;
TRI_XY = delaunayTriangulation(BC);
triplot(TRI_XY);
plot(252.163636, -330.181818,'g*')
title('Screen Space')
xlabel('x')
ylabel('y')



set(a, 'PaperPosition', [0 0 5 5]); 
set(a, 'PaperSize', [5 5]);
saveas(a,'GPS_full','pdf')

set(b, 'PaperPosition', [0 0 8 4.5]); 
set(b, 'PaperSize', [8 4.5]);
saveas(b,'screen_full','pdf')




