#include<graphics.h>
#include<conio.h>
#include<stdio.h>
#include<mmsystem.h>
#include<Windows.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<iostream>
#include<string>
#include<math.h>
#pragma comment(lib,"Winmm.lib")
//#pragma warning(default: 4996)
#define Sc_Width 800
#define Sc_High 600
#define Rain_Width 1000
#define Rain_High 800
#define Rain_Size 25
#define DEF_RAIN_NUM 91
#define Particle_Num 800
#define friction 0.96f
struct Particle {
	float r;
	float par_x, v_x;
	float par_y, v_y;
	COLORREF color;
};
struct Skill {
	clock_t start = 0, end = 0,start1 = 0,start2 = 0,start3 = 0;
	clock_t Start[8];
	int timeInterval[12] = { 0 };
	float CD;
	float X = 0, Y = 0;
	float X2 = 0, Y2 = 0;
	float x[8], y[8];
	float intervalx = 0, intervaly = 0;
	int maxPic;
	int isclick = 0;
	float timeleap;
	float v;
	int isClick = 0;
	int AttackRange;
	int i = 0;
	int maxBullet;
};
typedef struct _ZH {
	clock_t start = 0, end = 0;
	float timeLeap[1];
	int i = 0;
	int maxPic[1];
	int DifferentStatus = 0;
	int type = 0;
	float X;
	float Y;
	int width[1];
	int high[1];
}ZH;
typedef struct _O{
	int health;
	clock_t start = 0, end = 0, start1 = 0,start2 = 0;
	int timeInterval[3] = { 0 };
	float timeLeap[9];
	int i = 0 ;
	int maxPic[9];
	float speed;
	int DifferentStatus = 0;
	struct Skill skill;
	int hPlant = -1;
	int type = 0;
	float X;
	float Y;
	int isDying = 0;
	int width[9];
	int high[9];
	int isComplex;
	int isAttack = 0;
	int randomStatus = 0;
	float TimetoEffect = 0;
	struct _O* next = NULL;
	ZH Zombie_head;
}OP, Zombie;
typedef struct _L{
	float progress = 0;
	int checkpoint = 1;
	float timeLeap = 17;
	int cnt = 0;
	int cWin = 0;
	int num = 1;
	int pause = 0;
}Level;
typedef struct _CA {
	int health;
	int nSun;
	float uCD;
	float eCD = 0;
	int nPlant = -1;
	int cChoose = 1;
	clock_t start = 0, end = 10000;
	struct _CA* next;
}Card;
int mouseX, mouseY;
int preMouseX, preMouseY;
int isMouseDown = 0;
int r[3][4] = { {350,300,650,340},{350,400,650,440},{350,500,650,540} };
int r1[3][4] = { {398,300,602,350},{398,380,522,430},{398,460,802,510} };
int rCard[10][4], rOP[45][4];
int LogInStatus = 0;
int GameStatus = -1;
int MusicStatus = 0;
int MaxZombie = 0;
float pauseX = 193, pauseY = 56;
float preX = 0, preY = 0;
OP grid[9][5];
char name[10] = { '\0'}, code[20] = {'\0'}, keyWord[6] = {'\0'};
char CreateRandomNum();
void LogIn();
void CharRain();
void CharFall();
void InteractParticle();
void bkGradient();
void MouseOperation();
void delay(DWORD ms);
void BeginPage1();
void BeginPage2(int status);
int BeginPage1Judge(int x, int y);
int BeginPage2Judge(int x, int y);
void Encrypt(char b[6], char text[30], char Etext[30]);
void search(int i[2], char text, char a[5][5]);
void startup();
void show();
void updateWithoutInput();
void updateWithInput();
void PlantUpdateWithTime(OP* plant, int i, int j);
void gameover();
void LoseGame();
void addChooser(int event, Card C);
int deleteChooser(int event, Card* C);
int getChooserNum();
void LevelShow();
void PauseShow();
void addZombie(int num,int type = 0);
Zombie* CreateZombie(int num);
void DeleteZombie();
void SunFall(Skill *sun,int *SunNum,int top,int bottom,float a);
void MainMenu();
void delay(DWORD ms);
int BoxJudge(int x, int y, int rCard[10][4]);
int OPJudge(int x, int y, int OP[45][4]);
void ChoosePage();
void StartLevel();
void MidLevel();
void EndLevel();
void WinLevel();
int ChoosePageJudge(int x,int y,int rChoose[48][4]);
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color);
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha);
IMAGE StartScreen,map[2];
IMAGE _choosepage_;
IMAGE plantbox;
IMAGE _P_[48][3][3], _p_[48][2];
IMAGE _c_[48][2];
IMAGE _Sun_[2];
IMAGE mine1;
IMAGE _Bullet_PeaNormal[2];
IMAGE _StartLevel_[3];
IMAGE _Progress_[4];
IMAGE _MidLevel_;
IMAGE _EndLevel_;
IMAGE _Money_;
IMAGE _EndDraw_;
IMAGE _Z_[18][3][3][9];
IMAGE _Z_H[2];
IMAGE _shadow_;
IMAGE _shovel_;
IMAGE _shovelSlot_;
IMAGE _car_;
IMAGE plantboxM;
IMAGE _pause_;
IMAGE _bkToGame_;
IMAGE _restart_;
IMAGE _BkToMenu_;
IMAGE _Lose_;
Level level;
OP plant[5][9];
Card* _card = NULL;
Zombie* _Zom = NULL;
Card _card_[10];
ZH car[5];
struct Skill sun;
int MaxChoose = 8;
int hZombie[5] = { 0 };
int main() {
	startup();
	while (1) {
		show();
		updateWithoutInput();
		updateWithInput();
	}
	gameover();
}
void delay(DWORD ms)
{
	static DWORD oldtime = GetTickCount();
	while (GetTickCount() - oldtime < ms)
		Sleep(1);
	oldtime = GetTickCount();
}
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color, int alpha) {
	DWORD* imgp = GetImageBuffer(img);
	DWORD* bgimgp = GetImageBuffer();
	int w, bw, h, i, j;
	w = img->getwidth();
	bw = getwidth();
	h = img->getheight();
	color += 0xff000000;
	if (alpha < 0)alpha = 0;
	else if (alpha > 255)alpha = 255;
	for (i = 0; i < h; ++i)
		for (j = 0; j < w; ++j)
			if (dstx + j < Sc_Width && dstx + j> 0 && dsty + i < Sc_High && dsty + i > 0 && i*w + j != 0) {
				try {
					if (imgp[i * w + j] != color)
						bgimgp[(i + dsty) * bw + j + dstx] = RGB(
							((int)(alpha / 255.0 * GetRValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetRValue(bgimgp[(i + dsty) * bw + j + dstx]))),
							((int)(alpha / 255.0 * GetGValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetGValue(bgimgp[(i + dsty) * bw + j + dstx]))),
							((int)(alpha / 255.0 * GetBValue(imgp[i * w + j]) + (1 - alpha / 255.0) * GetBValue(bgimgp[(i + dsty) * bw + j + dstx])))
						);
				}catch (...) {
					break;
				}
			}
}
void putpicture(int dstx, int dsty, IMAGE* img, COLORREF color)
{
	DWORD* imgp = GetImageBuffer(img);
	DWORD* bgimgp = GetImageBuffer();
	int w, bw, h, i, j;
	w = img->getwidth();
	bw = getwidth();
	h = img->getheight();
	color += 0xff000000;
	for (i = 0; i < h; ++i)
		for (j = 0; j < w; ++j)
			if (dstx + j < Sc_Width && dstx + j> 0 && dsty + i < Sc_High && dsty + i > 0) 
				if (imgp[i * w + j] != color)
					bgimgp[(i + dsty) * bw + j + dstx] = imgp[i * w + j];
}
void MouseOperation()
{
	ExMessage m;
	while (MouseHit()) {
		m = getmessage(EM_MOUSE);
		if (m.message == WM_MOUSEMOVE) {
			mouseX = m.x;
			mouseY = m.y;
		}
		else if (m.message == WM_LBUTTONDOWN)
			isMouseDown = 1;
		else if (m.message == WM_LBUTTONUP)
			isMouseDown = 0;
	}
}
int getChooserNum()
{
	Card* t = _card;
	int i;
	for (i = 0; t != NULL; i++) {
		t = t->next;
	}
	return i;
}
void addChooser(int event,Card C)
{
	Card* t = _card,*add;
	while (t->next != NULL) 
		t = t->next;
	add = (Card*)malloc(sizeof(Card));
	add->next = NULL;
	add->nPlant = event;
	add->nSun = C.nSun;
	add->uCD = C.uCD;
	add->health = C.health;
	t->next = add;
}
int deleteChooser(int event, Card *C) 
{
	Card* p, * t = C;
	int b;
		for (int i = 1; i < event; i++){
			t = t->next;
		}
	p = t->next;
	b = p->nPlant;
	t->next = p->next;
	free(p);
	return b;
}
void startup()
{
	if (GameStatus == -1) LogIn();
	initgraph(Sc_Width, Sc_High);
	srand((unsigned)time(NULL));
	loadimage(&plantbox, ".\\Screen\\plantbox.png");
	loadimage(&_choosepage_, ".\\Screen\\PanelBackground.png");
	loadimage(&_c_[0][0], ".\\Card\\card_peashooter.png");
	loadimage(&_c_[1][0], ".\\Card\\card_sunflower.png");
	loadimage(&_c_[2][0], ".\\Card\\card_cherrybomb.png");
	loadimage(&_c_[3][0], ".\\Card\\card_wallnut.png");
	loadimage(&_c_[4][0], ".\\Card\\card_potatomine.png");
	loadimage(&_c_[5][0], ".\\Card\\card_snowpea.png");
	loadimage(&_c_[6][0], ".\\Card\\card_chomper.png");
	loadimage(&_c_[7][0], ".\\Card\\card_repeaterpea.png");
	loadimage(&_c_[8][0], ".\\Card\\card_puffshroom.png");
	loadimage(&_c_[9][0], ".\\Card\\card_sunshroom.png");
	loadimage(&_c_[10][0], ".\\Card\\card_fumeshroom.png");
	loadimage(&_c_[11][0], ".\\Card\\card_gravebuster.png");
	loadimage(&_c_[12][0], ".\\Card\\card_hypnoshroom.png");
	loadimage(&_c_[13][0], ".\\Card\\card_scaredyshroom.png");
	loadimage(&_c_[14][0], ".\\Card\\card_iceshroom.png");
	loadimage(&_c_[15][0], ".\\Card\\card_doomshroom.png");
	loadimage(&_c_[16][0], ".\\Card\\card_lilypad.png");
	loadimage(&_c_[17][0], ".\\Card\\card_squash.png");
	loadimage(&_c_[18][0], ".\\Card\\card_threepeashooter.png");
	loadimage(&_c_[19][0], ".\\Card\\card_tanglekelp.png");
	loadimage(&_c_[20][0], ".\\Card\\card_jalapeno.png");
	loadimage(&_c_[21][0], ".\\Card\\card_spikeweed.png");
	loadimage(&_c_[22][0], ".\\Card\\card_torchwood.png");
	loadimage(&_c_[23][0], ".\\Card\\card_tallnut.png");
	loadimage(&_c_[24][0], ".\\Card\\card_seashroom.png");
	loadimage(&_c_[25][0], ".\\Card\\card_plantern.png");
	loadimage(&_c_[26][0], ".\\Card\\card_cactus.png");
	loadimage(&_c_[27][0], ".\\Card\\card_blover.png");
	loadimage(&_c_[28][0], ".\\Card\\card_splitpea.png");
	loadimage(&_c_[29][0], ".\\Card\\card_starfruit.png");
	loadimage(&_c_[30][0], ".\\Card\\card_pumpkin.png");
	loadimage(&_c_[31][0], ".\\Card\\card_magnetshroom.png");
	loadimage(&_c_[32][0], ".\\Card\\card_cabbagepult.png");
	loadimage(&_c_[33][0], ".\\Card\\card_flowerpod.png");
	loadimage(&_c_[34][0], ".\\Card\\card_kernetpult.png");
	loadimage(&_c_[35][0], ".\\Card\\card_coffeebean.png");
	loadimage(&_c_[36][0], ".\\Card\\card_garlic.png");
	loadimage(&_c_[37][0], ".\\Card\\card_umbrellaleaf.png");
	loadimage(&_c_[38][0], ".\\Card\\card_marigold.png");
	loadimage(&_c_[39][0], ".\\Card\\card_melonpult.png");
	loadimage(&_c_[40][0], ".\\Card\\card_gatlingpea.png");
	loadimage(&_c_[41][0], ".\\Card\\card_twinsunflower.png");
	loadimage(&_c_[42][0], ".\\Card\\card_gloomshroom.png");
	loadimage(&_c_[43][0], ".\\Card\\card_cattail.png");
	loadimage(&_c_[44][0], ".\\Card\\card_wintermelon.png");
	loadimage(&_c_[45][0], ".\\Card\\card_goldmagnet.png");
	loadimage(&_c_[46][0], ".\\Card\\card_spikerock.png");
	loadimage(&_c_[47][0], ".\\Card\\card_cobcannon.png");
	loadimage(&_c_[0][1], ".\\Card\\card_peashooter_mask.png");
	loadimage(&_c_[1][1], ".\\Card\\card_sunflower_mask.png");
	loadimage(&_c_[2][1], ".\\Card\\card_cherrybomb_mask.png");
	loadimage(&_c_[3][1], ".\\Card\\card_wallnut_mask.png");
	loadimage(&_c_[4][1], ".\\Card\\card_potatomine_mask.png");
	loadimage(&_c_[5][1], ".\\Card\\card_snowpea_mask.png");
	loadimage(&_c_[6][1], ".\\Card\\card_chomper_mask.png");
	loadimage(&_c_[7][1], ".\\Card\\card_repeaterpea_mask.png");
	loadimage(&_c_[8][1], ".\\Card\\card_puffshroom_mask.png");
	loadimage(&_c_[9][1], ".\\Card\\card_sunshroom_mask.png");
	loadimage(&_c_[10][1], ".\\Card\\card_fumeshroom_mask.png");
	loadimage(&_c_[11][1], ".\\Card\\card_gravebuster_mask.png");
	loadimage(&_c_[12][1], ".\\Card\\card_hypnoshroom_mask.png");
	loadimage(&_c_[13][1], ".\\Card\\card_scaredyshroom_mask.png");
	loadimage(&_c_[14][1], ".\\Card\\card_iceshroom_mask.png");
	loadimage(&_c_[15][1], ".\\Card\\card_doomshroom_mask.png");
	loadimage(&_c_[16][1], ".\\Card\\card_lilypad_mask.png");
	loadimage(&_c_[17][1], ".\\Card\\card_squash_mask.png");
	loadimage(&_c_[18][1], ".\\Card\\card_threepeashooter_mask.png");
	loadimage(&_c_[19][1], ".\\Card\\card_tanglekelp_mask.png");
	loadimage(&_c_[20][1], ".\\Card\\card_jalapeno_mask.png");
	loadimage(&_c_[21][1], ".\\Card\\card_spikeweed_mask.png");
	loadimage(&_c_[22][1], ".\\Card\\card_torchwood_mask.png");
	loadimage(&_c_[23][1], ".\\Card\\card_tallnut_mask.png");
	loadimage(&_c_[24][1], ".\\Card\\card_seashroom_mask.png");
	loadimage(&_c_[25][1], ".\\Card\\card_plantern_mask.png");
	loadimage(&_c_[26][1], ".\\Card\\card_cactus_mask.png");
	loadimage(&_c_[27][1], ".\\Card\\card_blover_mask.png");
	loadimage(&_c_[28][1], ".\\Card\\card_splitpea_mask.png");
	loadimage(&_c_[29][1], ".\\Card\\card_starfruit_mask.png");
	loadimage(&_c_[30][1], ".\\Card\\card_pumpkin_mask.png");
	loadimage(&_c_[31][1], ".\\Card\\card_magnetshroom_mask.png");
	loadimage(&_c_[32][1], ".\\Card\\card_cabbagepult_mask.png");
	loadimage(&_c_[33][1], ".\\Card\\card_flowerpod_mask.png");
	loadimage(&_c_[34][1], ".\\Card\\card_kernetpult_mask.png");
	loadimage(&_c_[35][1], ".\\Card\\card_coffeebean_mask.png");
	loadimage(&_c_[36][1], ".\\Card\\card_garlic_mask.png");
	loadimage(&_c_[37][1], ".\\Card\\card_umbrellaleaf_mask.png");
	loadimage(&_c_[38][1], ".\\Card\\card_marigold_mask.png");
	loadimage(&_c_[39][1], ".\\Card\\card_melonpult_mask.png");
	loadimage(&_c_[40][1], ".\\Card\\card_gatlingpea_mask.png");
	loadimage(&_c_[41][1], ".\\Card\\card_twinsunflower_mask.png");
	loadimage(&_c_[42][1], ".\\Card\\card_gloomshroom_mask.png");
	loadimage(&_c_[43][1], ".\\Card\\card_cattail_mask.png");
	loadimage(&_c_[44][1], ".\\Card\\card_wintermelon_mask.png");
	loadimage(&_c_[45][1], ".\\Card\\card_goldmagnet_mask.png");
	loadimage(&_c_[46][1], ".\\Card\\card_spikerock_mask.png");
	loadimage(&_c_[47][1], ".\\Card\\card_cobcannon_mask.png");

	loadimage(&_P_[0][0][0], ".\\Plant\\PeaShooter.png");
	loadimage(&_P_[0][1][0], ".\\Plant\\PeaShooter_mask.png");
	loadimage(&_P_[0][2][0], ".\\Plant\\PeaShooter_light.png");
	loadimage(&_P_[1][0][0], ".\\Plant\\SunFlower.png");
	loadimage(&_P_[1][1][0], ".\\Plant\\SunFlower_mask.png");
	loadimage(&_P_[1][2][0], ".\\Plant\\SunFlower_light.png");
	loadimage(&_P_[2][0][0], ".\\Plant\\CherryBomb.png");
	loadimage(&_P_[2][1][0], ".\\Plant\\CherryBomb_mask.png");
	loadimage(&_P_[2][2][0], ".\\Plant\\CherryBomb.png");
	loadimage(&_P_[2][0][1], ".\\Plant\\Boomm.png");
	loadimage(&_P_[2][1][1], ".\\Plant\\Boomm_mask.png");
	loadimage(&_P_[3][0][0], ".\\Plant\\WallNut.png");
	loadimage(&_P_[3][1][0], ".\\Plant\\WallNut_mask.png");
	loadimage(&_P_[3][2][0], ".\\Plant\\WallNut_light.png");
	loadimage(&_P_[3][0][1], ".\\Plant\\WallNut_cracked_1.png");
	loadimage(&_P_[3][1][1], ".\\Plant\\WallNut_cracked_1_mask.png");
	loadimage(&_P_[3][2][1], ".\\Plant\\WallNut_cracked_1_light.png");
	loadimage(&_P_[3][0][2], ".\\Plant\\WallNut_cracked_2.png");
	loadimage(&_P_[3][1][2], ".\\Plant\\WallNut_cracked_2_mask.png");
	loadimage(&_P_[3][2][2], ".\\Plant\\WallNut_cracked_2_light.png");
	loadimage(&_P_[4][0][0], ".\\Plant\\PotatoMine.png");
	loadimage(&_P_[4][1][0], ".\\Plant\\PotatoMine_mask.png");
	loadimage(&_P_[4][2][0], ".\\Plant\\PotatoMine_light.png");
	loadimage(&_P_[4][0][1], ".\\Plant\\Potato_down.png");
	loadimage(&_P_[4][1][1], ".\\Plant\\Potato_down_mask.png");
	loadimage(&_P_[4][2][1], ".\\Plant\\Potato_down_light.png");
	loadimage(&_P_[4][0][2], ".\\Plant\\PotatoMash.png");
	loadimage(&_P_[4][1][2], ".\\Plant\\Boom.png");
	loadimage(&_P_[7][0][0], ".\\Plant\\Repeater.png");
	loadimage(&_P_[7][1][0], ".\\Plant\\Repeater_mask.png");
	loadimage(&_P_[7][2][0], ".\\Plant\\Repeater_light.png");
	loadimage(&_P_[17][0][0], ".\\Plant\\Squash.png");
	loadimage(&_P_[17][1][0], ".\\Plant\\Squash_mask.png");
	loadimage(&_P_[17][2][0], ".\\Plant\\Squash_light.png");
	loadimage(&_P_[17][0][1], ".\\Plant\\Squash_attack.png");
	loadimage(&_P_[17][1][1], ".\\Plant\\Squash_attack_mask.png");
	loadimage(&_P_[17][2][1], ".\\Plant\\Squash_attack.png");
	loadimage(&_P_[20][0][0], ".\\Plant\\Jalapeno.png");
	loadimage(&_P_[20][1][0], ".\\Plant\\Jalapeno_mask.png");
	loadimage(&_P_[20][2][0], ".\\Plant\\Jalapeno.png");
	loadimage(&_P_[20][0][1], ".\\Plant\\Ex----plo------sion.png");
	loadimage(&_P_[20][1][1], ".\\Plant\\Ex----plo------sion_mask.png");
	loadimage(&_P_[21][0][0], ".\\Plant\\SpikeWeed.png");
	loadimage(&_P_[21][1][0], ".\\Plant\\SpikeWeed_mask.png");
	loadimage(&_P_[21][2][0], ".\\Plant\\SpikeWeed_light.png");
	loadimage(&_P_[40][0][0], ".\\Plant\\GatlingPea.png");
	loadimage(&_P_[40][1][0], ".\\Plant\\GatlingPea_mask.png");
	loadimage(&_P_[40][2][0], ".\\Plant\\GatlingPea_light.png");

	loadimage(&_p_[0][0], ".\\Plant\\Pea.png");
	//loadimage(&_p_[0][1], ".\\Plant\\Pea_dark.png");
	loadimage(&_p_[1][0], ".\\Plant\\SunF.png");
	//loadimage(&_p_[1][1], ".\\Plant\\SunF_dark.png");
	loadimage(&_p_[2][0], ".\\Plant\\Cherry.png");
	loadimage(&_p_[3][0], ".\\Plant\\WallN.png");
	//loadimage(&_p_[3][1], ".\\Plant\\WallN_dark.png");
	loadimage(&_p_[4][0], ".\\Plant\\Potato.png");
	loadimage(&_p_[7][0], ".\\Plant\\Repea.png");
	//loadimage(&_p_[4][1], ".\\Plant\\Potato_dark.png");
	loadimage(&_p_[17][0], ".\\Plant\\Squ.png");
	//loadimage(&_p_[17][1], ".\\Plant\\Squ_dark.png");
	loadimage(&_p_[20][0], ".\\Plant\\Jala.png");
	loadimage(&_p_[21][0], ".\\Plant\\Spike.png");
	//loadimage(&_p_[21][1], ".\\Plant\\Spike_dark.png");
	loadimage(&_p_[40][0], ".\\Plant\\Gat.png");
	
	loadimage(&map[0], ".\\Map\\map0.jpg");
	loadimage(&map[1], ".\\Map\\map1.jpg");
	loadimage(&_pause_, ".\\Screen\\pause.png");
	loadimage(&_bkToGame_, ".\\Screen\\bkToGame.png");
	loadimage(&_restart_, ".\\Screen\\restart.png");
	loadimage(&_BkToMenu_, ".\\Screen\\BackMenu.png");
	loadimage(&_Sun_[0], ".\\Screen\\sun.png");
	loadimage(&_Sun_[1], ".\\Screen\\sun_mask.png");
	loadimage(&_Lose_, ".\\Screen\\ZombiesWon.png");

	loadimage(&_StartLevel_[0], ".\\Screen\\StartLevel_1.png");
	loadimage(&_StartLevel_[1], ".\\Screen\\StartLevel_2.png");
	loadimage(&_StartLevel_[2], ".\\Screen\\StartLevel_3.png");
	loadimage(&_MidLevel_, ".\\Screen\\MidLevel.png");
	loadimage(&_EndLevel_, ".\\Screen\\EndLevel.png");
	loadimage(&_Money_, ".\\Screen\\Money.png");
	loadimage(&_EndDraw_, ".\\Screen\\EndDraw.png");
	loadimage(&_car_, ".\\Screen\\car.png");
	loadimage(&_shovel_, ".\\Screen\\shovel.png");
	loadimage(&_shovelSlot_, ".\\Screen\\shovelSlot.png");
	loadimage(&_Progress_[0], ".\\Screen\\progress0.png");
	loadimage(&_Progress_[1], ".\\Screen\\progress1.png");
	loadimage(&_Progress_[2], ".\\Screen\\progress2.png");
	loadimage(&_Progress_[3], ".\\Screen\\progress3.png");

	loadimage(&_Z_[0][0][0][0], ".\\Zombie\\Zom_walk_1.png");
	loadimage(&_Z_[0][1][0][0], ".\\Zombie\\Zom_walk_1_mask.png");
	loadimage(&_Z_[0][2][0][0], ".\\Zombie\\Zom_walk_1_light.png");
	loadimage(&_Z_[0][0][1][0], ".\\Zombie\\Zom_walk_2.png");
	loadimage(&_Z_[0][1][1][0], ".\\Zombie\\Zom_walk_2_mask.png");
	loadimage(&_Z_[0][2][1][0], ".\\Zombie\\Zom_walk_2_light.png");
	loadimage(&_Z_[0][0][2][0], ".\\Zombie\\Zom_walk_3.png");
	loadimage(&_Z_[0][1][2][0], ".\\Zombie\\Zom_walk_3_mask.png");
	loadimage(&_Z_[0][2][2][0], ".\\Zombie\\Zom_walk_3_light.png");
	loadimage(&_Z_[0][0][0][1], ".\\Zombie\\Zom_LostHead.png");
	loadimage(&_Z_[0][1][0][1], ".\\Zombie\\Zom_LostHead_mask.png");
	loadimage(&_Z_[0][2][0][1], ".\\Zombie\\Zom_LostHead_light.png");
	loadimage(&_Z_[0][0][0][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[0][1][0][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[0][0][1][1], ".\\Zombie\\Zom_LostHead.png");
	loadimage(&_Z_[0][1][1][1], ".\\Zombie\\Zom_LostHead_mask.png");
	loadimage(&_Z_[0][2][1][1], ".\\Zombie\\Zom_LostHead_light.png");
	loadimage(&_Z_[0][0][1][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[0][1][1][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[0][0][2][1], ".\\Zombie\\Zom_LostHead.png");
	loadimage(&_Z_[0][1][2][1], ".\\Zombie\\Zom_LostHead_mask.png");
	loadimage(&_Z_[0][2][2][1], ".\\Zombie\\Zom_LostHead_light.png");
	loadimage(&_Z_[0][0][2][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[0][1][2][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[0][0][0][3], ".\\Zombie\\Zom_attack.png");
	loadimage(&_Z_[0][1][0][3], ".\\Zombie\\Zom_attack_mask.png");
	loadimage(&_Z_[0][2][0][3], ".\\Zombie\\Zom_attack_light.png");
	loadimage(&_Z_[0][0][0][4], ".\\Zombie\\Zom_LostHeadAttack.png");
	loadimage(&_Z_[0][1][0][4], ".\\Zombie\\Zom_LostHeadAttack_mask.png");
	loadimage(&_Z_[0][2][0][4], ".\\Zombie\\Zom_LostHeadAttack_light.png");
	loadimage(&_Z_[0][0][1][3], ".\\Zombie\\Zom_attack.png");
	loadimage(&_Z_[0][1][1][3], ".\\Zombie\\Zom_attack_mask.png");
	loadimage(&_Z_[0][2][1][3], ".\\Zombie\\Zom_attack_light.png");
	loadimage(&_Z_[0][0][1][4], ".\\Zombie\\Zom_LostHeadAttack.png");
	loadimage(&_Z_[0][1][1][4], ".\\Zombie\\Zom_LostHeadAttack_mask.png");
	loadimage(&_Z_[0][2][1][4], ".\\Zombie\\Zom_LostHeadAttack_light.png");
	loadimage(&_Z_[0][0][2][3], ".\\Zombie\\Zom_attack.png");
	loadimage(&_Z_[0][1][2][3], ".\\Zombie\\Zom_attack_mask.png");
	loadimage(&_Z_[0][2][2][3], ".\\Zombie\\Zom_attack_light.png");
	loadimage(&_Z_[0][0][2][4], ".\\Zombie\\Zom_LostHeadAttack.png");
	loadimage(&_Z_[0][1][2][4], ".\\Zombie\\Zom_LostHeadAttack_mask.png");
	loadimage(&_Z_[0][2][2][4], ".\\Zombie\\Zom_LostHeadAttack_light.png");
	loadimage(&_Z_[0][0][0][5], ".\\Zombie\\Zom_BoomDie.png");
	loadimage(&_Z_[0][1][0][5], ".\\Zombie\\Zom_BoomDie_mask.png");
	loadimage(&_Z_[0][0][1][5], ".\\Zombie\\Zom_BoomDie.png");
	loadimage(&_Z_[0][1][1][5], ".\\Zombie\\Zom_BoomDie_mask.png");
	loadimage(&_Z_[0][0][2][5], ".\\Zombie\\Zom_BoomDie.png");
	loadimage(&_Z_[0][1][2][5], ".\\Zombie\\Zom_BoomDie_mask.png");
	loadimage(&_Z_[1][0][0][0], ".\\Zombie\\ConeZom_walk.png");
	loadimage(&_Z_[1][1][0][0], ".\\Zombie\\ConeZom_walk_mask.png");
	loadimage(&_Z_[1][2][0][0], ".\\Zombie\\ConeZom_walk_light.png");
	loadimage(&_Z_[1][0][0][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[1][1][0][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[1][0][0][3], ".\\Zombie\\ConeZom_attack.png");
	loadimage(&_Z_[1][1][0][3], ".\\Zombie\\ConeZom_attack_mask.png");
	loadimage(&_Z_[1][2][0][3], ".\\Zombie\\ConeZom_attack_light.png");
	loadimage(&_Z_[2][0][1][0], ".\\Zombie\\BucketZom_walk.png");
	loadimage(&_Z_[2][1][1][0], ".\\Zombie\\BucketZom_walk_mask.png");
	loadimage(&_Z_[2][2][1][0], ".\\Zombie\\BucketZom_walk_light.png");
	loadimage(&_Z_[2][0][1][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[2][1][1][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[2][0][1][3], ".\\Zombie\\BucketZom_attack.png");
	loadimage(&_Z_[2][1][1][3], ".\\Zombie\\BucketZom_attack_mask.png");
	loadimage(&_Z_[2][2][1][3], ".\\Zombie\\BucketZom_attack_light.png");
	loadimage(&_Z_[3][0][0][0], ".\\Zombie\\FlagZom_walk.png");
	loadimage(&_Z_[3][1][0][0], ".\\Zombie\\FlagZom_walk_mask.png");
	loadimage(&_Z_[3][2][0][0], ".\\Zombie\\FlagZom_walk_light.png");
	loadimage(&_Z_[3][0][0][1], ".\\Zombie\\FlagZom_LostHead.png");
	loadimage(&_Z_[3][1][0][1], ".\\Zombie\\FlagZom_LostHead_mask.png");
	loadimage(&_Z_[3][2][0][1], ".\\Zombie\\FlagZom_LostHead_light.png");
	loadimage(&_Z_[3][0][0][2], ".\\Zombie\\Zom_die.png");
	loadimage(&_Z_[3][1][0][2], ".\\Zombie\\Zom_die_mask.png");
	loadimage(&_Z_[3][0][0][3], ".\\Zombie\\FlagZom_attack.png");
	loadimage(&_Z_[3][1][0][3], ".\\Zombie\\FlagZom_attack_mask.png");
	loadimage(&_Z_[3][2][0][3], ".\\Zombie\\FlagZom_attack_light.png");
	loadimage(&_Z_[3][0][0][4], ".\\Zombie\\FlagZom_LostHeadAttack.png");
	loadimage(&_Z_[3][1][0][4], ".\\Zombie\\FlagZom_LostHeadAttack_mask.png");
	loadimage(&_Z_[3][2][0][4], ".\\Zombie\\FlagZom_LostHeadAttack_light.png");
	loadimage(&_Z_[3][0][0][5], ".\\Zombie\\Zom_BoomDie.png");
	loadimage(&_Z_[3][1][0][5], ".\\Zombie\\Zom_BoomDie_mask.png");
	loadimage(&_Z_[4][0][0][0], ".\\Zombie\\FootballZom_walk.png");
	loadimage(&_Z_[4][1][0][0], ".\\Zombie\\FootballZom_walk_mask.png");
	loadimage(&_Z_[4][2][0][0], ".\\Zombie\\FootballZom_walk_light.png");
	loadimage(&_Z_[4][0][0][1], ".\\Zombie\\FootballZom_LostHead.png");
	loadimage(&_Z_[4][1][0][1], ".\\Zombie\\FootballZom_LostHead_mask.png");
	loadimage(&_Z_[4][2][0][1], ".\\Zombie\\FootballZom_LostHead_light.png");
	loadimage(&_Z_[4][0][0][2], ".\\Zombie\\FootballZom_die.png");
	loadimage(&_Z_[4][1][0][2], ".\\Zombie\\FootballZom_die_mask.png");
	loadimage(&_Z_[4][0][0][3], ".\\Zombie\\FootballZom_attack.png");
	loadimage(&_Z_[4][1][0][3], ".\\Zombie\\FootballZom_attack_mask.png");
	loadimage(&_Z_[4][2][0][3], ".\\Zombie\\FootballZom_attack_light.png");
	loadimage(&_Z_[4][0][0][4], ".\\Zombie\\FootballZom_LostHeadAttack.png");
	loadimage(&_Z_[4][1][0][4], ".\\Zombie\\FootballZom_LostHeadAttack_mask.png");
	loadimage(&_Z_[4][2][0][4], ".\\Zombie\\FootballZom_LostHeadAttack_light.png");
	loadimage(&_Z_[4][0][0][5], ".\\Zombie\\Zom_BoomDie.png");
	loadimage(&_Z_[4][1][0][5], ".\\Zombie\\Zom_BoomDie_mask.png");
	loadimage(&_Z_[4][0][0][6], ".\\Zombie\\FootballZom_OrnLost.png");
	loadimage(&_Z_[4][1][0][6], ".\\Zombie\\FootballZom_OrnLost_mask.png");
	loadimage(&_Z_[4][2][0][6], ".\\Zombie\\FootballZom_OrnLost_light.png");
	loadimage(&_Z_[4][0][0][7], ".\\Zombie\\FootballZom_OrnLostAttack.png");
	loadimage(&_Z_[4][1][0][7], ".\\Zombie\\FootballZom_OrnLostAttack_mask.png");
	loadimage(&_Z_[4][2][0][7], ".\\Zombie\\FootballZom_OrnLostAttack_light.png");
	loadimage(&_Z_H[0], ".\\Zombie\\Zom_head.png");
	loadimage(&_Z_H[1], ".\\Zombie\\Zom_head_mask.png");

	loadimage(&_Bullet_PeaNormal[0], ".\\Bullets\\PeaNormal.png");
	loadimage(&_Bullet_PeaNormal[1], ".\\Bullets\\PeaNormal_mask.png");

	loadimage(&_shadow_, ".\\Screen\\shadow.png");
	for (int i = 0; i < 10; i++) {
		rCard[i][0] = 90 + 54 * i;
		rCard[i][1] = 8;
		rCard[i][2] = 141 + 54 * i;
		rCard[i][3] = 78;
	}
	for (int i = 0; i < 45; i++) {
		rOP[i][0] = 36 + 84 * (i % 9) - 15;
		rOP[i][1] = 86 + (i / 9) * 98;
		rOP[i][2] = 120 + 84 * (i % 9) -15;
		rOP[i][3] = 184 + (i / 9) * 98;
	}
	BeginBatchDraw();
}


void show()
{
	if (GameStatus == 0) MainMenu();
	else if (GameStatus == 1) LevelShow();
	if (GameStatus == 2) {
		LevelShow();
		PauseShow();
	}
	if (GameStatus == 3) LoseGame();
}
void MidLevel()
{
	static float alpha = 0;
	if (!level.pause) alpha = 0;
	putpicture(400 - _MidLevel_.getwidth() / 2, 300 - _MidLevel_.getheight() / 2, &_MidLevel_ , RGB(0, 0, 0), alpha);
	alpha += 0.4;
}
void EndLevel()
{
	static float alpha = 50;
	if (level.cnt == 25) {
		alpha = 50;
		mciSendString("close final", NULL, 0, NULL);
		mciSendString("open .\\audio\\finalwav.mp3 alias final", NULL, 0, NULL);
		mciSendString("play final", NULL, 0, NULL);
	}
	putpicture(400 - _EndLevel_.getwidth() / 2, 300 - _EndLevel_.getheight() / 2, &_EndLevel_, RGB(0, 0, 0), alpha);
	alpha += 0.2;
}
void WinLevel()
{
	level.checkpoint++;
	level.progress = 0;
	for (Card* p = _card,*pr = _card; p != NULL; p = p->next) {
		pr = p;
		p = p->next;
		free(pr);
	}
}
void PauseShow()
{
	putpicture(pauseX, pauseY, &_pause_, RGB(0, 0, 0));
	if (mouseX >= pauseX + 38 && mouseX < pauseX + 380 && mouseY > pauseY + 392 && mouseY < pauseY + 475) {
		putpicture(pauseX + 38, pauseY + 392, &_bkToGame_, RGB(0, 0, 0));
	}
	if(mouseX >= pauseX + 108 && mouseX < pauseX + 313 && mouseY > pauseY + 286 && mouseY < pauseY + 328)
		putpicture(pauseX + 108, pauseY + 282, &_restart_, RGB(0, 0, 0));
	if (mouseX >= pauseX + 108 && mouseX < pauseX + 313 && mouseY > pauseY + 328 && mouseY < pauseY + 370)
		putpicture(pauseX + 108, pauseY + 325, &_BkToMenu_, RGB(0, 0, 0));
	FlushBatchDraw();
}
void LoseGame() 
{
	float alpha = 0;
	char c = 0;
	mciSendString("play .\\audio\\scream.mp3", NULL, 0, NULL);
	while (c != 27) {
		putpicture(400 - _Lose_.getwidth() / 2, 300 - _Lose_.getheight() / 2, &_Lose_, RGB(255,255,255),alpha);
		if (alpha <= 256) alpha += 0.05;
		if(_kbhit())
			c = _getch();
		FlushBatchDraw();
	}
	GameStatus = 1;
	level.progress = 0;
	for (Card* p = _card, *pr = _card; p != NULL;) {
		pr = p;
		p = p->next;
		free(pr);
	}
	for (Zombie* p = _Zom, *pr = _Zom; p != NULL;) {
		pr = p;
		p = p->next;
		free(pr);
	}
}
void LevelShow()
{
	static int SunNum;
	static int ZType;
	static float moneyX = 700, moneyY = 200;
	static float intervalX = 0, intervalY = 0;
	static clock_t start = clock(), end = clock(), start1 = 0;
	static int timeInterval[3] = { 0 };
	if(level.checkpoint<=4) putimage(0, 0, 800, 600, &map[0], 230, 0);
	else putimage(0, 0, 800, 600, &map[1], 230, 0);
	if (level.progress < 0.1) {
		//start = clock(), end = clock(), start1 = 0;
		_Zom = NULL;
		_card = NULL;
		level.timeLeap = 17;
		if (level.checkpoint >= 4) {
			MaxChoose = 10;
			loadimage(&plantboxM, ".\\Screen\\plantboxM.png");
		}
		if (level.checkpoint <= 3) {
			if (!MusicStatus) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\GrassWalk2.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 600", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
			else {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\GrassWalk1.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
		}
		else if (level.checkpoint == 4) {
			if (!MusicStatus) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\ultimate_tatakai2.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
			else {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\ultimate_tatakai1.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
		}
		else if (level.checkpoint == 5) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\Zen_Garden.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
		}
		else if(level.checkpoint == 6){
			if (!MusicStatus) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\pool.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
			else {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\night.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
		}
		else if (level.checkpoint == 7) {
			if (!MusicStatus) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\Brainiac_Maniac1.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
			else {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("open .\\audio\\Brainiac_Maniac2.mp3 alias bkmusic", NULL, 0, NULL);
				mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
				mciSendString("play bkmusic repeat", NULL, 0, NULL);
			}
		}
		else if(level.checkpoint == 8){
			mciSendString("close bkmusic", NULL, 0, NULL);
			mciSendString("open .\\audio\\Ode_to_Azhdaha.mp3 alias bkmusic", NULL, 0, NULL);
			mciSendString("setaudio bkmusic volume to 300", NULL, 0, NULL);
			mciSendString("play bkmusic repeat", NULL, 0, NULL);
		}
		ZType = 0;
		if (level.checkpoint <= 3) SunNum = 50;
		else SunNum = 150;
		level.cnt = 0;
		level.num = 1;
		level.timeLeap = 17;
		sun.timeleap = 0.08;
		sun.maxPic = 29;
		sun.v = 1.8;
		sun.CD = 5;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				plant[i][j].hPlant = -1;
			}
			car[i].X = -40;
			car[i].Y = rOP[i * 9][1] + 20;
			car[i].type = 1;
		}
		clock_t s = clock(), e;
		if (level.checkpoint <= 4) putimage(0, 0, 800, 600, &map[0], 0, 0);
		else putimage(0, 0, 800, 600, &map[1], 0, 0);
		FlushBatchDraw();
		Sleep(2000);
		for (int i = 0; i < 1200; ) {
			e = clock();
			if ((double)(e - s) / CLOCKS_PER_SEC >= 0.001) {
				s = clock();
				i++;
				if(level.checkpoint <= 4) putimage(0, 0, 800, 600, &map[0], 0.5 * i, 0);
				else putimage(0, 0, 800, 600, &map[1], 0.5 * i, 0);
				FlushBatchDraw();
			}
		}
		ChoosePage();
		if(level.checkpoint <= 4)putimage(0, 0, 800, 600, &map[0], 600, 0);
		else putimage(0, 0, 800, 600, &map[1], 600, 0);
		Sleep(500);
		for (int i = 0; i < 1000;) {
			e = clock();
			if ((double)(e - s) / CLOCKS_PER_SEC >= 0.001) {
				s = clock();
				i++;
				if(level.checkpoint <= 4)putimage(0, 0, 800, 600, &map[0], 600 - 0.37 * i, 0);
				else putimage(0, 0, 800, 600, &map[1], 600 - 0.37 * i, 0);
				if(MaxChoose == 8)putpicture(i * 0.01, 0, &plantbox, RGB(255, 255, 255));
				else putpicture(i * 0.01, 0, &plantboxM, RGB(255, 255, 255));
				for (int j = 0; j < MaxChoose; j++) {
					putimage(rCard[j][0] - 10 + 0.01 * i, rCard[j][1], &_c_[_card_[j].nPlant][0]);
				}
				FlushBatchDraw();
			}
		}
	}
	if (level.progress < 0.6 && level.progress > 0.1) {
		StartLevel();
		sun.start2 = clock();
		start = clock();
		level.progress = 0.8;
	}
	MouseOperation();
	if (GameStatus == 1) {
		end = clock();
		if (timeInterval[0]) {
			start = clock() - timeInterval[0];
			start1 = clock() - timeInterval[1];
			timeInterval[0] = 0;
			timeInterval[1] = 0;
		}
	}
	if ((float)(end - start) / CLOCKS_PER_SEC >= level.timeLeap && !level.pause && level.cnt != 9 && level.cnt != 15 && level.cnt != 24 && level.cnt != 28) {
		addZombie(level.num, ZType);
		if (level.cnt == 10 || level.cnt == 16 || level.cnt == 25) {
			mciSendString("close aw", NULL, 0, NULL);
			mciSendString("open .\\audio\\awooga.mp3 alias aw", NULL, 0, NULL);
			mciSendString("play aw", NULL, 0, NULL);
		}
		int a = rand() % 6 + 1;
		switch (a) {
		case 1:	
			//mciSendString("close g1", NULL, 0, NULL);
			//mciSendString("open .\\audio\\groan1.mp3 alias g1", NULL, 0, NULL);
			mciSendString("play .\\audio\\groan1.mp3", NULL, 0, NULL);
			break;
		case 2:
			/*mciSendString("close g2", NULL, 0, NULL);
			mciSendString("open .\\audio\\groan2.mp3 alias g2", NULL, 0, NULL);*/
			mciSendString("play .\\audio\\groan2.mp3", NULL, 0, NULL);
			break;
		case 3:
			/*mciSendString("close g3", NULL, 0, NULL);
			mciSendString("open .\\audio\\groan3.mp3 alias g3", NULL, 0, NULL);*/
			mciSendString("play .\\audio\\groan3.mp3", NULL, 0, NULL);
			break;
		case 4:
			/*mciSendString("close g1", NULL, 0, NULL);
			mciSendString("open .\\audio\\groan1.mp3 alias g1", NULL, 0, NULL);*/
			mciSendString("play .\\audio\\groan4.mp3", NULL, 0, NULL);
			break;
		case 5:
			/*mciSendString("close g1", NULL, 0, NULL);
			mciSendString("open .\\audio\\groan1.mp3 alias g1", NULL, 0, NULL);*/
			mciSendString("play .\\audio\\groan5.mp3", NULL, 0, NULL);
			break;
		case 6:
			/*mciSendString("close g1", NULL, 0, NULL);
			mciSendString("open .\\audio\\groan1.mp3 alias g1", NULL, 0, NULL);*/
			mciSendString("play .\\audio\\groan6.mp3", NULL, 0, NULL);
			break;
		}
	
		if (level.cnt == 0) {
			mciSendString("close siren", NULL, 0, NULL);
			mciSendString("open .\\audio\\siren.mp3 alias siren", NULL, 0, NULL);
			mciSendString("play siren", NULL, 0, NULL);
			level.progress = 1;
		}
		level.cnt++;
		if (level.checkpoint == 2) {
			if (level.cnt == 3 || level.cnt == 7) ZType = 1;
			else if (level.cnt == 4 || level.cnt == 8) ZType = 0;
		}
		if (level.cnt > 3 && level.cnt <= 7) level.num = 2;
		else if (level.cnt > 7 && level.cnt <= 9) {
			level.num = 3;
			level.timeLeap = 9;
		}
		start = clock();
	}
	if ((level.cnt == 9 || level.cnt == 15 || level.cnt == 24 || level.cnt == 28) && _Zom == NULL && level.progress - (int)level.progress < 0.1 &&
		fabs(level.progress - 5) > 0.1) {
		level.progress += 0.5;
	}
	if(MaxChoose == 8)putpicture(10, 0, &plantbox, RGB(255, 255, 255));
	else putpicture(10, 0, &plantboxM, RGB(255, 255, 255));
	if (level.progress > 0.9) putpicture(600, 556, &_Progress_[(int)(level.progress) - 1], RGB(255, 255, 255));
	static Card MHP;
	static clock_t s = 0, e = 0;
	char str[5];
	int event = -1, bevent = -1;
	static int hevent = 0;
	itoa(SunNum, str, 10);
	setbkmode(TRANSPARENT);
	settextstyle(15, 0, _T("Minecraft"));
	settextcolor(BLACK);
	if (MaxChoose == 8) {
		putpicture(532, 0, &_shovelSlot_, RGB(255, 255, 255));
	}
	else if (MaxChoose == 10) {
		putpicture(640, 0, &_shovelSlot_, RGB(255, 255, 255));
	}
	if (MHP.nPlant != -2) {
		if (MaxChoose == 8)
			putpicture(532, 0, &_shovel_, RGB(255, 255, 255));
		else putpicture(640, 0, &_shovel_, RGB(255, 255, 255));
	}
	if (SunNum == 0) {
		outtextxy(44, 66, str);
	}
	else if (SunNum > 0 && SunNum < 100) {
		outtextxy(38, 66, str);
	}
	else if (SunNum >= 100 && SunNum < 1000) {
		outtextxy(32, 66, str);
	}
	else outtextxy(27, 66, str);
	for (int i = 0; i < MaxChoose; i++) {
		if (_card_[i].cChoose) putimage(rCard[i][0], rCard[i][1], &_c_[_card_[i].nPlant][0]);
		else if (!_card_[i].cChoose) putimage(rCard[i][0], rCard[i][1], &_c_[_card_[i].nPlant][1]);

	}
	if (event == -1) event = BoxJudge(mouseX, mouseY, rCard);
	if (GameStatus == 1) {
		e = clock();
		if (timeInterval[2]) {
			s = clock() - timeInterval[2];
			timeInterval[2] = 0;
		}
	}
	else if (GameStatus == 2 && timeInterval[0] == 0) {
		timeInterval[0] = end - start;
		timeInterval[1] = end - start1;
		timeInterval[2] = e - s;
	}
	bevent = OPJudge(mouseX, mouseY, rOP);
	for (int i = 0; i < MaxChoose; i++) {
		if ((float)(_card_[i].end - _card_[i].start)/CLOCKS_PER_SEC >= _card_[i].uCD && SunNum >= _card_[i].nSun) {
			_card_[i].cChoose = 1;
		}
		else _card_[i].cChoose = 0;
		if (!_card_[i].cChoose) _card_[i].end = clock();
	}
	if (MHP.nPlant != -1 && bevent != -1) {
		if (MHP.nPlant != 40 && plant[bevent / 9][bevent % 9].hPlant == -1 && MHP.nPlant != -2) {
			putpicture(rOP[bevent][0], rOP[bevent][1], &_p_[MHP.nPlant][0], RGB(255, 255, 255), 100);
			if (isMouseDown) {
				_card_[hevent].start = clock();
				_card_[hevent].cChoose = 0;
				mciSendString("close plant1", NULL, 0, NULL);
				mciSendString("open .\\audio\\plant1.mp3 alias plant1", NULL, 0, NULL);
				mciSendString("play plant1", NULL, 0, NULL);
				int a = bevent / 9;
				int b = bevent % 9;
				SunNum -= MHP.nSun;
				plant[a][b].hPlant = MHP.nPlant;
				plant[a][b].health = MHP.health;
				plant[a][b].i = 0;
				plant[a][b].isAttack = 0;
				plant[a][b].timeLeap[0] = 0.09;
				switch (plant[a][b].hPlant) {
				case 0:
					plant[a][b].width[0] = 71;
					plant[a][b].high[0] = 71;
					plant[a][b].maxPic[0] = 13;
					plant[a][b].skill.CD = 1.4;
					plant[a][b].skill.Start[0] = clock();
					plant[a][b].skill.Start[1] = clock();
					plant[a][b].skill.v = 9.2;
					plant[a][b].skill.timeleap = 0.02;
					plant[a][b].skill.maxBullet = 2;
					break;
				case 1:
					plant[a][b].width[0] = 73;
					plant[a][b].high[0] = 74;
					plant[a][b].skill.start2 = clock() - 14000;
					plant[a][b].skill.timeleap = 0.08;
					plant[a][b].skill.maxPic = 29;
					plant[a][b].timeLeap[0] = 0.1;
					plant[a][b].skill.v = 0.05;
					plant[a][b].skill.CD = 24;
					plant[a][b].maxPic[0] = 18;
					break;
				case 2:
					plant[a][b].width[0] = 112;
					plant[a][b].high[0] = 81;
					plant[a][b].width[1] = 213;
					plant[a][b].high[1] = 196;
					plant[a][b].maxPic[0] = 7;
					plant[a][b].maxPic[1] = 12;
					plant[a][b].DifferentStatus = 0;
					break;
				case 3:
					plant[a][b].width[0] = 65;
					plant[a][b].high[0] = 73;
					plant[a][b].width[1] = 65;
					plant[a][b].high[1] = 73;
					plant[a][b].width[2] = 65;
					plant[a][b].high[2] = 73;
					plant[a][b].DifferentStatus = 0;
					plant[a][b].maxPic[0] = 16;
					plant[a][b].maxPic[1] = 11;
					plant[a][b].maxPic[2] = 15;
					break;
				case 4:
					plant[a][b].width[0] = 75;
					plant[a][b].high[0] = 80;
					plant[a][b].width[1] = 75;
					plant[a][b].high[1] = 80;
					plant[a][b].DifferentStatus = 1;
					plant[a][b].skill.start = clock();
					plant[a][b].skill.timeleap = 15;
					plant[a][b].maxPic[0] = 8;
					plant[a][b].maxPic[1] = 1;
					plant[a][b].maxPic[2] = 10;
					plant[a][b].timeLeap[0] = 0.2;
					break;
				case 7:
					plant[a][b].width[0] = 73;
					plant[a][b].high[0] = 71;
					plant[a][b].maxPic[0] = 15;
					plant[a][b].skill.CD = 1.4;
					plant[a][b].skill.Start[0] = clock();
					plant[a][b].skill.Start[1] = clock();
					plant[a][b].skill.v = 9.2;
					plant[a][b].skill.timeleap = 0.02;
					plant[a][b].skill.maxBullet = 4;
					break;
				case 17:
					plant[a][b].width[0] = 100;
					plant[a][b].high[0] = 226;
					plant[a][b].width[1] = 100;
					plant[a][b].high[1] = 226;
					plant[a][b].DifferentStatus = 0;
					plant[a][b].maxPic[0] = 17;
					plant[a][b].maxPic[1] = 28;
					break;
				case 20:
					plant[a][b].width[0] = 68;
					plant[a][b].high[0] = 89;
					plant[a][b].width[1] = 755;
					plant[a][b].DifferentStatus = 0;
					plant[a][b].high[1] = 131;
					plant[a][b].maxPic[0] = 8;
					plant[a][b].maxPic[1] = 8;
					break;
				case 21:
					plant[a][b].width[0] = 85;
					plant[a][b].high[0] = 90;
					plant[a][b].maxPic[0] = 19;
					plant[a][b].skill.timeleap = 1;
					plant[a][b].start1 = clock();
					break;
				}
				MHP.nPlant = -1;
			}
		}
		else if (MHP.nPlant == 40 && plant[bevent / 9][bevent % 9].hPlant == 7) {
			int a = bevent / 9;
			int b = bevent % 9;
			putpicture(rOP[bevent][0], rOP[bevent][1], &_p_[MHP.nPlant][0], RGB(255, 255, 255), 100);
			if (isMouseDown) {
				_card_[hevent].start = clock();
				_card_[hevent].cChoose = 0;
				mciSendString("close plant1", NULL, 0, NULL);
				mciSendString("open .\\audio\\plant1.mp3 alias plant1", NULL, 0, NULL);
				mciSendString("play plant1", NULL, 0, NULL);
				SunNum -= MHP.nSun;
				plant[a][b].hPlant = MHP.nPlant;
				plant[a][b].health = MHP.health;
				plant[a][b].i = 0;
				plant[a][b].width[0] = 88;
				plant[a][b].high[0] = 84;
				plant[a][b].isAttack = 0;
				plant[a][b].maxPic[0] = 13;
				plant[a][b].skill.CD = 1.4;
				plant[a][b].skill.Start[0] = clock();
				plant[a][b].skill.Start[1] = clock();
				plant[a][b].skill.v = 9.2;
				plant[a][b].skill.timeleap = 0.02;
				plant[a][b].skill.maxBullet = 8;
				MHP.nPlant = -1;
			}
		}
		else if (MHP.nPlant == -2 && plant[bevent / 9][bevent % 9].hPlant != -1) {
			int a = bevent / 9;
			int b = bevent % 9;
			plant[a][b].isAttack = 1;
			if (isMouseDown){
				mciSendString("close plant2", NULL, 0, NULL);
				mciSendString("open .\\audio\\plant2.mp3 alias plant2", NULL, 0, NULL);
				mciSendString("play plant2", NULL, 0, NULL);
			//	if(plant[a][b].hPlant == 21) plant[a][b].hPlant = -1;
				plant[a][b].health = 0;
			}
		}
	}
	if (GameStatus == 1) {
		if ((float)(e - s) / CLOCKS_PER_SEC >= 0.5) {
			if (event != -1 && MHP.nPlant == -1 && _card_[event].cChoose) {
				hevent = event;
				mciSendString("close t", NULL, 0, NULL);
				mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
				mciSendString("play t", NULL, 0, NULL);
				MHP = _card_[event];
				s = clock();
			}
			else if (event != -1 && MHP.nPlant != -1 && _card_[event].cChoose && MHP.nPlant != -2) {
				mciSendString("close t", NULL, 0, NULL);
				mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
				mciSendString("play t", NULL, 0, NULL);
				MHP.nPlant = -1;
				s = clock();
			}
			else if (MHP.nPlant == -1 && mouseY >= 0 && mouseY <= 70 && isMouseDown) {
				if((MaxChoose == 8 && mouseX >= 532 && mouseX <= 600) || (MaxChoose == 10 && mouseX >= 640 && mouseX <= 708)){
					mciSendString("close s", NULL, 0, NULL);
					mciSendString("open .\\audio\\shovel.mp3 alias s", NULL, 0, NULL);
					mciSendString("play s", NULL, 0, NULL);
					MHP.nPlant = -2;
					s = clock();
				}
			}
			else if (MHP.nPlant == -2 && isMouseDown) {
				mciSendString("close s", NULL, 0, NULL);
				mciSendString("open .\\audio\\shovel.mp3 alias s", NULL, 0, NULL);
				mciSendString("play s", NULL, 0, NULL);
				MHP.nPlant = -1;
				s = clock();
			}
		}
	}
	else MHP.nPlant = -1;
	if(MHP.nPlant != -1 && MHP.nPlant != -2){
		putpicture(mouseX - _p_[MHP.nPlant][0].getwidth()/2, mouseY - _p_[MHP.nPlant][0].getheight()/2, &_p_[MHP.nPlant][0],RGB(255,255,255));
	}
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 9; j++) {
			if(GameStatus == 1)sun.end = clock();
			if (plant[i][j].hPlant != -1) {
				if ((double)(sun.end - sun.start2) / CLOCKS_PER_SEC >= sun.CD) {
					if (mouseX > sun.X && mouseX < sun.X + 76 && mouseY > sun.Y && mouseY < sun.Y + 75 && isMouseDown) sun.isClick = 1;
					SunFall(&sun, &SunNum, 88, 458, 0.02);
				}
				int a = i * 9 + j ;
				if ((float)(plant[i][j].end - plant[i][j].start) / CLOCKS_PER_SEC >=plant[i][j].timeLeap[0]) {
					plant[i][j].start = clock();
					plant[i][j].i++;
				}
				if(GameStatus == 1)PlantUpdateWithTime(&plant[i][j], i, j);
				if (plant[i][j].i >= plant[i][j].maxPic[plant[i][j].DifferentStatus]) {
					if ((plant[i][j].hPlant == 17 && plant[i][j].DifferentStatus == 1) || (plant[i][j].hPlant == 4 && plant[i][j].DifferentStatus == 2)
						|| (plant[i][j].hPlant == 2 && plant[i][j].DifferentStatus == 1) || (plant[i][j].hPlant == 20 && plant[i][j].DifferentStatus == 1))
					{
						plant[i][j].DifferentStatus = 0;
						plant[i][j].hPlant = -1;
					}
					else {
						plant[i][j].i = 0;
						if (plant[i][j].hPlant == 2) {
							plant[i][j].DifferentStatus = 1;
							plant[i][j].timeLeap[0] = 0.1;
						}
						else if (plant[i][j].hPlant == 20) {
							plant[i][j].DifferentStatus = 1;
							plant[i][j].timeLeap[0] = 0.18;
						}
					}
				}
				if (plant[i][j].hPlant != 17 && plant[i][j].hPlant != 21 && plant[i][j].hPlant != 2 && (plant[i][j].hPlant!=4 || plant[i][j].DifferentStatus!=2)
					&& (plant[i][j].hPlant !=20 || plant[i][j].DifferentStatus!=1)) {
					if(plant[i][j].hPlant!=4) putpicture(rOP[a][0] + 6, rOP[a][1] + plant[i][j].high[0] - 23, &_shadow_, RGB(255, 255, 255), 100);
					putimage(rOP[a][0], rOP[a][1], plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][1][plant[i][j].DifferentStatus], plant[i][j].i * plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCAND);
					putimage(rOP[a][0], rOP[a][1], plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][0 + 2 * plant[i][j].isAttack][plant[i][j].DifferentStatus], plant[i][j].i * plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCINVERT);
				}
				else if (plant[i][j].hPlant == 2) {
					if (plant[i][j].DifferentStatus == 0) {
						putimage(rOP[a][0] - 12, rOP[a][1] - 10, plant[i][j].width[0], plant[i][j].high[0], &_P_[2][1][0], plant[i][j].i* plant[i][j].width[0],0, SRCAND);
						putimage(rOP[a][0] - 12, rOP[a][1] - 10, plant[i][j].width[0], plant[i][j].high[0], &_P_[2][0][0], plant[i][j].i* plant[i][j].width[0],0, SRCINVERT);
					}
					else {
						if (!plant[i][j].i) {
							mciSendString("close cb", NULL, 0, NULL);
							mciSendString("open .\\audio\\cherrybomb.mp3 alias cb", NULL, 0, NULL);
							mciSendString("play cb", NULL, 0, NULL);
						}
						putimage(rOP[a][0] - 60, rOP[a][1] - 50, plant[i][j].width[1], plant[i][j].high[1], &_P_[2][1][1], plant[i][j].i* plant[i][j].width[1],0, SRCAND);
						putimage(rOP[a][0] - 60, rOP[a][1] - 50, plant[i][j].width[1], plant[i][j].high[1], &_P_[2][0][1], plant[i][j].i* plant[i][j].width[1],0, SRCINVERT);
					}
				}
				else if(plant[i][j].hPlant == 17){
					if (plant[i][j].DifferentStatus == 1 && plant[i][j].i >= 10) {
						if (plant[i][j].i == 15) {
							mciSendString("close thu", NULL, 0, NULL);
							mciSendString("open .\\audio\\gargantuar_thump.mp3 alias thu", NULL, 0, NULL);
							mciSendString("play thu", NULL, 0, NULL);
						}
						putimage(plant[i][j].X - 15, plant[i][j].Y - 100, plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][1][plant[i][j].DifferentStatus], plant[i][j].i* plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCAND);
						putimage(plant[i][j].X - 15, plant[i][j].Y - 100, plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][0 + 2 * plant[i][j].isAttack][plant[i][j].DifferentStatus], plant[i][j].i* plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCINVERT);
					}
					else {
						if (plant[i][j].DifferentStatus == 1 && !plant[i][j].i) {
							mciSendString("close hmm", NULL, 0, NULL);
							mciSendString("open .\\audio\\squash_hmm.mp3 alias hmm", NULL, 0, NULL);
							mciSendString("play hmm", NULL, 0, NULL);
						}
						putpicture(rOP[a][0] + 6, rOP[a][1] + plant[i][j].high[0] - 163, &_shadow_, RGB(255, 255, 255), 100);
						putimage(rOP[a][0] - 15, rOP[a][1] - 140, plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][1][plant[i][j].DifferentStatus], plant[i][j].i * plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCAND);
						putimage(rOP[a][0] - 15, rOP[a][1] - 140, plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][0 + 2 * plant[i][j].isAttack][plant[i][j].DifferentStatus], plant[i][j].i * plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCINVERT);
					}
				}
				else if (plant[i][j].hPlant == 20) {
					if (!plant[i][j].i) {
						mciSendString("close jala", NULL, 0, NULL);
						mciSendString("open .\\audio\\jalapeno.mp3 alias jala", NULL, 0, NULL);
						mciSendString("play jala", NULL, 0, NULL);
					}
					putimage(rOP[i * 9][0], rOP[i * 9][1]-40, plant[i][j].width[1], plant[i][j].high[1], &_P_[20][1][1], plant[i][j].i* plant[i][j].width[1], 0, SRCAND);
					putimage(rOP[i * 9][0], rOP[i * 9][1]-40, plant[i][j].width[1], plant[i][j].high[1], &_P_[20][0][1], plant[i][j].i* plant[i][j].width[1], 0, SRCINVERT);
				}
				else if (plant[i][j].hPlant == 21) {
					putimage(rOP[a][0] - 15, rOP[a][1], plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][1][plant[i][j].DifferentStatus], plant[i][j].i* plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCAND);
					putimage(rOP[a][0] - 15, rOP[a][1], plant[i][j].width[plant[i][j].DifferentStatus], plant[i][j].high[plant[i][j].DifferentStatus], &_P_[plant[i][j].hPlant][0 + 2 * plant[i][j].isAttack][plant[i][j].DifferentStatus], plant[i][j].i* plant[i][j].width[plant[i][j].DifferentStatus], 0, SRCINVERT);
				}
				else if(plant[i][j].hPlant == 4){
					putpicture(rOP[a][0]-18,rOP[a][1],&_P_[4][0][2],RGB(0,0,0));
					if (plant[i][j].i < 5) {
						putpicture(rOP[a][0] + 12 + rand()%5, rOP[a][1] - 28 + rand()%5, &_P_[4][1][2], RGB(255, 255, 255));
					}
				}
				if(GameStatus == 1)plant[i][j].end = clock();
				if (plant[i][j].hPlant == 0 || plant[i][j].hPlant == 7 || plant[i][j].hPlant == 40) {
					//plant[i][j].skill.end = clock();
					for(int k = 0;k < plant[i][j].skill.maxBullet;k ++){
						if (plant[i][j].skill.x[k] > 0.1) {
							putpicture(plant[i][j].skill.x[k], plant[i][j].skill.y[k], &_Bullet_PeaNormal[0], RGB(0,0,0));
						}
					}
				}
				else if (plant[i][j].hPlant == 1) {
					if(GameStatus == 1)plant[i][j].skill.end = clock();
					if ((double)(plant[i][j].skill.end - plant[i][j].skill.start2) / CLOCKS_PER_SEC >= plant[i][j].skill.CD) {
						if (plant[i][j].skill.X < 1) {
							plant[i][j].skill.X = rOP[a][0];
							plant[i][j].skill.Y = rOP[a][1] - 20;
						}
						if (mouseX > plant[i][j].skill.X && mouseX < plant[i][j].skill.X + 76 && mouseY > plant[i][j].skill.Y && mouseY < plant[i][j].skill.Y + 75 && isMouseDown) plant[i][j].skill.isClick = 1;
						SunFall(&plant[i][j].skill, &SunNum, rOP[a][1] - 20, rOP[a][1],0.00);
					}
				}
				/*if (level.progress > 4.6 && level.progress < 5.1) {
					putpicture(moneyX, moneyY, &_Money_, RGB(255, 255, 255));
					if (fabs(moneyX - 400) < 0.1 && fabs(moneyY - 300) < 0.1) {
						static float alpha = 0;
						static clock_t endStart = clock(), endEnd;
						putpicture(0, 0, &_EndDraw_, RGB(0, 0, 0), alpha);
						alpha += 0.5;
						endEnd = clock();
						if ((float)(endEnd - endStart) / CLOCKS_PER_SEC >= 8) {
							WinLevel();
						}
					}
					else {
						moneyX -= intervalX;
						moneyY -= intervalY;
					}
				}*/
				plant[i][j].isAttack = 0;
			}
		}
	}
	if (_Zom != NULL && _Zom->next == NULL) {
		moneyX = _Zom->X;
		moneyY = _Zom->Y;
	}
	if (GameStatus == 1 && level.progress > 1.1 && level.progress < 1.6 || (level.progress > 2.1 && level.progress < 2.6) || (level.progress > 3.1 && level.progress < 3.6)) {
		MidLevel();
		if (!level.pause) {
			mciSendString("close mid", NULL, 0, NULL);
			mciSendString("open .\\audio\\hugewave.mp3 alias mid", NULL, 0, NULL);
			mciSendString("play mid", NULL, 0, NULL);
			level.pause = 1;
			start = clock();
		}
		if (level.progress > 1.1 && level.progress < 1.6) {
			if ((float)(end - start) / CLOCKS_PER_SEC >= 8) {
				start = clock() - 17000;
				level.progress = 2;
				level.timeLeap = 17;
				level.pause = 0;
				level.num = 6;
				level.cnt = 10;
			}
		}
		else if (level.progress > 2.1 && level.progress < 2.6) {
			if ((float)(end - start) / CLOCKS_PER_SEC >= 8) {
				start = clock() - 17000;
				level.progress = 3;
				level.timeLeap = 17;
				level.pause = 0;
				level.num = 8;
				level.cnt = 17;
			}
		}
		else if (level.progress > 3.1 && level.progress < 3.6) {
			if ((float)(end - start) / CLOCKS_PER_SEC >= 8) {
				start = clock() - 17000;
				level.progress = 4;
				start1 = clock();
				level.timeLeap = 15;
				level.pause = 0;
				level.num = 10;
				level.cnt = 25;
			}
		}
	}
	if (level.progress > 3.6 && level.progress < 4.1 && (float)(end - start1) / CLOCKS_PER_SEC <= 5) {
		EndLevel();
	}
	if (level.progress > 4.1 && level.progress < 4.6) {
			if (mouseX >= moneyX && mouseX <= moneyX + 91 && mouseY >= moneyY && mouseY <= moneyY + 82 && isMouseDown) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("play .\\audio\\winmusic.mp3", NULL, 0, NULL);
				level.progress = 5;
				intervalX = (moneyX - 400) / 500;
				intervalY = (moneyY - 300) / 500;
			}
			else{
				static float alpha = 100;
				static float jkl = 0.6;
				putpicture(moneyX, moneyY, &_Money_, RGB(255, 255, 255), alpha);
				alpha += jkl;
				if (alpha >= 280) jkl = -0.6;
				else if (alpha <= 100) jkl = 0.6;
		}
	}
	for (Zombie* p = _Zom; p != NULL; p = p->next) {
		if (p->type != 3 && p->type != 4) {
			try{
				putpicture(p->X + 25, p->Y + 106, &_shadow_, RGB(255, 255, 255), 100);
			}
			catch (...) {

			}
		}
		else if (p->type == 3){
			try {
				putpicture(p->X + 45, p->Y + 106, &_shadow_, RGB(255, 255, 255), 100);
			}catch(...){}
		}
		else if (p->type == 4) {
			try{
				putpicture(p->X + 60, p->Y + 106, &_shadow_, RGB(255, 255, 255), 100);
			}catch(...){}
		}
		if(p->DifferentStatus !=1 && p->DifferentStatus!=2 && p->DifferentStatus!=5)putimage(p->X, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][1][p->randomStatus][p->DifferentStatus], p->i * p->width[p->DifferentStatus], 0, SRCAND);
		if(p->DifferentStatus == 1)putimage(p->X + 10, p->Y - p->high[1] + 130, p->width[1], p->high[1], &_Z_[p->type][1][p->randomStatus][1], p->i * p->width[1], 0, SRCAND);
		else if (p->DifferentStatus == 2) {
			if(p->type!=4){
				if (p->i < p->maxPic[2]) putimage(p->X - 75, p->Y - p->high[2] + 130, p->width[2], p->high[2], &_Z_[p->type][1][p->randomStatus][2], p->i * p->width[2], 0, SRCAND);
				else putimage(p->X - 75, p->Y - p->high[2] + 130, p->width[2], p->high[2], &_Z_[p->type][1][p->randomStatus][2], (p->maxPic[2] - 1) * p->width[p->DifferentStatus], 0, SRCAND);
			}
			else {
				if (p->i < p->maxPic[2]) putimage(p->X + 24, p->Y - p->high[2] + 130, p->width[2], p->high[2], &_Z_[p->type][1][p->randomStatus][2], p->i * p->width[2], 0, SRCAND);
				else putimage(p->X + 24, p->Y - p->high[2] + 130, p->width[2], p->high[2], &_Z_[p->type][1][p->randomStatus][2], (p->maxPic[2] - 1) * p->width[p->DifferentStatus], 0, SRCAND);
			}
		}
		else if (p->DifferentStatus == 5) {
			if(p->i < p->maxPic[5])putimage(p->X, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][1][p->randomStatus][p->DifferentStatus], p->i * p->width[p->DifferentStatus], 0, SRCAND);
			else putimage(p->X, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][1][p->randomStatus][p->DifferentStatus], (p->maxPic[5] - 1) * p->width[p->DifferentStatus], 0, SRCAND);
		}

		if(p->DifferentStatus !=  1 && p->DifferentStatus != 2){
			if(p->DifferentStatus == 5 && p->i>=p->maxPic[5])
				putimage(p->X, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0][p->randomStatus][p->DifferentStatus], (p->maxPic[5] - 1) * p->width[p->DifferentStatus], 0, SRCINVERT);
			else putimage(p->X, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0 + 2*p->isAttack][p->randomStatus][p->DifferentStatus], p->i * p->width[p->DifferentStatus], 0, SRCINVERT);
		}
		else if (p->DifferentStatus == 1) {
			putimage(p->X + 10, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0 + 2*p->isAttack][p->randomStatus][p->DifferentStatus], p->i * p->width[p->DifferentStatus], 0, SRCINVERT);
		}
		else if (p->DifferentStatus == 2 ) {
			if(p->type!=4){
				if(p->i < p->maxPic[p->DifferentStatus]) putimage(p->X - 75, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0][p->randomStatus][p->DifferentStatus], p->i* p->width[p->DifferentStatus], 0, SRCINVERT);
				else putimage(p->X - 75, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0][p->randomStatus][p->DifferentStatus], (p->maxPic[p->DifferentStatus]-1) * p->width[p->DifferentStatus], 0, SRCINVERT);
			}
			else {
				if (p->i < p->maxPic[p->DifferentStatus]) putimage(p->X + 24, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0][p->randomStatus][p->DifferentStatus], p->i * p->width[p->DifferentStatus], 0, SRCINVERT);
				else putimage(p->X + 24, p->Y - p->high[p->DifferentStatus] + 130, p->width[p->DifferentStatus], p->high[p->DifferentStatus], &_Z_[p->type][0][p->randomStatus][p->DifferentStatus], (p->maxPic[p->DifferentStatus] - 1) * p->width[p->DifferentStatus], 0, SRCINVERT);
			}
		}
		//if(p->health <=90 && p->health >0){
		if (p->Zombie_head.DifferentStatus) {
			if(GameStatus == 1)p->Zombie_head.end = clock();
			if (p->Zombie_head.i < p->Zombie_head.maxPic[0]) {
				putimage(p->Zombie_head.X, p->Zombie_head.Y, p->Zombie_head.width[0], p->Zombie_head.high[0], &_Z_H[1], p->Zombie_head.i * p->Zombie_head.width[0],0, SRCAND);
				putimage(p->Zombie_head.X, p->Zombie_head.Y, p->Zombie_head.width[0], p->Zombie_head.high[0], &_Z_H[0], p->Zombie_head.i * p->Zombie_head.width[0],0, SRCINVERT);
			}
			else {
				putimage(p->Zombie_head.X, p->Zombie_head.Y, p->Zombie_head.width[0], p->Zombie_head.high[0], &_Z_H[1], (p->Zombie_head.maxPic[0]-1) * p->Zombie_head.width[0],0, SRCAND);
				putimage(p->Zombie_head.X, p->Zombie_head.Y, p->Zombie_head.width[0], p->Zombie_head.high[0], &_Z_H[0], (p->Zombie_head.maxPic[0]-1) * p->Zombie_head.width[0],0, SRCINVERT);
			}
		}
		//}
		if(GameStatus == 1){
			p->end = clock();
			sun.end = clock();
		}
		if ((double)(sun.end - sun.start2) / CLOCKS_PER_SEC >= sun.CD) {
			if (mouseX > sun.X && mouseX < sun.X + 76 && mouseY > sun.Y && mouseY < sun.Y + 75 && isMouseDown) sun.isClick = 1;
			SunFall(&sun,&SunNum,88,458,0.02);
		}
	}
	if(GameStatus == 1)sun.end = clock();
	for (int i = 0; i < 5; i++) {
		if (car[i].type) {
			putpicture(car[i].X + 5, car[i].Y + 35, &_shadow_, RGB(255, 255, 255), 100);
			putpicture(car[i].X, car[i].Y, &_car_, RGB(255, 255, 255));
		}
	}
	if ((double)(sun.end - sun.start2) / CLOCKS_PER_SEC >= sun.CD) {
		if (mouseX > sun.X && mouseX < sun.X + 76 && mouseY > sun.Y && mouseY < sun.Y + 75 && isMouseDown) sun.isClick = 1;
		SunFall(&sun, &SunNum, 88, 458,0.02);
	}
	/*if (MaxChoose == 8) {
		putpicture(532, 0, &_shovelSlot_, RGB(255, 255, 255));
	}
	else if (MaxChoose == 10) {
		putpicture(640, 0, &_shovelSlot_, RGB(255, 255, 255));
	}*/
	if (MHP.nPlant == -2) {
		putpicture(mouseX - _shovel_.getwidth() / 2, mouseY - _shovel_.getheight() / 2, &_shovel_, RGB(255, 255, 255));
	}
	/*else {
		if(MaxChoose == 8)
			putpicture(532, 0, &_shovel_, RGB(255, 255, 255));
		else putpicture(640, 0, &_shovel_, RGB(255, 255, 255));
	}*/
	if (level.progress > 4.6 && level.progress < 5.1) {
		putpicture(moneyX, moneyY, &_Money_, RGB(255, 255, 255));
		if (fabs(moneyX - 400) < 0.1 && fabs(moneyY - 300) < 0.1) {
			static float alpha = 0;
			static clock_t endStart = clock(), endEnd;
			putpicture(0, 0, &_EndDraw_, RGB(0, 0, 0), alpha);
			alpha += 0.5;
			endEnd = clock();
			if ((float)(endEnd - endStart) / CLOCKS_PER_SEC >= 8) {
				WinLevel();
			}
		}
		else {
			moneyX -= intervalX;
			moneyY -= intervalY;
		}
	}
	if(GameStatus == 1)FlushBatchDraw();
}
void PlantUpdateWithTime(OP *plant, int i, int j)
{
	int a = i * 9 + j;
	if (plant->hPlant == 0 || plant->hPlant == 7 || plant->hPlant == 40){
		plant->skill.end = clock();
		if ((double)(plant->skill.end - plant->skill.start) / CLOCKS_PER_SEC >= plant->skill.CD && hZombie[i]) {
			if (plant->hPlant == 0) {
				for (int i = 0; i < 2; i++) {
					if (plant->skill.x[i] < 0.1) {
						int random = rand() % 100;
						char r[4];
						itoa(random, r, 10);
						char c1[20] = "play shoot";
						char c2[45] = "open .\\audio\\shoop.mp3 alias shoot";
						char c3[20] = "close shoot";
						char* command1 = &c1[0];
						char* command2 = &c2[0];
						char* command3 = &c3[0];
						strcat(command1, r);
						strcat(command2, r);
						strcat(command3, r);
						mciSendString(command3, NULL, 0, NULL);
						mciSendString(command2, NULL, 0, NULL);
						mciSendString("setaudio .\\audio\\shoop.mp3 volume to 500", NULL, 0, NULL);
						mciSendString(command1, NULL, 0, NULL);
						plant->skill.x[i] = rOP[a][0] + 60;
						plant->skill.y[i] = rOP[a][1] + 5;
						plant->skill.start = clock();
						break;
					}
				}
			}
			else if (plant->hPlant == 7) {
				for (int i = 0; i < 2; i++) {
					if (plant->skill.x[i] < 0.1) {
						int random = rand() % 100;
						char r[4];
						itoa(random, r, 10);
						char c1[20] = "play shoot";
						char c2[45] = "open .\\audio\\shoop.mp3 alias shoot";
						char c3[20] = "close shoot";
						char* command1 = &c1[0];
						char* command2 = &c2[0];
						char* command3 = &c3[0];
						strcat(command1, r);
						strcat(command2, r);
						strcat(command3, r);
						mciSendString(command3, NULL, 0, NULL);
						mciSendString(command2, NULL, 0, NULL);
						mciSendString("setaudio .\\audio\\shoop.mp3 volume to 500", NULL, 0, NULL);
						mciSendString(command1, NULL, 0, NULL);
						plant->skill.x[i * 2] = rOP[a][0] + 60;
						plant->skill.y[i * 2] = rOP[a][1] + 5;
						plant->skill.start = clock();
						break;
					}
				}
			}
			else if (plant->hPlant == 40) {
				for (int i = 0; i < 2; i++) {
					if (plant->skill.x[i] < 0.1) {
						int random = rand() % 100;
						char r[4];
						itoa(random, r, 10);
						char c1[20] = "play shoot";
						char c2[45] = "open .\\audio\\shoop.mp3 alias shoot";
						char c3[20] = "close shoot";
						char* command1 = &c1[0];
						char* command2 = &c2[0];
						char* command3 = &c3[0];
						strcat(command1, r);
						strcat(command2, r);
						strcat(command3, r);
						mciSendString(command3, NULL, 0, NULL);
						mciSendString(command2, NULL, 0, NULL);
						mciSendString("setaudio .\\audio\\shoop.mp3 volume to 500", NULL, 0, NULL);
						mciSendString(command1, NULL, 0, NULL);
						plant->skill.x[i * 4] = rOP[a][0] + 60;
						plant->skill.y[i * 4] = rOP[a][1] + 20;
						plant->skill.start = clock();
						break;
					}
				}
			}
		}
		for(int i = 0;i < plant->skill.maxBullet;i ++){
			if (plant->skill.x[i] > 0.1) {
				if ((double)(plant->skill.end - plant->skill.Start[i]) / CLOCKS_PER_SEC >= plant->skill.timeleap) {
					plant->skill.x[i] += plant->skill.v;
					plant->skill.Start[i] = clock();
					if (plant->skill.x[i] > 780) {
						plant->skill.x[i] = 0;
					}
					for (Zombie* p = _Zom; p != NULL; p = p->next) {
						if (plant->skill.y[i] - p->Y < 80 && p->Y < plant->skill.y[i] && p->X - plant->skill.x[i]<10 && p->X >plant->skill.x[i] - 50 && p->DifferentStatus != 2 && p->DifferentStatus != 5) {
							int random = rand() % 1000;
							char r[4];
							itoa(random, r, 10);
							char c1[20] = "play splat";
							char c2[45] = "open .\\audio\\splat2.mp3 alias splat";
							char c3[20] = "close splat";
							char* command1 = &c1[0];
							char* command2 = &c2[0];
							char* command3 = &c3[0];
							strcat(command1, r);
							strcat(command2, r);
							strcat(command3, r);
							mciSendString(command3, NULL, 0, NULL);
							mciSendString(command2, NULL, 0, NULL);
							mciSendString(command1, NULL, 0, NULL);							
							p->health -= 20;
							p->isAttack = 1;
							p->start1 = clock();
							plant->skill.x[i] = 0;
							if (p->health <= 0) {
								mciSendString("close chomp", NULL, 0, NULL);
								p->DifferentStatus = 2;
								p->isDying = 0;
								p->i = 0;
								p->Zombie_head.type = 0;
								p->health = 100;
							}
							break;
						}
					}
				}
			}
		}
		if (plant->hPlant == 7) {
			for (int i = 0; i < 2; i++) {
				if (plant->skill.x[i * 2] > rOP[a][0] + 140 && plant->skill.x[i * 2 + 1] < 0.1) {
					if (level.progress < 2) {
						mciSendString("close shoot", NULL, 0, NULL);
						mciSendString("open .\\audio\\shoop.mp3 alias shoot", NULL, 0, NULL);
						mciSendString("setaudio shoot volume to 500", NULL, 0, NULL);
						mciSendString("play shoot", NULL, 0, NULL);
					}
					plant->skill.x[i * 2 + 1] = rOP[a][0] + 60;
					plant->skill.y[i * 2 + 1] = rOP[a][1] + 5;
				} 
			}
		}
		else if (plant->hPlant == 40) {
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					if (plant->skill.x[i * 4 + j] > rOP[a][0] + 140 && plant->skill.x[i * 4 + 1 + j] < 0.1 && plant->skill.x[i * 4 + j] < rOP[a][0] + 150) {
						if (level.progress < 2) {
							mciSendString("close shoot", NULL, 0, NULL);
							mciSendString("open .\\audio\\shoop.mp3 alias shoot", NULL, 0, NULL);
							mciSendString("setaudio shoot volume to 500", NULL, 0, NULL);
							mciSendString("play shoot", NULL, 0, NULL);
						}
						plant->skill.x[i * 4 + j + 1] = rOP[a][0] + 60;
						plant->skill.y[i * 4 + j + 1] = rOP[a][1] + 20;
						break;
					}
				}
			}
		}
	}
	else if (plant->hPlant == 4) {
		plant->skill.end = clock();
		if (plant->DifferentStatus == 1 && (double)(plant->skill.end - plant->skill.start) / CLOCKS_PER_SEC >= plant->skill.timeleap) {
			mciSendString("close dr", NULL, 0, NULL);
			mciSendString("open .\\audio\\dirt_rise.mp3 alias dr", NULL, 0, NULL);
			mciSendString("play dr", NULL, 0, NULL);
			plant->DifferentStatus = 0;
		}
		else if (plant->DifferentStatus == 0) {
			int isBomb = 0;
			for (Zombie* k = _Zom; k != NULL;) {
				if (rOP[a][1] - k->Y < 50 && k->Y < rOP[a][1] && k->X - rOP[a][0]<45 && k->X >rOP[a][0] - 90 && k->DifferentStatus != 2) {
					k->health -= 1800;
					k = k->next;
					DeleteZombie();
					mciSendString("close pm", NULL, 0, NULL);
					mciSendString("open .\\audio\\potato_mine.mp3 alias pm", NULL, 0, NULL);
					mciSendString("play pm", NULL, 0, NULL);
					isBomb = 1;
				}
				else {
					k = k->next;
				}
			}
			if (isBomb) {
				plant->DifferentStatus = 2;
				plant->i = 0;
			}
		}
	}
}
void updateWithoutInput()
{
	if (GameStatus == 1) {
		for (Zombie* p = _Zom; p != NULL; p = p->next) {
			if ((double)(p->Zombie_head.end - p->Zombie_head.start) / CLOCKS_PER_SEC >= 0.08 && p->Zombie_head.DifferentStatus) {
				if ((p->Zombie_head.i != 9 && p->Zombie_head.i != 10) || (double)(p->Zombie_head.end - p->Zombie_head.start) / CLOCKS_PER_SEC >= 0.16) {
					p->Zombie_head.start = clock();
					p->Zombie_head.i++;
					if (p->Zombie_head.i >= p->Zombie_head.maxPic[0] + 10) {
						p->Zombie_head.DifferentStatus = 0;
						p->Zombie_head.type = 1;
					}
				}
			}
			if (p->DifferentStatus != 2 && p->DifferentStatus != 5 && p->X <= -100 && GameStatus!=3 && level.progress > 0.1) {
				GameStatus = 3;
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("play .\\audio\\losemusic.mp3", NULL, 0, NULL);
				Sleep(2000);
				break;
			}
			if (p->type != 0 && p->type != 3 && p->health <= 270) {
				if (p->type == 1 || p->type == 2) {
					if (p->type == 1) {
						p->maxPic[0] = 22;
						p->speed = 1.4;
						p->width[0] = 106;
						p->timeLeap[0] = 0.09;
					}
					else if (p->type == 2) {
						p->maxPic[0] = 31;
						p->speed = 1;
						p->width[0] = 110;
						p->timeLeap[0] = 0.10;
					}
					p->type = 0;
					p->high[0] = 130;
					p->width[3] = 110;
					p->high[3] = 130;
					p->maxPic[3] = 21;
					p->timeLeap[3] = 0.1;

				}
				else {
					if (p->DifferentStatus == 0) p->DifferentStatus = 6;
				}
			}
			if (p->health <= 0) {
				mciSendString("close chomp", NULL, 0, NULL);
				p->DifferentStatus = 2;
				p->isDying = 0;
				p->i = 0;
				p->Zombie_head.type = 0;
				p->health = 100;
			}
			if ((double)(p->end - p->start) / CLOCKS_PER_SEC >= p->timeLeap[p->DifferentStatus]) {
				if (p->randomStatus || p->i % 2 == 0 || (double)(p->end - p->start) / CLOCKS_PER_SEC >= 2 * p->timeLeap[p->DifferentStatus] || p->DifferentStatus) {
					p->start = clock();
					p->i++;
					if ((double)(p->end - p->start1) / CLOCKS_PER_SEC >= 0.2 && p->isAttack) {
						p->isAttack = 0;
						p->start1 = clock();
					}
					if (p->health <= 90 && p->health > 0) {
						if (p->DifferentStatus != 4) {
							p->DifferentStatus = 1;
						}
						p->isDying = 1;
						if (p->Zombie_head.DifferentStatus == 0 && p->Zombie_head.type == 0) {
							if (p->type != 4) p->Zombie_head.X = p->X + 10;
							else p->Zombie_head.X = p->X + 40;
							p->Zombie_head.Y = p->Y;
							p->Zombie_head.i = 0;
							p->Zombie_head.DifferentStatus = 1;
						}
					}
					if (p->isDying) {
						p->health -= 3;
					}
					if (p->health <= 0) {
						mciSendString("close chomp", NULL, 0, NULL);
						p->DifferentStatus = 2;
						p->isDying = 0;
						p->i = 0;
						p->Zombie_head.type = 0;
						p->health = 100;
					}
					if (p->X < Sc_Width - 30)hZombie[((int)(p->Y - 46)) / 98] = 1;
					if (p->i >= p->maxPic[p->DifferentStatus]) {
						if (p->DifferentStatus != 2 && p->DifferentStatus != 5) p->i = 0;
						else if (p->i >= p->maxPic[p->DifferentStatus] + 10) p->health = 0;
					}

					if (p->DifferentStatus != 2 && p->DifferentStatus != 3 && p->DifferentStatus != 4 && p->DifferentStatus != 5 && p->DifferentStatus != 7)p->X -= p->speed;
				}
			}

		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 9; j++) {
				if (plant[i][j].hPlant != -1) {
					int a = i * 9 + j;
					if ((double)(plant[i][j].end - plant[i][j].start1) / CLOCKS_PER_SEC >= plant[i][j].timeLeap[0]) {
						plant[i][j].start1 = clock();
						for (Zombie* p = _Zom; p != NULL; p = p->next) {
							if (rOP[a][1] - p->Y < 50 && p->Y < rOP[a][1] && p->X - rOP[a][0]<25 && p->X >rOP[a][0] - 20 && p->DifferentStatus != 2 && plant[i][j].hPlant != 21 ) {
								if (plant[i][j].health <= 0) {
									mciSendString("close chomp", NULL, 0, NULL);
									for (Zombie* l = _Zom; l != NULL; l = l->next) {
										if (rOP[a][1] - l->Y < 50 && l->Y < rOP[a][1] && l->X - rOP[a][0]<50 && l->X >rOP[a][0] && l->DifferentStatus != 2 && l->DifferentStatus != 8) {
											if (l->type != 4 || l->health > 270) l->DifferentStatus = 0;
											else l->DifferentStatus = 6;
										}
									}
									plant[i][j].hPlant = -1;
									break;
								}
								else {
									if (p->DifferentStatus == 0) {
										p->i = 0;
										p->DifferentStatus = 3;
										plant[i][j].start2 = clock();
									}
									if (p->DifferentStatus == 3) {
										if(plant[i][j].hPlant != 17 || plant[i][j].DifferentStatus == 0)
											plant[i][j].health -= 10;
										plant[i][j].isAttack = 0;
										if ((double)(plant[i][j].end - plant[i][j].start2) / CLOCKS_PER_SEC > 1)
											plant[i][j].start2 = clock();
										if ((double)(plant[i][j].end - plant[i][j].start2) / CLOCKS_PER_SEC <= 0.3) {
											plant[i][j].isAttack = 1;
										}
									}
									if (p->DifferentStatus == 6 || (p->DifferentStatus == 3 && p->health < 270 && p->isComplex)) {
										p->DifferentStatus = 7;
										plant[i][j].start2 = clock();
									}
									if (p->DifferentStatus == 7) {
										if (plant[i][j].hPlant != 17 || plant[i][j].DifferentStatus == 0)
											plant[i][j].health -= 10;
										plant[i][j].isAttack = 0;
										if ((double)(plant[i][j].end - plant[i][j].start2) / CLOCKS_PER_SEC > 1)
											plant[i][j].start2 = clock();
										if ((double)(plant[i][j].end - plant[i][j].start2) / CLOCKS_PER_SEC <= 0.3) {
											plant[i][j].isAttack = 1;
										}
									}
									if (p->health <= 90 && p->health > 0 && p->DifferentStatus != 4) {
										p->DifferentStatus = 4;
										p->i = 0;
									}
									if (plant[i][j].health <= 0) {
										mciSendString("close chomp", NULL, 0, NULL);
										for (Zombie* l = _Zom; l != NULL; l = l->next) {
											if (rOP[a][1] - l->Y < 50 && l->Y < rOP[a][1] && l->X - rOP[a][0]<50 && l->X >rOP[a][0] && l->DifferentStatus != 2 && l->DifferentStatus != 8) {
												if (l->type != 4 || l->health > 270) l->DifferentStatus = 0;
												else l->DifferentStatus = 6;
											}
										}
										plant[i][j].hPlant = -1;
										break;
									}
									if (p->DifferentStatus != 5) {
										mciSendString("open .\\audio\\chomp.mp3 alias chomp", NULL, 0, NULL);
										mciSendString("play chomp repeat", NULL, 0, NULL);
									}
								}
							}
						}
						if (plant[i][j].health <= 0) plant[i][j].hPlant = -1;
					}
					if (plant[i][j].hPlant == 2 && plant[i][j].DifferentStatus) {
						for (Zombie* p = _Zom; p != NULL; p = p->next) {
							if ((abs(rOP[a][1] - p->Y) < 140 || abs(p->Y - rOP[a][3]) < 50) && (abs(p->X + 88 - rOP[a][2]) < 84 || abs(rOP[a][0] - p->X) < 84) 
								&& p->DifferentStatus!=1 && p->DifferentStatus!= 2 && p->DifferentStatus!=4) {
								p->DifferentStatus = 5;
								p->type = 0;
								p->i = 0;
							}
						}
						mciSendString("close chomp", NULL, 0, NULL);
					}
					else if (plant[i][j].hPlant == 3) {
						if (plant[i][j].health <= 2666 && plant[i][j].health > 1333) {
							plant[i][j].DifferentStatus = 1;
						}
						else if (plant[i][j].health <= 1333 && plant[i][j].health > 0) {
							plant[i][j].DifferentStatus = 2;
						}
					}
					else if (plant[i][j].hPlant == 17) {
						for (Zombie* p = _Zom; p != NULL; p = p->next) {
							if (rOP[a][1] - p->Y < 60 && p->Y < rOP[a][1] && ((p->X - rOP[a][0]<120 && p->X >rOP[a][0]) || (rOP[a][0] - p->X < 90 && rOP[a][0] > p->X)) && p->DifferentStatus != 2) {
								if (plant[i][j].DifferentStatus == 0) {
									plant[i][j].i = 0;
									plant[i][j].DifferentStatus = 1;
									plant[i][j].X = p->X;
									plant[i][j].Y = p->Y;
								}
								else {
									if (plant[i][j].i == 15 && fabs(p->X - plant[i][j].X < 10)) p->health = 0;
								}
							}
						}
					}
					else if (plant[i][j].hPlant == 20 && plant[i][j].DifferentStatus) {
						for (Zombie* p = _Zom; p != NULL; p = p->next) {
							if (((int)(p->Y - 46)) / 98 == i && p->DifferentStatus != 1 && p->DifferentStatus != 2 && p->DifferentStatus != 4) {
								p->DifferentStatus = 5;
								p->type = 0;
								p->i = 0;
							}
						}
						mciSendString("close chomp", NULL, 0, NULL);
					}
					else if (plant[i][j].hPlant == 21) {
						plant[i][j].skill.end = clock();
						if ((double)(plant[i][j].skill.end - plant[i][j].skill.start1) / CLOCKS_PER_SEC >= plant[i][j].skill.timeleap) {
							plant[i][j].skill.start1 = clock();
							for (Zombie* p = _Zom; p != NULL; p = p->next) {
								if (rOP[a][1] - p->Y < 60 && p->Y < rOP[a][1] && p->X - rOP[a][0]<40 && p->X >rOP[a][0] - 90 && p->DifferentStatus != 2) {
									p->health -= 10;
									if (!p->isAttack) p->start1 = clock();
									p->isAttack = 1;
									if (p->health <= 0) {
										p->isAttack = 0;
										mciSendString("close chomp", NULL, 0, NULL);
										p->DifferentStatus = 2;
										p->isDying = 0;
										p->i = 0;
										p->Zombie_head.type = 0;
										p->health = 100;
									}
								}
							}
						}
					}
				}
			}
			if (car[i].type == 2) {
				car[i].end = clock();
				if ((float)(car[i].end - car[i].start) / CLOCKS_PER_SEC >= 0.02) {
					car[i].X += 10;
					car[i].start = clock();
				}
			}
			if (car[i].X >= 880) car[i].type = 0;
			for (Zombie* p = _Zom; p != NULL; p = p->next) {
				if (car[i].type && car[i].Y - p->Y < 80 && p->Y < car[i].Y && p->X - car[i].X < 40 && p->X >car[i].X) {
					if (car[i].type == 1) {
						car[i].start = clock();
						car[i].type = 2;
						car[i].X += 10;
					}
					mciSendString("close chomp", NULL, 0, NULL);
					p->DifferentStatus = 2;
					if (p->type != 4) p->i = 5;
					else p->i = 0;
					if (p->Zombie_head.DifferentStatus == 0 && p->Zombie_head.type == 0) {
						if (p->type != 4) p->Zombie_head.X = p->X + 10;
						else p->Zombie_head.X = p->X + 40;
						p->Zombie_head.Y = p->Y;
						p->Zombie_head.i = 0;
						p->Zombie_head.DifferentStatus = 1;
					}

				}
			}
		}
		DeleteZombie();
	}
	else if (GameStatus == 2) {
		preX = mouseX;
		preY = mouseY;
		MouseOperation();
		if (isMouseDown && mouseX >= pauseX && mouseX <= pauseX + 414 && mouseY >= pauseY && mouseY <= pauseY + 488) {
			if (pauseX >= 0 && pauseX <= Sc_Width - 414) {
				pauseX += (mouseX - preX);
				if (pauseX < 0) pauseX = 0;
				else if (pauseX > Sc_Width - 414) pauseX = Sc_Width - 414;
			}
			if (pauseY >= 0 && pauseY <= Sc_High -488) {
				pauseY += (mouseY - preY);
				if (pauseY < 0) pauseY = 0;
				else if (pauseY > Sc_High - 488) pauseY = Sc_High - 488;
			}
			if (mouseX >= pauseX + 38 && mouseX < pauseX + 380 && mouseY > pauseY + 392 && mouseY < pauseY + 475) {
				GameStatus = 1;
				for (int i = 0; i < 5; i++) {
					for (int j = 0; j < 9; j++) {
						if (plant[i][j].hPlant != -1) {
							plant[i][j].start = clock() - plant[i][j].timeInterval[0];
							if (plant[i][j].start1 > 0.1)plant[i][j].start1 = clock() - plant[i][j].timeInterval[1];
							if (plant[i][j].start2 > 0.1)plant[i][j].start2 = clock() - plant[i][j].timeInterval[2];
							if (plant[i][j].hPlant == 1) {
								plant[i][j].skill.start = clock() - plant[i][j].skill.timeInterval[0];
								plant[i][j].skill.start1 = clock() - plant[i][j].skill.timeInterval[1];
								plant[i][j].skill.start2 = clock() - plant[i][j].skill.timeInterval[2];
								plant[i][j].skill.start3 = clock() - plant[i][j].skill.timeInterval[3];
							}
							else if (plant[i][j].hPlant == 0 || plant[i][j].hPlant == 7 || plant[i][j].hPlant == 40) {
								plant[i][j].skill.start = clock() - plant[i][j].skill.timeInterval[0];
								for (int k = 0; k < plant[i][j].skill.maxBullet; k++) {
									plant[i][j].skill.Start[k] = clock() - plant[i][j].skill.timeInterval[k + 1];
								}
							}
						}
					}
				}
				for (Zombie* p = _Zom; p != NULL; p = p->next) {
					p->start = clock() - p->timeInterval[0];
					p->start1 = clock() - p->timeInterval[1];
					if (p->Zombie_head.DifferentStatus)p->Zombie_head.start = clock() - p->timeInterval[2];
				}
				sun.start = clock() - sun.timeInterval[0];
				sun.start1 = clock() - sun.timeInterval[1];
				sun.start2 = clock() - sun.timeInterval[2];
				sun.start3 = clock() - sun.timeInterval[3];
			}
			else if (mouseX >= pauseX + 108 && mouseX < pauseX + 313 && mouseY > pauseY + 286 && mouseY < pauseY + 328) {
				GameStatus = 1;
				level.progress = 0;
				for (Card* p = _card, *pr = _card; p != NULL;) {
					pr = p;
					p = p->next;
					free(pr);
				}
				for (Zombie* p = _Zom, *pr = _Zom; p != NULL;) {
					pr = p;
					p = p->next;
					free(pr);
				}
			}
			else if (mouseX >= pauseX + 108 && mouseX < pauseX + 313 && mouseY > pauseY + 328 && mouseY < pauseY + 370) {
				GameStatus = 0;
				level.progress = 0;
				for (Card* p = _card, *pr = _card; p != NULL;) {
					pr = p;
					p = p->next;
					free(pr);
				}
				for (Zombie* p = _Zom, *pr = _Zom; p != NULL;) {
					pr = p;
					p = p->next;
					free(pr);
				}
			}	
		}
	}
}
void updateWithInput()
{
	char input = 0;
	if (_kbhit()) {
		input = _getch();
		if (input == 27 && GameStatus != 2 && GameStatus != 0 && level.progress >= 0.7 && (level.progress < 4.6 || level.progress > 5.1)) {
			GameStatus = 2;
			for (int i = 0; i < 5; i++) {
				for (int j = 0; j < 9; j++) {
					if (plant[i][j].hPlant != -1) {
						plant[i][j].timeInterval[0] = plant[i][j].end - plant[i][j].start;
						if (plant[i][j].start1 > 0.1)plant[i][j].timeInterval[1] = plant[i][j].end - plant[i][j].start1;
						if (plant[i][j].start2 > 0.1)plant[i][j].timeInterval[2] = plant[i][j].end - plant[i][j].start2;
						if (plant[i][j].hPlant == 1) {
							plant[i][j].skill.timeInterval[0] = plant[i][j].skill.end - plant[i][j].skill.start;
							plant[i][j].skill.timeInterval[1] = plant[i][j].skill.end - plant[i][j].skill.start1;
							plant[i][j].skill.timeInterval[2] = plant[i][j].skill.end - plant[i][j].skill.start2;
							plant[i][j].skill.timeInterval[3] = plant[i][j].skill.end - plant[i][j].skill.start3;
						}
						else if (plant[i][j].hPlant == 0 || plant[i][j].hPlant == 7 || plant[i][j].hPlant == 40) {
							plant[i][j].skill.timeInterval[0] = plant[i][j].skill.end - plant[i][j].skill.start;
							for (int k = 0; k < plant[i][j].skill.maxBullet; k++) {
								plant[i][j].skill.timeInterval[k + 1] = plant[i][j].skill.end - plant[i][j].skill.Start[k];
							}
						}
					}
				}
			}
			for (Zombie* p = _Zom; p != NULL; p = p->next) {
				p->timeInterval[0] = p->end - p->start;
				p->timeInterval[1] = p->end - p->start1;
				if (p->Zombie_head.DifferentStatus)p->timeInterval[2] = p->Zombie_head.end - p->Zombie_head.start;
			}
			sun.timeInterval[0] = sun.end - sun.start;
			sun.timeInterval[1] = sun.end - sun.start1;
			sun.timeInterval[2] = sun.end - sun.start2;
			sun.timeInterval[3] = sun.end - sun.start3;
		}
	}
}
void gameover()
{
	return;
}
int ChoosePageJudge(int x, int y,int rChoose[48][4])
{
	if(isMouseDown){
		/*mciSendString("close t", NULL, 0, NULL);
		mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
		mciSendString("play t", NULL, 0, NULL);*/
		for (int i = 0; i < 48; i++) {
			if (x > rChoose[i][0] && x<rChoose[i][2] && y>rChoose[i][1] && y < rChoose[i][3])
				return i;
		}
	}
	return -1;
}
int OPJudge(int x, int y, int rOP[45][4])
{
	
	for (int i = 0; i < 48; i++) {
		if (x > rOP[i][0] && x<rOP[i][2] && y>rOP[i][1] && y < rOP[i][3])
			return i;
		
	}
	return -1;
}
int BoxJudge(int x, int y, int rCard[10][4])
{
	if(isMouseDown){
		/*mciSendString("close t", NULL, 0, NULL);
		mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
		mciSendString("play t", NULL, 0, NULL);*/
		for (int i = 0; i < MaxChoose; i++) {
			if (x > rCard[i][0]  && x<rCard[i][2]  && y > rCard[i][1] && y < rCard[i][3])
				return i;
		}
	}
	return -1;
}

