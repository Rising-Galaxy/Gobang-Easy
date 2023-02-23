#include "GoBang.h"

#pragma region �����ֵ

#define FreeTwo 500    // ��� 0220
#define ZTTZTZ 2500    // 022020
#define ZTTZTO 600     // 022021
#define ZTTZTT 5000    // 022022
#define OTTZTT 5000    // 122022
#define DeadTwo 100    // ��2
#define ZTTTZT 5000    // 022202
#define FreeThree 3000 // ���� 02220
#define OTTTZT 5000    // 122202
#define OTTTZZ 1000    // 122200
#define FreeFour 30000 // ��4 022220
#define DeadFour 5000  // ��4
#define Win5 99999     // ��5(ʤ��)
#define CutTwo 50      // ��ϻ��
#define ZOOZOZ 600     // 011010
#define OZZOZT 100     // 011012
#define ZOOZOO 800     // 011011
#define CutFour 8000   // ��ϻ���
#define ZOOOZO 1000    // 011101 || 101110
#define CutThree 700   // ��ϻ��� 01110
#define TOOOZO 1000    // 211101 101112 - �������
#define TOOOZZ 300     // 211100 001112 - �������
#define CutWin 60000   // �������(��ֹ��һ�ʤ)

#pragma endregion

#pragma region ����ͼƬ����Ϣ����

IMAGE bgl, bgr, bg_temp, _picture, loadgui;
IMAGE victory_player_black_a, victory_ai_white_a;
IMAGE victory_player_black_b, victory_ai_white_b;
IMAGE tips_0_a, tips_1_a, tips_2_a;
IMAGE tips_0_b, tips_1_b, tips_2_b;
IMAGE same_a, same_b;
IMAGE menu_0_a, menu_1_a, menu_2_a;
IMAGE menu_0_b, menu_1_b, menu_2_b;
IMAGE piece_b, piece_w;

ExMessage mouse;
ExMessage _kb;
ExMessage _kb_1;
ExMessage _kb_2;

#pragma endregion

// ��¼��ť����
Button bt_start = {980, 1180, 300, 360};
Button bt_end = {980, 1180, 700, 750};
Button bt_easy = {980, 1180, 300, 355};
Button bt_hard = {980, 1180, 400, 450};
Button bt_menu = {980, 1134, 400, 460};

/// @brief ��������
//! 0 - �� 1 - �� 2 - ��
int chess[15][15];
int __bgm = 0;  // �ж�bgm�Ĳ�������ͣ
int piece_nums; // �ж������Ƿ�����
char retStr[80];
bool if_screenshot_folder = false;

int main()
{
    // ��ʼ������
    initgraph(1200, 1000);
    // ���Ĵ��ڱ���
    SetWindowText(GetHWnd(), _T("��������Ϸ"));
    // ����
    load_gui();
    cleardevice();
    // ����ͼƬ����ʾ��
    _bg_put();
    menu_put_1();
    _tips_put();
    // ����������˵���
    setlinecolor(BLACK);
    line(970, 0, 970, 1000);
    // ���ű�������
    mciSendStringA("open res/music/bgm.wav alias BGM", NULL, 0, NULL);
    mciSendStringA("play BGM", NULL, 0, NULL);
    _begin__();
    return 0;
}

/**
 * @brief �ж��Ƿ��ڰ�ť������
 *
 * @param x ��� - x����
 * @param y ��� - y����
 * @param mode 1 - �˵�1 | 2 - �˵�2 | 3 - �˵�3
 * @return int ����: 0 - �����κΰ�ť | 1 - ��ʼ | 2 - �ر� | 3 - �� | 4 - ���� | 5 - ���˵�
 */
int bt_if(const int x, const int y, const int mode)
{
    switch (mode)
    {
    // menu 1
    case 1:
    {
        if (x >= bt_start.left && x <= bt_start.right && y >= bt_start.up && y <= bt_start.down)
        {
            return 1;
        }
        else if (x >= bt_end.left && x <= bt_end.right && y >= bt_end.up && y <= bt_end.down)
        {
            return 2;
        }
    }
    break;
    // menu 2
    case 2:
    {
        if (x >= bt_easy.left && x <= bt_easy.right && y >= bt_easy.up && y <= bt_easy.down)
        {
            return 3;
        }
        else if (x >= bt_end.left && x <= bt_end.right && y >= bt_end.up && y <= bt_end.down)
        {
            return 2;
        }
        else if (x >= bt_hard.left && x <= bt_hard.right && y >= bt_hard.up && y <= bt_hard.down)
        {
            return 4;
        }
    }
    break;
    // menu 3
    case 3:
    {
        if (x >= bt_end.left && x <= bt_end.right && y >= bt_end.up && y <= bt_end.down)
        {
            return 2;
        }
        else if (x >= bt_menu.left && x <= bt_menu.right && y >= bt_menu.up && y <= bt_menu.down)
        {
            return 5;
        }
    }
    break;
    }
    return 0;
}

//? ͸��PNG
void drawPNG(IMAGE *picture, int picture_x, int picture_y)
{
    DWORD *dst = GetImageBuffer(); // GetImageBuffer()���������ڻ�ȡ��ͼ�豸���Դ�ָ�룬EASYX�Դ�
    DWORD *draw = GetImageBuffer();
    DWORD *src = GetImageBuffer(picture);      // ��ȡpicture���Դ�ָ��
    int picture_width = picture->getwidth();   // ��ȡpicture�Ŀ�ȣ�EASYX�Դ�
    int picture_height = picture->getheight(); // ��ȡpicture�ĸ߶ȣ�EASYX�Դ�
    int graphWidth = getwidth();               // ��ȡ��ͼ���Ŀ�ȣ�EASYX�Դ�
    int graphHeight = getheight();             // ��ȡ��ͼ���ĸ߶ȣ�EASYX�Դ�
    int dstX = 0;                              // ���Դ������صĽǱ�
    // ʵ��͸����ͼ ��ʽ�� Cp=��p*FP+(1-��p)*BP �� ��Ҷ˹���������е���ɫ�ĸ��ʼ���
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width;        // ���Դ������صĽǱ�
            int sa = ((src[srcX] & 0xff000000) >> 24); // 0xAArrggbb;AA��͸����
            int sr = ((src[srcX] & 0xff0000) >> 16);   // ��ȡRGB���R
            int sg = ((src[srcX] & 0xff00) >> 8);      // G
            int sb = src[srcX] & 0xff;                 // B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; // ���Դ������صĽǱ�
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  // ��ʽ�� Cp=��p*FP+(1-��p)*BP  �� ��p=sa/255 , FP=sr , BP=dr
                             | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8) // ��p=sa/255 , FP=sg , BP=dg
                             | (sb * sa / 255 + db * (255 - sa) / 255);       // ��p=sa/255 , FP=sb , BP=db
            }
        }
    }
}
//? �������ֵĲ�������ͣ
void _bgm_play()
{
    if (__bgm)
    {
        mciSendStringA("resume BGM", NULL, 0, NULL);
        __bgm = 0;
    }
    else
    {
        mciSendStringA("pause BGM", NULL, 0, NULL);
        __bgm = 1;
    }
}
//? �ҷ��ص�״̬��Ϣ���Ƿ����stop
int mycmp(char *a)
{
    int i = 0;
    while (a[i + 4] != '0')
    {
        if (a[i] == 's' && a[i + 1] == 't' && a[i + 2] == 'o' && a[i + 3] == 'p')
        {
            return 1;
        }
        i++;
    }
    return 0;
}
//? ��������ѭ��
void _bgm_repeat()
{
    // ��ȡBGM��״̬���ص��ַ���retStr
    mciSendString("Status BGM mode", retStr, 80, NULL);
    if (mycmp(retStr)) // �������bgmͣ��
    {
        mciSendString("Seek BGM to start", NULL, 0, NULL); // ��������λ��Ϊ��ͷ
        mciSendString("Play BGM", NULL, 0, NULL);          // ����BGM
    }
}

//? �������
void clear_chess()
{
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            chess[i][j] = 0;
        }
    }
}

