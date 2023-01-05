/* -------------------------------------------------------------------------- *
 *                    OpenSim: MuscleReflexCircuit.cpp                        *
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
#include "MuscleReflexCircuit.h"
#include <OpenSim/OpenSim.h>


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
MuscleReflexCircuit::MuscleReflexCircuit()
{
    constructProperties();
}

/* Convenience constructor. */
MuscleReflexCircuit::MuscleReflexCircuit(const std::string& name,
                                         const Muscle& muscle,
                                         double threshold,
                                         double timeDelay,
                                         double defaultControlSignal,
                                         double normalizedRestLength)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    connectSocket_muscle(muscle);
    //connectSocket_spindle(spindle);
    //connectSocket_golgi(golgi);
    
    constructProperties();
    set_threshold(threshold);
    set_timeDelay(timeDelay);
    set_defaultControlSignal(defaultControlSignal);
    set_normalizedRestLength(normalizedRestLength);


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
void MuscleReflexCircuit::constructProperties()
{
    constructProperty_defaultControlSignal(1.0);
    constructProperty_timeDelay(0.1);
    constructProperty_threshold(0.1);
    constructProperty_normalizedRestLength(1.0);
    constructProperty_weights();

    constructProperty_SimpleSpindle(SimpleSpindle());
    constructProperty_GolgiTendon(GolgiTendon());
    constructProperty_Delay(Delay());
    constructProperty_Interneuron(Interneuron());
    
}


void MuscleReflexCircuit::extendConnectToModel(Model& model)
{
    Super::extendConnectToModel(model);
    
    // connect inputs to outputs and sockets to components
    const SimpleSpindle& spindle = get_SimpleSpindle();
    const GolgiTendon& golgi = get_GolgiTendon();
    const Interneuron& interneuron = getInterneuron();
    
    // connect muscles to spindle and golgi sockets
    upd_SimpleSpindle().updSocket("muscle").connect(getMuscle());
    upd_GolgiTendon().updSocket("muscle").connect(getMuscle());

    
    // connect the interneuron inputs to the spindle and golgi outputs
    updInterneuron().updInput("afferents").connect(spindle.getOutput("spindle_length"));
    updInterneuron().updInput("afferents").connect(spindle.getOutput("spindle_speed"));
    updInterneuron().updInput("afferents").connect(golgi.getOutput("golgiLength"));
    
    // Connect the delay component input to the interneuron output
    updDelay().updInput("signal").connect(interneuron.getOutput("interneuron_signal"));
     

}

void MuscleReflexCircuit::extendFinalizeFromProperties()
{
    Super::extendFinalizeFromProperties();
    
    
    //const double& weight[4] = {0,1,2,3};
    SimTK::Vector weights = getWeights();
    
    //Interneuron& interneuron = updInterneuron();
    //Delay& delay = updDelay();
    
    // Setup the property values for the simplespindle, interneuron and delay component
    upd_SimpleSpindle().setNormalizedRestLength(get_normalizedRestLength());
    updInterneuron().setWeights(weights);
    updInterneuron().setThreshold(get_threshold());
    updDelay().setDelayValue(get_timeDelay());
    updDelay().setDefaultSignal(get_defaultControlSignal());
    
    OPENSIM_THROW_IF_FRMOBJ(get_timeDelay() < SimTK::Eps, InvalidPropertyValue, getName(), "Delay value cannot be less than SimTK::Eps, if it is we throw the delay component from the simulation");
     
}


//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
// Properties
//-----------------------------------------------------------------------------

Delay& MuscleReflexCircuit::updDelay()
{
    return upd_Delay();
}
const Delay& MuscleReflexCircuit::getDelay() const
{
    return get_Delay();
}

Interneuron& MuscleReflexCircuit::updInterneuron()
{
    return upd_Interneuron();
}
const Interneuron& MuscleReflexCircuit::getInterneuron() const
{
    return get_Interneuron();
}

void MuscleReflexCircuit::setWeights(SimTK::Vector weights)
{
    set_weights(weights);
}
SimTK::Vector MuscleReflexCircuit::getWeights() const
{
    int size = getProperty_weights().size();
    Vector v(size);
    for(int i = 0; i < size; i++)
    {
        v[i] = get_weights(i);
    }
    return v;
}

void MuscleReflexCircuit::setThreshold(double threshold)
{
    set_threshold(threshold);
}
double MuscleReflexCircuit::getThreshold() const
{
    get_threshold();
}

void MuscleReflexCircuit::setTimeDelay(double timedelay)
{
    set_timeDelay(timedelay);
}
double MuscleReflexCircuit::getTimeDelay() const
{
    get_timeDelay();
}

void MuscleReflexCircuit::setDefaultControlSignal(double defaultControlSignal)
{
    set_defaultControlSignal(defaultControlSignal);
}
double MuscleReflexCircuit::getDefaultControlSignal() const
{
    get_defaultControlSignal();
}

void MuscleReflexCircuit::setNormalizedRestLength(double normalizedRestLength)
{
    set_normalizedRestLength(normalizedRestLength);
}
double MuscleReflexCircuit::getNormalizedRestLength() const
{
    get_normalizedRestLength();
}


//-----------------------------------------------------------------------------
// SOCKETS
//-----------------------------------------------------------------------------
const Muscle& MuscleReflexCircuit::getMuscle() const
{
    return getSocket<Muscle>("muscle").getConnectee();
}

/*
const SimpleSpindle& MuscleReflexCircuit::getSpindle() const
{
    return getSocket<SimpleSpindle>("spindle").getConnectee();
}

const GolgiTendon& MuscleReflexCircuit::getGolgi() const
{
    return getSocket<GolgiTendon>("golgi").getConnectee();
}
*/


//=============================================================================
// SIGNALS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals after a delay /tau
 *
 * @param s         current state of the system
 */

double MuscleReflexCircuit::getMuscleSignal(const SimTK::State &s) const
{

    double muscle_signal = 0;
     
    const Delay& delay = getDelay();
    
    muscle_signal = delay.getSignal(s);
    
    return muscle_signal;

}

