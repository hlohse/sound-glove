function magnetometer_bias()

fit_flag    = 0;
type        = '.csv';
seperator   = ' ';
output      = 'magnetometer';
output_file = [output type];
% Adjust raw values to [-1, 1] range
divisor     = 10000;
scale       = 1 / divisor;
axis_factor = 10000;

file_hand_raw_x        = ['hand_m_raw_x'        type];
file_hand_raw_y        = ['hand_m_raw_y'        type];
file_hand_raw_z        = ['hand_m_raw_z'        type];
file_hand_raw_all      = ['hand_m_raw_all'      type];
file_lower_arm_raw_x   = ['lower_arm_m_raw_x'   type];
file_lower_arm_raw_y   = ['lower_arm_m_raw_y'   type];
file_lower_arm_raw_z   = ['lower_arm_m_raw_z'   type];
file_lower_arm_raw_all = ['lower_arm_m_raw_all' type];
file_upper_arm_raw_x   = ['upper_arm_m_raw_x'   type];
file_upper_arm_raw_y   = ['upper_arm_m_raw_y'   type];
file_upper_arm_raw_z   = ['upper_arm_m_raw_z'   type];
file_upper_arm_raw_all = ['upper_arm_m_raw_all' type];

hand_raw_x        = importdata(file_hand_raw_x,        seperator);
hand_raw_y        = importdata(file_hand_raw_y,        seperator);
hand_raw_z        = importdata(file_hand_raw_z,        seperator);
hand_raw_all      = importdata(file_hand_raw_all,      seperator);
lower_arm_raw_x   = importdata(file_lower_arm_raw_x,   seperator);
lower_arm_raw_y   = importdata(file_lower_arm_raw_y,   seperator);
lower_arm_raw_z   = importdata(file_lower_arm_raw_z,   seperator);
lower_arm_raw_all = importdata(file_lower_arm_raw_all, seperator);
upper_arm_raw_x   = importdata(file_upper_arm_raw_x,   seperator);
upper_arm_raw_y   = importdata(file_upper_arm_raw_y,   seperator);
upper_arm_raw_z   = importdata(file_upper_arm_raw_z,   seperator);
upper_arm_raw_all = importdata(file_upper_arm_raw_all, seperator);
elements_axis = size(hand_raw_x,1);
elements_all = size(hand_raw_all,1);

hand_raw_x_cross = [0 0 0];
hand_raw_y_cross = [0 0 0];
hand_raw_z_cross = [0 0 0];
lower_arm_raw_x_cross = [0 0 0];
lower_arm_raw_y_cross = [0 0 0];
lower_arm_raw_z_cross = [0 0 0];
upper_arm_raw_x_cross = [0 0 0];
upper_arm_raw_y_cross = [0 0 0];
upper_arm_raw_z_cross = [0 0 0];
for i = 1:(elements_axis-1)
    hand_raw_x_cross = hand_raw_x_cross + cross(hand_raw_x(i,:), hand_raw_x(i+1,:)) / (elements_axis-1);
    hand_raw_y_cross = hand_raw_y_cross + cross(hand_raw_y(i,:), hand_raw_y(i+1,:)) / (elements_axis-1);
    hand_raw_z_cross = hand_raw_z_cross + cross(hand_raw_z(i,:), hand_raw_z(i+1,:)) / (elements_axis-1);
    lower_arm_raw_x_cross = lower_arm_raw_x_cross + cross(lower_arm_raw_x(i,:), lower_arm_raw_x(i+1,:)) / (elements_axis-1);
    lower_arm_raw_y_cross = lower_arm_raw_y_cross + cross(lower_arm_raw_y(i,:), lower_arm_raw_y(i+1,:)) / (elements_axis-1);
    lower_arm_raw_z_cross = lower_arm_raw_z_cross + cross(lower_arm_raw_z(i,:), lower_arm_raw_z(i+1,:)) / (elements_axis-1);
    upper_arm_raw_x_cross = upper_arm_raw_x_cross + cross(upper_arm_raw_x(i,:), upper_arm_raw_x(i+1,:)) / (elements_axis-1);
    upper_arm_raw_y_cross = upper_arm_raw_y_cross + cross(upper_arm_raw_y(i,:), upper_arm_raw_y(i+1,:)) / (elements_axis-1);
    upper_arm_raw_z_cross = upper_arm_raw_z_cross + cross(upper_arm_raw_z(i,:), upper_arm_raw_z(i+1,:)) / (elements_axis-1);
