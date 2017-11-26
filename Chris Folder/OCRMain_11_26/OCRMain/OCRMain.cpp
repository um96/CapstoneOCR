#include <windows.h>
#include <Commdlg.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <string>


#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>


using namespace std;


OPENFILENAME ffc; //File_For_conversion
wchar_t fnc[100]; //File_name_converter
				  //BACKGROUND BITMAP "ocrbg.bmp";

tesseract::TessBaseAPI *api;



/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {


	enum { ID_QUIT, ID_NEXT }; //Button IDs


	switch (Message) {

		/* Upon destruction, tell the main thread to stop */
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	case WM_COMMAND: {
		if (LOWORD(wParam) == ID_QUIT) {
			//Quit was pressed, exit the program.
			DestroyWindow(hwnd);
		}
		if (LOWORD(wParam) == ID_NEXT) {

			ZeroMemory(&ffc, sizeof(ffc));
			ffc.lStructSize = sizeof(ffc);
			ffc.hwndOwner = NULL;
			ffc.lpstrFile = fnc;
			ffc.lpstrFile[0] = '\0';
			ffc.nMaxFile = sizeof(fnc);
			ffc.lpstrFilter = _T(".pdf, .jpg , .jpeg, .png, .bmp\0*.pdf;*.jpg;*.jpeg;*.png;*.bmp\0\0");
			ffc.nFilterIndex = 1;
			ffc.lpstrFileTitle = NULL;
			ffc.nMaxFileTitle = 0;
			ffc.lpstrInitialDir = NULL;
			ffc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			GetOpenFileName(&ffc);

			if (_tcslen(ffc.lpstrFile) == 0) {
				break;
			}

			// Run OCR software using the file. . . 

			char buffer[500];
			wcstombs(buffer, ffc.lpstrFile, 500);

			// PDF file
			if (strcmp(buffer + ffc.nFileExtension, "pdf") == 0) {

				char cmd[500];

				strcpy(cmd, "PDFToImage.exe ");
				strcat(cmd, buffer);
				strcat(cmd, " output.png");
				system(cmd);

				strcpy(buffer, "output.png");
			}

			char *outText;

			// Open input image with leptonica library
			Pix *image = pixRead(buffer);

			api->SetImage(image);
			// Get OCR result

			outText = api->GetUTF8Text();

			ofstream outFile;
			outFile.open("output.txt");
			outFile << outText;
			outFile.close();

			// Delete temp image file
			if (strcmp(buffer + ffc.nFileExtension, "pdf") == 0) {
				char cmd[500];

				strcpy(cmd, "del ");
				strcat(cmd, buffer);
				strcat(cmd, " output.png");
				system(cmd);
			}

			// Print result of detection
			//cout << outText;

			// Release variable & memory
			api->End();
			pixDestroy(&image);

			MessageBox(NULL, _T("File created in directory "), _T("Finished"), MB_OK);
			DestroyWindow(hwnd);
		}
		break;
	}

	case WM_CLOSE: {
		DestroyWindow(hwnd);
	}

				   /* This section allows for printed text to appear on the window */
	case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);


		TextOut(
			hdc,
			10,//Horizontality, higher number => goes East
			10, //Verticality, higher number => goes South
			_T("Welcome to the OSR Converter"),
			28 //Size of the text above, in char, including space
		);


		TextOut(
			hdc,
			10,//H
			25, //V
			_T("Made by Anne Liang, Christopher Eidangbe, Ryan McCormick and Uros Milanovic"),
			76 //Size of the text above, in char, including space
		);

		TextOut(
			hdc,
			10,//H
			100, //V
			_T("Select an image by clicking 'Begin'"),
			35 //Size of the text above, in char, including space
		);



		EndPaint(hwnd, &ps);
		break;
	}
	case WM_CREATE: {

		//CreateWindow() //make listbox


		CreateWindow(TEXT("button"), TEXT("Quit"),
			WS_VISIBLE | WS_CHILD,
			20, // Horizontality, higher number => goes East
			400,// Verticality, higher number => goes South
			80, //Size(s) of button. Leave as is
			25,
			hwnd, (HMENU)ID_QUIT, NULL, NULL);

		CreateWindow(TEXT("button"), TEXT("Begin"),
			WS_VISIBLE | WS_CHILD,
			500, //H
			400, //V
			80,
			25,
			hwnd, (HMENU)ID_NEXT, NULL, NULL);
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

	api = new tesseract::TessBaseAPI();

	if (api->Init(NULL, "eng")) {
		//fprintf(stderr, "Could not initialize tesseract ORC.\n");
		//exit(1);

		MessageBox(NULL, _T("Could not initialize tesseract OCR"), _T("File Error"), MB_OK);

		return 0;
		//Check with wrong input to see what happens
	}

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.hbrBackground = CreatePatternBrush((HBITMAP)LoadImage(0, _T("ocrbg.bmp"), // 0,_T(
		IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE));
	wc.lpszClassName = _T("WindowClass");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, _T("WindowClass"), _T("OSR Project"), WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, /* x */
		CW_USEDEFAULT, /* y */
		640, /* width */
		480, /* height */
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
	This is the heart of our program where all input is processed and
	sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
	this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}

