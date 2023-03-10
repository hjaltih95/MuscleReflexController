#ifndef _osimMuscleReflexControllerDLL_h_
#define _osimMuscleReflexControllerDLL_h_
/* -------------------------------------------------------------------------- *
 *                  OpenSim:  osimMuscleReflexControllerDLL.h                 *
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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.   *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */

// UNIX PLATFORM
#ifndef _WIN32

#define OSIMMUSCLEREFLEXCONTROLLER_API

// WINDOWS PLATFORM
#else

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#ifdef OSIMMUSCLEREFLEXCONTROLLER_EXPORTS
#define OSIMMUSCLEREFLEXCONTROLLER_API __declspec(dllexport)
#else
#define OSIMMUSCLEREFLEXCONTROLLER_API __declspec(dllimport)
#endif

#endif // PLATFORM


#endif // __osimMuscleReflexControllerDLL_h__
