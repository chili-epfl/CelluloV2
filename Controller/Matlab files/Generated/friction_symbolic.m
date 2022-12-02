function D = friction_symbolic(c,c_F,c_v,d,dth,dx,dy,th)
%FRICTION_SYMBOLIC
%    D = FRICTION_SYMBOLIC(C,c_F,C_V,d,DTH,DX,DY,TH)

%    This function was generated by the Symbolic Math Toolbox version 9.0.
%    11-Dec-2021 17:00:49

t2 = cos(th);
t3 = sin(th);
t4 = d.*dth.*2.0;
t5 = 1.0./c_F;
t7 = sqrt(3.0);
t10 = pi./3.0;
t6 = -t4;
t8 = dx.*t2.*2.0;
t9 = dy.*t3.*2.0;
t11 = t2./2.0;
t12 = t3./2.0;
t13 = t10+th;
t16 = t2.*t7.*(-1.0./2.0);
t17 = t3.*t7.*(-1.0./2.0);
t14 = t7.*t11;
t15 = t7.*t12;
t18 = t6+t8+t9;
t22 = t11+t17;
t23 = t12+t16;
t19 = sign(t18);
t20 = t11+t15;
t21 = t12+t14;
t26 = dx.*t22.*2.0;
t27 = dy.*t23.*2.0;
t24 = dx.*t20.*2.0;
t25 = dy.*t21.*2.0;
t28 = t4+t25+t26;
t29 = t4+t24+t27;
t30 = sign(t28);
t31 = sign(t29);
D = [c_v.*dx.*t5.*(3.0./2.0)+c.*t2.*t5.*t19+c.*t5.*t30.*cos(t13)+c.*t5.*t31.*cos(-t10+th);c_v.*dy.*t5.*(3.0./2.0)+c.*t3.*t5.*t19-c.*t5.*t31.*cos(th+pi./6.0)+c.*t5.*t30.*sin(t13);-c.*d.*t5.*t19+c.*d.*t5.*t30+c.*d.*t5.*t31+c_v.*d.^2.*dth.*t5.*3.0];
