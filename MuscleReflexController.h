#ifndef OPENSIM_MuscleReflexController_H_
#define OPENSIM_MuscleReflexController_H_
/* -------------------------------------------------------------------------- *
 *                   OpenSim: MuscleReflexController.h                        *
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
#include "osimMuscleReflexControllerDLL.h"
#include "OpenSim/Simulation/Control/Controller.h"
#include "OpenSim/Simulation/Model/Muscle.h"
#include "MuscleReflexCircuit.h"
#include <OpenSim/Common/Set.h>
#include <OpenSim/Simulation/Model/Model.h>



namespace OpenSim {


//=============================================================================
//=============================================================================
/**
 * MuscleReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a spindle and golgi-tendon stretch reflex.
 *
 * @author  Hjalti Hilmarsson
 */
class OSIMMUSCLEREFLEXCONTROLLER_API MuscleReflexController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(MuscleReflexController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    
    OpenSim_DECLARE_PROPERTY(gain, double, "The gain that is applied to the controller signal");
    
    OpenSim_DECLARE_LIST_PROPERTY(muscleReflexCircuit_list, std::string, "The list of model Muscle Reflex Circuits that this controller will depend upon for control");

//==============================================================================
// SOCKETS
//==============================================================================
    
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    MuscleReflexController();
    MuscleReflexController(const std::string& name,
                           double gain);
    
    

    // Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.
    
    //--------------------------------------------------------------------------
    // Controller Interface
    //--------------------------------------------------------------------------
    
     
    /** replace the current set of MuscleReflexCircuits with the provided set*/
    void setMuscleReflexCircuit(const Set<MuscleReflexCircuit>& muscleReflexCircuits);
    /**add the current set of MuscleReflexCircuits*/
    void addMuscleReflexCircuit(const MuscleReflexCircuit& muscleReflexCircuit);
    /**get a const refernece to the current set of MuscleReflexCircuits*/
    const Set<const MuscleReflexCircuit>& getMuscleReflexCircuitSet() const;
    /** get a writable reference to the set of const MuscleReflexCircuits for this controllerr*/
    Set<const MuscleReflexCircuit>& updMuscleReflexCircuit();

  
    
    /** Compute the controls for stretch reflex
     *  This method defines the behavior of the stretch reflex
     *
     * @param s         system state 
     * @param controls  writable model controls
     */
    void computeControls(const SimTK::State& s,
                         SimTK::Vector &controls) const override;


private:

    // the set of Model Muscle Reflex Circuits that this controller controls
    Set<const MuscleReflexCircuit> _muscleReflexCircuitSet;
    
    
protected:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& model) override;
    
    //=========================================================================
};  // END of class MuscleReflexController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_MuscleReflexController_H_
