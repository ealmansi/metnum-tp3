function Y = images_2_rows(X)

	Y = zeros(size(X,3),28*28);
	for	ii = 1:size(X,3)
		Y(ii,:) = reshape(X(:,:,ii), 1, 28*28);
	end
end