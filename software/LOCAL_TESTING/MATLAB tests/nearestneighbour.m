% nearestneighbour.m
function [centr_dists,nn] = nearestneighbour(q,C)
	len = size(C,1);
	centr_dists = sqrt(sum((repmat(q,len,1)-C).^2,2));
	% centr_dists = (sum((repmat(q,len,1)-C).^2,2));
	[mini,nn] = min(centr_dists);
end