void ChoosePage()
{
	
	int Sun[48] = { 100,50,150,50,25,175,150,200,0,25,75,75,75,25,75,125,25,50,325,25,125,100,175,125,
					0,25,125,100,125,125,125,100,100,25,100,75,50,100,50,300,250,150,150,225,200,50,125,500 };
	float CD[48] = { 0 };
	CD[0] = 7.5, CD[1] = 7.5, CD[2] = 50, CD[3] = 30, CD[4] = 30, CD[7] = 7.5, CD[17] = 30, CD[20] = 40, CD[21] = 7.5, CD[40] = 40;
	int Health[48];
	for (int i = 0; i < 48; i++) Health[i] = 300;
	Health[3] = 4000;
	Health[23] = 8000;
	Health[30] = 4000;
	int rChoose[48][4];
	int hChoose = 0;
	int someBox;
	int event = -1;
	int bevent = -1;
	int StandMax = 10;
	int prevent = -1;
	static clock_t end = clock(), start = clock(),start1 = clock();
	float cardx=0, cardy=0;
	float intervalx = 0, intervaly = 0;
	IMAGE Zom_Stand[18][3][2];
	IMAGE start_button[3];
	Card Choose[48];
	Zombie ZomStand[10];
	if(level.checkpoint == 2 || level.checkpoint == 6)ZomStand[0].type = 1;
	else if (level.checkpoint == 3 || level.checkpoint == 7) {
		ZomStand[0].type = 1;
		ZomStand[1].type = 2;
	}
	else if (level.checkpoint == 4 || level.checkpoint == 8) {
		ZomStand[0].type = 1;
		ZomStand[1].type = 2;
		ZomStand[2].type = 4;
	}
	for (int i = 0; i < StandMax; i++) {
		ZomStand[i].X = (float)(470 + rand() % 225);
		ZomStand[i].Y = (float)(86 + rand() % 357);
		if (!ZomStand[i].type) {
			ZomStand[i].randomStatus = rand() % 3;
			switch (ZomStand[i].randomStatus) {
			case 0:
				ZomStand[i].width[0] = 106;
				ZomStand[i].high[0] = 125;
				ZomStand[i].timeLeap[0] = 0.15;
				ZomStand[i].maxPic[0] = 11;
				break;
			case 1:
				ZomStand[i].width[0] = 106;
				ZomStand[i].high[0] = 130;
				ZomStand[i].timeLeap[0] = 0.1;
				ZomStand[i].maxPic[0] = 22;
				break;
			case 2:
				ZomStand[i].width[0] = 106;
				ZomStand[i].high[0] = 130;
				ZomStand[i].timeLeap[0] = 0.1;
				ZomStand[i].maxPic[0] = 11;
				break;
			}
		}
		else {
			switch (ZomStand[i].type) {
			case 1:
				ZomStand[i].width[0] = 90;
				ZomStand[i].high[0] = 144;
				ZomStand[i].timeLeap[0] = 0.12;
				ZomStand[i].maxPic[0] = 8;
				break;
			case 2:
				ZomStand[i].width[0] = 90;
				ZomStand[i].high[0] = 144;
				ZomStand[i].timeLeap[0] = 0.15;
				ZomStand[i].maxPic[0] = 6;
				break;
			case 4:
				ZomStand[i].width[0] = 128;
				ZomStand[i].high[0] = 144;
				ZomStand[i].timeLeap[0] = 0.1;
				ZomStand[i].maxPic[0] = 15;
			}
		}
		for (int j = 0; j < i; j++) {
			if (fabs(ZomStand[i].X - ZomStand[j].X) < 50.0 && fabs(ZomStand[i].Y - ZomStand[j].Y) < 60.0) {
				ZomStand[i].X = (float)(470 + rand() % 225);
				ZomStand[i].Y = (float)(86 + rand() % 357);
				j = -1;
			}
		}
	}
	for (int i = 0, min; i < StandMax - 1; i++) {
		min = i;
		Zombie tem;
		for (int j = i + 1; j < StandMax; j++) {
			if (ZomStand[j].Y < ZomStand[min].Y) 
				min = j;
		}
		tem = ZomStand[i];
		ZomStand[i] = ZomStand[min];
		ZomStand[min] = tem;
	}
	loadimage(&Zom_Stand[0][0][0], ".\\Zombie\\Zom_stand_1.png");
	loadimage(&Zom_Stand[0][0][1], ".\\Zombie\\Zom_stand_1_mask.png");
	loadimage(&Zom_Stand[0][1][0], ".\\Zombie\\Zom_stand_2.png");
	loadimage(&Zom_Stand[0][1][1], ".\\Zombie\\Zom_stand_2_mask.png");
	loadimage(&Zom_Stand[0][2][0], ".\\Zombie\\Zom_stand_3.png");
	loadimage(&Zom_Stand[0][2][1], ".\\Zombie\\Zom_stand_3_mask.png");
	loadimage(&Zom_Stand[1][0][0], ".\\Zombie\\ConeZom_stand.png");
	loadimage(&Zom_Stand[1][0][1], ".\\Zombie\\ConeZom_stand_mask.png");
	loadimage(&Zom_Stand[2][0][0], ".\\Zombie\\BucketZom_stand.png");
	loadimage(&Zom_Stand[2][0][1], ".\\Zombie\\BucketZom_stand_mask.png");
	loadimage(&Zom_Stand[4][0][0], ".\\Zombie\\FootballZom_stand.png");
	loadimage(&Zom_Stand[4][0][1], ".\\Zombie\\FootballZom_stand_mask.png");
	loadimage(&start_button[0], ".\\Screen\\StartButton.png");
	loadimage(&start_button[1], ".\\Screen\\StartButton_mask.png");
	loadimage(&start_button[2], ".\\Screen\\StartButton_light.png");
	for (int i = 0; i < 48; i++){
		rChoose[i][0] = 20 + 53 * (i % 8);
		rChoose[i][1] = 123 + (i / 8) * 70;
		rChoose[i][2] = 73 + 53 * (i % 8);
		rChoose[i][3] = 193 + (i / 8) * 70;
	}
	while (1) {
		if(level.checkpoint <= 4)putimage(0, 0, 800, 600, &map[0], 600, 0);
		else putimage(0, 0, 800, 600, &map[1], 600, 0);
		putimage(0, 90, &_choosepage_);
		if(MaxChoose == 8)putpicture(0, 0, &plantbox,RGB(255,255,255));
		else putpicture(0, 0, &plantboxM, RGB(255, 255, 255));
		for (int i = 0; i < 48; i++) {
			if (Choose[i].cChoose == 1) {
				putimage(rChoose[i][0], rChoose[i][1], &_c_[i][0]);
			}
			else {
				putimage(rChoose[i][0], rChoose[i][1], &_c_[i][1]);
			}
			Choose[i].nSun = Sun[i];
			Choose[i].uCD = CD[i];
			Choose[i].health = Health[i];
		}		
		MouseOperation();
		prevent = ChoosePageJudge(mouseX, mouseY, rChoose);
		if(event == -1 && hChoose < MaxChoose && Choose[prevent].cChoose) event = ChoosePageJudge(mouseX, mouseY, rChoose);
		else if (event!=-1 ) {
			if (cardx == 0) {
				mciSendString("close t", NULL, 0, NULL);
				mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
				mciSendString("play t", NULL, 0, NULL);
				Choose[event].cChoose = 0;
				cardx = rChoose[event][0];
				cardy = rChoose[event][1];
				intervalx = (rCard[getChooserNum()][0] - 10 - cardx) / 10;
				intervaly = (cardy - rCard[getChooserNum()][1]) /10;
				hChoose++;
				someBox = event;
			}
			else if ((cardy - (float)(rCard[getChooserNum()][1]))* (cardy - (float)(rCard[getChooserNum()][1])) < 0.01 ) {
				cardx = 0;
				cardy = 0;
				intervalx = 0;
				intervaly = 0;
				if (getChooserNum() == 0) {
					_card = (Card*)malloc(sizeof(Card));
					_card->nPlant = event;
					_card->next = NULL;
					_card->nSun = Choose[event].nSun;
					_card->uCD = Choose[event].uCD;
					_card->health = Choose[event].health;
				}
				else addChooser(event,Choose[event]);
				event = -1;
			}
		}
		if (getChooserNum() < bevent || (getChooserNum() == bevent && cardx == 0)) bevent = -1;
		if ((double)(end - start1) / CLOCKS_PER_SEC >= 0.08) {
			if (bevent == -1 && cardx == 0)bevent = BoxJudge(mouseX + 10, mouseY, rCard);
			start1 = clock();
		}
		else if (bevent != -1 && (getChooserNum() > bevent || (getChooserNum() == bevent && cardx!=0))) {
			if(getChooserNum() > bevent){
				if (cardx == 0) {
					mciSendString("close t", NULL, 0, NULL);
					mciSendString("open .\\audio\\tap.mp3 alias t", NULL, 0, NULL);
					mciSendString("play t", NULL, 0, NULL);
					if (bevent == 0) {
						someBox = _card->nPlant;
						Card* first = _card;
						_card = _card->next;
						free(first);
					}
					else someBox = deleteChooser(bevent, _card);
					hChoose--;
					cardx = rCard[bevent][0] - 10;
					cardy = rCard[bevent][1];
					intervalx = (rChoose[someBox][0] - cardx) / 10;
					intervaly = (cardy - rChoose[someBox][1]) / 10;
				}
			}
			if (((float)(rChoose[someBox][1]) - cardy)* ((float)(rChoose[someBox][1]) - cardy) < 0.01) {
				cardx = 0;
				cardy = 0;
				intervalx = 0;
				intervaly = 0;
				bevent = -1;
				Choose[someBox].cChoose = 1;
			}
		}
		if (cardx != 0 && cardy != 0) {
			if ((double)(end - start) / CLOCKS_PER_SEC >= 0.01) {
				cardx += intervalx;
				cardy -= intervaly;
				start = clock();
			}
			putimage(cardx, cardy, &_c_[someBox][0]);
		}
		end = clock();
		int k = 0;
		for (Card* p = _card; p != NULL; p = p->next) {
			putimage(rCard[k][0] - 10, rCard[k][1], &_c_[p->nPlant][0]);
			k++;
		}
		if(hChoose < MaxChoose)
			putimage(154, 546, &start_button[1]);
		else putimage(154, 546, &start_button[0]);
		for (int i = 0; i < StandMax; i++) {
			if((float)(ZomStand[i].end - ZomStand[i].start)/CLOCKS_PER_SEC >= ZomStand[i].timeLeap[0]){
				ZomStand[i].start = clock();
				ZomStand[i].i++;
				if (ZomStand[i].i == ZomStand[i].maxPic[0]) ZomStand[i].i =0;
			}
			if(ZomStand[i].type !=4)putpicture(ZomStand[i].X + 30, ZomStand[i].Y + ZomStand[i].high[0] - 24, &_shadow_, RGB(255, 255, 255), 100);
			else putpicture(ZomStand[i].X + 38, ZomStand[i].Y + ZomStand[i].high[0] - 24, &_shadow_, RGB(255, 255, 255), 100);
			putimage(ZomStand[i].X, ZomStand[i].Y, ZomStand[i].width[0], ZomStand[i].high[0], &Zom_Stand[ZomStand[i].type][ZomStand[i].randomStatus][1], ZomStand[i].i * ZomStand[i].width[0], 0, SRCAND);
			putimage(ZomStand[i].X, ZomStand[i].Y, ZomStand[i].width[0], ZomStand[i].high[0], &Zom_Stand[ZomStand[i].type][ZomStand[i].randomStatus][0], ZomStand[i].i * ZomStand[i].width[0], 0, SRCINVERT);
			ZomStand[i].end = clock();
		}
		MouseOperation();
		if (mouseX >= 154 && mouseX <= 308 && mouseY >= 546 && mouseY <= 584 && hChoose == MaxChoose) {
			putimage(154, 546, &start_button[2]);
			if(isMouseDown){
				level.progress = 0.5;
				int i = 0;
				for (Card* p = _card,*pr = p; p != NULL; ) {
					_card_[i] = *p;
					i++;
					pr = p;
					p = p->next;
					free(pr);
				}
				_card = NULL;
				break;
			}
		}
		FlushBatchDraw();
	}
}
void StartLevel()
{
	int alpha[3] = { 0 };
	mciSendString("close rsp", NULL, 0, NULL);
	mciSendString("open .\\audio\\rsp.mp3 alias rsp", NULL, 0, NULL);
	mciSendString("play rsp", NULL, 0, NULL);
	while (1) {
		if(level.checkpoint <= 4)putimage(0, 150, 800, 450, &map[0], 230, 150);
		else putimage(0, 150, 800, 450, &map[1], 230, 150);
		if (alpha[0] < 256) {
			putpicture(400 - _StartLevel_[0].getwidth() / 2, 300 - _StartLevel_[0].getheight() / 2, &_StartLevel_[0], RGB(0, 0, 0), alpha[0]);
			alpha[0] += 8;
		}
		else if (alpha[1] < 256) {
			putpicture(400 - _StartLevel_[1].getwidth() / 2, 300 - _StartLevel_[1].getheight() / 2, &_StartLevel_[1], RGB(0, 0, 0), alpha[1]);
			alpha[1] += 6;
		}
		else if (alpha[2] < 256) {
			putpicture(400 - _StartLevel_[2].getwidth() / 2, 300 - _StartLevel_[2].getheight() / 2, &_StartLevel_[2], RGB(0, 0, 0), alpha[2]);
			alpha[2] += 3;
		}
		else break;
		delay(5);
		FlushBatchDraw();
	}
}
void addZombie(int num,int type)
{
	Zombie* p;
	Zombie* add;
	int limit[3];
	if (level.checkpoint == 2 || level.checkpoint == 6) limit[0] = num / 3;
	else if (level.checkpoint == 3 || level.checkpoint == 7) {
		limit[0] = num / 2;
		limit[1] = num / 3;
	}
	else if (level.checkpoint == 4 || level.checkpoint == 8) {
		limit[0] = num / 2;
		limit[1] = num / 3;
		limit[2] = num / 4;
	}
	for (int i = 0; i < num; i++) {
		add = (Zombie*)malloc(sizeof(Zombie));
		add->X = 800;
		add->Y = rOP[rand() % 45][1] - 40;
		add->i = 0;
		add->isAttack = 0;
		add->DifferentStatus = 0;
		add->isComplex = 0;
		add->maxPic[1] = 18;
		add->maxPic[2] = 10;
		add->maxPic[3] = 21;
		add->maxPic[4] = 11;
		add->maxPic[5] = 20;
		add->timeLeap[1] = 0.18;
		add->timeLeap[2] = 0.1;
		add->timeLeap[3] = 0.1;
		add->timeLeap[4] = 0.1;
		add->timeLeap[5] = 0.1;
		add->width[1] = 95;
		add->high[1] = 130;
		add->width[2] = 166;
		add->high[2] = 130;
		add->width[3] = 110;
		add->high[3] = 130;
		add->width[4] = 110;
		add->high[4] = 130;
		add->width[5] = 106;
		add->high[5] = 130;
		add->isDying = 0;
		add->health = 270;
		add->type = type;
		if ((level.cnt == 10 || level.cnt == 17 ||level.cnt == 25) && i == 0) add->type = 3;
		if(level.checkpoint == 2 || level.checkpoint == 6){
			if (!type && i > 0 && i <= limit[0]) add->type = 1;
		}
		else if (level.checkpoint == 3 || level.checkpoint ==  7) {
			if (!type && i > 0 && i <= limit[1]) add->type = 2;
			else if (!type && i > 0 && i <= limit[0]) add->type = 1;
		}
		else if (level.checkpoint == 4 || level.checkpoint == 8) {
			if (!type && i > 0 && i <= limit[2]) add->type = 4;
			else if (!type && i > 0 && i <= limit[1]) add->type = 2;
			else if (!type && i > 0 && i <= limit[0]) add->type = 1;
		}
		add->randomStatus = 0;
		add->next = NULL;
		add->Zombie_head.width[0] = 150;
		add->Zombie_head.high[0] = 186;
		add->Zombie_head.maxPic[0] = 12;
		add->Zombie_head.DifferentStatus = 0;
		add->Zombie_head.i = 0;
		add->Zombie_head.type = 0;
		if (!add->type) {
			add->randomStatus = rand() % 3;
			switch (add->randomStatus) {
			case 0:
				add->width[0] = 106;
				add->high[0] = 130;
				add->speed = 1.4;
				add->maxPic[0] = 22;
				add->timeLeap[0] = 0.09;
				break;
			case 1:
				add->width[0] = 110;
				add->high[0] = 130;
				add->maxPic[0] = 31;
				add->speed = 1;
				add->timeLeap[0] = 0.10;
				break;
			case 2:
				add->width[0] = 110;
				add->high[0] = 130;
				add->maxPic[0] = 18;
				add->speed = 2.2;
				add->timeLeap[0] = 0.20;
				break;
			}
		}
		else {
			switch (add->type) {
			case 1:
				add->width[0] = 90;
				add->high[0] = 144;
				add->width[3] = 90;
				add->high[3] = 144;
				add->speed = 2;
				add->maxPic[0] = 21;
				add->maxPic[3] = 11;
				add->health = 640;
				add->timeLeap[0] = 0.18;
				add->timeLeap[3] = 0.09;
				break;
			case 2:
				add->width[0] = 90;
				add->high[0] = 144;
				add->width[3] = 95;
				add->high[3] = 144;
				add->speed = 1.4;
				add->maxPic[0] = 15;
				add->maxPic[3] = 11;
				add->health = 1370;
				add->timeLeap[0] = 0.18;
				add->timeLeap[3] = 0.09;
				add->randomStatus = 1;
				break;
			case 3:
				add->width[0] = 110;
				add->high[0] = 144;
				add->width[1] = 110;
				add->high[1] = 144;
				add->width[3] = 110;
				add->high[3] = 144;
				add->width[4] = 110;
				add->high[4] = 144;
				add->speed = 5;
				add->maxPic[0] = 12;
				add->maxPic[1] = 12;
				add->maxPic[3] = 11;
				add->maxPic[4] = 11;
				add->health = 270;
				add->timeLeap[0] = 0.18;
				add->timeLeap[3] = 0.09;
				break;
			case 4:
				add->width[0] = 154;
				add->high[0] = 160;
				add->width[1] = 154;
				add->high[1] = 152;
				add->width[2] = 194;
				add->high[2] = 138;
				add->width[3] = 128;
				add->high[3] = 144;
				add->width[4] = 128;
				add->high[4] = 144;
				add->width[6] = 156;
				add->high[6] = 160;
				add->width[7] = 128;
				add->high[7] = 138;
				add->speed = 3.8;
				add->maxPic[0] = 11;
				add->maxPic[1] = 10;
				add->maxPic[2] = 7;
				add->maxPic[3] = 10;
				add->maxPic[4] = 10;
				add->maxPic[6] = 11;
				add->maxPic[7] = 11;
				add->health = 1670;//1670
				add->timeLeap[0] = 0.09;
				add->timeLeap[1] = 0.1;
				add->timeLeap[3] = 0.09;
				add->timeLeap[6] = 0.09;
				add->timeLeap[7] = 0.09;
				add->isComplex = 1;
				break;
			}
		}
		if (_Zom == NULL) {
			_Zom = add;
			continue;
		}
		else if (add->Y < _Zom->Y) {
			p = _Zom;
			_Zom = add;
			_Zom->next = p;
		}
		else {
			for (p = _Zom; ; p = p->next) {
				if (add->Y >= p->Y && (p->next == NULL || add->Y <= p->next->Y)) {
					if (p->next != NULL) {
						if (fabs(p->Y - p->next->Y) > 0.1 ) {
							if (fabs(add->Y - p->Y) < 0.1 && p->X > 799) add->X = p->X + 30;
							else if (fabs(add->Y - p->next->Y) < 0.1 && p->next->X > 799) add->X = p->next->X + 30;
						}
						else if (fabs(p->Y - p->next->Y) < 0.1 && fabs(add->Y - p->Y) < 0.1 && p->X > 799 && p->next->X > 799) {
							if (p->X < p->next->X) add->X = p->next->X + 30;
							else add->X = p->X + 30;
						}
					}
					else if (fabs(add->Y - p->Y) < 0.1 && p->X > 799) add->X = p->X + 30;
					add->next = p->next;
					p->next = add;
					break;
				}
			}
		}
		if (add->type == 3) add->X = 800;
		/*p->next = add;
		p = p->next;*/
	}
	
}
void DeleteZombie()
{
	if(_Zom!=NULL){
		if ( _Zom->health <= 0) {
			mciSendString("close chomp", NULL, 0, NULL);
			Zombie* p = _Zom;
			_Zom = _Zom->next;
			hZombie[((int)(p->Y - 46)) / 98] = 0;
			free(p);
		}
		else {
			for (Zombie* p = _Zom;p!=NULL && p->next != NULL; ) {
				if (p->next->health <= 0) {
					mciSendString("close chomp", NULL, 0, NULL);
					Zombie* de = p->next;
					p->next = de->next;
					hZombie[((int)(de->Y - 46)) / 98] = 0;
					free(de);
				}
				else p = p->next;
			}
		}
	}
}
void MainMenu()
{
	IMAGE mainmenu,grass,grass_mask,grave,grave_mask,Adventure, Adventure_mask,Adventure_light, Adventure_light_mask;
	IMAGE ZomHand,ZomHand_mask;
	loadimage(&mainmenu, ".\\Screen\\MainMenu.png");
	loadimage(&grass, ".\\Screen\\Grass.png");
	loadimage(&grass_mask, ".\\Screen\\Grass_mask.png");
	loadimage(&grave, ".\\Screen\\Grave.png");
	loadimage(&grave_mask, ".\\Screen\\Grave_mask.png");
	loadimage(&Adventure, ".\\Screen\\Adventure.png");
	loadimage(&Adventure_mask, ".\\Screen\\Adventure_mask.png");
	loadimage(&Adventure_light, ".\\Screen\\Adventure_light.png");
	loadimage(&Adventure_light_mask, ".\\Screen\\Adventure_light_mask.png");
	loadimage(&ZomHand, ".\\Screen\\Zom_Hand.png");
	loadimage(&ZomHand_mask, ".\\Screen\\Zom_Hand_mask.png");
	char l[4] ;
	char input;
	clock_t s = clock(), e;
	if (!MusicStatus) {
		mciSendString("close bkmusic", NULL, 0, NULL);
		mciSendString("open .\\audio\\Intro_Theme2.mp3 alias bkmusic", NULL, 0, NULL);
		mciSendString("play bkmusic repeat", NULL, 0, NULL);
	}
	else {
		mciSendString("close bkmusic", NULL, 0, NULL);
		mciSendString("open .\\audio\\Intro_Theme1.mp3 alias bkmusic", NULL, 0, NULL);
		mciSendString("setaudio bkmusic volume to 500", NULL, 0, NULL);
		mciSendString("play bkmusic repeat", NULL, 0, NULL);
	}
	for (int i = 0; i < 25;) {
		e = clock();
		if((double)(e - s)/CLOCKS_PER_SEC >= 0.01){
			s = clock();
			putimage(0, 0, &mainmenu);
			putimage(70, 400 - 13.6 * i, &grave_mask,SRCAND);
			putimage(70, 400 - 13.6 * i, &grave,SRCINVERT);
			putimage(408, 400 - 13.6 * i + 26, &Adventure_mask, SRCAND);
			putimage(408, 400 - 13.6 * i + 26, &Adventure, SRCINVERT);
			i++;
			FlushBatchDraw();
		}
	}
	settextstyle(20, 0, "∫⁄ÃÂ");
	settextcolor(RGB(136,136,140));
	setbkmode(TRANSPARENT);
	while (1) {
		putimage(0, 0, &mainmenu);
		putimage(70, 40, &grave_mask, SRCAND);
		putimage(70, 40, &grave, SRCINVERT);
		putimage(408, 66, &Adventure_mask, SRCAND);
		putimage(408, 66, &Adventure, SRCINVERT);
		itoa(level.checkpoint, l, 10);
		MouseOperation();
		static int cnt = 0;
		if (_kbhit()) {
			input = _getch();
			if (input >= 49 && input <= 56) level.checkpoint = input - '0';
		}
		if (mouseX > 408 && mouseX < 740 && mouseY > 66 && mouseY < 206) {
			if (!cnt) {
				mciSendString("close b", NULL, 0, NULL);
				mciSendString("open .\\audio\\bleep.mp3 alias b", NULL, 0, NULL);
				mciSendString("play b", NULL, 0, NULL);
				cnt++;
			}
			putimage(408, 66, &Adventure_light_mask, SRCAND);
			putimage(408, 66, &Adventure_light, SRCINVERT);
			if (isMouseDown) {
				mciSendString("close bkmusic", NULL, 0, NULL);
				mciSendString("close l", NULL, 0, NULL);
				mciSendString("open .\\audio\\losemusic.mp3 alias l", NULL, 0, NULL);
				mciSendString("play l", NULL, 0, NULL);
				mciSendString("close e", NULL, 0, NULL);
				mciSendString("open .\\audio\\evillaugh.mp3 alias e", NULL, 0, NULL);
				mciSendString("play e", NULL, 0, NULL);
				clock_t start = 0, end = 10000, st = 0, en = 10000;
				for (int i = 0; ;) {
					if ((double)(end - start) / CLOCKS_PER_SEC >= 0.04 && i < 15) {
						start = clock();
						putimage(0, 0, &mainmenu);
						putimage(70, 40, &grave_mask, SRCAND);
						putimage(70, 40, &grave, SRCINVERT);
						putimage(408, 66, &Adventure_mask, SRCAND);
						putimage(408, 66, &Adventure, SRCINVERT);
						putimage(202, 278, 192, 312, &ZomHand_mask, i * 192, 0, SRCAND);
						putimage(202, 278, 192, 312, &ZomHand, i * 192, 0, SRCINVERT);
						i++;
					}
					else if (i >= 15) {
						putimage(0, 0, &mainmenu);
						putimage(70, 40, &grave_mask, SRCAND);
						putimage(70, 40, &grave, SRCINVERT);
						putimage(408, 66, &Adventure_mask, SRCAND);
						putimage(408, 66, &Adventure, SRCINVERT);
						putimage(202, 278, 192, 312, &ZomHand_mask, 2668, 0, SRCAND);
						putimage(202, 278, 192, 312, &ZomHand, 2668, 0, SRCINVERT);
					}
					if ((double)(en - st) / CLOCKS_PER_SEC >= 0.08 && (double)(en - st) / CLOCKS_PER_SEC < 0.16) {
						putimage(408, 66, &Adventure_light_mask, SRCAND);
						putimage(408, 66, &Adventure_light, SRCINVERT);

					}
					else if ((double)(en - st) / CLOCKS_PER_SEC >= 0.16) {
						st = clock();
						putimage(408, 66, &Adventure_mask, SRCAND);
						putimage(408, 66, &Adventure, SRCINVERT);

					}
					end = clock();
					en = clock();
					outtextxy(535, 88, "µ⁄");
					outtextxy(560, 89, l);
					outtextxy(575, 90, "πÿ");
					FlushBatchDraw();
					if ((double)(end - start) / CLOCKS_PER_SEC >= 4) {
						GameStatus = 1;
						break;
					}
				}

				break;
			}
		}
		else cnt = 0;
		outtextxy(535, 88, "µ⁄");
		outtextxy(560, 89, l);
		outtextxy(575, 90, "πÿ");
		FlushBatchDraw();
	}
}
void SunFall(Skill *sun,int *SunNum,int top,int bottom,float a)
{
	if (sun->Y < 1) {
		sun->X = 254 + rand() % 338;
		sun->Y = 88;
		sun->start1 = clock();
		sun->start3 = clock();
		sun->intervalx = (sun->X - 15) / 300;
		sun->intervaly = (sun->Y - 16) / 300;
		
	}
	if (!sun->isClick) {
		sun->isclick = 0;
		sun->intervalx = (sun->X - 15) / 300;
		sun->intervaly = (sun->Y - 16) / 300;
		if (sun->Y < top + 1) sun->start1 = clock();
		if ((double)(sun->end - sun->start3) / CLOCKS_PER_SEC >= a) {
			if (sun->Y < bottom) sun->Y += sun->v;
			sun->start3 = clock();
		}

		if ((double)(sun->end - sun->start1) / CLOCKS_PER_SEC >= 10) {
			sun->start2 = clock();
			sun->X = 0;
			sun->Y = 0;
		}
	}
	else {	
		if (!sun->isclick) {
			mciSendString("close point", NULL, 0, NULL);
			mciSendString("open .\\audio\\points.mp3 alias point", NULL, 0, NULL);
			mciSendString("play point", NULL, 0, NULL);
			sun->isclick = 1;
		}
		if ((double)(sun->end - sun->start1) / CLOCKS_PER_SEC >= 0.001) {
			sun->start1 = clock();
			sun->X -= sun->intervalx;
			sun->Y -= sun->intervaly;
		}
		if (sun->Y < 15) {
			sun->X = 0;
			sun->Y = 0;
			*SunNum += 25;
			sun->isClick = 0;
			sun->start2 = clock();
		}
	}
	if ((double)(sun->end - sun->start) / CLOCKS_PER_SEC >= sun->timeleap) {
		sun->start = clock();
		sun->i++;
		if (sun->i == sun->maxPic) sun->i = 0;
	}
	putimage(sun->X, sun->Y, 79, 79, &_Sun_[1], sun->i * 79, 0, SRCAND);
	putimage(sun->X, sun->Y, 79, 79, &_Sun_[0], sun->i * 79, 0, SRCINVERT);
}
char CreateRandomNum() {
	char nRandomNum = 0;
	while (1) {
		nRandomNum = rand() % 123;
		if ((nRandomNum >= 65 && nRandomNum <= 90) || nRandomNum >= 97) {
			return nRandomNum;
		}
	}
}
void LogIn()
{
	srand((unsigned)time(NULL));
	int LogStatus = rand() % 3 + 1;
	loadimage(&mine1, ".\\Screen\\MinecraftLogo.bmp");
	if (LogStatus == 1) 
		CharRain();
	else if (LogStatus == 2)
		CharFall();
	else if (LogStatus == 3)
		InteractParticle();
}

