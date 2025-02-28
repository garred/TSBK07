// MicroGlut is a stripped-down reimplementation of the classic GLUT/FreeGLUT library. By Ingemar Ragnemalm 2012-2015.
// This is the Linux version. There is also a Mac version (and drafts for a Windows version).

// Why do we use MicroGlut?
// 1) Transparency! Including it as a single source file makes it easy to see what it does.
// 2) Editability. Missing something? Want something to work a bit different? Just hack it in.
// 3) No obsolete stuff! The old GLUT has a lot of functions that are inherently obsolete. Avoid!

// Please note that this is still in an early stage. A lot of functionality is still missing.
// If you add/change something of interest, especially if it follows the GLUT API, please sumbit
// to me so I can consider adding that to the official version.

// 2012: Made a first draft by extracting context creation from other code.
// 130131: Fixed bugs in keyboard and update events. Seems to work pretty well! Things are missing, GL3 untested, but otherwise it runs stable with glutgears.
// 130206: Timers now tested and working. CPU load kept decent when not intentionally high.
// 130907: Bug fixes, test for non-existing gReshape, glutKeyboardUpFunc corrected.
// 130916: Fixed the event bug. Cleaned up most comments left from debugging.
// 130926: Cleaned up warnings, added missing #includes.
// 140130: A bit more cleanup for avoiding warnings (_BSD_SOURCE below).
// 140401: glutKeyIsDown and glutWarpPointer added and got an extra round of testing.
// 150205: glutRepeatingTimer new, better name for glutRepeatingTimerFunc
// Added a default glViewport call when the window is resized.
// Made a bug fix in the event processing so that mouse drag events won't stack up.
// Somewhere here I added a kind of full-screen support (but without removing window borders).
// 150216: Added proper OpenGL3 initalization. (Based on a patch by Sebastian Parborg.)
// 150223: Finally, decent handling on the GLUT configuration!
// 150227: Resize triggers an update!
// 150302: Window position, multisample, even better config

#define _BSD_SOURCE
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glext.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "MicroGlut.h"
#include <sys/time.h>
#include <unistd.h>

#ifndef M_PI
#define M_PI 3.14159265
#endif


unsigned int winWidth = 300, winHeight = 300;
unsigned int winPosX = 40, winPosY = 40;
int mode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
int gContextVersionMajor = 0;
int gContextVersionMinor = 0;

Display *dpy;
Window win;
GLXContext ctx;
char *dpyName = NULL;
int gMode; // NOT YET USED
char animate = 1; // Use for glutNeedsRedisplay?
struct timeval timeStart;
static Atom wmDeleteMessage; // To handle delete msg
char gKeymap[256];

void glutInit(int *argc, char *argv[])
{
	gettimeofday(&timeStart, NULL);
	memset(gKeymap, 0, sizeof(gKeymap));
}
void glutInitDisplayMode(unsigned int mode)
{
	gMode = mode; // NOT YET USED
}
void glutInitWindowSize(int w, int h)
{
	winWidth = w;
	winHeight = h;
}

void glutInitWindowPosition (int x, int y)
{
	winPosX = x;
	winPosY = y;
}

static void checktimers();

/*
 * Create an RGB, double-buffered window.
 * Return the window and context handles.
 */

