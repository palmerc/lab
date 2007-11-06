function draw_field()
cla;
global sensor_location;
global field_size;
global num_dimension;
global num_sensor;
global manual_deployed;
    plot(sensor_location(1:num_sensor,1),sensor_location(1:num_sensor,2),'.');
axis([0 field_size 0 field_size]);
axis square;