end
hand_raw_axis_dir_x = hand_raw_x_cross / norm(hand_raw_x_cross);
hand_raw_axis_dir_y = hand_raw_y_cross / norm(hand_raw_y_cross);
hand_raw_axis_dir_z = hand_raw_z_cross / norm(hand_raw_z_cross);
lower_arm_raw_axis_dir_x = lower_arm_raw_x_cross / norm(lower_arm_raw_x_cross);
lower_arm_raw_axis_dir_y = lower_arm_raw_y_cross / norm(lower_arm_raw_y_cross);
lower_arm_raw_axis_dir_z = lower_arm_raw_z_cross / norm(lower_arm_raw_z_cross);
upper_arm_raw_axis_dir_x = upper_arm_raw_x_cross / norm(upper_arm_raw_x_cross);
upper_arm_raw_axis_dir_y = upper_arm_raw_y_cross / norm(upper_arm_raw_y_cross);
upper_arm_raw_axis_dir_z = upper_arm_raw_z_cross / norm(upper_arm_raw_z_cross);

hand_rot = [hand_raw_axis_dir_x; hand_raw_axis_dir_y; hand_raw_axis_dir_z].';
lower_arm_rot = [lower_arm_raw_axis_dir_x; lower_arm_raw_axis_dir_y; lower_arm_raw_axis_dir_z].';
upper_arm_rot = [upper_arm_raw_axis_dir_x; upper_arm_raw_axis_dir_y; upper_arm_raw_axis_dir_z].';

hand_perpendicular_x   = [];
hand_perpendicular_y   = [];
hand_perpendicular_z   = [];
hand_perpendicular_all = [];
lower_arm_perpendicular_x   = [];
lower_arm_perpendicular_y   = [];
lower_arm_perpendicular_z   = [];
lower_arm_perpendicular_all = [];
upper_arm_perpendicular_x   = [];
upper_arm_perpendicular_y   = [];
upper_arm_perpendicular_z   = [];
upper_arm_perpendicular_all = [];
for i = 1:elements_axis
    hand_perpendicular_x = vertcat(hand_perpendicular_x, hand_raw_x(i,:) * hand_rot);
    hand_perpendicular_y = vertcat(hand_perpendicular_y, hand_raw_y(i,:) * hand_rot);
    hand_perpendicular_z = vertcat(hand_perpendicular_z, hand_raw_z(i,:) * hand_rot);
    lower_arm_perpendicular_x = vertcat(lower_arm_perpendicular_x, lower_arm_raw_x(i,:) * lower_arm_rot);
    lower_arm_perpendicular_y = vertcat(lower_arm_perpendicular_y, lower_arm_raw_y(i,:) * lower_arm_rot);
    lower_arm_perpendicular_z = vertcat(lower_arm_perpendicular_z, lower_arm_raw_z(i,:) * lower_arm_rot);
    upper_arm_perpendicular_x = vertcat(upper_arm_perpendicular_x, upper_arm_raw_x(i,:) * upper_arm_rot);
    upper_arm_perpendicular_y = vertcat(upper_arm_perpendicular_y, upper_arm_raw_y(i,:) * upper_arm_rot);
    upper_arm_perpendicular_z = vertcat(upper_arm_perpendicular_z, upper_arm_raw_z(i,:) * upper_arm_rot);
