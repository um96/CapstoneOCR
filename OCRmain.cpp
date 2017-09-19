#include <windows.h>
#include <string>

using namespace std;


/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		/* This section allows for printed text to appear on the window */
		case WM_PAINT: {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint (hwnd, &ps);
			TextOut (
				hdc,
				10,//Horizontality, higher number => goes East
				10, //Verticality, higher number => goes South
				"Welcome to the OSR Converter", 
				28 //Size of the text above, in char, including space
				);
				
				
			TextOut (
				hdc,
				10,//H
				50, //V
				"Made by Uros, Anne, Ryan and Christopher", 
				41 //Size of the text above, in char, including space
				);	
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_CREATE: {
			CreateWindow(TEXT("button"), TEXT("Quit"),    
		             WS_VISIBLE | WS_CHILD ,
		             20, // Horizontality, higher number => goes East
					 400,// Verticality, higher number => goes South
					 80, //Size(s) of button. Leave as is
					 25,
		             hwnd, (HMENU) 1, NULL, NULL);    

	    	CreateWindow(TEXT("button"), TEXT("Next"),    
		             WS_VISIBLE | WS_CHILD ,
		             500, //H
					 400, //V
					 80,
					 25,        
		             hwnd, (HMENU) 2, NULL, NULL);  
			break;
		}
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}



/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	wc.hInstance	 = hInstance;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","OSR Project",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL,NULL,hInstance,NULL);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
