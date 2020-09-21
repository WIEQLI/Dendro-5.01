/**
 * @file nlsmCtx.h
 * @author Milinda Fernando (milinda@cs.utah.edu)
 * @brief Nonlinear Sigma Model contex file for time stepper 
 * @version 0.1
 * @date 2020-03-12
 * 
 * School of Computing, University of Utah.
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once
#include "parameters.h"
#include "rhs.h"
#include "ctx.h"
#include "nlsmUtils.h"
#include "oct2vtk.h"
#include "checkPoint.h"
#include "mathMeshUtils.h"

namespace nlsm
{

    class NLSMCtx : public ts::Ctx<NLSMCtx,DendroScalar, DendroIntL>
    {

        
        #ifdef __PROFILE_CTX__
            public:
                using ts::Ctx<DendroScalar, DendroIntL>::m_uiCtxpt;
        #endif

        protected:
            /**@brief: evolution var (zip)*/
            DVec m_uiEVar;
            
            /**@brief: constraint var (zip)*/    
            DVec m_uiCVar;

            /**@brief: primitive var (zip)*/
            DVec m_uiPVar;

            /**@brief: Evolution var unzip 0 - unzip in , 1 - unzip out */
            DVec m_uiEUnzip[2];
            
            

        public :

            /**@brief: default constructor*/
            NLSMCtx(ot::Mesh* pMesh);

            /**@brief: default deconstructor*/
            ~NLSMCtx();

            /**@brief: initial solution*/
            int initialize();
            
            /**
             * @brief computes the BSSN rhs 
             * 
             * @param in : zipped input
             * @param out : zipped output
             * @param sz  : number of variables. 
             * @param time : current time. 
             * @return int : status. (0) on success. 
             */
            int rhs(DVec* in , DVec* out, unsigned int sz , DendroScalar time);

            /**
             * @brief compute the block for the rhs (used in ENUTS). 
             * @param in :  blk vectot in
             * @param out : blk vector out
             * @param local_blk_id : blkid
             * @param blk_time : blk time 
             * @return int 
             */
            int rhs_blk(const DendroScalar* in, DendroScalar* out, unsigned int dof, unsigned int local_blk_id, DendroScalar  blk_time) ;
            
            /**@brief: function execute before each stage
             * @param sIn: stage var in. 
            */
            int pre_stage(DVec sIn); 

            /**@brief: function execute after each stage
             * @param sIn: stage var in. 
            */
            int post_stage(DVec sIn);

            /**@brief: function execute before each step*/
            int pre_timestep(DVec sIn); 

            /**@brief: function execute after each step*/
            int post_timestep(DVec sIn);

            /**@brief: function execute after each step*/
            bool is_remesh();

            /**@brief: write to vtu. */
            int write_vtu();

            /**@brief: writes checkpoint*/
            int write_checkpt();

            /**@brief: restore from check point*/
            int restore_checkpt();

            /**@brief: should be called for free up the contex memory. */
            int finalize();

            /**@brief: pack and returns the evolution variables to one DVector*/
            DVec get_evolution_vars();

            /**@brief: pack and returns the constraint variables to one DVector*/
            DVec get_constraint_vars();

            /**@brief: pack and returns the primitive variables to one DVector*/
            DVec get_primitive_vars();

            /**@brief: updates the application variables from the variable list. */
            int update_app_vars();

            /**@brief: prints any messages to the terminal output. */
            int terminal_output();

            /**@brief: returns the async communication batch size. */
            unsigned int get_async_batch_sz() {return NLSM_ASYNC_COMM_K;}

            /**@brief: returns the number of variables considered when performing refinement*/
            unsigned int get_num_refine_vars() {return NLSM_NUM_REFINE_VARS;}

            /**@brief: return the pointer for containing evolution refinement variable ids*/
            const unsigned int* get_refine_var_ids() { return NLSM_REFINE_VARIABLE_INDICES;}
            
            /**@brief return the wavelet tolerance function / value*/
            std::function<double(double, double, double,double *)> get_wtol_function(){

                double wtol = NLSM_WAVELET_TOL;
                std::function<double(double,double,double,double*)> waveletTolFunc =[wtol](double x,double y, double z,double * hx){ return nlsm::computeWTol(x,y,z,hx);};
                return waveletTolFunc;
                
            }

            static unsigned int getBlkTimestepFac(unsigned int blev, unsigned int lmin, unsigned int lmax);
            




    };




}