int BeginPage1Judge(int x, int y)
{
	if (x > r[0][0] && x<r[0][2] && y>r[0][1] && y < r[0][3])return 1;
	if (x > r[1][0] && x<r[1][2] && y>r[1][1] && y < r[1][3])return 2;
	if (x > r[2][0] && x<r[2][2] && y>r[2][1] && y < r[2][3])return 3;
	return 0;
}

int BeginPage2Judge(int x, int y)
{
	if (x > r1[0][0] && x<r1[0][2] && y>r1[0][1] && y < r1[0][3])return 1;
	if (x > r1[1][0] && x<r1[1][2] && y>r1[1][1] && y < r1[1][3])return 2;
	if (x > r1[2][0] && x<r1[2][2] && y>r1[2][1] && y < r1[2][3])return 3;
	return 0;

}

void search(int i[2], char text, char a[5][5])
{
	int k, l, isFound = 0;
	for (k = 0; k < 5; k++) {
		for (l = 0; l < 5; l++) {
			if (a[k][l] == text) {
				i[0] = k;
				i[1] = l;
				isFound = 1;
				break;
			}
		}
		if (isFound)
			break;
	}
}

void Encrypt(char b[6], char text[30], char Etext[30])
{
	char a[5][5], c[5][5];
	int i1[2] = { 0 }, i2[2] = { 0 };
	int i, j, k, l;
	char b_[6];
	strncpy(a[0], b, 5);
	char ch[3] = { "A\0" };
	char ch_[3] = { "a\0" };
	for (i = 0; i < 6; i++)
		b_[i] = (char)(b[i] + 32);
	strncpy(c[0], b_, 5);
	for (i = 1; i < 5; i++) {
		for (j = 0; j < 5; ch[0]++) {
			if (!(strstr(b, ch))) {
				a[i][j] = ch[0];
				j++;
			}
		}
		for (k = 0; k < 5; ch_[0]++) {
			if (!(strstr(b_, ch_))) {
				c[i][k] = ch_[0];
				k++;
			}
		}
	}
	for (j = 0; j < strlen(text) - 1; j += 2) {
		if (text[j] < 91 && text[j] >64) {
			if (text[j] != b[5] && text[j + 1] != b[5]) {
				search(i1, text[j], a);
				search(i2, text[j + 1], a);
				Etext[j] = a[i1[0]][i2[1]];
				Etext[j + 1] = a[i2[0]][i1[1]];
			}
			else {
				Etext[j] = text[j + 1];
				Etext[j + 1] = text[j];
			}
		}
		else if (text[j] < 123 && text[j] > 96) {
			if (text[j] != b_[5] && text[j + 1] != b_[5]) {
				search(i1, text[j], c);
				search(i2, text[j + 1], c);
				Etext[j] = c[i1[0]][i2[1]];
				Etext[j + 1] = c[i2[0]][i1[1]];
			}
			else {
				Etext[j] = text[j + 1];
				Etext[j + 1] = text[j];
			}
		}
		else {
			Etext[j] = text[j];
			Etext[j + 1] = text[j + 1];
		}
	}
}
void BeginPage1()
{
	static int event = 0;
	static TCHAR s[3][10] = { "µ«¬º","‘≠∞Ê“Ù¿÷","ÕÀ≥ˆ”Œœ∑" };
	RECT R[3];
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 35;
	f.lfWidth = 13;
	_tcscpy(f.lfFaceName, _T("Œ¢»Ì—≈∫⁄"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setfillstyle(0);
	event = BeginPage1Judge(mouseX, mouseY);
	for (int i = 0; i < 3; i++) {
		if (event - 1 == i) {
			settextcolor(WHITE);
			setlinecolor(WHITE);
			setfillcolor(BLACK);
			R[i] = { r[i][0] - 30, r[i][1] - 4, r[i][2] + 30, r[i][3] + 4 };
			fillrectangle(r[i][0] - 30, r[i][1] - 4, r[i][2] + 30, r[i][3] + 4);
			drawtext(s[i], &R[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
		else {
			settextcolor(BLACK);
			setlinecolor(BLACK);
			setfillcolor(WHITE);
			R[i] = { r[i][0],r[i][1],r[i][2],r[i][3] };
			clearrectangle(r[i][0] - 30, r[i][1] - 4, r[i][2] + 30, r[i][3] + 4);
			fillrectangle(r[i][0], r[i][1], r[i][2], r[i][3]);
			drawtext(s[i], &R[i], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}
	if (event && isMouseDown) {
		for (int i = 0; i < 3; i++)
			clearrectangle(r[i][0] - 30, r[i][1] - 4, r[i][2] + 30, r[i][3] + 4);
		if (event == 1) LogInStatus = 1;
		if (event == 2) {
			if(strcmp(s[1],"‘≠∞Ê“Ù¿÷") == 0){
				strcpy(s[1], "Õ¨»À“Ù¿÷");
				MusicStatus = 1;
			}
			else {
				strcpy(s[1], "‘≠∞Ê“Ù¿÷");
				MusicStatus = 0;
			}
		}
		if (event == 3) LogInStatus = 3;
	}
}
void BeginPage2(int status)
{
	static int event = 0, prevent;
	static int isEnter = 0;
	static char EncryptCode[30] = { '\0' };
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	settextstyle(30, 0, _T("Minecraft"));
	outtextxy(r1[0][0] - 100, r1[0][1], "Name");
	outtextxy(r1[1][0] - 100, r1[1][1], "Key");
	outtextxy(r1[2][0] - 100, r1[2][1], "Code");
	setfillstyle(0);
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 2);
	settextcolor(WHITE);
	settextstyle(25, 20, _T("Minecraft"));
	MouseOperation();
	prevent = BeginPage2Judge(mouseX, mouseY);
	if (!prevent) isEnter = 0;
	if (!isEnter) event = prevent;
	for (int i = 0; i < 3; i++) {
		if (event - 1 == i) {
			setfillcolor(DARKGRAY);
			fillrectangle(r1[i][0], r1[i][1], r1[i][2], r1[i][3]);
		}
		else {
			setfillcolor(BLACK);
			fillrectangle(r1[i][0], r1[i][1], r1[i][2], r1[i][3]);
		}
	}
	static int n_size = 0, k_size = 0, c_size = 0;
	if (_kbhit()) {
		char c = _getch();
		if (c == 27) {
			cleardevice;
			LogInStatus = 0;
		}
		if (event == 1) {
			if (c != ' ') {
				if (c == '\b' && n_size > 0 && c != '\r') {
					n_size--;
					name[n_size] = '\0';
				}
				else if (n_size < 10 && c != '\b' && c != '\r' && ((c>=48 && c<=57) || (c>=65 && c<=90) || (c>=97 && c<=122))) {
					name[n_size] = c;
					n_size++;

				}
				else if (c == '\r') {
					isEnter = 1;
					event++;
				}
			}
		}
		else if (event == 2) {
			if (c != ' ') {
				if (c == '\b' && k_size > 0 && c != '\r') {
					k_size--;
					keyWord[k_size] = '\0';
				}
				else if (k_size < 6 && c != '\b' && c != '\r' && ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))) {
					keyWord[k_size] = c;
					k_size++;
				}
				else if (c == '\r') {
					isEnter = 1;
					event++;
				}
			}
		}
		else if (event == 3) {
			if (c != ' ') {
				if (c == '\b' && c_size > 0) {
					c_size--;
					code[c_size] = '\0';
				}
				else if (c_size < 18 && c != '\b' && c != '\r' && ((c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122))) {
					code[c_size] = c;
					c_size++;
				}
				else if (c == '\r') {
					isEnter = 1;
					event++;
				}
			}
			if (strlen(keyWord) == 6)Encrypt(keyWord, code, EncryptCode);
			else strcpy(EncryptCode, code);
		}
	}
	static float fps = 0;
	if (!status) fps++;
	else if (status) fps += 0.15;
	int n[3];
	for (int i = 0; i < 3; i++) {
		for (n[0] = 0; n[0] < n_size; n[0] ++)
			outtextxy(r1[0][0] + 2 + n[0] * 20, r1[0][1] + 15, name[n[0]]);
		for (n[1] = 0; n[1] < k_size; n[1] ++)
			outtextxy(r1[0][0] + 2 + n[1] * 20, r1[1][1] + 15, "Öd");
		for (n[2] = 0; n[2] < c_size; n[2] ++)
			outtextxy(r1[0][0] + 2 + n[2] * 20, r1[2][1] + 15, EncryptCode[n[2]]);
	}
	for (int i = 0; i < 3; i++) {
		if (fps > 4 && fps < 10) {
			if (event - 1 == i) {
				line(r1[0][0] + n[i] * 20, r1[i][1] + 5, r1[0][0] + n[i] * 20, r1[i][1] + 45);
				break;
			}
		}
		else if (fps >= 10)
			fps = 0;
	}
	if (event == 4) {
		if (!strcmp(name, "Dango") && !strcmp(code, "Minecraft123")) {
			outtextxy(800, 600, "µ«¬º≥…π¶");
			GameStatus = 0;
		}
		else {
			outtextxy(800, 600, "µ«¬º ß∞‹");
		}
	}
}

void bkGradient()
{
	for (int i = 255; i > 0; i -= 5)
	{
		setbkcolor(RGB(i, i, i));
		cleardevice();
		Sleep(15);
	}
}

void CharRain() {
	mciSendString("close bkmusic", NULL, 0, NULL);
	mciSendString("open .\\audio\\Sweden.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	initgraph(Rain_Width, Rain_High);
	bkGradient();
	BeginBatchDraw();
	setfont(Rain_Size, 10, _T("Courier"));
	setcolor(GREEN);
	int i, j, k;
	clock_t start = 0, end;
	int alpha = 0;
	int Rain_HighNum = Rain_High / Rain_Size;
	int Rain_WidthNum = Rain_Width / Rain_Size;
	char Char[Rain_Width / Rain_Size + 1][Rain_High / Rain_Size + 1];
	int CharNum[Rain_Width / Rain_Size];
	int color[Rain_Width / Rain_Size];
	srand((unsigned)time(NULL));
	for (i = 0; i < Rain_WidthNum; i++) {
		CharNum[i] = rand() % (Rain_HighNum * 8 / 10 + 1) + Rain_HighNum / 10;
		color[i] = 255;
		for (j = 0; j < CharNum[i]; j++) {
			Char[i][j] = rand() % (26) + 65;
			outtextxy(i * Rain_Size, j * Rain_Size, Char[i][j]);
		}
	}
	while (1) {
		if (GameStatus != -1) break;
		end = clock();
		setbkmode(OPAQUE);
		setfont(Rain_Size, 10, _T("Courier"));
		setcolor(GREEN);
		delay(1);
		if ((float)(end - start) / CLOCKS_PER_SEC >= 0.08) {
			start = clock();
			for (i = 0; i < Rain_WidthNum; i++) {
				if (CharNum[i] <= Rain_HighNum) {
					CharNum[i]++;
					for (j = CharNum[i] - 1; j >= 0; j--)
						Char[i][j + 1] = Char[i][j];
					Char[i][0] = rand() % (26) + 65;
				}
				else if (color[i] > 40) {
					color[i] -= 20;
				}
				else {
					CharNum[i] = rand() % (Rain_HighNum * 8 / 10 + 1) + Rain_HighNum / 10;
					color[i] = rand() % (76) + 180;
					for (j = 0; j < CharNum[i]; j++)
						Char[i][j] = rand() % (26) + 65;
				}
			}
			alpha += 30;
		}
		MouseOperation();
		for (i = 0; i < Rain_WidthNum; i++) {
			setcolor(RGB(0, color[i], 0));
			for (j = 0; j < CharNum[i]; j++)
				outtextxy(i * Rain_Size, j * Rain_Size, Char[i][j]);
		}
		putpicture(150, 100, &mine1, RGB(0, 0, 0), alpha);
		if (LogInStatus == 0) BeginPage1();
		if (LogInStatus == 1) BeginPage2(1);
		if (LogInStatus == 3) exit(0);
		FlushBatchDraw();
	}
	EndBatchDraw();
}

void CharFall()
{
	mciSendString("close bkmusic", NULL, 0, NULL);
	mciSendString("open .\\audio\\Minecraft.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("setaudio bkmusic volume to 1000", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	int g_nRainPos[DEF_RAIN_NUM] = { 0 };
	char g_strRain[DEF_RAIN_NUM][10] = { 0 };
	srand((unsigned)time(NULL));
	initgraph(Rain_Width, Rain_High);
	bkGradient();
	for (int i = 0; i < DEF_RAIN_NUM; i++) {
		g_nRainPos[i] = rand() % Rain_High;
	}
	for (int i = 0; i < DEF_RAIN_NUM; i++) {
		for (int j = 0; j < 10; j++) {
			g_strRain[i][j] = CreateRandomNum();
		}
	}
	clock_t start = 0, end;
	int alpha = 0;
	BeginBatchDraw();
	while (1) {
		if (GameStatus != -1) break;
		end = clock();
		delay(1);
		setbkmode(OPAQUE);
		settextstyle(20, 0, _T("ø¨ÃÂ"));
		if ((float)(end - start) / CLOCKS_PER_SEC >= 0.08){
			start = clock();
			for (int i = 0; i < DEF_RAIN_NUM; i++) {
				for (int j = 0; j < 10; j++) {
					g_strRain[i][j] = CreateRandomNum();
				}
			}
			for (int i = 0; i < DEF_RAIN_NUM; i++) {
				g_nRainPos[i] += 15;
			}
			for (int i = 0; i < DEF_RAIN_NUM; i++) {
				if (g_nRainPos[i] - 10 * 15 >= Rain_High) {
					g_nRainPos[i] = 0;
				}
			}
			alpha += 30;
		}
		MouseOperation();
		for (int i = 0; i < DEF_RAIN_NUM; i++) {
			settextcolor(RGB(255, 255, 255));
			for (int j = 0; j < 10; j++) {
				outtextxy(i * 15, g_nRainPos[i] - 15 * j, g_strRain[i][j]);
				settextcolor(RGB(0, 255 - 28 * j, 0));
			}
		}
		putpicture(150, 100, &mine1, RGB(0, 0, 0), alpha);
		if (LogInStatus == 0) BeginPage1();
		if (LogInStatus == 1) BeginPage2(1);
		if (LogInStatus == 3) exit(0);
		FlushBatchDraw();
		cleardevice();
	}
	EndBatchDraw();
	closegraph();
}
void InteractParticle()
{
	mciSendString("close bkmusic", NULL, 0, NULL);
	mciSendString("open .\\audio\\Wet_Hands.mp3 alias bkmusic", NULL, 0, NULL);
	mciSendString("play bkmusic repeat", NULL, 0, NULL);
	static Particle par[Particle_Num];
	srand((unsigned int)time(NULL));
	int i, j;
	mouseX = preMouseX = Rain_Width / 2;
	mouseY = preMouseY = Rain_High / 2;
	isMouseDown = 0;
	for (i = 0; i < Particle_Num; i++) {
		par[i].par_x = rand() % Rain_Width;
		par[i].par_y = rand() % Rain_High;
		par[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		par[i].v_x = float(cos(float(i))) * (rand() % 34);
		par[i].v_y = float(sin(float(i))) * (rand() % 34);
		par[i].r = (rand() % 34) / 15.0;
	}
	initgraph(Rain_Width, Rain_High);
	bkGradient();
	BeginBatchDraw();
	while (1) {
		if (GameStatus != -1) break;
		clearrectangle(0, 0, Rain_Width - 1, Rain_High - 1);
		for (i = 0; i < Particle_Num; i++) {
			setfillstyle(par[i].color);
			setcolor(par[i].color);
			fillcircle(int(par[i].par_x + 0.5), int(par[i].par_y + 0.5), int(par[i].r + 0.5));
		}
		delay(5);
		MouseOperation();
		static int alpha = 0;
		putpicture(150, 100, &mine1, RGB(0, 0, 0), alpha);
		alpha += 3;
		if (LogInStatus == 0) BeginPage1();
		if (LogInStatus == 1) BeginPage2(1);
		if (LogInStatus == 3) exit(0);
		FlushBatchDraw();
		float aDis = Rain_Width * 0.86;
		float blowDis = Rain_High * 0.5;
		float disturbDis = Rain_Width * 0.125;
		int mouseVX = mouseX - preMouseX;
		int mouseVY = mouseY - preMouseY;
		preMouseX = mouseX;
		preMouseY = mouseY;
		for (int i = 0; i < Particle_Num; i++) {
			float dx = par[i].par_x - mouseX;
			float dy = par[i].par_y - mouseY;
			float d = sqrt(dx * dx + dy * dy);
			/*dx /= fabs(dx);
			dy /= fabs(dy);*/
			dx /= d;
			dy /= d;
			if (d < aDis) {
				float aAcc = (1 - d / aDis) * 1.0;
				par[i].v_x -= dx * aAcc;
				par[i].v_y -= dy * aAcc;
			}
			if (isMouseDown && d < blowDis) {
				float blowAcc = (1 - d / blowDis) * 10.0;
				par[i].v_x = par[i].v_x + dx * blowAcc + 0.5f - ((float)rand()) / RAND_MAX;
				par[i].v_y = par[i].v_y + dy * blowAcc + 0.5f - ((float)rand()) / RAND_MAX;
			}
			if (d < disturbDis) {
				float mAcc = (1 - d / disturbDis) * 0.26;
				par[i].v_x += mouseVX * mAcc;
				par[i].v_y += mouseVY * mAcc;
			}
			float x = par[i].par_x + par[i].v_x;
			float y = par[i].par_y + par[i].v_y;
			par[i].par_x = x > Rain_Width ? (par[i].v_x = -par[i].v_x, Rain_Width) : x < 0 ? (par[i].v_x = -par[i].v_x, 0) : x;
			par[i].par_y = y > Rain_High ? (par[i].v_y = -par[i].v_y, Rain_High) : y < 0 ? (par[i].v_y = -par[i].v_y, 0) : y;
			par[i].v_x *= friction;
			par[i].v_y *= friction;
			float avgVX = fabs(par[i].v_x);
			float avgVY = fabs(par[i].v_y);
			float avgV = (avgVX + avgVY) * 0.5f;
			avgVX < 0.1 ? (par[i].v_x *= float(rand()) / RAND_MAX * 3) : NULL;
			avgVY < 0.1 ? (par[i].v_y *= float(rand()) / RAND_MAX * 3) : NULL;
			par[i].r = max(min(avgV * 0.45f, 3.5f), 0.4f);
		}
	}
	EndBatchDraw();
	closegraph();
}