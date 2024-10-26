#include <graphics.h>
#include <iostream>

#define PT_BUF_LENGTH 256
#define KEY_DOWN(VK_NONAME) ((GetKeyState(VK_NONAME) & 0x8000) ? 1:0)

using namespace std;

ExMessage m;
bool prev_q, prst_q;
bool prev_w, prst_w;
bool prev_e, prst_e;
bool is_pres_q() {
	return ((prev_q == 0) && (prst_q == 1));
}
bool is_pres_w() {
	return ((prev_w == 0) && (prst_w == 1));
}
bool is_pres_e() {
	return ((prev_e == 0) && (prst_e == 1));
}

struct PT {
	double x;
	double y;
	PT operator ^ (const double k) const {
		return PT{ k * x,k * y };
	}
	PT operator * (const PT b) const {
		return PT{ x * b.x,y * b.y };
	}
	PT operator + (const PT b) const {
		return PT{ x + b.x,y + b.y };
	}
	PT operator - (const PT b) const {
		return PT{ x - b.x,y - b.y };
	}
	bool operator == (const PT b) const {
		if (b.x == x && b.y == y) {
			return 1;
		}
		else {
			return 0;
		}
	}
	bool operator != (const PT b) const {
		if (b.x == x && b.y == y) {
			return 0;
		}
		else {
			return 1;
		}
	}
};
PT global_offset = PT{ 300,300 };
PT global_scale = PT{ 256,-256 };
POINT _dkru(PT p) {
	return POINT{ int(p.x),int(p.y) };
}
struct character {
	char name[64];
	unsigned long long index;
	PT pointbuf[PT_BUF_LENGTH];
};
void exline(PT p1, PT p2) {
	line(p1.x,p1.y,p2.x,p2.y);
}
void excircle(PT p1, double r) {
	circle(p1.x, p1.y, r);
}
PT pointbuf[PT_BUF_LENGTH];
int prst_pos;
int mode;
void addpoint(PT p) {
	pointbuf[prst_pos++] = p;
}
void output_buffer(PT offset, PT scale) {
	setlinecolor(0x003300);
	rectangle(offset.x - scale.x, offset.y - scale.y, offset.x + scale.x, offset.y + scale.y);
	rectangle(offset.x - scale.x * 0.7, offset.y - scale.y * 0.7, offset.x + scale.x * 0.7, offset.y + scale.y * 0.7);

	setlinecolor(0x00ff00);
	int polylen = 0;
	for (int i = 1; i < PT_BUF_LENGTH; i++) {
		if (pointbuf[i] == PT{ 0,0 }) {
			;
		}
		else {
			if (pointbuf[i-1] != PT{0,0})
				exline(offset + (scale * pointbuf[i - 1]), offset + (scale * pointbuf[i]));
		}
	}
}
void obj_update() {
	prev_q = prst_q;
	prev_w = prst_w;
	prev_e = prst_e;
	prst_q = KEY_DOWN('Q');
	prst_w = KEY_DOWN('W');
	prst_e = KEY_DOWN('E');

	if (is_pres_q())prst_pos--;
	if (is_pres_w())prst_pos++;
	if (is_pres_e())pointbuf[prst_pos] = PT{ 0,0 };

	if (prst_pos == -1)prst_pos = 0;

	if (KEY_DOWN(VK_LBUTTON)) {
		pointbuf[prst_pos].x = 1.0 * (m.x-300) / 256;
		pointbuf[prst_pos].y = -1.0 * (m.y-300) / 256;
	}

	setlinecolor(0xffff00);
	circle(m.x, m.y, 3);
}
void obj_bond() {
	;
}
void draw_framework() {
	if (mode == 0) {
		;
	}
	if (mode == 1) {
		setlinecolor(0x00ffff);
		excircle(global_offset + (global_scale * pointbuf[prst_pos]), 3);
		output_buffer(PT{ 300,300 }, PT{ 256,-256 });
		output_buffer(PT{ 500,50 }, PT{ 20,-20 });
		output_buffer(PT{ 560,50 }, PT{ 10,-10 });
	}
}
int main() {
	initgraph(800, 600);
	setbkcolor(0x000000);
	setfillcolor(0xffffff);
	setlinecolor(0xffffff);
	settextstyle(12, 6, _T("PMingLiU"));

	addpoint(PT{ -1,0 });
	addpoint(PT{ 1,0 });

	prst_pos = 0;
	mode = 1;

	for (;;) {
		BeginBatchDraw();
		cleardevice();
		obj_update();
		obj_bond();
		draw_framework();
		FlushBatchDraw();
		peekmessage(&m);
	}
}