end
for i = 1:elements_all
    hand_perpendicular_all = vertcat(hand_perpendicular_all, hand_raw_all(i,:) * hand_rot);
    lower_arm_perpendicular_all = vertcat(lower_arm_perpendicular_all, lower_arm_raw_all(i,:) * lower_arm_rot);
    upper_arm_perpendicular_all = vertcat(upper_arm_perpendicular_all, upper_arm_raw_all(i,:) * upper_arm_rot);
end

hand_perpendicular_x_cross = [0 0 0];
hand_perpendicular_y_cross = [0 0 0];
hand_perpendicular_z_cross = [0 0 0];
lower_arm_perpendicular_x_cross = [0 0 0];
lower_arm_perpendicular_y_cross = [0 0 0];
lower_arm_perpendicular_z_cross = [0 0 0];
upper_arm_perpendicular_x_cross = [0 0 0];
upper_arm_perpendicular_y_cross = [0 0 0];
upper_arm_perpendicular_z_cross = [0 0 0];
for i = 1:(elements_axis-1)
    hand_perpendicular_x_cross = hand_perpendicular_x_cross + cross(hand_perpendicular_x(i,:), hand_perpendicular_x(i+1,:)) / (elements_axis-1);
    hand_perpendicular_y_cross = hand_perpendicular_y_cross + cross(hand_perpendicular_y(i,:), hand_perpendicular_y(i+1,:)) / (elements_axis-1);
    hand_perpendicular_z_cross = hand_perpendicular_z_cross + cross(hand_perpendicular_z(i,:), hand_perpendicular_z(i+1,:)) / (elements_axis-1);
    lower_arm_perpendicular_x_cross = lower_arm_perpendicular_x_cross + cross(lower_arm_perpendicular_x(i,:), lower_arm_perpendicular_x(i+1,:)) / (elements_axis-1);
    lower_arm_perpendicular_y_cross = lower_arm_perpendicular_y_cross + cross(lower_arm_perpendicular_y(i,:), lower_arm_perpendicular_y(i+1,:)) / (elements_axis-1);
    lower_arm_perpendicular_z_cross = lower_arm_perpendicular_z_cross + cross(lower_arm_perpendicular_z(i,:), lower_arm_perpendicular_z(i+1,:)) / (elements_axis-1);
    upper_arm_perpendicular_x_cross = upper_arm_perpendicular_x_cross + cross(upper_arm_perpendicular_x(i,:), upper_arm_perpendicular_x(i+1,:)) / (elements_axis-1);
    upper_arm_perpendicular_y_cross = upper_arm_perpendicular_y_cross + cross(upper_arm_perpendicular_y(i,:), upper_arm_perpendicular_y(i+1,:)) / (elements_axis-1);
    upper_arm_perpendicular_z_cross = upper_arm_perpendicular_z_cross + cross(upper_arm_perpendicular_z(i,:), upper_arm_perpendicular_z(i+1,:)) / (elements_axis-1);
end
hand_perpendicular_axis_dir_x = hand_perpendicular_x_cross / norm(hand_perpendicular_x_cross);
hand_perpendicular_axis_dir_y = hand_perpendicular_y_cross / norm(hand_perpendicular_y_cross);
hand_perpendicular_axis_dir_z = hand_perpendicular_z_cross / norm(hand_perpendicular_z_cross);
lower_arm_perpendicular_axis_dir_x = lower_arm_perpendicular_x_cross / norm(lower_arm_perpendicular_x_cross);
lower_arm_perpendicular_axis_dir_y = lower_arm_perpendicular_y_cross / norm(lower_arm_perpendicular_y_cross);
lower_arm_perpendicular_axis_dir_z = lower_arm_perpendicular_z_cross / norm(lower_arm_perpendicular_z_cross);
upper_arm_perpendicular_axis_dir_x = upper_arm_perpendicular_x_cross / norm(upper_arm_perpendicular_x_cross);
upper_arm_perpendicular_axis_dir_y = upper_arm_perpendicular_y_cross / norm(upper_arm_perpendicular_y_cross);
upper_arm_perpendicular_axis_dir_z = upper_arm_perpendicular_z_cross / norm(upper_arm_perpendicular_z_cross);

