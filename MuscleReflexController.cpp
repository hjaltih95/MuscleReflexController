/* -------------------------------------------------------------------------- *
 *                   OpenSim:  MuscleReflexController.cpp                     *
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



//=============================================================================
// INCLUDES
//=============================================================================
#include "MuscleReflexController.h"
#include <OpenSim/OpenSim.h>
#include <OpenSim/Common/IO.h>



// This allows us to use OpenSim functions, classes, etc., without having to
// prefix the names of those things with "OpenSim::".
using namespace OpenSim;
using namespace std;
using namespace SimTK;


//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/* Default constructor. */
MuscleReflexController::MuscleReflexController()
{
    constructProperties();
}

MuscleReflexController::MuscleReflexController(const std::string& name,
                                               double gain)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    
    constructProperties();
    set_gain(gain);
}


//=============================================================================
// SETUP PROPERTIES
//=============================================================================
//
/* Connect properties to local pointers.
 *
 * Properties should be given a meaningful name and an informative comment.
 * The name you give each property is the tag that will be used in the XML
 * file. The comment will appear before the property in the XML file.
 * In addition, the comments are used for tool tips in the OpenSim GUI.
 *
 * All properties are added to the property set. Once added, they can be
 * read in and written to files.
 ____________________________________________________________________________


 * Construct Properties
 */
void MuscleReflexController::constructProperties()
{

    constructProperty_muscleReflexCircuit_list();
    _muscleReflexCircuitSet.setMemoryOwner(false);
     
    constructProperty_gain(1.0);
}


void MuscleReflexController::extendConnectToModel(Model& model)
{
    Super::extendConnectToModel(model);
   
    _muscleReflexCircuitSet.setMemoryOwner(false);
    _muscleReflexCircuitSet.setSize(0);
    
    int nac = getProperty_muscleReflexCircuit_list().size();
    if (nac == 0)
        return;
    
    auto muscleReflexCircuits = model.getComponentList<MuscleReflexCircuit>();
    if (IO::Uppercase(get_muscleReflexCircuit_list(0)) == "ALL") {
        for (auto& muscleReflexCircuit : muscleReflexCircuits) {
            _muscleReflexCircuitSet.adoptAndAppend(&muscleReflexCircuit);
        }
        return;
    }
    
    else {
        for (int i = 0; i < nac; ++i) {
            bool found = false;
            for (auto& muscleReflexCircuit : muscleReflexCircuits) {
                if (get_muscleReflexCircuit_list(i) == muscleReflexCircuit.getName()) {
                    _muscleReflexCircuitSet.adoptAndAppend(&muscleReflexCircuit);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cerr << "WARN: MuscleReflexController::connectToModel : MuscleReflexCircuit "
                << get_muscleReflexCircuit_list(i) <<
                " was not found and will be ignored." << endl;
            }
        }
    }
    
    
    
    Set<const Actuator>& actuators = updActuators();

    int cnt=0;
    
    while(cnt < actuators.getSize()){
        const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[cnt]);
        // control muscles only
        if(!musc){
            log_warn("MuscleReflexController assigned a non-muscle actuator '{}', "
                    "which will be ignored.", actuators[cnt].getName());
            actuators.remove(cnt);
        }else
            cnt++;
    }
     
}

//=============================================================================
// GET AND SET
//=============================================================================

// MuscleReflexCircuit getter and setters
void MuscleReflexController::setMuscleReflexCircuit(const Set<MuscleReflexCircuit>& muscleReflexCircuits)
{
    _muscleReflexCircuitSet.setMemoryOwner(false);
    
    _muscleReflexCircuitSet.setSize(0);
    updProperty_muscleReflexCircuit_list().clear();
    for (int i  = 0; i<muscleReflexCircuits.getSize(); i++)
    {
        addMuscleReflexCircuit(muscleReflexCircuits[i]);
    }
}

void MuscleReflexController::addMuscleReflexCircuit(const MuscleReflexCircuit& muscleReflexCircuit)
{
    _muscleReflexCircuitSet.adoptAndAppend(&muscleReflexCircuit);
    
    int found = updProperty_muscleReflexCircuit_list().findIndex(muscleReflexCircuit.getName());
    if (found < 0)
        updProperty_muscleReflexCircuit_list().appendValue(muscleReflexCircuit.getName());
}

Set <const MuscleReflexCircuit>& MuscleReflexController::updMuscleReflexCircuit()
{
    return _muscleReflexCircuitSet;
}

const Set <const MuscleReflexCircuit>& MuscleReflexController::getMuscleReflexCircuitSet() const
{
    return _muscleReflexCircuitSet;
}

//=============================================================================
// COMPUTATIONS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals for spindles
 *
 * @param s         current state of the system
 * @param controls  system wide controls to which this component can read off
 */

void MuscleReflexController::computeControls(const State& s,
                                          Vector &controls) const {
    // get time
    double t = s.getTime();
    double control = 0;
    
    const Set<const MuscleReflexCircuit>& muscleReflexCircuits = getMuscleReflexCircuitSet();
    
    for(int i = 0; i < muscleReflexCircuits.getSize(); i++) {
        //const Muscle* musc = dynamic_cast<const Muscle*>(&getActuatorSet().get(i));
        const MuscleReflexCircuit& muscleReflexCircuit = muscleReflexCircuits.get(i);
        
        control = get_gain()*muscleReflexCircuit.getMuscleSignal(s);
        
        
        
        const Muscle& musc = muscleReflexCircuit.getMuscle();
        
        SimTK::Vector actControls(1,control);
        //musc->addInControls(actControls, controls);
        musc.addInControls(actControls, controls);
    }
}

