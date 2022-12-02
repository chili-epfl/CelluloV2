function cst = cellulo_constants()

cst.d = 0.028;       % 28 mm
cst.r_mag = 0.005;   % 5 mm
cst.h_mag = 0.005;   % 5 mm
cst.r_steel = 0.007; % 7 mm
cst.r_wheel = 0.008; % 8 mm

cst.rho_mag = 7.4*0.001/(0.01^3);    % 7.4 g/cm^3
cst.rho_NBR = 1.3*0.001/(0.01^3);    % 1.3 g/cm^3 (rubber)
cst.rho_steel = 7.87*0.001/(0.01^3); % 7.87 g/cm^3

cst.m = 0.2;                                                  % 200 gr
cst.m_mag = cst.rho_mag*pi*cst.r_mag^2*cst.h_mag;             % 2.9 gr
cst.m_NBR = cst.rho_NBR*4/3*pi*(cst.r_wheel^3-cst.r_steel^3); % 0.92 gr
cst.m_steel = cst.rho_steel*4/3*pi*cst.r_steel^3;             % 11.3 gr
cst.Iz = 0.5*cst.m*0.03^2; % Approx as a cylinder of r=3cm    % 900 g cm^2

g = 9.807;
cst.c_omega = 60*0.001*g/2/pi/150; % 6.24e-4 Nm*s/rad
cst.c_U = 420*0.001*g/6/150;       % 0.0046 Nm/V
cst.c_v = cst.c_omega/cst.r_mag;   % 0.1249 Nm*s/m = Ns

cst.c_F = 1/128;    % 0.0078 Nm/N
cst.c = 0.0441/128; % 3.445e-4 Nm

I_steel = 2/5*cst.m_steel*cst.r_steel^2;
I_NBR = 2/5*cst.m_NBR*(cst.r_wheel^2-cst.r_steel^2);
cst.I_wheel = I_steel + I_NBR;         % 2.27 g cm^2
cst.I_drive = 1/2*cst.m_mag*cst.r_mag^2; % 0.363 g cm^2

end