static void
make_window( Display *dpy, const char *name,
             int x, int y, int width, int height,
             Window *winRet, GLXContext *ctxRet)
{
   int scrnum;
   XSetWindowAttributes attr;
   unsigned long mask;
   Window root;
   Window win;
   GLXContext ctx;
   XVisualInfo *visinfo;

   scrnum = DefaultScreen( dpy );
   root = RootWindow( dpy, scrnum );

// 3.2 support
//#ifdef glXCreateContextAttribsARB
	if (gContextVersionMajor > 2)
	{
// We asked for OpenGL3+, but can we do it?

		typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

		// Verify GL driver supports glXCreateContextAttribsARB()
		glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;

		// Verify that GLX implementation supports the new context create call
		if ( strstr( glXQueryExtensionsString( dpy, scrnum ),
			"GLX_ARB_create_context" ) != 0 )
		glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
			glXGetProcAddress( (const GLubyte *) "glXCreateContextAttribsARB" );

		if ( !glXCreateContextAttribsARB )
		{
			printf( "Can't create new-style GL context\n" );
		}

// We need this for OpenGL3
		int elemc;
		GLXFBConfig *fbcfg;

	   int attribs[] = { GLX_RENDER_TYPE, GLX_RGBA_BIT,
                     GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, // ?
                     GLX_RED_SIZE, 1, // 1 = prefer high precision
                     GLX_GREEN_SIZE, 1,
                     GLX_BLUE_SIZE, 1,
                     GLX_ALPHA_SIZE, 1,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None };

		int i = 12;
		if (gMode & GLUT_DOUBLE)
		{
			attribs[i++] = GLX_DOUBLEBUFFER;
			attribs[i++] = 1;
		}
		if (gMode & GLUT_DEPTH)
		{
			attribs[i++] = GLX_DEPTH_SIZE;
			attribs[i++] = 1;
		}
		if (gMode & GLUT_STENCIL)
		{
			attribs[i++] = GLX_STENCIL_SIZE;
			attribs[i++] = 8; // Smallest available, at least 8. Configurable setting needed!
		}
		if (gMode & GLUT_MULTISAMPLE)
		{
			attribs[i++] = GLX_SAMPLE_BUFFERS;
			attribs[i++] = 1;
			attribs[i++] = GLX_SAMPLES;
			attribs[i++] = 4;
		}

		fbcfg = glXChooseFBConfig(dpy, scrnum, attribs, &elemc);
		if (!fbcfg)
		{
			fbcfg = glXChooseFBConfig(dpy, scrnum, NULL, &elemc);
		}
		if (!fbcfg)
			printf("Couldn't get FB configs\n");

		int gl3attr[] =
		{
			GLX_CONTEXT_MAJOR_VERSION_ARB, gContextVersionMajor,
			GLX_CONTEXT_MINOR_VERSION_ARB, gContextVersionMinor,
//			GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
			GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
			None
		};
		ctx = glXCreateContextAttribsARB(dpy, fbcfg[0], NULL, 1, gl3attr);
		if (ctx == NULL) printf("No ctx!\n");

        visinfo = glXGetVisualFromFBConfig(dpy, fbcfg[0]);
        if (!visinfo)
            printf("Error: couldn't create OpenGL window with this pixel format.\n");

	}
	else // old style
//#endif
	{
	   int attribs[] = { GLX_RGBA,
                     GLX_RED_SIZE, 1, // 1 = prefer high precision
                     GLX_GREEN_SIZE, 1,
                     GLX_BLUE_SIZE, 1,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None,
                     None };

		int i = 7;
		if (gMode & GLUT_DOUBLE)
			attribs[i++] = GLX_DOUBLEBUFFER;
		if (gMode & GLUT_DEPTH)
		{
			attribs[i++] = GLX_DEPTH_SIZE;
			attribs[i++] = 1;
		}
		if (gMode & GLUT_STENCIL)
		{
			attribs[i++] = GLX_STENCIL_SIZE;
			attribs[i++] = 8; // Smallest available, at least 8. Configurable setting needed!
		}
    	visinfo = glXChooseVisual( dpy, scrnum, attribs );
		if (!visinfo)
		{
			printf("Error: couldn't get a visual according to settings\n");
			exit(1);
		}

		ctx = glXCreateContext( dpy, visinfo, 0, True );
		if (ctx == NULL) printf("No ctx!\n");
	}

   /* window attributes */
   attr.background_pixel = 0;
   attr.border_pixel = 0;
   attr.colormap = XCreateColormap( dpy, root, visinfo->visual, AllocNone);
   attr.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPress | ButtonReleaseMask | Button1MotionMask | PointerMotionMask;
   attr.override_redirect = 0;
   mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect;

   win = XCreateWindow( dpy, root, x, y, width, height,
		        0, visinfo->depth, InputOutput,
		        visinfo->visual, mask, &attr );

// Register delete!
	wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &wmDeleteMessage, 1); // Register

   /* set hints and properties */
      XSizeHints sizehints;
      sizehints.x = x;
      sizehints.y = y;
      sizehints.width  = width;
      sizehints.height = height;
      sizehints.flags = USSize | USPosition;
      XSetNormalHints(dpy, win, &sizehints);
      XSetStandardProperties(dpy, win, name, name,
                              None, (char **)NULL, 0, &sizehints);

   if (!ctx)
   {
      printf("Error: glXCreateContext failed\n");
      exit(1);
   }

   XFree(visinfo);

   *winRet = win;
   *ctxRet = ctx;
}

void glutCreateWindow(char *windowTitle)
{
   dpy = XOpenDisplay(NULL);
   if (!dpy)
   {
      printf("Error: couldn't open display %s\n",
	     windowTitle ? windowTitle : getenv("DISPLAY"));
   }

   make_window(dpy, windowTitle, winPosX, winPosY, winWidth, winHeight, &win, &ctx);

   XMapWindow(dpy, win);
   glXMakeCurrent(dpy, win, ctx);
}