hand_m_perpendicular_all_adjusted      = hand_perpendicular_all      * scale;
lower_arm_m_perpendicular_all_adjusted = lower_arm_perpendicular_all * scale;
upper_arm_m_perpendicular_all_adjusted = upper_arm_perpendicular_all * scale;

[hand_center_fit, hand_radii_fit, hand_evecs_fit, hand_pars_fit] = ...
    ellipsoid_fit(hand_m_perpendicular_all_adjusted,      fit_flag);
[lower_arm_center_fit, lower_arm_radii_fit, lower_arm_evecs_fit, lower_arm_pars_fit] = ...
    ellipsoid_fit(lower_arm_m_perpendicular_all_adjusted, fit_flag);
[upper_arm_center_fit, upper_arm_radii_fit, upper_arm_evecs_fit, upper_arm_pars_fit] = ...
    ellipsoid_fit(upper_arm_m_perpendicular_all_adjusted, fit_flag);

hand_center = hand_center_fit.' * divisor;
lower_arm_center = lower_arm_center_fit.' * divisor;
upper_arm_center = upper_arm_center_fit.' * divisor;

hand_calibrated_x = bsxfun(@(x,y) x-y, hand_perpendicular_x, hand_center);
hand_calibrated_y = bsxfun(@(x,y) x-y, hand_perpendicular_y, hand_center);
hand_calibrated_z = bsxfun(@(x,y) x-y, hand_perpendicular_z, hand_center);
hand_calibrated_all = bsxfun(@(x,y) x-y, hand_perpendicular_all, hand_center);
lower_arm_calibrated_x = bsxfun(@(x,y) x-y, lower_arm_perpendicular_x, lower_arm_center);
lower_arm_calibrated_y = bsxfun(@(x,y) x-y, lower_arm_perpendicular_y, lower_arm_center);
lower_arm_calibrated_z = bsxfun(@(x,y) x-y, lower_arm_perpendicular_z, lower_arm_center);
lower_arm_calibrated_all = bsxfun(@(x,y) x-y, lower_arm_perpendicular_all, lower_arm_center);
upper_arm_calibrated_x = bsxfun(@(x,y) x-y, upper_arm_perpendicular_x, upper_arm_center);
upper_arm_calibrated_y = bsxfun(@(x,y) x-y, upper_arm_perpendicular_y, upper_arm_center);
upper_arm_calibrated_z = bsxfun(@(x,y) x-y, upper_arm_perpendicular_z, upper_arm_center);
upper_arm_calibrated_all = bsxfun(@(x,y) x-y, upper_arm_perpendicular_all, upper_arm_center);

hand_calibrated_x_cross = [0 0 0];
hand_calibrated_y_cross = [0 0 0];
hand_calibrated_z_cross = [0 0 0];
lower_arm_calibrated_x_cross = [0 0 0];
lower_arm_calibrated_y_cross = [0 0 0];
lower_arm_calibrated_z_cross = [0 0 0];
upper_arm_calibrated_x_cross = [0 0 0];
upper_arm_calibrated_y_cross = [0 0 0];
upper_arm_calibrated_z_cross = [0 0 0];
for i = 1:(elements_axis-1)
    hand_calibrated_x_cross = hand_calibrated_x_cross + cross(hand_calibrated_x(i,:), hand_calibrated_x(i+1,:)) / (elements_axis-1);
    hand_calibrated_y_cross = hand_calibrated_y_cross + cross(hand_calibrated_y(i,:), hand_calibrated_y(i+1,:)) / (elements_axis-1);
    hand_calibrated_z_cross = hand_calibrated_z_cross + cross(hand_calibrated_z(i,:), hand_calibrated_z(i+1,:)) / (elements_axis-1);
    lower_arm_calibrated_x_cross = lower_arm_calibrated_x_cross + cross(lower_arm_calibrated_x(i,:), lower_arm_calibrated_x(i+1,:)) / (elements_axis-1);
    lower_arm_calibrated_y_cross = lower_arm_calibrated_y_cross + cross(lower_arm_calibrated_y(i,:), lower_arm_calibrated_y(i+1,:)) / (elements_axis-1);
    lower_arm_calibrated_z_cross = lower_arm_calibrated_z_cross + cross(lower_arm_calibrated_z(i,:), lower_arm_calibrated_z(i+1,:)) / (elements_axis-1);
    upper_arm_calibrated_x_cross = upper_arm_calibrated_x_cross + cross(upper_arm_calibrated_x(i,:), upper_arm_calibrated_x(i+1,:)) / (elements_axis-1);
    upper_arm_calibrated_y_cross = upper_arm_calibrated_y_cross + cross(upper_arm_calibrated_y(i,:), upper_arm_calibrated_y(i+1,:)) / (elements_axis-1);
    upper_arm_calibrated_z_cross = upper_arm_calibrated_z_cross + cross(upper_arm_calibrated_z(i,:), upper_arm_calibrated_z(i+1,:)) / (elements_axis-1);
