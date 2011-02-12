/*
The program will show how to use the
basics of the VideoDriver, the GUIEnvironment and the
SceneManager.

To use the engine, we will have to include the header file
irrlicht.h, which can be found in the Irrlicht Engine SDK
directory \include.
*/
#include <irrlicht.h>
#include <IrrBP.h>

/*
In the Irrlicht Engine, everything can be found in the namespace
'irr'. So if you want to use a class of the engine, you have to
write an irr:: before the name of the class. For example to use
the IrrlichtDevice write: irr::IrrlichtDevice. To get rid of the
irr:: in front of the name of every class, we tell the compiler
that we use that namespace from now on, and we will not have to
write that 'irr::'.
*/
using namespace irr;

/*
There are 5 sub namespaces in the Irrlicht Engine. Take a look
at them, you can read a detailed description of them in the
documentation by clicking on the top menu item 'Namespace List'
or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
Like the irr Namespace, we do not want these 5 sub namespaces now,
to keep this example simple. Hence we tell the compiler again
that we do not want always to write their names:
*/
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/*
This is the main method. We can use void main() on every platform.
On Windows platforms, we could also use the WinMain method
if we would want to get rid of the console window, which pops up when
starting a program with main(), but to keep this example simple,
we use main().
*/

#define N_BOXES 13
char* boxes[N_BOXES]={
"data/table/mdl/Cube.irrmesh",
"data/table/mdl/Cube.002.irrmesh",
"data/table/mdl/Cube.003.irrmesh",
"data/table/mdl/Cube.004.irrmesh",
"data/table/mdl/Cube.005.irrmesh",
"data/table/mdl/Cube.006.irrmesh",
"data/table/mdl/Cube.007.irrmesh",
"data/table/mdl/Cube.008.irrmesh",
"data/table/mdl/Cube.009.irrmesh",
"data/table/mdl/Cube.010.irrmesh",
"data/table/mdl/Cube.011.irrmesh",
"data/table/mdl/Cube.012.irrmesh",
"data/table/mdl/Cube.013.irrmesh"
};
	const double k1=.35;
	const double z1=.025;
	const double k2=.275;
    const double z2=.03;

  vector3df pos_boxes[N_BOXES] = {
    vector3df(0,z2,1.061),       //Cube
    vector3df(0,z2,-1.061),      //Cube.002
    vector3df(1.511,z2,0),       //Cube.003
    vector3df(-1.511,z2,0),      //Cube.004
    vector3df(-1.3,z2,.65),      //Cube.005
    vector3df(1.3,z2,.65),       //Cube.006
    vector3df(.7,z2,-.99),       //Cube.007
    vector3df(-.7,z2,-.99),      //Cube.008
    vector3df(1.039,z2,-.865),   //Cube.009
    vector3df(.361,z2,-.865),    //Cube.010
    vector3df(-.361,z2,-.865),  //Cube.011
    vector3df(-1.039,z2,-.865),  //Cube.012
    vector3df(0,-.105,0),       //Cube.013

  };


#define N_PIONS 15
  vector3df pos_pions[N_PIONS] = {
    vector3df(0*k1,z1,0*k1),

	vector3df(1*k1,z1,2*k1),
	vector3df(2*k1,z1,1*k1),
	vector3df(1*k1,z1,-2*k1),
	vector3df(2*k1,z1,-2*k1),

	vector3df(-1*k1,z1,2*k1),
	vector3df(-2*k1,z1,1*k1),
	vector3df(-1*k1,z1,-2*k1),
	vector3df(-2*k1,z1,-2*k1),

	vector3df(1.25,z1,1*k1-k2*1),
	vector3df(1.25,z1,1*k1-k2*2),
	vector3df(1.25,z1,1*k1-k2*4),

	vector3df(-1.25,z1,1*k1-k2*1),
	vector3df(-1.25,z1,1*k1-k2*2),
	vector3df(-1.25,z1,1*k1-k2*4)
};

#define N_ROIS 2
vector3df pos_rois[N_ROIS] = {
vector3df(1.25,z1,1*k1),
vector3df(-1.25,z1,1*k1)
};

#define N_REINES 2
vector3df pos_reines[N_REINES] = {
vector3df(1.25,z1,1*k1-k2*3),
vector3df(-1.25,z1,1*k1-k2*3)
};

class K : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	K()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

static CIrrBPManager * bulletmgr;


