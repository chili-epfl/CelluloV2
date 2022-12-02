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
	 * @file adaptive_ctrl.h
	 * @brief Header for adaptive MRAC controller functionality
	 * @author Hadrien Sprumont
	 * @date 2021-12-10
	 */
	
	#ifndef ADAPTIVE_CTRL_H
	#define ADAPTIVE_CTRL_H
	
	#ifdef  __cplusplus
	extern "C" {
	#endif
	
    #include <stdbool.h>
        
	/**
	 * @brief Enum of possible coordinates
	 */
	typedef enum {X, Y, THETA} COORD;
	
	/**
	 * @brief Object describing one MRAC controller and its state
	 */
	typedef struct {
		COORD coord;	///< Associated coordinate (x=0,y=1,theta=2)
	
		float k_pm;		///< Reference model feedback coeff
		float k_rm;		///< Reference model feed-forward coeff
		float y_m;		///< Reference model state
	
		float k_p;		///< Adaptive feedback coeff
		float k_r;		///< Adaptive feed-forward coeff
		float th[4];	///< Adaptive disturbance coeffs
		
		float old_ref;		///< Previous reference for computing update
		float old_y;		///< Previous state value for computing update
		float old_phi[4];	///< Previous disturbance value for computing update
	
		float gamma_p;		///< Learning rate for feedback coeff
		float gamma_r;		///< Learning rate for feed-forward coeff
		float Gamma_th[4];	///< Learning rate for disturbance coeff
        
	} MRAC_t;
	
	/**
	 * @brief Object describing the adaptive controller and its states
	 */
	typedef struct {
		MRAC_t mrac_x;	///< MRAC controller for the x axis
		MRAC_t mrac_y;	///< MRAC controller for the y axis
		MRAC_t mrac_th;	///< MRAC controller for the rotation
	} ACtrl_t;
	
    bool paramRequest;
	/**
	 * @brief Initializes the MRAC controller
	 *
	 * @param mrac		MRAC controller to initialize
	 * @param coord		Coordinate associated with the specific MRAC controller
	 * @param k_pm		Reference model feedback coeff
	 * @param k_rm		Reference model feed-forward coeff
	 * @param k_p		Controller initial feedback coeff
	 * @param k_r		Controller initial feed-forward coeff
	 * @param th		Controller initial disturbance coeff
	 */
	void initMRAC(MRAC_t* mrac, COORD coord, float k_p, float k_r, float th[4], float gamma_p, float gamma_r, float Gamma[4]);
	
	/**
	 * @brief Initializes the adaptive controller
	 *
	 * @param ctrl		Controller to initialize
	 */
	void initACtrl(ACtrl_t* ctrl);
	
	/**
	 * @brief Calculates the output of the MRAC controller, performing one step
	 *
	 * @param mrac		Controller to update
	 * @param ref		Desired plant output of reference for the specific coordinate
	 * @param state		Complete measured state of the plant
	 * @param deltaT	Time length of the step
	 * @return			Control output
	 */
	float updateMRAC(MRAC_t* mrac, float ref, float state[6], float deltaT);
	
	/**
	 * @brief Calculates the output of the adaptive controller, performing one step
	 *
	 * @param ctrl		Controller to update
	 * @param ref		Desired plant output of reference for all coordinates
	 * @param state		Complete measured state of the plant
	 * @param deltaT	Time length of the step
	 * @param u_out		output array of control values
	 */
	void updateACtrl(ACtrl_t* ctrl, float ref[3], float state[6], float deltaT, float* u_out);
	
    void ACtrl_Set_saturation(bool is_saturating);
    
    void ACtrl_Get_Params(ACtrl_t* ctrl, float* p_out);
    
	#ifdef  __cplusplus
	}
	#endif
	
	#endif //ADAPTIVE_CTRL_H