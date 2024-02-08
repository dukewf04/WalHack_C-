#pragma once

//#include <olectl.h> // Äëÿ saveBitmap
#include "BaseClasses.h"

bool HERASE = false;

D3DVIEWPORT9 Viewport;

//----------------------------------------------------


template <typename t>
static bool is_valid_ptr(t* ptr)
{
	return (ptr) ? true : false;
}

struct CALLFUNC {

	bool bWH = false;
}func;




bool elimination_team(i_actor* m_actor, i_actor* p_actor)
{
	return ((m_actor->get_team_id() != p_actor->get_team_id() || p_actor->get_team_id() == 0) && (p_actor != m_actor));
}

void set_silhouettes(i_entity_proxy* p_render_proxy, float r, float g, float b, float a)
{
	int bb = 12;
	if (p_render_proxy)
	{
		p_render_proxy->set_render_flags(0x80018);
		p_render_proxy->set_hudsilhouettes_params(r, g, b, a);
	}
}

void create_silhouettes()
{
	int a = 12;
	SSystemGlobalEnvironment* m_global = SSystemGlobalEnvironment::GetInstance();
	if (!is_valid_ptr(m_global))
		return;

	IGameFramework* m_game_framework = IGameFramework::GetGameFramework();
	if (!is_valid_ptr(m_game_framework))
		return;

	i_entity_system* m_entity_system = m_global->get_entity_system();
	if (!is_valid_ptr(m_entity_system))
		return;

	i_actor_system* m_actor_system = m_game_framework->get_actor_system();
	if (!is_valid_ptr(m_actor_system))
		return;
	int asdfsdf = 12;
	i_actor* m_actor;
	if (m_game_framework->get_client_actor(&m_actor))
	{
		i_entity_it* p_entity_iteartor = m_entity_system->get_entity_iterator();
		while (i_entity* p_entity = p_entity_iteartor->entity_next())
		{
			if (i_actor* p_actor = m_actor_system->get_actor(p_entity->get_entity_id()))
			{
				if (!elimination_team(p_actor, m_actor))
					continue;

				func.bWH ? set_silhouettes(p_actor->get_entity()->get_proxy(e_entity_proxy::entity_proxy_render), 209, 205, 205, 0.f) : set_silhouettes(p_actor->get_entity()->get_proxy(e_entity_proxy::entity_proxy_render), 0.0f, 0.0f, 0.0f, 0.0f);
			}
		}
	}//1.f, 0.30f, 0.f, 0.f
}



///////////////////////////////// BitBlt ///////////////////////////////////////////////
/*
bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);

bool screenCapturePart(int x, int y, int w, int h, LPCSTR fname) {
	HDC hdcSource = GetDC(NULL);
	HDC hdcMemory = CreateCompatibleDC(hdcSource);

	int capX = GetDeviceCaps(hdcSource, HORZRES);
	int capY = GetDeviceCaps(hdcSource, VERTRES);

	HBITMAP hBitmap = CreateCompatibleBitmap(hdcSource, w, h);
	HBITMAP hBitmapOld = (HBITMAP)SelectObject(hdcMemory, hBitmap);

	BitBlt(hdcMemory, 0, 0, w, h, hdcSource, x, y, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hdcMemory, hBitmapOld);

	DeleteDC(hdcSource);
	DeleteDC(hdcMemory);

	HPALETTE hpal = NULL;
	if (saveBitmap(fname, hBitmap, hpal)) return true;
	return false;
}



bool saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
	bool result = false;
	PICTDESC pd;

	pd.cbSizeofstruct = sizeof(PICTDESC);
	pd.picType = PICTYPE_BITMAP;
	pd.bmp.hbitmap = bmp;
	pd.bmp.hpal = pal;

	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
		reinterpret_cast<void**>(&picture));

	if (!SUCCEEDED(res))
		return false;

	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	HANDLE file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (!SUCCEEDED(res) || !file)
	{
		stream->Release();
		picture->Release();
		return false;
	}

	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);
	LPVOID data = GlobalLock(mem);

	DWORD bytes_written;

	result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
	result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

	GlobalUnlock(mem);
	CloseHandle(file);

	stream->Release();
	picture->Release();

	return result;
}
*/