int main(int argc, char** argv)
{

	/*
	The most important function of the engine is the 'createDevice'
	function. The Irrlicht Device can be created with it, which is the
	root object for doing everything with the engine.
	createDevice() has 7 paramters:
	deviceType: Type of the device. This can currently be the Null-device,
	   the Software device, DirectX8, DirectX9, or OpenGL. In this example we use
	   EDT_SOFTWARE, but to try out, you might want to change it to
	   EDT_NULL, EDT_DIRECTX8 , EDT_DIRECTX9, or EDT_OPENGL.
	windowSize: Size of the Window or FullscreenMode to be created. In this
	   example we use 640x480.
	bits: Amount of bits per pixel when in fullscreen mode. This should
	   be 16 or 32. This parameter is ignored when running in windowed mode.
	fullscreen: Specifies if we want the device to run in fullscreen mode
	   or not.
	stencilbuffer: Specifies if we want to use the stencil buffer for drawing shadows.
	vsync: Specifies if we want to have vsync enabled, this is only useful in fullscreen
	  mode.
	eventReceiver: An object to receive events. We do not want to use this
	   parameter here, and set it to 0.
	*/

	K* k = new K();


	SIrrlichtCreationParameters* params=new SIrrlichtCreationParameters();
	params->AntiAlias=(16);
	params->Bits=(32);
	params->WindowSize=(dimension2d<u32>(1680, 1050));
	params->Fullscreen=1;
	params->DriverType=(EDT_OPENGL);
	params->EventReceiver=k;

	IrrlichtDevice *device = createDeviceEx(*params);

	/*
	Set the caption of the window to some nice text. Note that there is
	a 'L' in front of the string. The Irrlicht Engine uses wide character
	strings when displaying text.
	*/
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	/*
	Get a pointer to the video driver, the SceneManager and the
	graphical user interface environment, so that
	we do not always have to write device->getVideoDriver(),
	device->getSceneManager() and device->getGUIEnvironment().
	*/
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	/*
	We add a hello world label to the window, using the GUI environment.
	*/
	guienv->addStaticText(L"Hello World!",
						  rect<int>(10,10,200,22), true);

	/*
	To display something interesting, we load a Quake 2 model
	and display it. We only have to get the Mesh from the Scene
	Manager (getMesh()) and add a Scenetable to display the mesh.
	(addAnimatedMeshSceneNode()). Instead of writing the filename
	sydney.md2, it would also be possible to load a Maya object file
	(.obj), a complete Quake3 map (.bsp) or a Milshape file (.ms3d).
	By the way, that cool Quake 2 model called sydney was modelled
	by Brian Collins.
	*/
	IMesh* mesh = smgr->getMesh("data/table/mdl/Table.irrmesh");
	IMeshSceneNode* table = smgr->addMeshSceneNode( mesh );

	if (table)
	{
		table->setMaterialFlag(EMF_LIGHTING, false);
		//table->setFrameLoop(0, 310);
		table->setMaterialType(video::EMT_LIGHTMAP);
		table->setMaterialTexture( 0, driver->getTexture("data/table/tex/textable.png") );
		table->setMaterialTexture( 1, driver->getTexture("data/table/tex/table_lightmap.png") );
	}

	IAnimatedMesh* meshpion = smgr->getMesh("data/pion/mdl/pion.irrmesh");
	IAnimatedMeshSceneNode* pion = smgr->addAnimatedMeshSceneNode( meshpion );

	if (pion)
	{
		pion->setMaterialFlag(EMF_LIGHTING, false);
		//pion->setFrameLoop(0, 310);
		pion->setMaterialType(video::EMT_LIGHTMAP);
		pion->setMaterialTexture( 0, driver->getTexture("data/pion/tex/texpion.png") );
		pion->setMaterialTexture( 1, driver->getTexture("data/pion/tex/pion-lightmap.png") );
	}

	IAnimatedMesh* meshreine = smgr->getMesh("data/reine/mdl/reine.irrmesh");
	IAnimatedMeshSceneNode* reine = smgr->addAnimatedMeshSceneNode( meshreine );

	if (reine)
	{
		reine->setMaterialFlag(EMF_LIGHTING, false);
		//reine->setFrameLoop(0, 310);
		reine->setMaterialType(video::EMT_LIGHTMAP);
		reine->setMaterialTexture( 0, driver->getTexture("data/reine/tex/texreine.png") );
		reine->setMaterialTexture( 1, driver->getTexture("data/reine/tex/reine-lightmap.png") );
	}

	IAnimatedMesh* meshroi = smgr->getMesh("data/roi/mdl/roi.irrmesh");
	IAnimatedMeshSceneNode* roi = smgr->addAnimatedMeshSceneNode( meshroi );

	if (roi)
	{
		roi->setMaterialFlag(EMF_LIGHTING, false);
		//roi->setFrameLoop(0, 310);
		roi->setMaterialType(video::EMT_LIGHTMAP);
		roi->setMaterialTexture( 0, driver->getTexture("data/roi/tex/texroi.png") );
		roi->setMaterialTexture( 1, driver->getTexture("data/roi/tex/roi-lightmap.png") );
	}




	pion->setPosition(vector3df(0,1,0));
	reine->setPosition(vector3df(.5,0.15,.5));
	roi->setPosition(vector3df(.5,-1,.5));

	/*
	To look at the mesh, we place a camera into 3d space at the position
	(0, 30, -40). The camera looks from there to (0,5,0).
	*/
	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0, vector3df(2,2,1), vector3df(0,-.5,0));

	/*
	Ok, now we have set up the scene, lets draw everything:
	We run the device in a while() loop, until the device does not
	want to run any more. This would be when the user closed the window
	or pressed ALT+F4 in windows.
	*/
	//ITriangleSelector* selector = 0;

	/*if (pion)
	{
		selector = smgr->createOctreeTriangleSelector(
					   pion->getMesh(), pion, 128);
		pion->setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}*/









	bulletmgr = createBulletManager(device);
        bulletmgr->getWorld()->setGravity(vector3df(0,-9.81,0));
        bulletmgr->addTrimesh(table,0);



