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





#endif // #ifndef __Water_h_
