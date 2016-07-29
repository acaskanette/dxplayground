// DirectX Tutorial from directxtutorial.com
// Modified by: Andy Caskanette


// Include the basic windows header file
#include <windows.h>
#include <windowsx.h>
// DirectX Includes
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// Include lib files
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

//Global declarations
IDXGISwapChain *swapchain;			// Swap chain interace
ID3D11Device *dev;					// Direct3D device interface
ID3D11DeviceContext *devContext;	// Direct3D device context
ID3D11RenderTargetView *backBuffer; // Where to render to


// Predeclarations

//WindowProc callback
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam,	LPARAM lParam);
// Initialize D3D
void InitD3D(HWND hWnd);
// closes Direct3D and releases memory
void CleanD3D(void);
void RenderFrame(void);

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

	// Set up and Initialize D3D
	InitD3D(hWnd);
	
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
			RenderFrame();
		}

	}

	 /////////////////////
	// Main loop ends: //
	////////////////////


	// Clean up and shutdown D3D
	CleanD3D();

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

// Initializes Direct3D
void InitD3D(HWND hWnd) {

	// Swap chain descriptor
	DXGI_SWAP_CHAIN_DESC scd;
	// Initialize scd
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Fill the scd with initial variables
	scd.BufferCount = 1;									// number of back buffers in swap chain
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// colour format
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// swap chain usage (back buffer in this case)
	scd.OutputWindow = hWnd;								// handle to the window for D3D to draw in
	scd.SampleDesc.Count = 4;								// amount of anti-aliasing
	scd.Windowed = TRUE;									// windowed or fullscreen?

	// Create a device, context and swap chain
	D3D11CreateDeviceAndSwapChain(
		NULL,												// What graphics adapter D3D should use (NULL means DXGI decides which)
		D3D_DRIVER_TYPE_HARDWARE,							// What driver type to use, in this case "Use the GPU"
		NULL,												// Sets software, not used
		NULL,												// Flags for singlethreading, debugging, reference mode, Direct2D integration	
		NULL,												// List of feature levels you expect D3D top work with
		NULL,												// How many feature levels are in the above list
		D3D11_SDK_VERSION,									// SDK version (for compatability)
		&scd,												// Give it the parameters we just set up for the scd
		&swapchain,											// and the swapchain itself
		&dev,												// and the device ...
		NULL,												// callback variable for what feature level the device can handle
		&devContext											// ... and the device context
	);

	// Get the address of backbuffer
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// Using backbuffer, create a render target
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backBuffer);
	pBackBuffer->Release();

	// Set the Render target to be the back buffer
	devContext->OMSetRenderTargets(1, &backBuffer, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devContext->RSSetViewports(1, &viewport);		// activates the list of viewport structs, in our case only one


}

// Cleans up and closes D3D and COM
void CleanD3D() {

	// Close and release all objects
	swapchain->Release();
	backBuffer->Release();
	dev->Release();
	devContext->Release();

}


// Renders a frame to the backbuffer
void RenderFrame(void) {

	// Clear back buffer to a given colour (green)
	devContext->ClearRenderTargetView(backBuffer, D3DXCOLOR(0.0f, 0.4f, 0.0f, 1.0f));
	
	// Render to back buffer here
	// ....

	// Switch buffers
	swapchain->Present(0, 0);

}