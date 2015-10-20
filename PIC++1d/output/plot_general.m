clear;
load general.dat;
N1=1;
N2=size(general,1);

Nsaturation = N2;
%Nsaturation = 140;
linearField(1:N2) = 0;
gamma = 0.2*2*3.14;

linearField(1) = general(Nsaturation, 12)/exp(gamma*general(Nsaturation,2));

for i = 2:Nsaturation,
    linearField(i) = linearField(1)*exp(gamma*general(i,2));
end
for i = Nsaturation+1:N2,
    linearField(i) = linearField(Nsaturation);
end


figure(1);
plot (general(1:N2,2), general(1:N2,4), 'green', general(1:N2,2), general(1:N2,5), 'blue', general(1:N2, 2), general(1:N2,6), 'black', general(1:N2, 2), general(1:N2,7), 'red');
title ('energy');
xlabel ('{{t \omega_p}/{2\pi}}');
ylabel ('E erg');
legend('particle', 'electric','magnetic', 'full','Location','northeast');
grid ;

figure(2);
plot (general(1:N2,2), general(1:N2,8), 'red', general(1:N2,2), general(1:N2,9), 'green', general(1:N2, 2), general(1:N2,10), 'blue');
title ('momentum');
xlabel ('{{t w_p}/{2\pi}}');
ylabel ('P g*cm/s');
legend('x', 'y','z','Location','northeast');
grid ;

figure(3);
%plot (general(1:N2,2), general(1:N2,11), 'red', general(1:N2,2), general(1:N2,12));
plot (general(1:N2,2), general(1:N2,11), 'red', general(1:N2,2), general(1:N2,12), 'green', general(1:N2,2), linearField(1:N2),'blue');
title ('max field');
xlabel ('{{t \omega_p}/{2\pi}}');
ylabel ('gauss');
%legend('electric field', 'magnetic field','Location','southeast');
legend('electric field', 'magnetic field', 'magnetic field with linear increment {{u}/{c} {\omega_p}}','Location','northwest');
grid ;

figure(2);
plot (general(1:N2,2), general(1:N2,13), 'red');
title ('dt');
xlabel ('{{t \omega_p}/{2\pi}}');
ylabel ('dt');
grid ;