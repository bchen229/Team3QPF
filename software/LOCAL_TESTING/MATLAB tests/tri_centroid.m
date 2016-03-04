function c = tri_centroid(A,TRI)
	trilen = size(TRI,1);
	c = [];
	for i = 1:trilen
		c = [c; centroid(A(TRI(i,:),:))];
	end
	% c = ones(trilen,1);
	% c = c * centroid(TRI())

end