//? �������ͼ
void _bg_put()
{
    BeginBatchDraw();
    putimage(0, 0, &bgl);
    putimage(1000, 0, &bgr);
    EndBatchDraw();
}
//? ����˵�1
void menu_put_1()
{
    BeginBatchDraw();
    putimage(980, 300, 200, 60, &tips_0_b, 0, 0, SRCAND);
    putimage(980, 300, 200, 60, &tips_0_a, 0, 0, SRCPAINT);
    putimage(980, 700, 200, 50, &tips_0_b, 0, 165, SRCAND);
    putimage(980, 700, 200, 50, &tips_0_a, 0, 165, SRCPAINT);
    EndBatchDraw();
}
//? ����˵�2
void menu_put_2()
{
    BeginBatchDraw();
    putimage(980, 300, 200, 55, &tips_0_b, 0, 60, SRCAND);
    putimage(980, 300, 200, 55, &tips_0_a, 0, 60, SRCPAINT);
    putimage(980, 400, 200, 50, &tips_0_b, 0, 115, SRCAND);
    putimage(980, 400, 200, 50, &tips_0_a, 0, 115, SRCPAINT);
    putimage(980, 700, 200, 50, &tips_0_b, 0, 165, SRCAND);
    putimage(980, 700, 200, 50, &tips_0_a, 0, 165, SRCPAINT);
    EndBatchDraw();
}
//? ����˵�3
void menu_put_3()
{
    BeginBatchDraw();
    putimage(980, 400, &menu_0_b, SRCAND);
    putimage(980, 400, &menu_0_a, SRCPAINT);
    putimage(980, 700, 200, 50, &tips_0_b, 0, 165, SRCAND);
    putimage(980, 700, 200, 50, &tips_0_a, 0, 165, SRCPAINT);
    EndBatchDraw();
}

//? �����ʾ�ı�
void _tips_put()
{
    LOGFONT tip;
    gettextstyle(&tip); // ��ȡ��ǰ��������
    tip.lfHeight = 30;  // ��������߶�Ϊ 30
    settextcolor(BLACK);
    _tcscpy_s(tip.lfFaceName, _T("����")); // ��������Ϊ�����塱
    tip.lfQuality = ANTIALIASED_QUALITY;   // �������Ч��Ϊ�����
    settextstyle(&tip);                    // ˢ����ʽ
    setbkmode(TRANSPARENT);
    BeginBatchDraw();
    outtextxy(1030, 800, _T("F - ����"));
    outtextxy(1030, 850, _T("M - BGM"));
    outtextxy(1030, 900, _T("S - ��ʼ"));
    outtextxy(1000, 950, _T("Esc - �˳�"));
    EndBatchDraw();
}
//? �����ʾ�ı�2
void _tips_put_2()
{
    LOGFONT tip;
    gettextstyle(&tip); // ��ȡ��ǰ��������
    tip.lfHeight = 30;  // ��������߶�Ϊ 48
    settextcolor(BLACK);
    _tcscpy_s(tip.lfFaceName, _T("����")); // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
    tip.lfQuality = ANTIALIASED_QUALITY;   // �������Ч��Ϊ�����
    settextstyle(&tip);                    // ����������ʽ
    setbkmode(TRANSPARENT);
    BeginBatchDraw();
    outtextxy(1030, 800, _T("M - BGM"));
    outtextxy(1030, 850, _T("E - ��"));
    outtextxy(1030, 900, _T("H - ����"));
    outtextxy(1000, 950, _T("Esc - �˳�"));
    EndBatchDraw();
}
//? �����ʾ�ı�3
void _tips_put_3()
{
    LOGFONT tip;
    gettextstyle(&tip); // ��ȡ��ǰ��������
    tip.lfHeight = 30;  // ��������߶�Ϊ 48
    settextcolor(BLACK);
    _tcscpy_s(tip.lfFaceName, _T("����")); // ��������Ϊ�����塱(�߰汾 VC �Ƽ�ʹ�� _tcscpy_s ����)
    tip.lfQuality = ANTIALIASED_QUALITY;   // �������Ч��Ϊ�����
    settextstyle(&tip);                    // ����������ʽ
    setbkmode(TRANSPARENT);
    // outtextxy(1030, 850, _T("F - ����"));
    BeginBatchDraw();
    outtextxy(1015, 800, _T("F2 - ��ͼ"));
    outtextxy(1030, 850, _T("M - BGM"));
    outtextxy(1030, 900, _T("R - ���˵�"));
    outtextxy(1000, 950, _T("Esc - �˳�"));
    EndBatchDraw();
}
//? �����ʾ�ı�4
void _tips_put_4()
{
    LOGFONT tip;
    gettextstyle(&tip); // ��ȡ��ǰ��������
    tip.lfHeight = 30;  // ��������߶�Ϊ 48
    settextcolor(BLACK);
    _tcscpy_s(tip.lfFaceName, _T("����")); // ��������Ϊ�����塱
    tip.lfQuality = ANTIALIASED_QUALITY;   // �������Ч��Ϊ�����
    settextstyle(&tip);                    // ����������ʽ
    setbkmode(TRANSPARENT);
    // outtextxy(1030, 850, _T("F - ����"));
    BeginBatchDraw();
    outtextxy(985, 800, _T("�ո� - ��һ��"));
    outtextxy(1030, 850, _T("M - BGM"));
    outtextxy(1030, 900, _T("R - ���˵�"));
    outtextxy(1000, 950, _T("Esc - �˳�"));
    EndBatchDraw();
}

//? ������
void draw_bg()
{
    int i, x = 40, y = 40;
    BeginBatchDraw();
    setlinecolor(BLACK);
    // ��
    for (i = 0; i < 15; i++)
    {
        line(40, y, 936, y);
        y += 64;
    }
    // ��
    for (i = 0; i < 15; i++)
    {
        line(x, 40, x, 936);
        x += 64;
    }
    // �߿���һ��
    line(38, 38, 938, 38);
    line(938, 38, 938, 938);
    line(38, 938, 938, 938);
    line(38, 38, 38, 938);
    // ���������С�ڵ�
    setfillcolor(BLACK);
    fillcircle(232, 232, 5); // ����
    fillcircle(232, 744, 5); // ����
    fillcircle(488, 488, 5); // ����
    fillcircle(744, 232, 5); // ����
    fillcircle(744, 744, 5); // ����
    EndBatchDraw();
}

//? ������ԭ
void restart_()
{
    cleardevice();
    _bg_put();
    menu_put_1();
    _tips_put();
    setlinecolor(BLACK);
    line(970, 0, 970, 1000);
    //! �������
    clear_chess();
}

//? �����ļ���
int folder(char *folder_name)
{
    int end;
    char *path = (char *)malloc(500 * sizeof(char));
    GetCurrentDirectory(500, path);
    strcat(path, "\\");
    strcat(path, folder_name);
    // ����ļ��в�����
    if (_access(path, 0) != 0)
        // �����ļ���
        end = mkdir(path); // ���� 0 ��ʾ�����ɹ�, -1 ��ʾʧ��
    else
        end = 1;
    return end;
}

