/* -------------------------------------------------------------------------- *
*                         OpenSim:  mainSpindle.cpp                          *
* -------------------------------------------------------------------------- *
* The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
* See http://opensim.stanford.edu and the NOTICE file for more information.  *
* OpenSim is developed at Stanford University and supported by the US        *
* National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
* through the Warrior Web program.                                           *
*                                                                            *
* Copyright (c) 2005-2017 Stanford University and the Authors                *
* Author(s): Peter Loan, Ajay Seth, Ayman Habib                              *
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
//=============================================================================
#include <OpenSim/OpenSim.h>
#include "MuscleReflexCircuit.h"
#include <OpenSim/Common/IO.h>
#include "OpenSim/Common/STOFileAdapter.h"
#include "MuscleReflexController.h"

using namespace OpenSim;
using namespace SimTK;

//_____________________________________________________________________________
/**
 * Run a simulation of a bicep being flexed
 */

int main() {
    
    Model osimModel("NewLandingReflexesModel.osim");
    
    /*
     double threshold,
     double timeDelay,
     double defaultControlSignal,
     double normalizedRestLength
     */
    
    double threshold = 0.015;
    double timedelay = 0.6;
    double defaultControlSignal = 0.09;
    double normalizedRestLength = 1.0;
    
    Vector weights(3);
    
    weights[0] = 1;     // spindle length / gain for SL; gSL
    weights[1] = 7;     // spindle speed  / gain for SS; gSS
    weights[2] = -0.5;  // tendon length  / gain for TL; gTL
    
    const Set<Muscle>& muscleSet = osimModel.getMuscles();
    
    double gain = 1.0;
    MuscleReflexController* brain = new MuscleReflexController("brain", gain);
    osimModel.addController(brain);
    
    
    for(int i = 0; i < muscleSet.getSize(); i++)
    {
        MuscleReflexCircuit* mrc = new MuscleReflexCircuit(muscleSet[i].getName(), muscleSet[i], threshold, timedelay, defaultControlSignal, normalizedRestLength);
        mrc->setWeights(weights);
        osimModel.addComponent(mrc);
        brain->addMuscleReflexCircuit(*mrc);
    }
    
    // Configure the model.
    State& state = osimModel.initSystem();
    
    osimModel.print("MRCLandingReflexesModel.osim");
    
    
    /*
    // create the muscle reflex circuits for each muscle
    MuscleReflexCircuit* mrc_glut_med1_r = new MuscleReflexCircuit(muscleSet[0].getName(), muscleSet[0], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_glut_med1_r->setWeights(weights);
    
    MuscleReflexCircuit* mrc_glut_med2_r = new MuscleReflexCircuit(muscleSet[1].getName(), muscleSet[1], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_glut_med2_r->setWeights(weights);
    
    MuscleReflexCircuit* mrc_glut_med3_r = new MuscleReflexCircuit(muscleSet[2].getName(), muscleSet[2], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_glut_med3_r->setWeights(weights);

    MuscleReflexCircuit* mrc_bifemlh_r = new MuscleReflexCircuit(muscleSet[3].getName(), muscleSet[3], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_bifemlh_r->setWeights(weights);
    
    MuscleReflexCircuit* mrc_bifemsh_r = new MuscleReflexCircuit(muscleSet[4].getName(), muscleSet[4], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_bifemsh_r->setWeights(weights);
    
    MuscleReflexCircuit* mrc_sar_r = new MuscleReflexCircuit(muscleSet[5].getName(), muscleSet[5], threshold, timedelay, defaultControlSignal, normalizedRestLength);
    mrc_sar_r->setWeights(weights);
    
    */
   

    /*
     glut_med1_r glut_med2_r glut_med3_r bifemlh_r bifemsh_r sar_r add_mag2_r tfl_r pect_r grac_r glut_max1_r glut_max2_r glut_max3_r iliacus_r psoas_r quad_fem_r gem_r peri_r rect_fem_r vas_int_r med_gas_r lat_gas_r soleus_r tib_post_r flex_dig_r flex_hal_r tib_ant_r per_brev_r per_long_r per_tert_r ext_dig_r ext_hal_r glut_med1_l glut_med2_l glut_med3_l bifemlh_l bifemsh_l sar_l add_mag2_l tfl_l pect_l grac_l glut_max1_l glut_max2_l glut_max3_l iliacus_l psoas_l quad_fem_l gem_l peri_l rect_fem_l vas_int_l med_gas_l lat_gas_l soleus_l tib_post_l flex_dig_l flex_hal_l tib_ant_l per_brev_l per_long_l per_tert_l ext_dig_l ext_hal_l ercspn_r ercspn_l intobl_r intobl_l extobl_r
     */
    
   /*
    Model model;
    model.setName("bicep_curl");
    model.setUseVisualizer(true);

    // Create two links, each with a mass of 1 kg, center of mass at the body's
    // origin, and moments and products of inertia of zero.
    OpenSim::Body* humerus = new OpenSim::Body("humerus", 1, Vec3(0), Inertia(0));
    OpenSim::Body* radius  = new OpenSim::Body("radius",  1, Vec3(0), Inertia(0));

    // Connect the bodies with pin joints. Assume each body is 1 m long.
    PinJoint* shoulder = new PinJoint("shoulder",
        // Parent body, location in parent, orientation in parent.
        model.getGround(), Vec3(0), Vec3(0),
        // Child body, location in child, orientation in child.
        *humerus, Vec3(0, 1, 0), Vec3(0));
    PinJoint* elbow = new PinJoint("elbow",
        *humerus, Vec3(0), Vec3(0), *radius, Vec3(0, 1, 0), Vec3(0));

    // Add a muscle that flexes the elbow.
    Millard2012EquilibriumMuscle* biceps = new
        Millard2012EquilibriumMuscle("biceps", 200, 0.6, 0.55, 0);
    biceps->addNewPathPoint("origin",    *humerus, Vec3(0, 0.8, 0));
    biceps->addNewPathPoint("insertion", *radius,  Vec3(0, 0.7, 0));
    
    
    model.addBody(humerus);    model.addBody(radius);
    model.addJoint(shoulder);  model.addJoint(elbow);
    model.addForce(biceps);
    
    // add spindle and golgi to model
    //SimpleSpindle* spindle = new SimpleSpindle("muscle_spindle", *biceps, 0.3);
    
    //GolgiTendon* golgi = new GolgiTendon("muscle_golgi", *biceps);
    
    // Create a vector that hold the weights for the interneuron inputs and fill it with default numbers
    Vector weights(3);
    
    weights[0] = 1;
    weights[1] = 7;
    weights[2] = -0.5;
    /*
    // add interneuron  and delay component
    Interneuron* interneuron = new Interneuron("interneuron", 0.001);
    interneuron->setWeights(weights);
    
    
    Delay* delay = new Delay("delay", 2.0, 10.0);
     */
    
    /*
    // create a muscle reflex circuit
    MuscleReflexCircuit* mrc = new MuscleReflexCircuit("mrc", *biceps, 0.015, 0.1, 0.09, 0.3);
    mrc->setWeights(weights);
    
    
    //model.addComponent(spindle);
    //model.addComponent(golgi);
    //model.addComponent(delay);
    //model.addComponent(interneuron);
    model.addComponent(mrc);
    
    // Add a controller that specifies the excitation of the muscle.
    MuscleReflexController* brain = new MuscleReflexController("brain", 2);
    brain->setActuators(model.updActuators());
    brain->addMuscleReflexCircuit(*mrc);
    
    /*
    PrescribedController* brain = new PrescribedController();
    brain->addActuator(*biceps);
    // Muscle excitation is 0.3 for the first 0.5 seconds, then increases to 1.
    brain->prescribeControlForActuator("biceps",
            new StepFunction(0.5, 3, 0.3, 1));
   */

     /*
    // Add controller to the model.
    model.addController(brain);

    
    /*
    model.updComponent("interneuron").updInput("afferents").connect(spindle->getOutput("spindle_length"));
    model.updComponent("interneuron").updInput("afferents").connect(spindle->getOutput("spindle_speed"));
    model.updComponent("interneuron").updInput("afferents").connect(golgi->getOutput("golgiLength"));
    
    //model.updComponent("delay").updInput("signal").connect(interneuron->getOutput("signal"));
    */
    /*
    // Add a console reporter to print the muscle fiber force and elbow angle.
    ConsoleReporter* reporter = new ConsoleReporter();
    reporter->set_report_time_interval(1.0);
    //reporter->addToReport(biceps->getOutput("fiber_force"));
    reporter->addToReport(biceps->getOutput("fiber_length"));
    reporter->addToReport(biceps->getOutput("tendon_length"));
    //reporter->addToReport(spindle->getOutput("spindle_length"));
    //reporter->addToReport(spindle->getOutput("spindle_speed"));
    //reporter->addToReport(golgi->getOutput("golgiLength"));
    //reporter->addToReport(interneuron->getOutput("signal"));
    //reporter->addToReport(delay->getOutput("controlSignal"));
    reporter->addToReport(mrc->getOutput("muscle_signal"));
    model.addComponent(reporter);

    // Add display geometry.
    Ellipsoid bodyGeometry(0.1, 0.5, 0.1);
    bodyGeometry.setColor(Gray);
    // Attach an ellipsoid to a frame located at the center of each body.
    PhysicalOffsetFrame* humerusCenter = new PhysicalOffsetFrame(
        "humerusCenter", *humerus, Transform(Vec3(0, 0.5, 0)));
    humerus->addComponent(humerusCenter);
    humerusCenter->attachGeometry(bodyGeometry.clone());
    PhysicalOffsetFrame* radiusCenter = new PhysicalOffsetFrame(
        "radiusCenter", *radius, Transform(Vec3(0, 0.5, 0)));
    radius->addComponent(radiusCenter);
    radiusCenter->attachGeometry(bodyGeometry.clone());

    // Configure the model.
    State& state = model.initSystem();
    
    
    // Fix the shoulder at its default angle and begin with the elbow flexed.
    shoulder->getCoordinate().setLocked(state, true);
    elbow->getCoordinate().setValue(state, 0.5 * Pi);
    model.equilibrateMuscles(state);
    

    // Configure the visualizer.
    model.updMatterSubsystem().setShowDefaultGeometry(true);
    Visualizer& viz = model.updVisualizer().updSimbodyVisualizer();
    viz.setBackgroundType(viz.SolidColor);
    viz.setBackgroundColor(White);

    model.print("mrc_model");
    // Simulate.
    simulate(model, state, 10.0);
    

    return 0;
     */
}
