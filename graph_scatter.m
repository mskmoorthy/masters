function r = graph_scatter(x, y, z, out1, out2, x_label, y_label, z_label)
figure
scale = abs(out2 - out1);
scatter3(x, y, z, 10, scale);

xlabel(x_label);
ylabel(y_label);
zlabel(z_label);

h = colorbar;
ylabel(h,'Difference in sales');
r = 0;