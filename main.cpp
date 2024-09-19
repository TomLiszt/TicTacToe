#include <graphics.h>

// 棋盘结构体，注意，如果要符合直觉的二维数组布局，其填充是反直觉的：第一维是 y 轴，第二维才是 x 轴
char board_data[3][3] =
{
	{'-', '-', '-'},
	{'-', '-', '-'},
	{'-', '-', '-'}
};

// 当前落子类型
char current_piece = 'O';

// 检测玩家是否获胜
bool CheckWin(char c)
{
	if (board_data[0][0] == c && board_data[0][1] == c && board_data[0][2] == c)
		return true;
	if (board_data[1][0] == c && board_data[1][1] == c && board_data[1][2] == c)
		return true;
	if (board_data[2][0] == c && board_data[2][1] == c && board_data[2][2] == c)
		return true;
	if (board_data[0][0] == c && board_data[1][0] == c && board_data[2][0] == c)
		return true;
	if (board_data[0][1] == c && board_data[1][1] == c && board_data[2][1] == c)
		return true;
	if (board_data[0][2] == c && board_data[1][2] == c && board_data[2][2] == c)
		return true;
	if (board_data[0][0] == c && board_data[1][1] == c && board_data[2][2] == c)
		return true;
	if (board_data[0][2] == c && board_data[1][1] == c && board_data[2][0] == c)
		return true;

	return false;
}

// 检测是否平局
bool CheckDraw()
{
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			if (board_data[i][j] == '-')
			{
				return false;
			}
		}
	}

	return true;
}

// 绘制棋盘网格
void DrawBoard()
{
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
}

// 绘制棋子
void DrawPiece()
{
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			switch (board_data[i][j])
			{
			case 'O':
				// i、j 是当前网格的左上角顶点，每个格子 200 px 的话，圆心就是 x、y 各自增加 100 px 的地方
				circle(200 * j + 100, 200 * i + 100, 100);
				break;
			case 'X':
				// 连接当前网格的对角线顶点，形成 X 号
				line(200 * j, 200 * i, 200 * (j + 1), 200 * (i + 1));
				line(200 * (j + 1), 200 * i, 200 * j, 200 * (i + 1));
				break;
			case '-':
				break;
			default:
				break;
			}
		}
	}
}

// 绘制提示信息
void DrawTipText()
{
	static TCHAR str[64];
	_stprintf_s(str, _T("当前棋子类型：%c"), current_piece);

	settextcolor(RGB(255, 175, 45));
	outtextxy(0, 0, str);
}

int main()
{
	initgraph(600, 600);

	bool running = true;

	ExMessage msg;

	BeginBatchDraw();

	while (running)
	{
		DWORD start_time = GetTickCount();

		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				// 计算鼠标点击位置
				int x = msg.x;
				int y = msg.y;

				int index_x = x / 200;
				int index_y = y / 200;

				// 映射到棋盘上
				if (board_data[index_y][index_x] == '-')
				{
					board_data[index_y][index_x] = current_piece;

					// 切换棋子类型
					if (current_piece == 'O')
						current_piece = 'X';
					else
						current_piece = 'O';

				}
			}
		}

		// 双缓冲区是“交替运行”的，如果不清除画布，就会导致图形残留在 A 画布，替换到 B 画布后，再替换回 A 画布，依然还有上上次的图形。
		cleardevice();

		DrawBoard();
		DrawPiece();
		DrawTipText();
		FlushBatchDraw();

		// 在绘制落子之后再进行胜负判断
		if (CheckWin('X'))
		{
			MessageBox(GetHWnd(), _T("X 玩家获胜"), _T("游戏结束"), MB_OK);
			running = false;
		}
		else if (CheckWin('O'))
		{
			MessageBox(GetHWnd(), _T("O 玩家获胜"), _T("游戏结束"), MB_OK);
			running = false;
		}
		else if (CheckDraw())
		{
			MessageBox(GetHWnd(), _T("平局"), _T("游戏结束"), MB_OK);
			running = false;
		}


		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		// 假设确保画面以最高 60帧/秒 的速率刷新，那么每帧的最长时间就是 1000毫秒/60帧。若实际耗时超过期望的单帧时间，则直接进入下一次循环；否则强制 sleep 剩余时间
		if (delta_time < 1000 / 60)
		{
			Sleep(1000 / 60 - delta_time);
		}
	}

	EndBatchDraw();
}