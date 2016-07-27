// DirectX Tutorial from directxtutorial.com
// Modified by: Andy Caskanette


// Include the basic windows header file
#include <windows.h>
#include <windowsx.h>


// Predeclaration of WindowProc callback
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);

// Entry point
int WINAPI WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	
	HWND hWnd;			// Handle for window, filled by a function
	WNDCLASSEX wc;		// Holds information for the window class
	
	ZeroMemory(&wc, sizeof(WNDCLASSEX));	// Clear out the window class for use

	// Default window class parameters
	wc.cbSize = sizeof(WNDCLASSEX);				// Size of parameters in memory
	wc.style = CS_HREDRAW | CS_VREDRAW;			// Windows style: redraw when moved vertically or horizontally
	wc.lpfnWndProc = WindowProc;				// What callback processes a window procedure
	wc.hInstance = hInstance;					// Handle for window instance
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	// Loads up mouse cursor image
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;	// Handle for the Brush background (fill colour)
	wc.lpszClassName = L"DXWindow1";			// Define the window class by name

	// Register the window class
	RegisterClassEx(&wc);

	// Adjusting window for workable client space
	RECT wr = { 0, 0, 800, 600 };						// Set the window size we want for the working space
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);	// Adjust the window size to include borders/menus if applicable

	// Create the window
	hWnd = CreateWindowEx(NULL,
		L"DXWindow1",							// Name of window class used
		L"Our First Windowed Program",			// Title (bar) text
		WS_OVERLAPPEDWINDOW,					// Window style
		300,									// x-position to create the window
		300,									// y-position to create the window
		wr.right - wr.left,						// Calculated window width
		wr.bottom - wr.top,						// Calculated window height
		NULL,									// Handle to parent window, NULL in our case
		NULL,									// Handle to the menu, NULL in our case
		hInstance,								// Handle to this application
		NULL);									// Only used with multiple windows, NULL

	// Show the window on the screen
	ShowWindow(hWnd, nCmdShow);

	
	////////////////////////
	// Main loop begins: //
	//////////////////////

	// Holds Windows event messages
	MSG msg;

	// Infinite loop
	while (true)
	{		
		// Check for a message in the queue, storing the result in 'msg' if there is one
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Handle Message
			TranslateMessage(&msg);		// Translate keystroke messages into the right format		
			DispatchMessage(&msg);		// Sends the message to the WindowProc callback function

			// Break infinite loop on Quit
			if (msg.message == WM_QUIT)
				break;
			
		}

		else
		{
			// Call game logic in here
		}

	}

	 /////////////////////
	// Main loop ends: //
	////////////////////

	// Upon completion, return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// Message handler callback for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
	switch (message)	// Determine the message given
	{		
		case WM_DESTROY: // occurs when the window is closed		
			PostQuitMessage(0); 	// Close the application entirely
			return 0;				// Message was handled (Quit gracefully)
			break;	
	}

	// Handle any uncaught messages
	return DefWindowProc(hWnd, message, wParam, lParam);

}