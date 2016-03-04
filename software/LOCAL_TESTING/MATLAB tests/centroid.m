function c = centroid(A)
%CENTROID Summary of this function goes here
%   Detailed explanation goes here
	s = size(A); 
	len = s(1);
	c = [];
	for i = 1:s(2)
		c(i) = sum(A(:,i))/len;
	end

end

