#include "Window.h"
#include "../lib/Helper.hpp"

Window::Window() {
	_instanceHandle = (HINSTANCE)GetModuleHandle(NULL);
	_previousInstanceHandle = NULL;
	_cmdLine = GetCommandLine();
	_cmdShow = 1;

	_windowHandle = NULL;
}

Window::~Window() {}

int Window::createWindow(const char* windowName, glm::vec2 size, glm::vec2 position, const char* icon, const char* cursor, int style, WNDPROC messageCallback) {
	const wchar_t* tempWindowName = Engine::GetLCWSTR(windowName);
	const wchar_t* tempWindowClassName = Engine::GetLCWSTR(windowName);
	const wchar_t* tempCursor = Engine::GetLCWSTR(cursor);
	const wchar_t* tempIcon = Engine::GetLCWSTR(icon);

	_windowInfo.cbSize = sizeof(WNDCLASSEX);
	_windowInfo.style = CS_HREDRAW | CS_VREDRAW;
	_windowInfo.lpfnWndProc = messageCallback;
	_windowInfo.cbClsExtra = 0;
	_windowInfo.cbWndExtra = 0;
	_windowInfo.hInstance = _instanceHandle;
	_windowInfo.hIcon = (HICON)LoadImage(_instanceHandle, tempIcon, IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
	_windowInfo.hCursor = (HCURSOR)LoadImage(NULL, tempCursor, IMAGE_ICON, ::GetSystemMetrics(SM_CXCURSOR), ::GetSystemMetrics(SM_CYCURSOR), LR_LOADFROMFILE);;
	_windowInfo.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	_windowInfo.lpszMenuName = NULL;
	_windowInfo.lpszClassName = tempWindowClassName;
	_windowInfo.hIconSm = (HICON)LoadImage(_instanceHandle, tempIcon, IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);

	if (!RegisterClassEx(&_windowInfo)) {
		MessageBox(NULL, L"Window registration failed!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	long windowStyle = NULL;

	if (style & ENGINE_WINDOWED) {
		windowStyle = WS_OVERLAPPEDWINDOW;
	}
	else if (style & ENGINE_BORDERLESS) {
		windowStyle = WS_POPUP;
	}
	else if (style & ENGINE_FULLSCREEN) {
		windowStyle = WS_POPUP;
		_cmdShow = SW_MAXIMIZE;
	}
	else {
		MessageBox(NULL, L"Could not determinate window style!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	_windowHandle = CreateWindowEx(NULL,							/*The extended window style of the window being created.*/
		tempWindowClassName,			/*A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function.*/
		tempWindowName,					/*The window name. If the window style specifies a title bar, the window title pointed to by lpWindowName is displayed in the title bar.*/
		windowStyle,					/*The style of the window being created.*/
		position.x, position.y,			/*Window position.*/
		size.x, size.y,					/*Window size.*/
		NULL,							/*A handle to the parent or owner window of the window being created.*/
		NULL,							/*A handle to a menu, or specifies a child - window identifier, depending on the window style.*/
		_instanceHandle,				/*A handle to the instance of the module to be associated with the window.*/
		this);							/*Pointer to a value to be passed to the window through the CREATESTRUCT structure (lpCreateParams member) pointed to by the lParam param of the WM_CREATE message.*/

	if (_windowHandle == NULL) {
		MessageBox(NULL, L"Window creation failed!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	ShowWindow(_windowHandle, _cmdShow);
	UpdateWindow(_windowHandle);

	return 1;
}

int Window::InitOpenGL() {
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);										/*Specifies the size of this data structure. This value should be set to sizeof(PIXELFORMATDESCRIPTOR).*/
	pfd.nVersion = 1;																	/*Specifies the version of this data structure. This value should be set to 1.*/
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;			/*A set of bit flags that specify properties of the pixel buffer.*/
	pfd.iPixelType = PFD_TYPE_RGBA;														/*Specifies the type of pixel data.*/
	pfd.cColorBits = 32;																	/*Specifies the number of color bitplanes in each color buffer.*/
	pfd.cDepthBits = 32;																	/*Specifies the depth of the depth (z-axis) buffer.*/
	pfd.iLayerType = PFD_MAIN_PLANE;														/*Ignored. Earlier implementations of OpenGL used this member, but it is no longer used.*/

	if (!(_deviceContextHandle = GetDC(_windowHandle))) {
		MessageBox(NULL, L"Could not create a GL device context!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	int pixelFormat = ChoosePixelFormat(_deviceContextHandle, &pfd);

	if (pixelFormat == 0) {
		MessageBox(NULL, L"Could not determine pixel format!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	if (!SetPixelFormat(_deviceContextHandle, pixelFormat, &pfd)) {
		MessageBox(NULL, L"Could not set pixel format!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	if (!(_glRenderingContextHandle = wglCreateContext(_deviceContextHandle))) {
		MessageBox(NULL, L"Could not create a gl rendering context!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	if (!wglMakeCurrent(_deviceContextHandle, _glRenderingContextHandle)) {
		MessageBox(NULL, L"Can't activate the gl rendering context!", L"ERROR!", MB_ICONERROR | MB_OK);
	}

	if (glewInit()) {
		MessageBox(NULL, L"Could not initialize GLEW!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}

	return 1;
}

int Window::Uninit() {
	wglMakeCurrent(_deviceContextHandle, NULL);
	ReleaseDC(_windowHandle, _deviceContextHandle);
	wglDeleteContext(_glRenderingContextHandle);

	_windowHandle = NULL;

	return 1;
}

int Window::getMessage() {
	if (PeekMessage(&_message, NULL, 0, 0, PM_REMOVE) == -1) {
		MessageBox(NULL, L"Message Error!", L"ERROR!", MB_ICONERROR | MB_OK);
		return -1;
	}
	else {
		TranslateMessage(&_message);
		DispatchMessage(&_message);
	}

	return _message.wParam;
}

void Window::SetSize(glm::vec2 newSize) {
	RECT rect;
	GetWindowRect(_windowHandle, &rect);
	SetWindowPos(_windowHandle, NULL, rect.left, rect.top, newSize.x, newSize.y, SWP_SHOWWINDOW);
}

void Window::SetPosition(glm::vec2 newPosition) {
	RECT rect;
	GetWindowRect(_windowHandle, &rect);
	SetWindowPos(_windowHandle, NULL, newPosition.x, newPosition.y, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
}

glm::vec2 Window::GetSize() {
	RECT rect;
	GetWindowRect(_windowHandle, &rect);
	return glm::vec2(rect.right - rect.left, rect.bottom - rect.top);
}

glm::vec2 Window::GetPosition() {
	RECT rect;
	GetWindowRect(_windowHandle, &rect);
	return glm::vec2(rect.left, rect.top);
}

glm::vec2 Window::GetMousePosition() {
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(_windowHandle, &p);
	return glm::vec2(p.x, p.y);
}

glm::vec2 Window::SetMousePosition(glm::vec2 newPos) {
	glm::vec2 mousePos = GetMousePosition();
	SetCursorPos(newPos.x, newPos.y);
	return mousePos - newPos;
};

void Window::HideCursor(bool hide) {
	ShowCursor(!hide);
}

void Window::Resize() {
	RECT rect;
	GetWindowRect(_windowHandle, &rect);
	glViewport(0, 0, rect.right - rect.left, rect.bottom - rect.top);
	//glFrustum(left * ratio, right * ratio, bottom, top, nearClip, farClip);
}

//Additional functions
void Window::ConfineMouse() {
	RECT rect;
	GetClientRect(_windowHandle, &rect);

	POINT pt = { rect.left, rect.top };
	POINT pt2 = { rect.right, rect.bottom };
	ClientToScreen(_windowHandle, &pt);
	ClientToScreen(_windowHandle, &pt2);
	SetRect(&rect, pt.x, pt.y, pt2.x, pt2.y);

	ClipCursor(&rect);
}

void Window::ReleaseMouse() {
	ClipCursor(NULL);
}