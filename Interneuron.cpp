/* -------------------------------------------------------------------------- *
 *                      OpenSim:  Interneuron.cpp                             *
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
#include "Interneuron.h"
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
Interneuron::Interneuron()
{
    constructProperties();
}

/* Convenience constructor. */
Interneuron::Interneuron(const std::string& name,
                         double threshold)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    
    constructProperties();
    set_threshold(threshold);

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
void Interneuron::constructProperties()
{
    constructProperty_threshold(0.5);
    constructProperty_weights();
}


void Interneuron::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);
}

void Interneuron::extendFinalizeFromProperties()
{
    Super::extendFinalizeFromProperties();
    
    //OPENSIM_THROW_IF(getProperty_weights()., <#EXCEPTION, ...#>)
}

//=============================================================================
// GET AND SET
//=============================================================================

void Interneuron::setWeights(SimTK::Vector weights)
{
    set_weights(weights);
}
SimTK::Vector Interneuron::getWeights() const
{
    int size = getProperty_weights().size();
    Vector v(size);
    for(int i = 0; i < size; i++)
    {
        v[i] = get_weights(i);
    }
    return v;
}

void Interneuron::setThreshold(double threshold)
{
    set_threshold(threshold);

}
double Interneuron::getThreshold() const
{
    get_threshold();
}


//-----------------------------------------------------------------------------
//  SOCKET
//-----------------------------------------------------------------------------


//=============================================================================
// SIGNALS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals after a delay /tau
 *
 * @param s         current state of the system
 */
double Interneuron::getSignal(const SimTK::State& s) const
{
    
    auto inputs = getInput<double>("afferents").getVector(s);
    double interneuron_signal = 0;
    double threshold = get_threshold();
    Vector weights = getWeights();

    for(int i = 0; i<inputs.size(); i++)
    {
        inputs[i] = weights[i]*inputs[i];
    }
    
    if (inputs.sum() > threshold)
    {
        interneuron_signal = inputs.sum();
    }
    else
    {
        interneuron_signal = 0;
    }
    
    return interneuron_signal;
}

