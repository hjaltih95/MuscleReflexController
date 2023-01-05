#ifndef OPENSIM_MuscleReflexCircuit_H_
#define OPENSIM_MuscleReflexCircuit_H_
/* -------------------------------------------------------------------------- *
 *                    OpenSim: MuscleReflexCircuit.h                          *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.     update for grants                                      *
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
#include "osimMuscleReflexCircuitDLL.h"
#include "OpenSim/Simulation/Model/Muscle.h"
#include "OpenSim/Simulation/Model/Model.h"
#include "OpenSim/Simulation/Model/ModelComponent.h"
#include "SimpleSpindle.h"
#include "GolgiTendon.h"
#include "Delay.h"
#include "Interneuron.h"

namespace OpenSim {



//=============================================================================
//=============================================================================
/**
 * ToyReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a simple stretch reflex. This controller 
 * is meant to serve as an example how to implement a controller in
 * OpenSim. It is intended for donstration purposes only. 
 *
 * @author  Ajay Seth
 */
class OSIMMUSCLEREFLEXCIRCUIT_API MuscleReflexCircuit : public ModelComponent {
OpenSim_DECLARE_CONCRETE_OBJECT(MuscleReflexCircuit, ModelComponent);

public:
//=============================================================================
// INPUT
//=============================================================================
    
//=============================================================================
// PROPERTIES
//=============================================================================

    OpenSim_DECLARE_PROPERTY(threshold, double, "The threshold property for the interneuron");
    
    OpenSim_DECLARE_PROPERTY(timeDelay, double, "The delay for the delay component");
    
    OpenSim_DECLARE_PROPERTY(defaultControlSignal, double, "The default control signal to send while the signal has not yet gotten their delaied signal");
    
    OpenSim_DECLARE_PROPERTY(normalizedRestLength, double, "The intended rest length of the spindle");
    
    OpenSim_DECLARE_LIST_PROPERTY(weights, double, "The weights given to the input signals of the interneuron");
    
    OpenSim_DECLARE_UNNAMED_PROPERTY(SimpleSpindle, "The muscle spindle component that measures muscles stretch and lengthening velocity");
    
    OpenSim_DECLARE_UNNAMED_PROPERTY(GolgiTendon, "The muscle golgi-tendon organ that measures tendon stretch and keeps muscle tension stable");
    
    OpenSim_DECLARE_UNNAMED_PROPERTY(Delay, "The delay component that will delay the muscle signal");
    
    OpenSim_DECLARE_UNNAMED_PROPERTY(Interneuron, "The interneuron component that takes in mucle sensor signals and sends an ouput signal if the muscle activation is large enough");
    
    /*
    OpenSim_DECLARE_LIST_PROPERTY(interneuron_list, std::string, "The list of model interneurons that this controller will depend upon for control");
    
    OpenSim_DECLARE_PROPERTY(delay_list, std::string, "The list of model delay components that this controller will depend upon");
    */
    
//==============================================================================
// SOCKETS
//==============================================================================
    OpenSim_DECLARE_SOCKET(muscle, Muscle, "The muscle that is being controlled");
    
    //OpenSim_DECLARE_SOCKET(spindle, SimpleSpindle, "The spindle that is sending out muscle length and muscle lengthening speed");
    
    //OpenSim_DECLARE_SOCKET(golgi, GolgiTendon, "The Golgi-Tendon Organ that is sending out muscle tendon length");
    
    
//=============================================================================
// OUTPUTS
//=============================================================================

    OpenSim_DECLARE_OUTPUT(muscle_signal, double, getMuscleSignal, SimTK::Stage::Velocity);
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    MuscleReflexCircuit();
    
    // convineance constructor
    MuscleReflexCircuit(const std::string& name,
                        const Muscle& muscle,
                        double threshold,
                        double timeDelay,
                        double defaultControlSignal,
                        double normalizedRestLength);

    // Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.
    
//--------------------------------------------------------------------------
// Muslce Reflex Circuit property getters and setters
//--------------------------------------------------------------------------
    
    Delay& updDelay();
    const Delay& getDelay() const;

    
    Interneuron& updInterneuron();
    const Interneuron& getInterneuron() const;
    
    SimTK::Vector getWeights() const;
    void setWeights(SimTK::Vector weights);
    
    void setThreshold(double threshold);
    double getThreshold() const;
    
    void setTimeDelay(double timeDelay);
    double getTimeDelay() const;
    
    void setDefaultControlSignal(double defaultControlSignal);
    double getDefaultControlSignal() const;
    
    void setNormalizedRestLength(double normalizedRestLength);
    double getNormalizedRestLength() const;
    
    
    
//--------------------------------------------------------------------------
// Muscle Reflex Circuit Socket getters and setters
//--------------------------------------------------------------------------
    
    // get a reference to the muscle that the muscle reflex circuit is acting upon
    const Muscle& getMuscle() const;
    
    // get a reference to the spindle attatched to the muscle
    //const SimpleSpindle& getSpindle() const;
    
    // get a reference to the golgi attatched to the muscle
    //const GolgiTendon& getGolgi() const;
    

//--------------------------------------------------------------------------
// Muslce Reflex Circuit output getters and setters
//--------------------------------------------------------------------------
/** @name Golgi Tendon State Dependendt Access Methods
    Get quanitites of interest common to all spindles*/
   /*
    void setInterneurons(const Set<Interneuron>& interneurons);
    void addInterneuron(const Interneuron& interneuron);
    const Set<const Interneuron>& getInterneuronSet() const;
    Set<const Interneuron>& updInterneurons();
    
    void setDelays(const Set<Delay>& delays);
    void addDelay(const Delay& delay);
    const Set<const Delay>& getDelaySet() const;
    Set<const Delay>& updDelays();
    */
    
    void setMuscleSignal(SimTK::State& s, double muscle_signal) const;
    double getMuscleSignal(const SimTK::State& s) const;
    

private:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& aModel) override;
    
    void extendFinalizeFromProperties() override;
    /*
    Set<const Interneuron> _interneuronSet;
    Set<const Delay> _delaySet;
     */

    
protected:
    //=========================================================================
};  // END of class MuscleReflexCircuit

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_MuscleReflexCircuit_H_


