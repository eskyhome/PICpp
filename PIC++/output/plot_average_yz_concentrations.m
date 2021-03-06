clear;
load concentrations.dat;
load particleTypes.dat;
load Xfile.dat;
load Yfile.dat;
load Zfile.dat;
load initialParameters.dat;

Nx = size(Xfile, 1)-1;
Ny = size(Yfile, 1)-1;
Nz = size(Zfile, 1)-1;

N = Nx*Ny*Nz;

Nt = size(concentrations, 1)/N;
%Nt = 23;
Ntypes = size(particleTypes, 1);

a = 0;
b = fix(Nt/2);
c = Nt-1;

charge_density(1:Nx, 1:3) = 0;
charge_density_hat(1:Nx, 1:3) = 0;
particle_concentrations(1:Nx, 1:3*Ntypes) = 0;

middleX(1:Nx) = 0;
cv = initialParameters(10);
omega = initialParameters(21);
omegaElectron = initialParameters(20);


for i=1:Nx,
   middleX(i) = (0.5*(Xfile(i) + Xfile(i+1)) - Xfile(2))*omegaElectron/cv;
   %middleX(i) = (0.5*(Xfile(i) + Xfile(i+1)) - Xfile(2));
   for t = 1:Ntypes,
        particle_concentrations(i, 1 + 3*(t-1)) = 0;
        particle_concentrations(i, 2 + 3*(t-1)) = 0;
        particle_concentrations(i, 3 + 3*(t-1)) = 0;
   end;
   charge_density(i, 1) = 0;
   charge_density(i, 2) = 0;
   charge_density(i, 3) = 0;
   charge_density_hat(i, 1) = 0;
   charge_density_hat(i, 2) = 0;
   charge_density_hat(i, 3) = 0;
   for j=1:Ny - 1,
       for k=1:Nz - 1,
           for t = 1:Ntypes,
               particle_concentrations(i, 1 + 3*(t-1)) = particle_concentrations(i, 1 + 3*(t-1)) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + a*N, 2 + t);
               particle_concentrations(i, 2 + 3*(t-1)) = particle_concentrations(i, 2 + 3*(t-1)) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + b*N, 2 + t);
               particle_concentrations(i, 3 + 3*(t-1)) = particle_concentrations(i, 3 + 3*(t-1)) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + c*N, 2 + t);
           end;
           charge_density(i, 1) = charge_density(i, 1) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + a*N, 1);
           charge_density(i, 2) = charge_density(i, 2) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + b*N, 1);
           charge_density(i, 3) = charge_density(i, 3) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + c*N, 1);
   
           charge_density_hat(i, 1) = charge_density_hat(i, 1) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + a*N, 2);
           charge_density_hat(i, 2) = charge_density_hat(i, 2) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + b*N, 2);
           charge_density_hat(i, 3) = charge_density_hat(i, 3) + concentrations(Nz*Ny*(i-1) + Nz*(j-1) + k + c*N, 2);
       end;
   end;
   
   for t = 1:Ntypes,
        particle_concentrations(i, 1 + 3*(t-1)) = particle_concentrations(i, 1 + 3*(t-1))/((Ny-1)*(Nz-1));
        particle_concentrations(i, 2 + 3*(t-1)) = particle_concentrations(i, 2 + 3*(t-1))/((Ny-1)*(Nz-1));
        particle_concentrations(i, 3 + 3*(t-1)) = particle_concentrations(i, 3 + 3*(t-1))/((Ny-1)*(Nz-1));
   end;
   charge_density(i, 1) = charge_density(i, 1)/((Ny-1)*(Nz-1));
   charge_density(i, 2) = charge_density(i, 2)/((Ny-1)*(Nz-1));
   charge_density(i, 3) = charge_density(i, 3)/((Ny-1)*(Nz-1));
   charge_density_hat(i, 1) = charge_density_hat(i, 1)/((Ny-1)*(Nz-1));
   charge_density_hat(i, 2) = charge_density_hat(i, 2)/((Ny-1)*(Nz-1));
   charge_density_hat(i, 3) = charge_density_hat(i, 3)/((Ny-1)*(Nz-1));
end;

for t = 1:Ntypes,
    if(particleTypes(t) > 0)
    figure(t);
    plot (middleX(1:Nx),particle_concentrations(1:Nx, 1 + 3*(t-1)), 'red', middleX(1:Nx), particle_concentrations(1:Nx, 2 + 3*(t-1)), 'green', middleX(1:Nx), particle_concentrations(1:Nx, 3 + 3*(t-1)), 'blue');
    title ('concentration');
    xlabel ('x/r_g');
    ylabel ('n cm^-3');
    grid ;    
    end;
end;

figure(Ntypes + 1);
plot (middleX(1:Nx),charge_density(1:Nx, 1), 'red', middleX(1:Nx), charge_density(1:Nx, 2), 'green', middleX(1:Nx), charge_density(1:Nx, 3), 'blue');
title ('charge density');
xlabel ('x/r_g');
ylabel ('rho cgs*cm^-3');
grid ;

figure(Ntypes + 2);
plot (middleX(1:Nx),charge_density_hat(1:Nx, 1), 'red', middleX(1:Nx), charge_density_hat(1:Nx, 2), 'green', middleX(1:Nx), charge_density_hat(1:Nx, 3), 'blue');
title ('charge density hat');
xlabel ('x/r_g');
ylabel ('rho cgs*cm^-3');
grid ;