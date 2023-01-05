/* -------------------------------------------------------------------------- *
 *                      OpenSim:  SimpleSpindle.cpp                           *
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
#include "SimpleSpindle.h"
#include <OpenSim/OpenSim.h>
#include "OpenSim/Simulation/Model/Muscle.h"


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
SimpleSpindle::SimpleSpindle()
{
    constructProperties();
}

/* Convenience constructor. */
SimpleSpindle::SimpleSpindle(const std::string& name,
                             const Muscle& muscle,
                             double normalizedRestLength)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
    
    setName(name);
    connectSocket_muscle(muscle);
    
    constructProperties();
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
void SimpleSpindle::constructProperties()
{
    /*
    constructProperty_gain_length(1.0);
    constructProperty_gain_velocity(1.0);
     */
    constructProperty_normalizedRestLength(1.0);
}

void SimpleSpindle::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);

    
}

void SimpleSpindle::setNormalizedRestLength(double normalizedRestLength)
{
    set_normalizedRestLength(normalizedRestLength);
}

//=============================================================================
// OUTPUTS
//=============================================================================

double SimpleSpindle::getSpindleLength(const SimTK::State& s) const
{
    
    double spindle_length = 0;
    double rest_length = get_normalizedRestLength();
    // optimal fiber length
    double f_o = 1;
    // muscle length
    double length = 0;
    // muscle stretsch
    double stretch = 0;
 

    const Muscle& musc = getMuscle();
    // get optimal fiber length and muscle length
    f_o = musc.getOptimalFiberLength();
    length = musc.getFiberLength(s);
    // Compute stretch, the muscle spindle only monitors the muscle fiber length not the muscle-tendon length, we are calculating how much the fibers are stretching.
    stretch = length-rest_length;
    // normalize the stretch of the muscle fibers
    spindle_length = 0.5*(fabs(stretch)+stretch)/f_o;
    
    return spindle_length;
}

double SimpleSpindle::getSpindleSpeed(const SimTK::State& s) const
{
    double spindle_speed = 0;
    double f_o = 1;
    double max_speed = 0;
    // muscle speed
    double speed = 0;
    // get a reference to the muscle
    const Muscle& musc = getMuscle();
    // get optimal fiber length
    f_o = musc.getOptimalFiberLength();
    // muscle lengthening speed
    speed = musc.getLengtheningSpeed(s);
    // the maximum lengthening speed of the muscle
    max_speed = f_o*musc.getMaxContractionVelocity();
    
    // make the muscle lengthening speed unit less
    spindle_speed = 0.5*(fabs(speed)+speed)/max_speed;
    
    
    return spindle_speed;
}

//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
// Spindle frame
//-----------------------------------------------------------------------------
const Muscle& SimpleSpindle::getMuscle() const
{
    return getSocket<Muscle>("muscle").getConnectee();
}



