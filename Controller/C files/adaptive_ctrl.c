/*
	 * Copyright (C) 2015 EPFL
	 *
	 * This program is free software: you can redistribute it and/or modify
	 * it under the terms of the GNU General Public License as published by
	 * the Free Software Foundation, either version 3 of the License, or
	 * (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program.  If not, see http://www.gnu.org/licenses/.
	 */
	
	/**
	 * @file adaptive_ctrl.c
	 * @brief Source for adaptive MRAC controller functionality
	 * @author Hadrien Sprumont
	 * @date 2021-12-10
	 */
	
	#include "adaptive_ctrl.h"
	
	#include <math.h>
	
	#define PI 3.14159265359f
	#define FL1 0.2422045193027136f
	#define FL2 0.002068074035998016f
	
	float k_pm = 20.;
	float k_rm = 20.;
	
	float k_r_x_i = 54.14;
	float k_p_x_i = 54.87;
	float th_x_i[4] = {113.71, 0.06, 0.33, -0.34};
	float gamma_r_x = 100.;
	float gamma_p_x = 100.;
	float Gamma_x[4] = {10.,1.,1.,1.};
	
	float k_r_y_i = 111.29;
	float k_p_y_i = 108.17;
	float th_y_i[4] = {111.16, 0.74, 0.65, 0.35};
	float gamma_r_y = 100.;
	float gamma_p_y = 100.;
	float Gamma_y[4] = {10.,1.,1.,1.};
	
	float k_r_th_i = 719.87;
	float k_p_th_i = 680.43;
	float th_th_i[4] = {301.45, 9.1, 6.51, -21.13};
	float gamma_r_th = 10.;
	float gamma_p_th = 10.;
	float Gamma_th[4] = {1.,1.,1.,1.};
	
    bool saturation = false;
    
	/**
	 * @brief Simple sign function util
	 */
	int sign(float val){
		if(val > 0) return 1;
		if(val < 0) return -1;
		return 0;
	}
    
	void initMRAC(MRAC_t* mrac, COORD coord, float k_p, float k_r, float th[4], float gamma_p, float gamma_r, float Gamma[4]){
	    
		mrac->coord = coord;
		
		mrac->k_pm = k_pm;
	    mrac->k_rm = k_rm;
		mrac->y_m = 0.;
	
	    mrac->k_p = k_p;
	    mrac->k_r = k_r;
		mrac->th[0] = th[0];
		mrac->th[1] = th[1];
		mrac->th[2] = th[2];
		mrac->th[3] = th[3];
		
		mrac->old_ref = 0;
		mrac->old_y = 0;
		mrac->old_phi[0] = 0;
		mrac->old_phi[1] = 0;
		mrac->old_phi[2] = 0;
		mrac->old_phi[3] = 0;
	
		mrac->gamma_p = gamma_p;
		mrac->gamma_r = gamma_r;
		mrac->Gamma_th[0] = Gamma[0];
		mrac->Gamma_th[1] = Gamma[1];
		mrac->Gamma_th[2] = Gamma[2];
		mrac->Gamma_th[3] = Gamma[3];
        
	}
	
	void initACtrl(ACtrl_t *ctrl){
		initMRAC(&ctrl->mrac_x,  X,     k_p_x_i,  k_r_x_i,  th_x_i,  gamma_p_x,  gamma_r_x,  Gamma_x);
		initMRAC(&ctrl->mrac_y,  Y,     k_p_y_i,  k_r_y_i,  th_y_i,  gamma_p_y,  gamma_r_y,  Gamma_y);
		initMRAC(&ctrl->mrac_th, THETA, k_p_th_i, k_r_th_i, th_th_i, gamma_p_th, gamma_r_th, Gamma_th);
        paramRequest =false;
	}
	
	float updateMRAC(MRAC_t* mrac, float ref, float state[6], float deltaT){
		
        if(mrac->coord == THETA){
            ref = 0.8*mrac->old_ref + 0.2*ref;
        }
        
		// Run reference model to compare outputs
		float y_m = mrac->y_m;
		y_m = y_m + deltaT * (-mrac->k_pm * y_m + mrac->k_rm * mrac->old_ref);
		mrac->y_m = y_m;
        
        // Compute error and update the parameters using old values
        // 'y' is here the output of the system, not the coordinate
        float y = state[3 + mrac->coord];
        float e = y - y_m;

        // Update only if not saturating
        if(!saturation){
            float k_p_change = mrac->gamma_p * e * mrac->old_y;
            mrac->k_p = mrac->k_p + deltaT * k_p_change;

            float k_r_change = -mrac->gamma_r * e * mrac->old_ref;
            mrac->k_r = mrac->k_r + deltaT * k_r_change;
            
            float th_change = -mrac->Gamma_th[0] * e * mrac->old_phi[0];
            mrac->th[0] = mrac->th[0] + deltaT * th_change;

            th_change = -mrac->Gamma_th[1] * e * mrac->old_phi[1];
            mrac->th[1] = mrac->th[1] + deltaT * th_change;

            th_change = -mrac->Gamma_th[2] * e * mrac->old_phi[2];
            mrac->th[2] = mrac->th[2] + deltaT * th_change;

            th_change = -mrac->Gamma_th[3] * e * mrac->old_phi[3];
            mrac->th[3] = mrac->th[3] + deltaT * th_change;
		}
        
		// Compute disturbance phi
		float phi[4] = {0, 0, 0, 0};
		float a = state[2];
		float c1 = sign(       -cosf(a) * state[3] - sinf(a) * state[4]         + 0.028 * state[5]);
		float c2 = sign(cosf(a - PI/3.) * state[3] - cosf(a + PI/6.) * state[4] + 0.028 * state[5]);
		float c3 = sign(cosf(a + PI/3.) * state[3] + sinf(a + PI/3.) * state[4] + 0.028 * state[5]);
		switch(mrac->coord){
			case X:
				phi[0] = y; 
				phi[1] = -cosf(a) * c1; 
				phi[2] = cosf(a - PI/3.) * c2;
				phi[3] = cosf(a + PI/3.) * c3;
			break;
			case Y:
				phi[0] = y;
				phi[1] = -sinf(a) * c1;
				phi[2] = -cosf(a + PI/6.) * c2;
				phi[3] = sinf(a + PI/3.) * c3;
			break;
			case THETA:
				phi[0] = y;
				phi[1] = c1;
				phi[2] = c2;
				phi[3] = c3;
			break;
		}
	
		// Compute control law
		float u = -mrac->k_p * y + mrac->k_r * ref;
		u = u + mrac->th[0]*phi[0];
		u = u + mrac->th[1]*phi[1];
		u = u + mrac->th[2]*phi[2];
		u = u + mrac->th[3]*phi[3];
	
		// Update memory
		mrac->old_ref = ref;
		mrac->old_y = y;
		mrac->old_phi[0] = phi[0];
		mrac->old_phi[1] = phi[1];
		mrac->old_phi[2] = phi[2];
		mrac->old_phi[3] = phi[3];
	
		return u;
	}
	
	void updateACtrl(ACtrl_t* ctrl, float ref[3], float state[6], float deltaT, float* u_out){
		float v[3] = {0, 0, 0};
		v[0] = updateMRAC(&ctrl->mrac_x, ref[0], state, deltaT);
		v[1] = updateMRAC(&ctrl->mrac_y, ref[1], state, deltaT);
		v[2] = updateMRAC(&ctrl->mrac_th, ref[2], state, deltaT);
	
		// Feedback linearisation
		float th = state[2];
		float u[3] = {0, 0, 0};
	
		u_out[0] =        -FL1 * cosf(th) * v[0] -         FL1 * sinf(th) * v[1] + FL2 * v[2];
		u_out[1] = FL1 * cosf(th - PI/3.) * v[0] - FL1 * cosf(th + PI/6.) * v[1] + FL2 * v[2];
		u_out[2] = FL1 * cosf(th + PI/3.) * v[0] + FL1 * sinf(th + PI/3.) * v[1] + FL2 * v[2];

        
	}
    
    void ACtrl_Set_saturation(bool is_saturating){
        saturation = is_saturating;
    }
    
    /*void ACtrl_Get_Params(ACtrl_t* ctrl, float* p_out){
        int i;
        
        p_out[0] = ctrl->mrac_x.k_p;
        p_out[1] = ctrl->mrac_x.k_r;
        for(i=0; i<4; i++){
            p_out[i+2] = ctrl->mrac_x.th[i];
        }
        
        p_out[6] = ctrl->mrac_y.k_p;
        p_out[7] = ctrl->mrac_y.k_r;
        for(i=0; i<4; i++){
            p_out[i+8] = ctrl->mrac_y.th[i];
        }
        
        p_out[12] = ctrl->mrac_th.k_p;
        p_out[13] = ctrl->mrac_th.k_r;
        for(i=0; i<4; i++){
            p_out[i+14] = ctrl->mrac_th.th[i];
        }
    }*/