//? �������
void free_list(review *head)
{
    review *p = head->next;
    review *q = NULL;
    while (p != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
}

//? ������
void draw_piece(int num, int m, int n)
{
    m -= 30;
    n -= 32;
    mciSendStringA("stop res/music/fall.wav", NULL, 0, NULL);
    // 2 - ����
    if (num == 2)
    {
        drawPNG(&piece_w, m, n);
    }
    // 1 - ����
    else
    {
        drawPNG(&piece_b, m, n);
    }
    mciSendStringA("play res/music/fall.wav", NULL, 0, NULL);
}

//? ʵ������
void change_chess(int num, const int m, const int n)
{
    chess[m][n] = num;
}

//! ����ģ��
void load_gui()
{
#pragma region ����ͼƬ
    loadimage(&loadgui, _T("PNG"), _T("P_loadgui"));
    loadimage(&bgl, _T("PNG"), _T("P_bgl"));
    loadimage(&bgr, _T("PNG"), _T("P_bgr"));
    loadimage(&victory_ai_white_a, _T("PNG"), _T("P_victory_ai_white_a"));
    loadimage(&victory_ai_white_b, _T("PNG"), _T("P_victory_ai_white_b"));
    loadimage(&victory_player_black_a, _T("PNG"), _T("P_victory_player_black_a"));
    loadimage(&victory_player_black_b, _T("PNG"), _T("P_victory_player_black_b"));
    loadimage(&tips_0_a, _T("PNG"), _T("P_tips_0_a"));
    loadimage(&tips_0_b, _T("PNG"), _T("P_tips_0_b"));
    loadimage(&tips_1_a, _T("PNG"), _T("P_tips_1_a"));
    loadimage(&tips_1_b, _T("PNG"), _T("P_tips_1_b"));
    loadimage(&tips_2_a, _T("PNG"), _T("P_tips_2_a"));
    loadimage(&tips_2_b, _T("PNG"), _T("P_tips_2_b"));
    loadimage(&menu_0_a, _T("PNG"), _T("P_menu_0_a"));
    loadimage(&menu_1_a, _T("PNG"), _T("P_menu_1_a"));
    loadimage(&menu_2_a, _T("PNG"), _T("P_menu_2_a"));
    loadimage(&menu_0_b, _T("PNG"), _T("P_menu_0_b"));
    loadimage(&menu_1_b, _T("PNG"), _T("P_menu_1_b"));
    loadimage(&menu_2_b, _T("PNG"), _T("P_menu_2_b"));
    loadimage(&same_a, _T("PNG"), _T("P_same_a"));
    loadimage(&same_b, _T("PNG"), _T("P_same_b"));
    loadimage(&piece_b, _T("PNG"), _T("P_piece_b"));
    loadimage(&piece_w, _T("PNG"), _T("P_piece_w"));
#pragma endregion
#pragma region ����ʱ����
    putimage(0, 0, &loadgui);
    LOGFONT tip;
    gettextstyle(&tip); // ��ȡ��ǰ��������
    tip.lfHeight = 30;  // ��������߶�Ϊ 30
    settextcolor(RGB(255, 255, 255));
    _tcscpy_s(tip.lfFaceName, _T("����")); // ��������Ϊ�����塱
    tip.lfQuality = ANTIALIASED_QUALITY;   // �������Ч��Ϊ�����
    settextstyle(&tip);                    // ����������ʽ
    setbkmode(TRANSPARENT);
    outtextxy(455, 770, _T("������"));
    setfillcolor(RGB(255, 255, 255));
    solidrectangle(200, 700, 800, 750);
    setfillcolor(RGB(0, 0, 0));
    BeginBatchDraw();
    for (int i = 200; i < 800; i += 2)
    {
        solidrectangle(200, 700, i, 750);
        FlushBatchDraw();
        Sleep(1);
    }
    EndBatchDraw();
    mciSendStringA("play res/music/loadover.wav", NULL, 0, NULL);
#pragma endregion
    // ������ͼ�ļ���
    char folder_name[500] = "screenshot";
    int folder_if;
    folder_if = folder(folder_name);
    if (folder_if == -1)
    {
        MessageBox(GetHWnd(), "��ͼ�ļ��д���ʧ��\n��ͼ�ļ������ڸ�Ŀ¼", "���󱨸�", MB_OK | MB_ICONERROR);
        if_screenshot_folder = false;
    }
    else
        if_screenshot_folder = true;
}

//! ��ͼģ��
void screenshot_()
{
    char times[30], shot[35];
    if (if_screenshot_folder)
        strcpy(shot, "screenshot/");
    getimage(&_picture, 0, 0, 970, 970);
    time_t now = time(NULL);
    struct tm now_ = *localtime(&now);
    strftime(times, 20, "%Y_%m_%d_%H_%M_%S", &now_);
    strcat(shot, times);
    strcat(shot, ".jpg");
    saveimage(shot, &_picture);
}

//! ����ģ��
void review_(review *head)
{
    int check_re = 1, if_;
    bool check_down = true;
    review *p = head->next;
    BeginBatchDraw();
    cleardevice();
    _bg_put();
    menu_put_3();
    _tips_put_4();
    setlinecolor(BLACK);
    line(970, 0, 970, 1000);
    EndBatchDraw();
    if (p == NULL)
    {
        LOGFONT Error_tip;
        gettextstyle(&Error_tip); // ��ȡ��ǰ��������
        Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
        settextcolor(RGB(100, 255, 85));
        _tcscpy_s(Error_tip.lfFaceName, _T("����"));
        Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
        settextstyle(&Error_tip);                  // ˢ����ʽ
        setbkmode(TRANSPARENT);
        outtextxy(300, 400, "��δ�洢���");
        check_re = 0;
    }
    else
    {
        // ��ӡ����
        draw_bg();
    re_piece:
        draw_piece(p->color, p->x, p->y);
        if (p->next == NULL)
        {
            LOGFONT Error_tip;
            gettextstyle(&Error_tip); // ��ȡ��ǰ��������
            Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
            settextcolor(RGB(0, 25, 155));
            _tcscpy_s(Error_tip.lfFaceName, _T("����"));
            Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
            settextstyle(&Error_tip);                  // ˢ����ʽ
            setbkmode(TRANSPARENT);
            outtextxy(980, 10, "���̽���");
            check_re = 0;
        }
    }
    while (true)
    {
        _bgm_repeat();
        mouse = getmessage(EX_KEY | EX_MOUSE);
        switch (mouse.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (mouse.x > 970)
                {
                    if_ = bt_if(mouse.x, mouse.y, 3);
                    switch (if_)
                    {
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���˵���ť
                    case 5:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, &menu_1_b, SRCAND);
                        putimage(980, 400, &menu_1_a, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_3();
                    }
                    break;
                    }
                    Sleep(10);
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(1000, 700, 180, 50, &bgr, 0, 700);
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                // ���˵���ť
                case 5:
                {
                    BeginBatchDraw();
                    putimage(1000, 400, 134, 60, &bgr, 0, 400);
                    putimage(980, 400, &menu_2_b, SRCAND);
                    putimage(980, 400, &menu_2_a, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = true;
            if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                // ���˵���ť
                case 5:
                {
                    restart_();
                    return;
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (mouse.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� R ���ص����˵�
            else if (mouse.vkcode == 0x52)
            {
                restart_();
                return;
            }
            // �� M ������ bgm �Ĳ�������ͣ
            else if (mouse.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� �ո� ��������һ��
            else if (mouse.vkcode == VK_SPACE)
            {
                // ���û�����һ��
                if (check_re)
                {
                    p = p->next;
                    goto re_piece;
                }
            }
        }
        break;
        }
    }
}

//! AI���
int AI_score(int x, int y)
{
    int m, n, score = 0, len, blank_m, blank_n, len_m, len_n, bb_m, bb_n;
#pragma region //! AS AI
#pragma region //? ����
    m = y + 1;
    n = y - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right
    while (m < 15 && chess[x][m] == 2)
    {
        len++;
        m++;
    }
    if (m < 15 && !chess[x][m]) // m�������Ӿ�ͷ���Ϊ��
    {
        blank_m = 1;
        m++;
        // �����ж�����
        while (m < 15 && chess[x][m] == 2)
        {
            len_m++;
            m++;
        }
        if (m < 15 && !chess[x][m]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left
    while (n >= 0 && chess[x][n] == 2)
    {
        len++;
        n--;
    }
    if (n >= 0 && !chess[x][n]) // n�������Ӿ�ͷ���Ϊ��
    {
        blank_n = 1;
        n--;
        // �����ж�����
        while (n >= 0 && chess[x][n] == 2)
        {
            len_n++;
            n--;
        }
        if (n >= 0 && !chess[x][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += FreeTwo; // ���
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZTTZTZ; // 022020
                    }
                    else
                    {
                        score += ZTTZTO; // 022021
                    }
                }
                else if (len_m > 1)
                {
                    score += ZTTZTT; // 022022
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZTTZTZ; // 020220
                    }
                    else
                    {
                        score += ZTTZTO; // 120220
                    }
                }
                else if (len_n > 1)
                {
                    score += ZTTZTT; // 220220
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += OTTZTT; // 122022
            }
            else if (bb_m)
            {
                score += DeadTwo; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += OTTZTT; // 220221
            }
            else if (bb_n)
            {
                score += DeadTwo; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m)
            {
                score += ZTTTZT; // 022202
            }
            if (len_n)
            {
                score += ZTTTZT; // 202220
            }
            //? ���ǰ���������㣬�൱��һ������
            else
            {
                score += FreeThree; // ���� 02220
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += OTTTZT; // 122202
            }
            else if (bb_m)
            {
                score += OTTTZZ; // 122200
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += OTTTZT; // 202221
            }
            else if (bb_n)
            {
                score += OTTTZZ; // 002221
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m && blank_n)
            score += FreeFour; // ��4 022220
        else if (blank_m || blank_n)
        {
            score += DeadFour; // ��4
        }
    }
    break;
    }
    if (len > 4)
    {
        score += Win5; // ��5(ʤ��)
    }
#pragma endregion
#pragma region //? ����
    m = x + 1;
    n = x - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // up
    while (m < 15 && chess[m][y] == 2)
    {
        len++;
        m++;
    }
    if (m < 15 && !chess[m][y]) // m�������Ӿ�ͷ���Ϊ��
    {
        blank_m = 1;
        m++;
        // �����ж�����
        while (m < 15 && chess[m][y] == 2)
        {
            len_m++;
            m++;
        }
        if (m < 15 && !chess[m][y]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // down
    while (n >= 0 && chess[n][y] == 2)
    {
        len++;
        n--;
    }
    if (n >= 0 && !chess[n][y])
    {
        blank_n = 1;
        n--;
        // �����ж�����
        while (n >= 0 && chess[n][y] == 2)
        {
            len_n++;
            n--;
        }
        if (n >= 0 && !chess[n][y]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += FreeTwo; // ���
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZTTZTZ; // 022020
                    }
                    else
                    {
                        score += ZTTZTO; // 022021
                    }
                }
                else if (len_m > 1)
                {
                    score += ZTTZTT; // 022022
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZTTZTZ; // 020220
                    }
                    else
                    {
                        score += ZTTZTO; // 120220
                    }
                }
                else if (len_n > 1)
                {
                    score += ZTTZTT; // 220220
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += OTTZTT; // 122022
            }
            else if (bb_m)
            {
                score += DeadTwo; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += OTTZTT; // 220221
            }
            else if (bb_n)
            {
                score += DeadTwo; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m)
            {
                score += ZTTTZT; // 022202
            }
            if (len_n)
            {
                score += ZTTTZT; // 202220
            }
            //? ���ǰ���������㣬�൱��һ������
            else
            {
                score += FreeThree; // ���� 02220
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += OTTTZT; // 122202
            }
            else if (bb_m)
            {
                score += OTTTZZ; // 122200
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += OTTTZT; // 202221
            }
            else if (bb_n)
            {
                score += OTTTZZ; // 002221
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m && blank_n)
            score += FreeFour; // ��4 022220
        else if (blank_m || blank_n)
        {
            score += DeadFour; // ��4
        }
    }
    break;
    }
    if (len > 4)
    {
        score += Win5; // ��5(ʤ��)
    }
#pragma endregion
#pragma region //? ��б��
    m = x + 1;
    n = y + 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right and down
    while (m < 15 && n < 15 && chess[m][n] == 2)
    {
        len++;
        m++;
        n++;
    }
    if (m < 15 && n < 15 && !chess[m][n])
    {
        blank_m = 1;
        m++;
        n++;
        // �����ж�����
        while (m < 15 && n < 15 && chess[m][n] == 2)
        {
            len_m++;
            m++;
            n++;
        }
        if (m < 15 && n < 15 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left and up
    m = x - 1;
    n = y - 1;
    while (m >= 0 && n >= 0 && chess[m][n] == 2)
    {
        len++;
        m--;
        n--;
    }
    if (m >= 0 && n >= 0 && !chess[m][n])
    {
        blank_n = 1;
        m--;
        n--;
        // �����ж�����
        while (m >= 0 && n >= 0 && chess[m][n] == 2)
        {
            len_n++;
            m--;
            n--;
        }
        if (m >= 0 && n >= 0 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += FreeTwo; // ���
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZTTZTZ; // 022020
                    }
                    else
                    {
                        score += ZTTZTO; // 022021
                    }
                }
                else if (len_m > 1)
                {
                    score += ZTTZTT; // 022022
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZTTZTZ; // 020220
                    }
                    else
                    {
                        score += ZTTZTO; // 120220
                    }
                }
                else if (len_n > 1)
                {
                    score += ZTTZTT; // 220220
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += OTTZTT; // 122022
            }
            else if (bb_m)
            {
                score += DeadTwo; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += OTTZTT; // 220221
            }
            else if (bb_n)
            {
                score += DeadTwo; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m)
            {
                score += ZTTTZT; // 022202
            }
            if (len_n)
            {
                score += ZTTTZT; // 202220
            }
            //? ���ǰ���������㣬�൱��һ������
            else
            {
                score += FreeThree; // ���� 02220
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += OTTTZT; // 122202
            }
            else if (bb_m)
            {
                score += OTTTZZ; // 122200
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += OTTTZT; // 202221
            }
            else if (bb_n)
            {
                score += OTTTZZ; // 002221
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m && blank_n)
            score += FreeFour; // ��4 022220
        else if (blank_m || blank_n)
        {
            score += DeadFour; // ��4
        }
    }
    break;
    }
    if (len > 4)
    {
        score += Win5; // ��5(ʤ��)
    }
#pragma endregion
#pragma region //? ��б��
    m = x + 1;
    n = y - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right and up
    while (m < 15 && n >= 0 && chess[m][n] == 2)
    {
        len++;
        m++;
        n--;
    }
    if (m < 15 && n >= 0 && !chess[m][n])
    {
        blank_m = 1;
        m++;
        n--;
        // �����ж�����
        while (m < 15 && n >= 0 && chess[m][n] == 2)
        {
            len_m++;
            m++;
            n--;
        }
        if (m < 15 && n >= 0 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left and down
    m = x - 1;
    n = y + 1;
    while (m >= 0 && n < 15 && chess[m][n] == 2)
    {
        len++;
        m--;
        n++;
    }
    if (m >= 0 && n < 15 && !chess[m][n])
    {
        blank_n = 1;
        m--;
        n++;
        // �����ж�����
        while (m >= 0 && n < 15 && chess[m][n] == 2)
        {
            len_n++;
            m--;
            n++;
        }
        if (m >= 0 && n < 15 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += FreeTwo; // ���
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZTTZTZ; // 022020
                    }
                    else
                    {
                        score += ZTTZTO; // 022021
                    }
                }
                else if (len_m > 1)
                {
                    score += ZTTZTT; // 022022
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZTTZTZ; // 020220
                    }
                    else
                    {
                        score += ZTTZTO; // 120220
                    }
                }
                else if (len_n > 1)
                {
                    score += ZTTZTT; // 220220
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += OTTZTT; // 122022
            }
            else if (bb_m)
            {
                score += DeadTwo; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += OTTZTT; // 220221
            }
            else if (bb_n)
            {
                score += DeadTwo; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m)
            {
                score += ZTTTZT; // 022202
            }
            if (len_n)
            {
                score += ZTTTZT; // 202220
            }
            //? ���ǰ���������㣬�൱��һ������
            else
            {
                score += FreeThree; // ���� 02220
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += OTTTZT; // 122202
            }
            else if (bb_m)
            {
                score += OTTTZZ; // 122200
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += OTTTZT; // 202221
            }
            else if (bb_n)
            {
                score += OTTTZZ; // 002221
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m && blank_n)
            score += FreeFour; // ��4 022220
        else if (blank_m || blank_n)
        {
            score += DeadFour; // ��4
        }
    }
    break;
    }
    if (len > 4)
    {
        score += Win5; // ��5(ʤ��)
    }
#pragma endregion
#pragma endregion
#pragma region //! AS Player
#pragma region //? ����
    m = y + 1;
    n = y - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right
    while (m < 15 && chess[x][m] == 1)
    {
        len++;
        m++;
    }
    if (m < 15 && !chess[x][m]) // m�������Ӿ�ͷ���Ϊ��
    {
        blank_m = 1;
        m++;
        // �����ж�����
        while (m < 15 && chess[x][m] == 1)
        {
            len_m++;
            m++;
        }
        if (m < 15 && !chess[x][m]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left
    while (n >= 0 && chess[x][n] == 1)
    {
        len++;
        n--;
    }
    if (n >= 0 && !chess[x][n])
    {
        blank_n = 1;
        n--;
        // �����ж�����
        while (n >= 0 && chess[x][n] == 1)
        {
            len_n++;
            n--;
        }
        if (n >= 0 && !chess[x][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += CutTwo; // ��ϻ��
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZOOZOZ; // 011010
                    }
                    else
                    {
                        score += OZZOZT; // 011012
                    }
                }
                else if (len_m > 1)
                {
                    score += ZOOZOO; // 011011
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZOOZOZ; // 010110
                    }
                    else
                    {
                        score += OZZOZT; // 210110
                    }
                }
                else if (len_n > 1)
                {
                    score += ZOOZOO; // 110110
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += 800; // 211011
            }
            else if (bb_m)
            {
                score += 5; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += 800; // 110112
            }
            else if (bb_n)
            {
                score += 5; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m && len_n)
            {
                score += CutFour; // �൱�ڴ�ϻ���
            }
            if (len_m || len_n)
            {
                score += ZOOOZO; // 011101 || 101110
            }
            else
            {
                score += CutThree; // ��ϻ��� 01110
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += TOOOZO; // 211101 - �������
            }
            else if (bb_m)
            {
                score += TOOOZZ; // 211100 - �������
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += TOOOZO; // 101112 - �������
            }
            else if (bb_n)
            {
                score += TOOOZZ; // 001112 - �������
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m == 1 && blank_n == 1)
            score += CutFour; // ��ϻ�4
        else if (blank_m || blank_n)
            score += TOOOZO; // �����4
    }
    break;
    }
    if (len > 4)
    {
        score += CutWin; // �������(��ֹ��һ�ʤ)
    }
#pragma endregion
#pragma region //? ����
    m = x + 1;
    n = x - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // up
    while (m < 15 && chess[m][y] == 1)
    {
        len++;
        m++;
    }
    if (m < 15 && !chess[m][y]) // m�������Ӿ�ͷ���Ϊ��
    {
        blank_m = 1;
        m++;
        // �����ж�����
        while (m < 15 && chess[m][y] == 1)
        {
            len_m++;
            m++;
        }
        if (m < 15 && !chess[m][y]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // down
    while (n >= 0 && chess[n][y] == 1)
    {
        len++;
        n--;
    }
    if (n >= 0 && !chess[n][y])
    {
        blank_n = 1;
        n--;
        // �����ж�����
        while (n >= 0 && chess[n][y] == 1)
        {
            len_n++;
            n--;
        }
        if (n >= 0 && !chess[n][y]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += CutTwo; // ��ϻ��
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZOOZOZ; // 011010
                    }
                    else
                    {
                        score += OZZOZT; // 011012
                    }
                }
                else if (len_m > 1)
                {
                    score += ZOOZOO; // 011011
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZOOZOZ; // 010110
                    }
                    else
                    {
                        score += OZZOZT; // 210110
                    }
                }
                else if (len_n > 1)
                {
                    score += ZOOZOO; // 110110
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += 800; // 211011
            }
            else if (bb_m)
            {
                score += 5; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += 800; // 110112
            }
            else if (bb_n)
            {
                score += 5; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m && len_n)
            {
                score += CutFour; // �൱�ڴ�ϻ���
            }
            if (len_m || len_n)
            {
                score += ZOOOZO; // 011101 || 101110
            }
            else
            {
                score += CutThree; // ��ϻ��� 01110
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += TOOOZO; // 211101 - �������
            }
            else if (bb_m)
            {
                score += TOOOZZ; // 211100 - �������
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += TOOOZO; // 101112 - �������
            }
            else if (bb_n)
            {
                score += TOOOZZ; // 001112 - �������
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m == 1 && blank_n == 1)
            score += CutFour; // ��ϻ�4
        else if (blank_m || blank_n)
            score += TOOOZO; // �����4
    }
    break;
    }
    if (len > 4)
    {
        score += CutWin; // �������(��ֹ��һ�ʤ)
    }
#pragma endregion
#pragma region //? ��б��
    m = x + 1;
    n = y + 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right and down
    while (m < 15 && n < 15 && chess[m][n] == 1)
    {
        len++;
        m++;
        n++;
    }
    if (m < 15 && n < 15 && !chess[m][n])
    {
        blank_m = 1;
        m++;
        n++;
        // �����ж�����
        while (m < 15 && n < 15 && chess[m][n] == 1)
        {
            len_m++;
            m++;
            n++;
        }
        if (m < 15 && n < 15 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left and up
    m = x - 1;
    n = y - 1;
    while (m >= 0 && n >= 0 && chess[m][n] == 1)
    {
        len++;
        m--;
        n--;
    }
    if (m >= 0 && n >= 0 && !chess[m][n])
    {
        blank_n = 1;
        m--;
        n--;
        // �����ж�����
        while (m >= 0 && n >= 0 && chess[m][n] == 1)
        {
            len_n++;
            m--;
            n--;
        }
        if (m >= 0 && n >= 0 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += CutTwo; // ��ϻ��
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZOOZOZ; // 011010
                    }
                    else
                    {
                        score += OZZOZT; // 011012
                    }
                }
                else if (len_m > 1)
                {
                    score += ZOOZOO; // 011011
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZOOZOZ; // 010110
                    }
                    else
                    {
                        score += OZZOZT; // 210110
                    }
                }
                else if (len_n > 1)
                {
                    score += ZOOZOO; // 110110
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += 800; // 211011
            }
            else if (bb_m)
            {
                score += 5; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += 800; // 110112
            }
            else if (bb_n)
            {
                score += 5; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m && len_n)
            {
                score += CutFour; // �൱�ڴ�ϻ���
            }
            if (len_m || len_n)
            {
                score += ZOOOZO; // 011101 || 101110
            }
            else
            {
                score += CutThree; // ��ϻ��� 01110
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += TOOOZO; // 211101 - �������
            }
            else if (bb_m)
            {
                score += TOOOZZ; // 211100 - �������
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += TOOOZO; // 101112 - �������
            }
            else if (bb_n)
            {
                score += TOOOZZ; // 001112 - �������
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m == 1 && blank_n == 1)
            score += CutFour; // ��ϻ�4
        else if (blank_m || blank_n)
            score += TOOOZO; // �����4
    }
    break;
    }
    if (len > 4)
    {
        score += CutWin; // �������(��ֹ��һ�ʤ)
    }
#pragma endregion
#pragma region //? ��б��
    m = x + 1;
    n = y - 1;
    len = 1;     // ����������Ѿ�����
    len_m = 0;   // ��¼m��߼��һ�����������
    len_n = 0;   // ��¼n��߼��һ�����������
    bb_m = 0;    // ���������Ӻ�����ӱ߽��Ƿ�Ϊ��
    bb_n = 0;    // 0 - ��Ϊ�� | 1 - Ϊ��
    blank_m = 0; // ����������ӱ߽��Ƿ�Ϊ��
    blank_n = 0; // 0 - ��Ϊ�� | 1 - Ϊ��
    // right and up
    while (m < 15 && n >= 0 && chess[m][n] == 1)
    {
        len++;
        m++;
        n--;
    }
    if (m < 15 && n >= 0 && !chess[m][n])
    {
        blank_m = 1;
        m++;
        n--;
        // �����ж�����
        while (m < 15 && n >= 0 && chess[m][n] == 1)
        {
            len_m++;
            m++;
            n--;
        }
        if (m < 15 && n >= 0 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_m = 1;
        }
    }
    // left and down
    m = x - 1;
    n = y + 1;
    while (m >= 0 && n < 15 && chess[m][n] == 1)
    {
        len++;
        m--;
        n++;
    }
    if (m >= 0 && n < 15 && !chess[m][n])
    {
        blank_n = 1;
        m--;
        n++;
        // �����ж�����
        while (m >= 0 && n < 15 && chess[m][n] == 1)
        {
            len_n++;
            m--;
            n++;
        }
        if (m >= 0 && n < 15 && !chess[m][n]) // ���Ӻ�ͷ���Ϊ��
        {
            bb_n = 1;
        }
    }
    switch (len)
    {
    case 2:
    {
        if (blank_m && blank_n)
        {
            if (!len_m && !len_n)
            {
                if (bb_m || bb_n)
                {
                    score += CutTwo; // ��ϻ��
                }
            }
            else
            {
                if (len_m == 1)
                {
                    if (bb_m)
                    {
                        score += ZOOZOZ; // 011010
                    }
                    else
                    {
                        score += OZZOZT; // 011012
                    }
                }
                else if (len_m > 1)
                {
                    score += ZOOZOO; // 011011
                }
                if (len_n == 1)
                {
                    if (bb_n)
                    {
                        score += ZOOZOZ; // 010110
                    }
                    else
                    {
                        score += OZZOZT; // 210110
                    }
                }
                else if (len_n > 1)
                {
                    score += ZOOZOO; // 110110
                }
            }
        }
        else if (blank_m)
        {
            if (len_m > 1)
            {
                score += 800; // 211011
            }
            else if (bb_m)
            {
                score += 5; // ��2
            }
        }
        else if (blank_n)
        {
            if (len_n > 1)
            {
                score += 800; // 110112
            }
            else if (bb_n)
            {
                score += 5; // ��2
            }
        }
    }
    break;
    case 3:
    {
        if (blank_m && blank_n)
        {
            if (len_m && len_n)
            {
                score += CutFour; // �൱�ڴ�ϻ���
            }
            if (len_m || len_n)
            {
                score += ZOOOZO; // 011101 || 101110
            }
            else
            {
                score += CutThree; // ��ϻ��� 01110
            }
        }
        else if (blank_m)
        {
            if (len_m)
            {
                score += TOOOZO; // 211101 - �������
            }
            else if (bb_m)
            {
                score += TOOOZZ; // 211100 - �������
            }
        }
        else if (blank_n)
        {
            if (len_n)
            {
                score += TOOOZO; // 101112 - �������
            }
            else if (bb_n)
            {
                score += TOOOZZ; // 001112 - �������
            }
        }
    }
    break;
    case 4:
    {
        if (blank_m == 1 && blank_n == 1)
            score += CutFour; // ��ϻ�4
        else if (blank_m || blank_n)
            score += TOOOZO; // �����4
    }
    break;
    }
    if (len > 4)
    {
        score += CutWin; // �������(��ֹ��һ�ʤ)
    }
#pragma endregion
#pragma endregion
    return score;
}