void (*gDisplay)(void);
void (*gReshape)(int width, int height);
void (*gIdle)(void);
void (*gKey)(unsigned char key, int x, int y);
void (*gKeyUp)(unsigned char key, int x, int y);
void (*gMouseMoved)(int x, int y);
void (*gMouseDragged)(int x, int y);
void (*gMouseFunc)(int button, int state, int x, int y);


void glutReshapeFunc(void (*func)(int width, int height))
{
	gReshape = func;
}
void glutDisplayFunc(void (*func)(void))
{
	gDisplay = func;
}
void glutIdleFunc(void (*func)(void))
{gIdle = func;}

void glutKeyboardFunc(void (*func)(unsigned char key, int x, int y))
{
	gKey = func;
}
void glutKeyboardUpFunc(void (*func)(unsigned char key, int x, int y))
{
	gKeyUp = func;
}

void glutMouseFunc(void (*func)(int button, int state, int x, int y))
{gMouseFunc = func;}
void glutMotionFunc(void (*func)(int x, int y))
{gMouseDragged = func;}
void glutPassiveMotionFunc(void (*func)(int x, int y))
{gMouseMoved = func;}

char gButtonPressed[10] = {0,0,0,0,0,0,0,0,0,0};

void glutMainLoop()
{
	char buffer[10];
//	int r; // code;
	char done = 0;

	char pressed = 0;
	int i;

	XAllowEvents(dpy, AsyncBoth, CurrentTime);

	while (!done)
	{
      while (XPending(dpy) > 0)
      {
         XEvent event;
         XNextEvent(dpy, &event);

         switch (event.type)
         {
         	case ClientMessage:
         		if (event.xclient.data.l[0] == wmDeleteMessage) // quit!
         			done = 1;
	         	break;
         	case Expose:
				break; // Update event! Should do draw here.
         	case ConfigureNotify:
				if (gReshape)
	      			gReshape(event.xconfigure.width, event.xconfigure.height);
				else
				{
					glViewport(0, 0, event.xconfigure.width, event.xconfigure.height);
				}
				animate = 1;
      			break;
      		case KeyPress:
      		case KeyRelease:
		        XLookupString(&event.xkey, buffer, sizeof(buffer),
                              NULL, NULL);

      			if (event.type == KeyPress)
	      		{	if (gKey) gKey(buffer[0], 0, 0); gKeymap[(int)buffer[0]] = 1;}
	      		else
	      		{	if (gKeyUp) gKeyUp(buffer[0], 0, 0); gKeymap[(int)buffer[0]] = 0;}
      			break;
			case ButtonPress:
				gButtonPressed[event.xbutton.button] = 1;
				if (gMouseFunc != NULL)
					gMouseFunc(GLUT_LEFT_BUTTON, GLUT_DOWN, event.xbutton.x, event.xbutton.y);
				break;
			case ButtonRelease:
				gButtonPressed[event.xbutton.button] = 0;
				if (gMouseFunc != NULL)
					gMouseFunc(GLUT_LEFT_BUTTON, GLUT_UP, event.xbutton.x, event.xbutton.y);
				break;
		case MotionNotify:
				pressed = 0;
				for (i = 0; i < 5; i++)
					if (gButtonPressed[i]) pressed = 1;
					if (pressed && gMouseDragged)
						gMouseDragged(event.xbutton.x, event.xbutton.y);
					else
					if (gMouseMoved)
						gMouseMoved(event.xbutton.x, event.xbutton.y);
				break;

		default:
			break;
         }
      }

      if (animate)
      {
      	animate = 0;
		if (gDisplay)
		  	gDisplay();
		else
			printf("No display function!\n");
      }
		else
		if (gIdle) gIdle();
      checktimers();
   }

	glXMakeCurrent(dpy, None, NULL);
   glXDestroyContext(dpy, ctx);
   XDestroyWindow(dpy, win);
   XCloseDisplay(dpy);
}

void glutSwapBuffers()
{
	glXSwapBuffers(dpy, win);
}

void glutPostRedisplay()
{
	animate = 1;
}

int glutGet(int type)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_usec - timeStart.tv_usec) / 1000 + (tv.tv_sec - timeStart.tv_sec)*1000;
}

// NOTE: The timer is not designed with any multithreading in mind!
typedef struct TimerRec
{
	int arg;
	int time;
	int repeatTime;
	void (*func)(int arg);
	char repeating;
	struct TimerRec *next;
	struct TimerRec *prev;
} TimerRec;

