function [ out ] = filterByDelta( X, delta)
out = {};
k = 1;
for i=1:size(X) 
    
    if (X{i}{1} == delta)
        out{k} = X{i};
    k = k+1;
    end
end