int i=0;
for(i=0;i<N_PIONS;++i)
{
    ISceneNode* tmp = pion->clone();
    tmp->setPosition(pos_pions[i]);
    bulletmgr->addRigidCylinder(tmp,4,Y);
}


for(i=0 ; i<N_BOXES; ++i)
{
    IMesh* mesh = smgr->getMesh(boxes[i]);
    IMeshSceneNode* box = smgr->addMeshSceneNode( mesh );
	if (box)
	{
	    box->setPosition(pos_boxes[i]);
		box->setVisible(false);
   //     CIrrBPBoxBody * bbox = bulletmgr->addRigidBox(box,0);
	}
}
//

CIrrBPRigidBody * broi = bulletmgr->addRigidCylinder(roi,3,Y);
CIrrBPRigidBody * bpion = bulletmgr->addRigidCylinder(pion,1,Y);
CIrrBPRigidBody * breine = bulletmgr->addRigidCylinder(reine,300,Y);











/*
	if (table)
	{
		selector = smgr->createOctreeTriangleSelector(
					   table->getMesh(), table, 128);
		table->setTriangleSelector(selector);
		// We're not done with this selector yet, so don't drop it.
	}

*/
	while(device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene clears the screen with a color and also the
		depth buffer if wanted. Then we let the Scene Manager and the
		GUI Environment draw their content. With the endScene() call
		everything is presented on the screen.
		*/
		driver->beginScene(true, true, SColor(0,200,200,255));


		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		vector3df p = reine->getPosition();

		if(k->IsKeyDown(KEY_ESCAPE))
			break;
		if(k->IsKeyDown(KEY_KEY_Z))
            breine->setPosition(vector3df(p.X-.01,p.Y,p.Z));
			//reine->setPosition(vector3df(p.X-.01,p.Y,p.Z));
		if(k->IsKeyDown(KEY_KEY_S))
			breine->setPosition(vector3df(p.X+.01,p.Y,p.Z));
		if(k->IsKeyDown(KEY_KEY_D))
			breine->setPosition(vector3df(p.X,p.Y,p.Z+.01));
		if(k->IsKeyDown(KEY_KEY_Q))
			breine->setPosition(vector3df(p.X,p.Y,p.Z-.01));

        bulletmgr->stepSimulation();

	}


	bulletmgr->drop();


	/*
	After we are finished, we have to delete the Irrlicht Device
	created before with createDevice(). In the Irrlicht Engine,
	you will have to delete all objects you created with a method or
	function which starts with 'create'. The object is simply deleted
	by calling ->drop().
	See the documentation at
	http://irrlicht.sourceforge.net//docu/classirr_1_1IUnknown.html#a3
	for more information.
	*/
	device->drop();

	return 0;
}