end
hand_calibrated_axis_dir_x = hand_calibrated_x_cross / norm(hand_calibrated_x_cross);
hand_calibrated_axis_dir_y = hand_calibrated_y_cross / norm(hand_calibrated_y_cross);
hand_calibrated_axis_dir_z = hand_calibrated_z_cross / norm(hand_calibrated_z_cross);
lower_arm_calibrated_axis_dir_x = lower_arm_calibrated_x_cross / norm(lower_arm_calibrated_x_cross);
lower_arm_calibrated_axis_dir_y = lower_arm_calibrated_y_cross / norm(lower_arm_calibrated_y_cross);
lower_arm_calibrated_axis_dir_z = lower_arm_calibrated_z_cross / norm(lower_arm_calibrated_z_cross);
upper_arm_calibrated_axis_dir_x = upper_arm_calibrated_x_cross / norm(upper_arm_calibrated_x_cross);
upper_arm_calibrated_axis_dir_y = upper_arm_calibrated_y_cross / norm(upper_arm_calibrated_y_cross);
upper_arm_calibrated_axis_dir_z = upper_arm_calibrated_z_cross / norm(upper_arm_calibrated_z_cross);

hand_raw_mean_x = mean(hand_raw_x);
hand_raw_mean_y = mean(hand_raw_y);
hand_raw_mean_z = mean(hand_raw_z);
lower_arm_raw_mean_x = mean(lower_arm_raw_x);
lower_arm_raw_mean_y = mean(lower_arm_raw_y);
lower_arm_raw_mean_z = mean(lower_arm_raw_z);
upper_arm_raw_mean_x = mean(upper_arm_raw_x);
upper_arm_raw_mean_y = mean(upper_arm_raw_y);
upper_arm_raw_mean_z = mean(upper_arm_raw_z);
hand_perpendicular_mean_x = mean(hand_perpendicular_x);
hand_perpendicular_mean_y = mean(hand_perpendicular_y);
hand_perpendicular_mean_z = mean(hand_perpendicular_z);
lower_arm_perpendicular_mean_x = mean(lower_arm_perpendicular_x);
lower_arm_perpendicular_mean_y = mean(lower_arm_perpendicular_y);
lower_arm_perpendicular_mean_z = mean(lower_arm_perpendicular_z);
upper_arm_perpendicular_mean_x = mean(upper_arm_perpendicular_x);
upper_arm_perpendicular_mean_y = mean(upper_arm_perpendicular_y);
upper_arm_perpendicular_mean_z = mean(upper_arm_perpendicular_z);
hand_calibrated_mean_x = mean(hand_calibrated_x);
hand_calibrated_mean_y = mean(hand_calibrated_y);
hand_calibrated_mean_z = mean(hand_calibrated_z);
lower_arm_calibrated_mean_x = mean(lower_arm_calibrated_x);
lower_arm_calibrated_mean_y = mean(lower_arm_calibrated_y);
lower_arm_calibrated_mean_z = mean(lower_arm_calibrated_z);
upper_arm_calibrated_mean_x = mean(upper_arm_calibrated_x);
upper_arm_calibrated_mean_y = mean(upper_arm_calibrated_y);
upper_arm_calibrated_mean_z = mean(upper_arm_calibrated_z);

