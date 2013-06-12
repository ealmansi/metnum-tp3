hacer_cuentas = true;
hacer_tests = true;
hacer_grafico = true;

if( hacer_cuentas )
	% cargo las imágenes
	addpath MNIST_Matlab
	load MNISTenmatlab

	% defino conjuntos de entrenamiento y de prueba
	total_imgs = size(imgg,3);
	train_set = 1:5:total_imgs; train_set_size = size(train_set,2);
	test_set = 2:5:total_imgs; test_set_size = size(test_set,2);

	% obtengo los conjuntos y transformo las imagenes a filas
	train_imgs = images_2_rows(double(imgg(:,:,train_set)));
	test_imgs = images_2_rows(double(imgg(:,:,test_set)));

	% obtengo las etiquetas
	train_labels = labels(train_set);
	test_labels = labels(test_set);

	% genero la matriz X
	avg_im = mean(train_imgs,1);
	X = train_imgs - ones(train_set_size,1) * avg_im;
	X = X / sqrt(train_set_size - 1);

	% obtengo la matriz de transformación V transpuesta, y transformo las imagenes
	[~, ~, V] = svd(X);
	t_train_imgs = train_imgs*V;

	% obtengo el promedio de cada clase en el espacio transformado
	t_train_imgs_0 = t_train_imgs(train_labels == 0,:); t_avg_0 = mean(t_train_imgs_0,1);
	t_train_imgs_1 = t_train_imgs(train_labels == 1,:); t_avg_1 = mean(t_train_imgs_1,1);
	t_train_imgs_2 = t_train_imgs(train_labels == 2,:); t_avg_2 = mean(t_train_imgs_2,1);
	t_train_imgs_3 = t_train_imgs(train_labels == 3,:); t_avg_3 = mean(t_train_imgs_3,1);
	t_train_imgs_4 = t_train_imgs(train_labels == 4,:); t_avg_4 = mean(t_train_imgs_4,1);
	t_train_imgs_5 = t_train_imgs(train_labels == 5,:); t_avg_5 = mean(t_train_imgs_5,1);
	t_train_imgs_6 = t_train_imgs(train_labels == 6,:); t_avg_6 = mean(t_train_imgs_6,1);
	t_train_imgs_7 = t_train_imgs(train_labels == 7,:); t_avg_7 = mean(t_train_imgs_7,1);
	t_train_imgs_8 = t_train_imgs(train_labels == 8,:); t_avg_8 = mean(t_train_imgs_8,1);
	t_train_imgs_9 = t_train_imgs(train_labels == 9,:); t_avg_9 = mean(t_train_imgs_9,1);

end

% clasifico algunas imágenes del set de pruebas, y cuento los aciertos
if( hacer_tests )
	n = 300;
	k = 50;

	hits = 0;
	for ii=1:n

		im_index = randi(test_set_size);
		x = test_imgs(im_index, :);
		label = test_labels(im_index);

		min_dist = +Inf;
		candidate = -1;

		for d=0:9
			eval(sprintf( 't_avg_d = t_avg_%d;' ,d));

			dist = x*V - t_avg_d;
			dist = dist(1:k);
			dist = norm(dist);

			if( dist < min_dist )
				min_dist = dist;
				candidate = d;
			end
		end

		if( candidate == label )
			hits = hits + 1;
		end
	end

	disp(sprintf('\tCantidad de componentes: %d' ,k));
	disp(sprintf('\tCasos: %d' ,n));
	disp(sprintf('\tAciertos: %d' ,hits));
	disp(sprintf('\tAciertos (%%): %f\n' ,(hits/n)));

end

% un grafico parecido al de la presentación
if( hacer_grafico )
	cc = jet(10);
	figure('units','normalized','outerposition',[0 0 1 1]);hold on;
	scatter3(t_train_imgs_0(1:2:end,1),t_train_imgs_0(1:2:end,2),t_train_imgs_0(1:2:end,3),20,cc((1+0),:), 'fill');
	scatter3(t_avg_0(1), t_avg_0(2), t_avg_0(3), 100, cc((1+0),:), 'fill');
	text(t_avg_0(1), t_avg_0(2), t_avg_0(3), '0','VerticalAlignment','bottom','HorizontalAlignment','right');
	scatter3(t_train_imgs_5(1:2:end,1),t_train_imgs_5(1:2:end,2),t_train_imgs_5(1:2:end,3),20,cc((1+5),:), 'fill');
	scatter3(t_avg_5(1), t_avg_5(2), t_avg_5(3), 100, cc((1+5),:), 'fill');
	text(t_avg_5(1), t_avg_5(2), t_avg_5(3), '5','VerticalAlignment','bottom','HorizontalAlignment','right');
	scatter3(t_train_imgs_8(1:2:end,1),t_train_imgs_8(1:2:end,2),t_train_imgs_8(1:2:end,3),20,cc((1+8),:), 'fill');
	scatter3(t_avg_8(1), t_avg_8(2), t_avg_8(3), 100, cc((1+8),:), 'fill');
	text(t_avg_8(1), t_avg_8(2), t_avg_8(3), '8','VerticalAlignment','bottom','HorizontalAlignment','right');

end