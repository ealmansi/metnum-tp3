
plotX = cellfun(@(x) x{1}, tiemposMetodos);
plotY = cellfun(@(x) x{2}, tiemposMetodos);

figure
bar(plotY);

set(gca, 'XTickLabel', plotX)

description = ['Tiempo de ejecucion para cada delta'];
title(description);
xlabel('Delta');
ylabel('Tiempo (seg)');

export_fig(['../tex/imagenes/tiempos.pdf']);