s = [axis_factor axis_factor axis_factor];
hand_raw_axis_x = [hand_raw_mean_x - hand_raw_axis_dir_x .* s; hand_raw_mean_x + hand_raw_axis_dir_x .* s];
hand_raw_axis_y = [hand_raw_mean_y - hand_raw_axis_dir_y .* s; hand_raw_mean_y + hand_raw_axis_dir_y .* s];
hand_raw_axis_z = [hand_raw_mean_z - hand_raw_axis_dir_z .* s; hand_raw_mean_z + hand_raw_axis_dir_z .* s];
lower_arm_raw_axis_x = [lower_arm_raw_mean_x - lower_arm_raw_axis_dir_x .* s; lower_arm_raw_mean_x + lower_arm_raw_axis_dir_x .* s];
lower_arm_raw_axis_y = [lower_arm_raw_mean_y - lower_arm_raw_axis_dir_y .* s; lower_arm_raw_mean_y + lower_arm_raw_axis_dir_y .* s];
lower_arm_raw_axis_z = [lower_arm_raw_mean_z - lower_arm_raw_axis_dir_z .* s; lower_arm_raw_mean_z + lower_arm_raw_axis_dir_z .* s];
upper_arm_raw_axis_x = [upper_arm_raw_mean_x - upper_arm_raw_axis_dir_x .* s; upper_arm_raw_mean_x + upper_arm_raw_axis_dir_x .* s];
upper_arm_raw_axis_y = [upper_arm_raw_mean_y - upper_arm_raw_axis_dir_y .* s; upper_arm_raw_mean_y + upper_arm_raw_axis_dir_y .* s];
upper_arm_raw_axis_z = [upper_arm_raw_mean_z - upper_arm_raw_axis_dir_z .* s; upper_arm_raw_mean_z + upper_arm_raw_axis_dir_z .* s];
hand_perpendicular_axis_x = [hand_perpendicular_mean_x - hand_perpendicular_axis_dir_x .* s; hand_perpendicular_mean_x + hand_perpendicular_axis_dir_x .* s];
hand_perpendicular_axis_y = [hand_perpendicular_mean_y - hand_perpendicular_axis_dir_y .* s; hand_perpendicular_mean_y + hand_perpendicular_axis_dir_y .* s];
hand_perpendicular_axis_z = [hand_perpendicular_mean_z - hand_perpendicular_axis_dir_z .* s; hand_perpendicular_mean_z + hand_perpendicular_axis_dir_z .* s];
lower_arm_perpendicular_axis_x = [lower_arm_perpendicular_mean_x - lower_arm_perpendicular_axis_dir_x .* s; lower_arm_perpendicular_mean_x + lower_arm_perpendicular_axis_dir_x .* s];
lower_arm_perpendicular_axis_y = [lower_arm_perpendicular_mean_y - lower_arm_perpendicular_axis_dir_y .* s; lower_arm_perpendicular_mean_y + lower_arm_perpendicular_axis_dir_y .* s];
lower_arm_perpendicular_axis_z = [lower_arm_perpendicular_mean_z - lower_arm_perpendicular_axis_dir_z .* s; lower_arm_perpendicular_mean_z + lower_arm_perpendicular_axis_dir_z .* s];
upper_arm_perpendicular_axis_x = [upper_arm_perpendicular_mean_x - upper_arm_perpendicular_axis_dir_x .* s; upper_arm_perpendicular_mean_x + upper_arm_perpendicular_axis_dir_x .* s];
upper_arm_perpendicular_axis_y = [upper_arm_perpendicular_mean_y - upper_arm_perpendicular_axis_dir_y .* s; upper_arm_perpendicular_mean_y + upper_arm_perpendicular_axis_dir_y .* s];
upper_arm_perpendicular_axis_z = [upper_arm_perpendicular_mean_z - upper_arm_perpendicular_axis_dir_z .* s; upper_arm_perpendicular_mean_z + upper_arm_perpendicular_axis_dir_z .* s];
hand_calibrated_axis_x = [hand_calibrated_mean_x - hand_calibrated_axis_dir_x .* s; hand_calibrated_mean_x + hand_calibrated_axis_dir_x .* s];
hand_calibrated_axis_y = [hand_calibrated_mean_y - hand_calibrated_axis_dir_y .* s; hand_calibrated_mean_y + hand_calibrated_axis_dir_y .* s];
hand_calibrated_axis_z = [hand_calibrated_mean_z - hand_calibrated_axis_dir_z .* s; hand_calibrated_mean_z + hand_calibrated_axis_dir_z .* s];
lower_arm_calibrated_axis_x = [lower_arm_calibrated_mean_x - lower_arm_calibrated_axis_dir_x .* s; lower_arm_calibrated_mean_x + lower_arm_calibrated_axis_dir_x .* s];
lower_arm_calibrated_axis_y = [lower_arm_calibrated_mean_y - lower_arm_calibrated_axis_dir_y .* s; lower_arm_calibrated_mean_y + lower_arm_calibrated_axis_dir_y .* s];
lower_arm_calibrated_axis_z = [lower_arm_calibrated_mean_z - lower_arm_calibrated_axis_dir_z .* s; lower_arm_calibrated_mean_z + lower_arm_calibrated_axis_dir_z .* s];
upper_arm_calibrated_axis_x = [upper_arm_calibrated_mean_x - upper_arm_calibrated_axis_dir_x .* s; upper_arm_calibrated_mean_x + upper_arm_calibrated_axis_dir_x .* s];
upper_arm_calibrated_axis_y = [upper_arm_calibrated_mean_y - upper_arm_calibrated_axis_dir_y .* s; upper_arm_calibrated_mean_y + upper_arm_calibrated_axis_dir_y .* s];
upper_arm_calibrated_axis_z = [upper_arm_calibrated_mean_z - upper_arm_calibrated_axis_dir_z .* s; upper_arm_calibrated_mean_z + upper_arm_calibrated_axis_dir_z .* s];