//! ���AI
void AI_random(int *x, int *y)
{
    srand((unsigned int)time(NULL));
Re__:
    *x = rand() % 15;
    *y = rand() % 15;
    if (chess[*x][*y])
    {
        goto Re__;
    }
    draw_piece(2, *x * 64 + 40, *y * 64 + 40);
    change_chess(2, *x, *y);
}

//! AI˼��ģ��
void AI_thinking(int *x, int *y)
{
    int i, j, score, max = 0, maxi, maxj;
    // �������Ϊ��������������
    if (piece_nums == 0)
    {
        *x = 7;
        *y = 7;
        draw_piece(2, 488, 488); // 7 * 64 + 40 = 488
        change_chess(2, 7, 7);
    }
    // ����˼��
    else
    {
        for (i = 0; i < 15; i++)
        {
            for (j = 0; j < 15; j++)
            {
                if (!chess[i][j])
                {
                    score = AI_score(i, j);
                    if (score > max)
                    {
                        max = score;
                        maxi = i;
                        maxj = j;
                    }
                }
            }
        }
        if (max)
        {
            *x = maxi;
            *y = maxj;
            draw_piece(2, maxi * 64 + 40, maxj * 64 + 40);
            change_chess(2, maxi, maxj);
        }
        else
        {
            AI_random(x, y);
        }
    }
}

