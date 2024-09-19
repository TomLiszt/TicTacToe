#include <graphics.h>

// ���̽ṹ�壬ע�⣬���Ҫ����ֱ���Ķ�ά���鲼�֣�������Ƿ�ֱ���ģ���һά�� y �ᣬ�ڶ�ά���� x ��
char board_data[3][3] =
{
	{'-', '-', '-'},
	{'-', '-', '-'},
	{'-', '-', '-'}
};

// ��ǰ��������
char current_piece = 'O';

// �������Ƿ��ʤ
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

// ����Ƿ�ƽ��
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

// ������������
void DrawBoard()
{
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
}

// ��������
void DrawPiece()
{
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			switch (board_data[i][j])
			{
			case 'O':
				// i��j �ǵ�ǰ��������ϽǶ��㣬ÿ������ 200 px �Ļ���Բ�ľ��� x��y �������� 100 px �ĵط�
				circle(200 * j + 100, 200 * i + 100, 100);
				break;
			case 'X':
				// ���ӵ�ǰ����ĶԽ��߶��㣬�γ� X ��
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

// ������ʾ��Ϣ
void DrawTipText()
{
	static TCHAR str[64];
	_stprintf_s(str, _T("��ǰ�������ͣ�%c"), current_piece);

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
				// ���������λ��
				int x = msg.x;
				int y = msg.y;

				int index_x = x / 200;
				int index_y = y / 200;

				// ӳ�䵽������
				if (board_data[index_y][index_x] == '-')
				{
					board_data[index_y][index_x] = current_piece;

					// �л���������
					if (current_piece == 'O')
						current_piece = 'X';
					else
						current_piece = 'O';

				}
			}
		}

		// ˫�������ǡ��������С��ģ����������������ͻᵼ��ͼ�β����� A �������滻�� B ���������滻�� A ��������Ȼ�������ϴε�ͼ�Ρ�
		cleardevice();

		DrawBoard();
		DrawPiece();
		DrawTipText();
		FlushBatchDraw();

		// �ڻ�������֮���ٽ���ʤ���ж�
		if (CheckWin('X'))
		{
			MessageBox(GetHWnd(), _T("X ��һ�ʤ"), _T("��Ϸ����"), MB_OK);
			running = false;
		}
		else if (CheckWin('O'))
		{
			MessageBox(GetHWnd(), _T("O ��һ�ʤ"), _T("��Ϸ����"), MB_OK);
			running = false;
		}
		else if (CheckDraw())
		{
			MessageBox(GetHWnd(), _T("ƽ��"), _T("��Ϸ����"), MB_OK);
			running = false;
		}


		DWORD end_time = GetTickCount();
		DWORD delta_time = end_time - start_time;

		// ����ȷ����������� 60֡/�� ������ˢ�£���ôÿ֡���ʱ����� 1000����/60֡����ʵ�ʺ�ʱ���������ĵ�֡ʱ�䣬��ֱ�ӽ�����һ��ѭ��������ǿ�� sleep ʣ��ʱ��
		if (delta_time < 1000 / 60)
		{
			Sleep(1000 / 60 - delta_time);
		}
	}

	EndBatchDraw();
}