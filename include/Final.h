#ifndef __Final_h_
#define __Final_h_
// ----------------------------------------------------------------------------
// Include the main OGRE header files
// Ogre.h just expands to including lots of individual OGRE header files
// ----------------------------------------------------------------------------
#include <Ogre.h>
#include <OgreTextAreaOverlayElement.h>

// ----------------------------------------------------------------------------
// Include the OGRE example framework
// This includes the classes defined to make getting an OGRE application running
// a lot easier. It automatically sets up all the main objects and allows you to
// just override the bits you want to instead of writing it all from scratch.
// ----------------------------------------------------------------------------
#include <ExampleApplication.h>

// ----------------------------------------------------------------------------
// Include the Hydrax plugin headers
// Main base headers (Hydrax.h) and especific headers (Noise/Water modules)
// ----------------------------------------------------------------------------
#include "Hydrax/Hydrax.h"
#include "Hydrax/Noise/Perlin/Perlin.h"
#include "Hydrax/Modules/ProjectedGrid/ProjectedGrid.h"
#include <Terrain/OgreTerrain.h>
#include <Terrain/OgreTerrainGroup.h>

#define _def_SkyBoxNum 3

// Hydrax pointer
Hydrax::Hydrax *mHydrax = 0;

Ogre::String mSkyBoxes[_def_SkyBoxNum] = 
{"Sky/ClubTropicana",
"Sky/EarlyMorning",
"Sky/Clouds"};

Ogre::Vector3 mSunPosition[_def_SkyBoxNum] = 
{Ogre::Vector3(0,10000,0),
Ogre::Vector3(0,10000,90000),
Ogre::Vector3(0,10000,0)};

Ogre::Vector3 mSunColor[_def_SkyBoxNum] = 
{Ogre::Vector3(1, 0.9, 0.6),
Ogre::Vector3(1,0.6,0.4),
Ogre::Vector3(0.45,0.45,0.45)};

int mCurrentSkyBox = 0;

#endif // #ifndef __Water_h_