//! �ж�ʤ�� | 1 - ʤ 0 - �� 2 - ƽ��
int victory(int chess_color, int x, int y)
{
    int m, n, _long;
#pragma region //?������
    _long = 0;
    m = y;
    n = y - 1;
    // ���ҿ�
    while (chess[x][m] == chess_color && m < 15)
    {
        _long++;
        m++;
    }
    // ����
    while (chess[x][n] == chess_color && n >= 0)
    {
        _long++;
        n--;
    }
    if (_long > 4)
    {
        return 1;
    }
#pragma endregion
#pragma region //?�������
    _long = 0;
    m = x;
    n = x - 1;
    // ���¿�
    while (chess[m][y] == chess_color && m < 15)
    {
        _long++;
        m++;
    }
    // ���Ͽ�
    while (chess[n][y] == chess_color && n >= 0)
    {
        _long++;
        n--;
    }
    if (_long > 4)
    {
        return 1;
    }
#pragma endregion
#pragma region //?�����б��
    _long = 0;
    // �����¿�
    m = x;
    n = y;
    while (chess[m][n] == chess_color && m < 15 && n < 15)
    {
        _long++;
        m++;
        n++;
    }
    // �����Ͽ�
    m = x - 1;
    n = y - 1;
    while (chess[m][n] == chess_color && m >= 0 && n >= 0)
    {
        _long++;
        m--;
        n--;
    }
    if (_long > 4)
    {
        return 1;
    }
#pragma endregion
#pragma region //?�����б��
    _long = 0;
    // �����¿�
    m = x;
    n = y;
    while (chess[m][n] == chess_color && m < 15 && n >= 0)
    {
        _long++;
        m++;
        n--;
    }
    // �����Ͽ�
    m = x - 1;
    n = y + 1;
    while (chess[m][n] == chess_color && m >= 0 && n < 15)
    {
        _long++;
        m--;
        n++;
    }
    if (_long > 4)
    {
        return 1;
    }
#pragma endregion
    // ���ƽ��
    if (piece_nums == 225)
    {
        return 2;
    }
    return 0;
}

