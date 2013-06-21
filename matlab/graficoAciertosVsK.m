X = resultados_60ktr_350ev;

figure
hold all;
xlabel('Cant. de Iteraciones (k)');
ylabel('Aciertos');

deltas = [0.5, 0.15, 0.1, 0.01, 1e-07];
labels = {'0.5', '0.15' , '0.1', '0.01' '1e-07'};

for j=1:size(deltas,2)

    j
    deltas(j)
    XF = filterByDelta(X,deltas(j));
    
    plotx = zeros(size(XF,1));
    ploty = zeros(size(XF,1));
    
    
    for i=1:size(XF,2)
        plotx(i) = XF{i}{2};
        ploty(i) = XF{i}{5};
    end
    
    
    plot(plotx, ploty);
    
    
end

    legend(labels);
    


export_fig(['../tex/imagenes/aciertos.pdf']);