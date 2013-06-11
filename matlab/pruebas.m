close all

cargar_fotos = false;
hacer_calculos = false;
graficar_todos = false;
correr_tests = true;

if( cargar_fotos )

	addpath MNIST_Matlab
	load MNISTenmatlab
	imgg = double(imgg);
end

if( hacer_calculos )
	
	for d=0:9
		% obtengo las imagenes del dígito d, y transformo a filas
		eval(sprintf( 'imgs_%d = imgg(:,:,labels == %d);' ,d,d));
		eval(sprintf( 'imgs_%d = images_2_rows(imgs_%d);' ,d,d));

		% obtengo la imagen promedio, y genero la matrix Xi
		eval(sprintf( 'avg_%d = mean(imgs_%d,1);' ,d,d));
		eval(sprintf( 'X_%d = (imgs_%d - ones(size(imgs_%d,1),1)*avg_%d)/sqrt(size(imgs_%d,1)-1);' ,d,d,d,d,d));

		% computo los autovectores de la matriz de covarianza (Xi' * Xi)
		eval(sprintf( '[V_d D_d] = eig(X_%d''*X_%d);' ,d,d));

		% me aseguro que todos los elementos de la diagonal sean >= 0
		for ii=1:size(V_d,2)
			if(D_d(ii,ii) < 0)
				D_d(ii,ii) = -D_d(ii,ii);
				V_d(:,ii) = -V_d(:,ii);
			end
		end

		% ordeno autovectores según orden decreciente de mangitud de los autovalores
		for ii=2:size(V_d,2)
			for jj=ii:-1:2
				if( D_d(jj,jj) > D_d(jj-1,jj-1) )
					temp = D_d(jj,jj); D_d(jj,jj) = D_d(jj-1,jj-1); D_d(jj-1,jj-1) = temp;
					temp = V_d(:,jj); V_d(:,jj) = V_d(:,jj-1); V_d(:,jj-1) = temp;
				end
			end
		end

		eval(sprintf( 'V_%d = V_d;' ,d));
		clear D_d;

		% transformo todas las imágenes del dígito d
		eval(sprintf( 't_imgs_%d = imgs_%d*V_%d; t_avg_%d = mean(t_imgs_%d,1);' ,d,d,d,d,d));
	end
end

%%% Gráfico de las primeras 3 componentes: subconjunto de imágenes + centros

if( graficar_todos )

	cc = hsv(10);
	figure('units','normalized','outerposition',[0 0 1 1]);hold on;
	for d=[0 1 2 3 4 5 6 7 8 9]
		eval(sprintf( 'scatter3(t_imgs_%d(1:20:end,1),t_imgs_%d(1:20:end,2),t_imgs_%d(1:20:end,3),5,cc((1+%d),:));' ,d,d,d,d));
		eval(sprintf( 'scatter3(t_avg_%d(1),t_avg_%d(2),t_avg_%d(3),100,cc((1+%d),:),''fill'');' ,d,d,d,d));
		eval(sprintf( 'text(t_avg_%d(1),t_avg_%d(2),t_avg_%d(3),''%d'',''VerticalAlignment'',''bottom'',''HorizontalAlignment'',''right'');' ,d,d,d,d));
	end
end

%%% Algunos tests para digitos random

if( correr_tests )

	n = 250;							% cantidad de tests
	k = 400;								% cantidad de componentes principales
	modo_debug = false;					% ante cada error, hace un gráfico como el anterior pero
										% solo con las clases de interés y el punto que pifió

	hits = 0;
	for ii=1:n

		im_indx = randi(length(labels));
		label = labels(im_indx);
		x = reshape(imgg(:,:,im_indx), 1, 28*28);

		min_dist = +Inf;
		digit = -1;
		t_x_digit = [];

		for d=0:9
			eval(sprintf( 'V_d = V_%d;' ,d));
			eval(sprintf( 't_avg_d = t_avg_%d;' ,d));

			t_x_d = x*V_d;
			dist = t_x_d - t_avg_d;
			dist = dist(1:k);
			dist = norm(dist);

			if( dist < min_dist )
				min_dist = dist;
				digit = d;
				t_x_digit = t_x_d;
			end
		end

		if( digit == label )
			hits = hits + 1;
		end

		if( modo_debug && digit ~= label )
			
			cc = jet(10);
			figure;
			subplot(1,3,[1 2]);hold on;
			for d=[label digit]
				eval(sprintf( 'scatter3(t_imgs_%d(1:5:end,1),t_imgs_%d(1:5:end,2),t_imgs_%d(1:5:end,3),5,cc((1+%d),:));' ,d,d,d,d));
				eval(sprintf( 'scatter3(t_avg_%d(1),t_avg_%d(2),t_avg_%d(3),100,cc((1+%d),:),''fill'');' ,d,d,d,d));
				eval(sprintf( 'text(t_avg_%d(1),t_avg_%d(2),t_avg_%d(3),''%d'',''VerticalAlignment'',''bottom'',''HorizontalAlignment'',''right'');' ,d,d,d,d));
			end

			scatter3(t_x_digit(1),t_x_digit(2),t_x_digit(3),100,'black','fill');
			title(sprintf( 'Etiqueta: %d, clasificado como: %d' ,label,digit));

			subplot(1,3,3);
			imshow(uint8(imgg(:,:,im_indx)));
			title(sprintf( 'Indice de la imagen: %d' ,im_indx));

			resp = input('Pasar a la proxima? (''no'' para salir)\n', 's');
			if( strcmpi(resp,'n') || strcmpi(resp,'no') )
				break;
			else
				close all;
			end
		end
	end

	if(~modo_debug)
		disp(sprintf('\tCantidad de componentes: %d' ,k));
		disp(sprintf('\tCasos: %d' ,n));
		disp(sprintf('\tAciertos: %d' ,hits));
		disp(sprintf('\tAciertos (%%): %f\n' ,(hits/n)));
	end

end