//! ����ģʽģ��
void _play_hard(review *head)
{
    int piece_color = 1, success_, x, y, if_, ai_x, ai_y;
    bool check_down;
    free_list(head);
    review *p = head;
    review *temp = NULL;
    // ���̿հױ�־����
    piece_nums = 0;
    BeginBatchDraw();
    cleardevice();
    _bg_put();
    menu_put_3();
    _tips_put_3();
    setlinecolor(BLACK);
    line(970, 0, 970, 1000);
    // ��ӡ����
    draw_bg();
    EndBatchDraw();
    //! ��������
    clear_chess();
Player__:
    check_down = true;
    while (true)
    {
        _bgm_repeat();
        mouse = getmessage(EX_KEY | EX_MOUSE);
        switch (mouse.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (mouse.x > 970)
                {
                    if_ = bt_if(mouse.x, mouse.y, 3);
                    switch (if_)
                    {
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���˵���ť
                    case 5:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, &menu_1_b, SRCAND);
                        putimage(980, 400, &menu_1_a, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_3();
                    }
                    break;
                    }
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                // ���˵���ť
                case 5:
                {
                    BeginBatchDraw();
                    putimage(980, 400, &menu_2_b, SRCAND);
                    putimage(980, 400, &menu_2_a, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = true;
            //? �������
            //! �����������ڲŻ�����
            if (mouse.x < 950 && mouse.y < 960)
            {
                x = (mouse.x - mouse.x % 64) / 64;
                y = (mouse.y - mouse.y % 64) / 64;
                if (chess[x][y])
                {
                    goto Player__;
                }
                draw_piece(1, x * 64 + 40, y * 64 + 40);
                change_chess(1, x, y);
                temp = (review *)malloc(sizeof(review));
                if (temp == NULL)
                {
                    cleardevice();
                    _bg_put();
                    LOGFONT Error_tip;
                    gettextstyle(&Error_tip); // ��ȡ��ǰ��������
                    Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
                    settextcolor(RGB(300, 55, 85));
                    _tcscpy_s(Error_tip.lfFaceName, _T("����"));
                    Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
                    settextstyle(&Error_tip);                  // ˢ����ʽ
                    setbkmode(TRANSPARENT);
                    outtextxy(300, 400, "�����ڴ�ʧ�ܣ��������");
                    _getch();
                    exit(1);
                }
                temp->color = 1;
                temp->x = x * 64 + 40;
                temp->y = y * 64 + 40;
                temp->next = NULL;
                p->next = temp;
                p = temp;
                piece_nums++;
                //? �ж���Ӯ ��� - ���� - 1
                success_ = victory(1, x, y);
                if (success_ == 1)
                {
                    putimage(300, 100, &victory_player_black_b, SRCAND);
                    putimage(300, 100, &victory_player_black_a, SRCPAINT);
                    goto KEY__;
                }
                else if (success_ == 2)
                {
                    putimage(300, 100, &same_b, SRCAND);
                    putimage(300, 100, &same_a, SRCPAINT);
                    goto KEY__;
                }
                //? ��������
                //*��������ӣ���װ����˼���ܾ�(doge)
                Sleep(400);
                AI_thinking(&ai_x, &ai_y);
                temp = (review *)malloc(sizeof(review));
                if (temp == NULL)
                {
                    cleardevice();
                    _bg_put();
                    LOGFONT Error_tip;
                    gettextstyle(&Error_tip); // ��ȡ��ǰ��������
                    Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
                    settextcolor(RGB(300, 55, 85));
                    _tcscpy_s(Error_tip.lfFaceName, _T("����"));
                    Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
                    settextstyle(&Error_tip);                  // ˢ����ʽ
                    setbkmode(TRANSPARENT);
                    outtextxy(300, 400, "�����ڴ�ʧ�ܣ��������");
                    _getch();
                    exit(1);
                }
                temp->color = 2;
                temp->x = ai_x * 64 + 40;
                temp->y = ai_y * 64 + 40;
                temp->next = NULL;
                p->next = temp;
                p = temp;
                piece_nums++;
                //? �ж���Ӯ ���� - ���� - 2
                success_ = victory(2, ai_x, ai_y);
                if (success_ == 1)
                {
                    putimage(300, 100, &victory_ai_white_b, SRCAND);
                    putimage(300, 100, &victory_ai_white_a, SRCPAINT);
                    goto KEY__;
                }
                else if (success_ == 2)
                {
                    putimage(300, 100, &same_b, SRCAND);
                    putimage(300, 100, &same_a, SRCPAINT);
                    goto KEY__;
                }
            }
            else if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                // ���˵���ť
                case 5:
                {
                    restart_();
                    return;
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (mouse.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� R ������
            else if (mouse.vkcode == 0x52)
            {
                restart_();
                return;
            }
            // �� M ������ bgm �Ĳ�������ͣ
            else if (mouse.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� F2 ����������ͼ
            else if (mouse.vkcode == VK_F2)
            {
                screenshot_();
            }
        }
        break;
        }
    }
KEY__:
    check_down = true;
    while (true)
    {
        _bgm_repeat();
        _kb_1 = getmessage(EX_KEY | EX_MOUSE);
        switch (_kb_1.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (_kb_1.x > 970)
                {
                    if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                    switch (if_)
                    {
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���˵���ť
                    case 5:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, &menu_1_b, SRCAND);
                        putimage(980, 400, &menu_1_a, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_3();
                    }
                    break;
                    }
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (_kb_1.x > 970)
            {
                if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                // ���˵���ť
                case 5:
                {
                    BeginBatchDraw();
                    putimage(980, 400, &menu_2_b, SRCAND);
                    putimage(980, 400, &menu_2_a, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = true;
            if (_kb_1.x > 970)
            {
                if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                // ���˵���ť
                case 5:
                {
                    restart_();
                    return;
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (_kb_1.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� R ������
            else if (_kb_1.vkcode == 0x52)
            {
                restart_();
                return;
            }
            // �� M ������ bgm �Ĳ�������ͣ
            else if (_kb_1.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� F2 ����������ͼ
            else if (_kb_1.vkcode == VK_F2)
            {
                screenshot_();
            }
        }
        break;
        }
    }
}
//! ��ģʽģ��
void _play_easy(review *head)
{
    int piece_color = 1, success_, x, y, if_, ai_x, ai_y;
    char times[30], shot[35] = "screenshot/";
    bool check_down;
    free_list(head);
    review *p = head;
    review *temp = NULL;
    // ���̿հױ�־����
    piece_nums = 0;
    BeginBatchDraw();
    cleardevice();
    _bg_put();
    menu_put_3();
    _tips_put_3();
    setlinecolor(BLACK);
    line(970, 0, 970, 1000);
    // ��ӡ����
    draw_bg();
    EndBatchDraw();
    //! ��������
    clear_chess();
Player__:
    check_down = true;
    while (true)
    {
        _bgm_repeat();
        mouse = getmessage(EX_KEY | EX_MOUSE);
        switch (mouse.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (mouse.x > 970)
                {
                    if_ = bt_if(mouse.x, mouse.y, 3);
                    switch (if_)
                    {
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���˵���ť
                    case 5:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, &menu_1_b, SRCAND);
                        putimage(980, 400, &menu_1_a, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_3();
                    }
                    break;
                    }
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                // ���˵���ť
                case 5:
                {
                    BeginBatchDraw();
                    putimage(980, 400, &menu_2_b, SRCAND);
                    putimage(980, 400, &menu_2_a, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = true;
            //? �������
            //! �����������ڲŻ�����
            if (mouse.x < 950 && mouse.y < 960)
            {
                x = (mouse.x - mouse.x % 64) / 64;
                y = (mouse.y - mouse.y % 64) / 64;
                if (chess[x][y])
                {
                    goto Player__;
                }
                draw_piece(1, x * 64 + 40, y * 64 + 40);
                change_chess(1, x, y);
                temp = (review *)malloc(sizeof(review));
                if (temp == NULL)
                {
                    cleardevice();
                    _bg_put();
                    LOGFONT Error_tip;
                    gettextstyle(&Error_tip); // ��ȡ��ǰ��������
                    Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
                    settextcolor(RGB(300, 55, 85));
                    _tcscpy_s(Error_tip.lfFaceName, _T("����"));
                    Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
                    settextstyle(&Error_tip);                  // ˢ����ʽ
                    setbkmode(TRANSPARENT);
                    outtextxy(300, 400, "�����ڴ�ʧ�ܣ��������");
                    _getch();
                    exit(1);
                }
                temp->color = 1;
                temp->x = x * 64 + 40;
                temp->y = y * 64 + 40;
                temp->next = NULL;
                p->next = temp;
                p = temp;
                piece_nums++;
                //? �ж���Ӯ ��� - ���� - 1
                success_ = victory(1, x, y);
                if (success_ == 1)
                {
                    putimage(300, 100, &victory_player_black_b, SRCAND);
                    putimage(300, 100, &victory_player_black_a, SRCPAINT);
                    goto KEY__;
                }
                else if (success_ == 2)
                {
                    putimage(300, 100, &same_b, SRCAND);
                    putimage(300, 100, &same_a, SRCPAINT);
                    goto KEY__;
                }
                //? ��������
                //*��������ӣ���װ����˼���ܾ�(doge)
                Sleep(400);
                AI_random(&ai_x, &ai_y);
                temp = (review *)malloc(sizeof(review));
                if (temp == NULL)
                {
                    cleardevice();
                    _bg_put();
                    LOGFONT Error_tip;
                    gettextstyle(&Error_tip); // ��ȡ��ǰ��������
                    Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
                    settextcolor(RGB(300, 55, 85));
                    _tcscpy_s(Error_tip.lfFaceName, _T("����"));
                    Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
                    settextstyle(&Error_tip);                  // ˢ����ʽ
                    setbkmode(TRANSPARENT);
                    outtextxy(300, 400, "�����ڴ�ʧ�ܣ��������");
                    _getch();
                    exit(1);
                }
                temp->color = 2;
                temp->x = ai_x * 64 + 40;
                temp->y = ai_y * 64 + 40;
                temp->next = NULL;
                p->next = temp;
                p = temp;
                piece_nums++;
                //? �ж���Ӯ ���� - ���� - 2
                success_ = victory(2, ai_x, ai_y);
                if (success_ == 1)
                {
                    putimage(300, 100, &victory_ai_white_b, SRCAND);
                    putimage(300, 100, &victory_ai_white_a, SRCPAINT);
                    goto KEY__;
                }
                else if (success_ == 2)
                {
                    putimage(300, 100, &same_b, SRCAND);
                    putimage(300, 100, &same_a, SRCPAINT);
                    goto KEY__;
                }
            }
            else if (mouse.x > 970)
            {
                if_ = bt_if(mouse.x, mouse.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                // ���˵���ť
                case 5:
                {
                    restart_();
                    return;
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (mouse.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� R ������
            else if (mouse.vkcode == 0x52)
            {
                restart_();
                return;
            }
            // �� M ���������ֲ�������ͣ
            else if (mouse.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� F2 ����������ͼ
            else if (mouse.vkcode == VK_F2)
            {
                screenshot_();
            }
        }
        break;
        }
    }
KEY__:
    check_down = true;
    while (true)
    {
        _bgm_repeat();
        _kb_1 = getmessage(EX_KEY | EX_MOUSE);
        switch (_kb_1.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (_kb_1.x > 970)
                {
                    if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                    switch (if_)
                    {
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���˵���ť
                    case 5:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, &menu_1_b, SRCAND);
                        putimage(980, 400, &menu_1_a, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_3();
                    }
                    break;
                    }
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (_kb_1.x > 970)
            {
                if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                // ���˵���ť
                case 5:
                {
                    BeginBatchDraw();
                    putimage(980, 400, &menu_2_b, SRCAND);
                    putimage(980, 400, &menu_2_a, SRCPAINT);
                    EndBatchDraw();
                    check_down = false;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = true;
            if (_kb_1.x > 970)
            {
                if_ = bt_if(_kb_1.x, _kb_1.y, 3);
                switch (if_)
                {
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                // ���˵���ť
                case 5:
                {
                    restart_();
                    return;
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (_kb_1.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� R ������
            else if (_kb_1.vkcode == 0x52)
            {
                restart_();
                return;
            }
            // �� M ���������ֲ�������ͣ
            else if (_kb_1.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� F2 ����������ͼ
            else if (_kb_1.vkcode == VK_F2)
            {
                screenshot_();
            }
        }
        break;
        }
    }
}

//! ѡ��ģ��
void _begin__()
{
    // ��������
    review *head = (review *)malloc(sizeof(review));
    if (head == NULL)
    {
        cleardevice();
        _bg_put();
        LOGFONT Error_tip;
        gettextstyle(&Error_tip); // ��ȡ��ǰ��������
        Error_tip.lfHeight = 50;  // ��������߶�Ϊ 50
        settextcolor(RGB(300, 55, 85));
        _tcscpy_s(Error_tip.lfFaceName, _T("����"));
        Error_tip.lfQuality = ANTIALIASED_QUALITY; // �������Ч��Ϊ�����
        settextstyle(&Error_tip);                  // ˢ����ʽ
        setbkmode(TRANSPARENT);
        outtextxy(300, 400, "�����ڴ�ʧ�ܣ��������");
        _getch();
        exit(1);
    }
    head->next = NULL;
    int if_, check_down;
Begin__:
    check_down = 1;
    while (true)
    {
        _bgm_repeat();
        _kb = getmessage(EX_KEY | EX_MOUSE);
        switch (_kb.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (_kb.x > 970)
                {
                    if_ = bt_if(_kb.x, _kb.y, 1);
                    switch (if_)
                    {
                    // ��ʼ��ť
                    case 1:
                    {
                        BeginBatchDraw();
                        putimage(980, 300, 200, 60, &tips_1_b, 0, 0, SRCAND);
                        putimage(980, 300, 200, 60, &tips_1_a, 0, 0, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_1();
                    }
                    break;
                    }
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (_kb.x > 970)
            {
                if_ = bt_if(_kb.x, _kb.y, 1);
                switch (if_)
                {
                // ��ʼ��ť
                case 1:
                {
                    BeginBatchDraw();
                    putimage(1000, 300, 180, 60, &bgr, 0, 300);
                    putimage(980, 300, 200, 60, &tips_2_b, 0, 0, SRCAND);
                    putimage(980, 300, 200, 60, &tips_2_a, 0, 0, SRCPAINT);
                    EndBatchDraw();
                    check_down = 0;
                }
                break;
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(1000, 700, 180, 50, &bgr, 0, 700);
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = 0;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = 1;
            if (_kb.x > 970)
            {
                if_ = bt_if(_kb.x, _kb.y, 1);
                switch (if_)
                {
                // ��ʼ��ť
                case 1:
                {
                    BeginBatchDraw();
                    cleardevice();
                    _bg_put();
                    menu_put_2();
                    _tips_put_2();
                    setlinecolor(BLACK);
                    line(970, 0, 970, 1000);
                    EndBatchDraw();
                    goto Mode_;
                }
                break;
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (_kb.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� S ����ʼ��Ϸ
            else if (_kb.vkcode == 0x53)
            {
                BeginBatchDraw();
                cleardevice();
                _bg_put();
                menu_put_2();
                _tips_put_2();
                setlinecolor(BLACK);
                line(970, 0, 970, 1000);
                EndBatchDraw();
                goto Mode_;
            }
            // �� M ������ bgm �Ĳ�������ͣ
            else if (_kb.vkcode == 0x4D)
            {
                _bgm_play();
            }
            // �� F ���򿪸��̹���
            else if (_kb.vkcode == 0x46)
            {
                review_(head);
            }
        }
        break;
        }
    }
Mode_:
    check_down = 1;
    while (true)
    {
        _bgm_repeat();
        _kb_2 = getmessage(EX_KEY | EX_MOUSE);
        switch (_kb_2.message)
        {
        case WM_MOUSEMOVE:
        {
            if (check_down)
            {
                if (_kb_2.x > 970)
                {
                    if_ = bt_if(_kb_2.x, _kb_2.y, 2);
                    switch (if_)
                    {
                    // �򵥰�ť
                    case 3:
                    {
                        BeginBatchDraw();
                        putimage(980, 300, 200, 55, &tips_1_b, 0, 60, SRCAND);
                        putimage(980, 300, 200, 55, &tips_1_a, 0, 60, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // �رհ�ť
                    case 2:
                    {
                        BeginBatchDraw();
                        putimage(980, 700, 200, 50, &tips_1_b, 0, 165, SRCAND);
                        putimage(980, 700, 200, 50, &tips_1_a, 0, 165, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    // ���Ѱ�ť
                    case 4:
                    {
                        BeginBatchDraw();
                        putimage(980, 400, 200, 50, &tips_1_b, 0, 115, SRCAND);
                        putimage(980, 400, 200, 50, &tips_1_a, 0, 115, SRCPAINT);
                        EndBatchDraw();
                    }
                    break;
                    default:
                    {
                        menu_put_2();
                    }
                    break;
                    }
                    Sleep(10);
                }
            }
        }
        break;
        case WM_LBUTTONDOWN:
        {
            if (_kb_2.x > 970)
            {
                if_ = bt_if(_kb_2.x, _kb_2.y, 2);
                switch (if_)
                {
                // �򵥰�ť
                case 3:
                {
                    BeginBatchDraw();
                    putimage(980, 300, 200, 55, &tips_2_b, 0, 60, SRCAND);
                    putimage(980, 300, 200, 55, &tips_2_a, 0, 60, SRCPAINT);
                    EndBatchDraw();
                    check_down = 0;
                }
                break;
                // �رհ�ť
                case 2:
                {
                    BeginBatchDraw();
                    putimage(980, 700, 200, 50, &tips_2_b, 0, 165, SRCAND);
                    putimage(980, 700, 200, 50, &tips_2_a, 0, 165, SRCPAINT);
                    EndBatchDraw();
                    check_down = 0;
                }
                break;
                // ���Ѱ�ť
                case 4:
                {
                    BeginBatchDraw();
                    putimage(980, 400, 200, 50, &tips_2_b, 0, 115, SRCAND);
                    putimage(980, 400, 200, 50, &tips_2_a, 0, 115, SRCPAINT);
                    EndBatchDraw();
                    check_down = 0;
                }
                break;
                }
            }
        }
        break;
        case WM_LBUTTONUP:
        {
            check_down = 1;
            if (_kb_2.x > 970)
            {
                if_ = bt_if(_kb_2.x, _kb_2.y, 2);
                switch (if_)
                {
                // �򵥰�ť
                case 3:
                {
                    _play_easy(head);
                    goto Begin__;
                }
                break;
                // ���Ѱ�ť
                case 4:
                {
                    _play_hard(head);
                    goto Begin__;
                }
                break;
                // �رհ�ť
                case 2:
                {
                    exit(0);
                }
                break;
                }
            }
        }
        break;
        case WM_KEYDOWN:
        {
            // �� ESC ���˳�����
            if (_kb_2.vkcode == VK_ESCAPE)
            {
                exit(0);
            }
            // �� E ����ʼ��ģʽ
            else if (_kb_2.vkcode == 0x45)
            {
                _play_easy(head);
                goto Begin__;
            }
            // �� H ����ʼ����ģʽ
            else if (_kb_2.vkcode == 0x48)
            {
                _play_hard(head);
                goto Begin__;
            }
            // �� M ������ bgm �Ĳ�������ͣ
            else if (_kb_2.vkcode == 0x4D)
            {
                _bgm_play();
            }
        }
        break;
        }
    }
}