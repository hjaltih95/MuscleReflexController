#ifndef OPENSIM_Interneuron_H_
#define OPENSIM_Interneuron_H_
/* -------------------------------------------------------------------------- *
 *                      OpenSim: Interneuron.h                                *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2021 Stanford University, TU Delft and the Authors      *
 * Author(s): Ajay Seth, Hjalti Hilmarsson                                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied    *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */


//============================================================================
// INCLUDE
//============================================================================
#include "osimInterneuronDLL.h"
#include "OpenSim/Simulation/Control/Controller.h"
#include "OpenSim/Simulation/Model/Muscle.h"
#include "OpenSim/Simulation/Model/Model.h"
#include "OpenSim/Simulation/Model/ModelComponent.h"



namespace OpenSim {

//=============================================================================
//=============================================================================
/**
 * ToyReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a simple stretch reflex. This controller
 * is meant to serve as an example how to implement a controller in
 * OpenSim. It is intended for demonstration purposes only.
 *
 * @author  Hjalti Hilmarsson
 */

class OSIMINTERNEURON_API Interneuron : public ModelComponent {
OpenSim_DECLARE_CONCRETE_OBJECT(Interneuron, ModelComponent);

public:
//=============================================================================
// INPUT
//=============================================================================
    // Input the signal from the proprioceptors
    OpenSim_DECLARE_LIST_INPUT(afferents, double, SimTK::Stage::Velocity,
        "The input signals from the proprieceptors");

    
//=============================================================================
// PROPERTIES
//=============================================================================
    OpenSim_DECLARE_PROPERTY(threshold, double, "The magnitude of activation required to send an output signal, greater than 0 and less or equal to 1");
    
    //OpenSim_DECCLARE_PROPERTY(default_weights, bool, "Performs a check to see weather default weights are applied, 1/(number of inputs), or specific weights are applied; this is true by default.");
    
    OpenSim_DECLARE_LIST_PROPERTY(weights, double, "The weights given to the input signals, can not sum to more than 1 and are between 0 and 1, ");
    
//==============================================================================
// SOCKETS
//==============================================================================
 
//=============================================================================
// OUTPUTS
//=============================================================================
    // we get our propriceptive afferents
    OpenSim_DECLARE_OUTPUT(interneuron_signal, double, getSignal, SimTK::Stage::Velocity);
    //
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    Interneuron();
    Interneuron(const std::string& name,
                double threshold);

    // Uses default (compiler-generated) destructor, copy constructor and copy
    // assignment operator.
    
//--------------------------------------------------------------------------
// Interneuron PARAMETER ACCESSORS
//--------------------------------------------------------------------------
    
  // get and set the weights
    void setWeights(SimTK::Vector weights);
    SimTK::Vector getWeights() const;
    
    void setThreshold(double threshold);
    double getThreshold() const;

//--------------------------------------------------------------------------
// Interneuron STATE DEPENDENT ACCESSORS
//--------------------------------------------------------------------------
/** @name Golgi Tendon State Dependendt Access Methods
    Get quanitites of interest common to all spindles*/
    void setSignal(SimTK::State& s, double interneuron_signal) const;
    double getSignal(const SimTK::State& s) const;
   
    
//--------------------------------------------------------------------------
// Interneuron INPUTS ACCESSORS
//--------------------------------------------------------------------------
    

private:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& aModel) override;
        void extendFinalizeFromProperties() override;

protected:
    

    //=========================================================================
};  // END of class Interneuron

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_Interneuron_H_