dlmwrite(output_file, int16(hand_center), 'delimiter', seperator);
dlmwrite(output_file, int16(lower_arm_center), '-append', 'delimiter', seperator);
dlmwrite(output_file, int16(upper_arm_center), '-append', 'delimiter', seperator);
dlmwrite(output_file, hand_rot.', '-append', 'precision', '%f', 'delimiter', seperator);
dlmwrite(output_file, lower_arm_rot.', '-append', 'precision', '%f', 'delimiter', seperator);
dlmwrite(output_file, upper_arm_rot.', '-append', 'precision', '%f', 'delimiter', seperator);

dlmwrite('hand_m_perpendicular_x.csv',   hand_perpendicular_x,   'delimiter', seperator);
dlmwrite('hand_m_perpendicular_y.csv',   hand_perpendicular_y,   'delimiter', seperator);
dlmwrite('hand_m_perpendicular_z.csv',   hand_perpendicular_z,   'delimiter', seperator);
dlmwrite('hand_m_perpendicular_all.csv', hand_perpendicular_all, 'delimiter', seperator);

dlmwrite('lower_arm_m_perpendicular_x.csv',   lower_arm_perpendicular_x,   'delimiter', seperator);
dlmwrite('lower_arm_m_perpendicular_y.csv',   lower_arm_perpendicular_y,   'delimiter', seperator);
dlmwrite('lower_arm_m_perpendicular_z.csv',   lower_arm_perpendicular_z,   'delimiter', seperator);
dlmwrite('lower_arm_m_perpendicular_all.csv', lower_arm_perpendicular_all, 'delimiter', seperator);

dlmwrite('upper_arm_m_perpendicular_x.csv',   upper_arm_perpendicular_x,   'delimiter', seperator);
dlmwrite('upper_arm_m_perpendicular_y.csv',   upper_arm_perpendicular_y,   'delimiter', seperator);
dlmwrite('upper_arm_m_perpendicular_z.csv',   upper_arm_perpendicular_z,   'delimiter', seperator);
dlmwrite('upper_arm_m_perpendicular_all.csv', upper_arm_perpendicular_all, 'delimiter', seperator);

dlmwrite('hand_m_calibrated_x.csv',   hand_calibrated_x,   'delimiter', seperator);
dlmwrite('hand_m_calibrated_y.csv',   hand_calibrated_y,   'delimiter', seperator);
dlmwrite('hand_m_calibrated_z.csv',   hand_calibrated_z,   'delimiter', seperator);
dlmwrite('hand_m_calibrated_all.csv', hand_calibrated_all, 'delimiter', seperator);

dlmwrite('lower_arm_m_calibrated_x.csv',   lower_arm_calibrated_x,   'delimiter', seperator);
dlmwrite('lower_arm_m_calibrated_y.csv',   lower_arm_calibrated_y,   'delimiter', seperator);
dlmwrite('lower_arm_m_calibrated_z.csv',   lower_arm_calibrated_z,   'delimiter', seperator);
dlmwrite('lower_arm_m_calibrated_all.csv', lower_arm_calibrated_all, 'delimiter', seperator);

dlmwrite('upper_arm_m_calibrated_x.csv',   upper_arm_calibrated_x,   'delimiter', seperator);
dlmwrite('upper_arm_m_calibrated_y.csv',   upper_arm_calibrated_y,   'delimiter', seperator);
dlmwrite('upper_arm_m_calibrated_z.csv',   upper_arm_calibrated_z,   'delimiter', seperator);
dlmwrite('upper_arm_m_calibrated_all.csv', upper_arm_calibrated_all, 'delimiter', seperator);

dlmwrite('hand_m_raw_axis.csv', [hand_raw_axis_x; hand_raw_axis_y; hand_raw_axis_z], 'delimiter', seperator);
dlmwrite('lower_arm_m_raw_axis.csv', [lower_arm_raw_axis_x; lower_arm_raw_axis_y; lower_arm_raw_axis_z], 'delimiter', seperator);
dlmwrite('upper_arm_m_raw_axis.csv', [upper_arm_raw_axis_x; upper_arm_raw_axis_y; upper_arm_raw_axis_z], 'delimiter', seperator);

dlmwrite('hand_m_perpendicular_axis.csv', [hand_perpendicular_axis_x; hand_perpendicular_axis_y; hand_perpendicular_axis_z], 'delimiter', seperator);
dlmwrite('lower_arm_m_perpendicular_axis.csv', [lower_arm_perpendicular_axis_x; lower_arm_perpendicular_axis_y; lower_arm_perpendicular_axis_z], 'delimiter', seperator);
dlmwrite('upper_arm_m_perpendicular_axis.csv', [upper_arm_perpendicular_axis_x; upper_arm_perpendicular_axis_y; upper_arm_perpendicular_axis_z], 'delimiter', seperator);

dlmwrite('hand_m_calibrated_axis.csv', [hand_calibrated_axis_x; hand_calibrated_axis_y; hand_calibrated_axis_z], 'delimiter', seperator);
dlmwrite('lower_arm_m_calibrated_axis.csv', [lower_arm_calibrated_axis_x; lower_arm_calibrated_axis_y; lower_arm_calibrated_axis_z], 'delimiter', seperator);
dlmwrite('upper_arm_m_calibrated_axis.csv', [upper_arm_calibrated_axis_x; upper_arm_calibrated_axis_y; upper_arm_calibrated_axis_z], 'delimiter', seperator);