TimerRec *gTimers = NULL;

void glutTimerFunc(int millis, void (*func)(int arg), int arg)
{
	TimerRec *t	= (TimerRec *)malloc(sizeof(TimerRec));
	t->arg = arg;
	t->time = millis + glutGet(GLUT_ELAPSED_TIME);
	t->repeatTime = 0;
	t->repeating = 0;
	t->func = func;
	t->next = gTimers;
	t->prev = NULL;
	if (gTimers != NULL)
		gTimers->prev = t;
	gTimers = t;
}

// Added by Ingemar
// void glutRepeatingTimerFunc(int millis)
void glutRepeatingTimer(int millis)
{
	TimerRec *t	= (TimerRec *)malloc(sizeof(TimerRec));
	t->arg = 0;
	t->time = millis + glutGet(GLUT_ELAPSED_TIME);
	t->repeatTime = millis;
	t->repeating = 1;
	t->func = NULL;
	t->next = gTimers;
	t->prev = NULL;
	if (gTimers != NULL)
		gTimers->prev = t;
	gTimers = t;
}

static void checktimers()
{
	if (gTimers != NULL)
	{
		TimerRec *t, *firethis = NULL;
		int now = glutGet(GLUT_ELAPSED_TIME);
		int nextTime = now + 1000; // Distant future, 1 second
		t = gTimers;
		for (t = gTimers; t != NULL; t = t->next)
		{
			if (t->time < nextTime) nextTime = t->time; // Time for the next one
			if (t->time < now) // See if this is due to fire
			{
				firethis = t;
			}
		}
		if (firethis != NULL)
		{
		// Fire the timer
			if (firethis->func != NULL)
				firethis->func(firethis->arg);
			else
				glutPostRedisplay();
		// Remove the timer if it was one-shot, otherwise update the time
			if (firethis->repeating)
			{
				firethis->time = now + firethis->repeatTime;
			}
			else
			{
				if (firethis->prev != NULL)
					firethis->prev->next = firethis->next;
				else
					gTimers = firethis->next;
                if (firethis->next != NULL)
					firethis->next->prev = firethis->prev;
				free(firethis);
			}
		}
		// Otherwise, sleep until any timer should fire
        if (!animate)
			if (nextTime > now)
            {
		usleep((nextTime - now)*1000);
            }
	}
    else
// If no timer and no update, sleep a little to keep CPU load low
        if (!animate)
            usleep(10);
}

void glutInitContextVersion(int major, int minor)
{
	gContextVersionMajor = major;
	gContextVersionMinor = minor;
}

// Based on FreeGlut glutWarpPointer
/*
 * Moves the mouse pointer to given window coordinates
 */
void glutWarpPointer( int x, int y )
{
    if (dpy == NULL)
    {
      fprintf(stderr, "glutWarpPointer failed: MicroGlut not initialized!\n");
    	return;
    }

    XWarpPointer(
        dpy, // fgDisplay.Display,
        None,
        win, // fgStructure.CurrentWindow->Window.Handle,
        0, 0, 0, 0,
        x, y
    );
    /* Make the warp visible immediately. */
    XFlush( dpy );
//    XFlush( fgDisplay.Display );
}

char glutKeyIsDown(unsigned char c)
{
	return gKeymap[(unsigned int)c];
}



// These were missing up to 150205

void glutReshapeWindow(int width, int height)
{
	XResizeWindow(dpy, win, width, height);
}
void glutPositionWindow(int x, int y)
{
	XMoveWindow(dpy, win, x, y);
}
void glutSetWindowTitle(char *title)
{
	XStoreName(dpy, win, title);
}

// Not complete full screen mode yet since the window frame and menu are not hidden yet

char gFullScreen = 0;
unsigned int savedHeight, savedWidth;
int savedX, savedY;

void glutFullScreen()
{
	gFullScreen = 1;


	Drawable d;
	unsigned int a, b;

	XGetGeometry(dpy, win, &d, &savedX, &savedY, &savedWidth, &savedHeight, &a, &b);

    int scrnum = DefaultScreen(dpy);
    int width = DisplayWidth( dpy, scrnum );
    int height = DisplayHeight( dpy, scrnum );

	XMoveResizeWindow(dpy, win, 0, 0, width, height);
}

void glutExitFullScreen()
{
	gFullScreen = 0;
	XMoveResizeWindow(dpy, win, savedX, savedY, savedWidth, savedHeight);
}

void glutToggleFullScreen()
{
	if (gFullScreen)
		glutExitFullScreen();
	else
		glutFullScreen();
}
