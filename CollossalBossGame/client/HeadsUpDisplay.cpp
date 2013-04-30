#include "HeadsUpDisplay.h"
#include "ConfigurationManager.h"

HeadsUpDisplay::HeadsUpDisplay(LPDIRECT3DDEVICE9 direct3dDevice)
{
	hudTopX = CM::get()->find_config_as_int("HUD_TOP_X");
	hudTopY = CM::get()->find_config_as_int("HUD_TOP_Y");


	D3DXCreateSprite(direct3dDevice,&sprite);
	D3DXCreateSprite(direct3dDevice,&sprite1);
	D3DXCreateSprite(direct3dDevice,&sprite2);
	
	D3DXCreateTextureFromFile(direct3dDevice,   //Direct3D Device
                             "res/nebula.jpg",       //File Name
                             &g_texture);    //Texture handle
	D3DXCreateTextureFromFile(direct3dDevice,   //Direct3D Device
                            "res/gameover.png",       //File Name
                            &test1_texture);    //Texture handle

	D3DXCreateFont(	direct3dDevice,     //D3D Device
				    22,                       //Font height
					0,					      //Font width
					FW_NORMAL,                //Font Weight
					1,                        //MipLevels
					false,                    //Italic
					DEFAULT_CHARSET,          //CharSet
					OUT_DEFAULT_PRECIS,       //OutputPrecision
					ANTIALIASED_QUALITY,      //Quality
					DEFAULT_PITCH|FF_DONTCARE,//PitchAndFamily
					"Georgia",                //pFacename,
					&direct3dText);     //ppFont
	D3DXCreateLine(direct3dDevice, &healthLine);
	D3DXCreateLine(direct3dDevice, &monsterLine);
	D3DXCreateLine(direct3dDevice, &backgroundLine);

	initTime = clock();
}


HeadsUpDisplay::~HeadsUpDisplay(void)
{
	sprite->Release();
	g_texture->Release();
	sprite1->Release();
	sprite2->Release();
    direct3dText->Release(); // close and release the Text
	healthLine->Release();
	backgroundLine->Release();
	monsterLine->Release();
}

void HeadsUpDisplay::displayText(string hudText, string monsterHUDText)
{
	RECT font_rect;
	RECT monstr_rect;

	//#define SCREEN_WIDTH 1024
    //#define SCREEN_HEIGHT 768 

	SetRect(&font_rect,
			hudTopX,
			hudTopY,
			1024,
			768);

	SetRect(&monstr_rect,
			hudTopX,
			hudTopY + 100,
			1024,
			768);

	sprite1->Begin(D3DXSPRITE_ALPHABLEND);
    direct3dText->DrawText(sprite1,        //pSprite
						   hudText.c_str(),	 //pString
                           -1,          //Count
                           &font_rect,  //pRect
                           DT_LEFT|DT_NOCLIP,//Format,
                           0xFFFFFFFF);//0xFF000000); //Color

    direct3dText->DrawText(sprite1,        //pSprite
						   monsterHUDText.c_str(),	 //pString
                           -1,          //Count
                           &monstr_rect,  //pRect
                           DT_LEFT|DT_NOCLIP,//Format,
                           0xFFFFFFFF);//0xFF000000); //Color
	sprite1->End();

}

void HeadsUpDisplay::displayHealthBars(int playerHealth, int monsterHealth)
{

	D3DXVECTOR2 blines[] = {D3DXVECTOR2(10.0f, 40.0f), D3DXVECTOR2(110.0f, 40.0f)};
	backgroundLine->SetWidth(15.0f);
	backgroundLine->Draw(blines, 2, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 hlines[] = {D3DXVECTOR2(10.0f, 40.0f), D3DXVECTOR2(playerHealth + 10.f , 40.0f)};
	healthLine->SetWidth(15.0f);
	healthLine->Draw(hlines, 2, D3DCOLOR_ARGB(255, (int)(255.0 * (100.0 - playerHealth) / 100.0), (int)(255.0 * playerHealth / 100.0), 0));

    blines[0] = D3DXVECTOR2(10.0f, 140.0f); blines[1] = D3DXVECTOR2(110.0f, 140.0f);
	backgroundLine->SetWidth(15.0f);
	backgroundLine->Draw(blines, 2, D3DCOLOR_ARGB(255, 0, 0, 0));

	D3DXVECTOR2 mlines[] = {D3DXVECTOR2(10.0f, 140.0f), D3DXVECTOR2(monsterHealth + 10.f , 140.0f)};
	monsterLine->SetWidth(15.0f);
	monsterLine->Draw(mlines, 2, D3DCOLOR_ARGB(255, (int)(255.0 * (100.0 - monsterHealth) / 100.0), (int)(255.0 * monsterHealth / 100.0), 0));

	if(playerHealth == 0) diplayGameOver();
}

void HeadsUpDisplay::displayBackground()
{
	D3DXVECTOR3 pos;

	pos.x=0.0f;
	pos.y=0.0f;
	pos.z=1.0f;

	D3DXVECTOR2 spriteCentre=D3DXVECTOR2(1920.0f/2, 1920.0f/2);
	D3DXVECTOR2 trans=D3DXVECTOR2(-700.0f,-700.0f);
	float rotation=(clock()-initTime)/100000.0f;
	D3DXMATRIX mat;
	D3DXVECTOR2 scaling(1.f,1.f);
	D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
	sprite->SetTransform(&mat);

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	sprite->Draw(g_texture,NULL,NULL,&pos,0xFFFFFFFF);
	sprite->End();

}

void HeadsUpDisplay::diplayGameOver()
{

	D3DXVECTOR3 test1;

	test1.x= 0; //CM::get()->find_config_as_float("TEST1_X");
	test1.y= 0; //CM::get()->find_config_as_float("TEST1_Y");
	test1.z= 0; //CM::get()->find_config_as_float("TEST1_Z");
	
	sprite2->Begin(D3DXSPRITE_ALPHABLEND);
	sprite2->Draw(test1_texture,NULL,NULL,&test1,0xFFFFFFFF);
	sprite2->End();
}	