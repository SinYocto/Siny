#ifndef GUI_H
#define GUI_H

#include"D3DUtility.h"
#include"Math.h"
#include<vector>
#include<list>
#include<map>

enum ButtonState { Normal, Hover, Active };

class LabelStyle
{
public:
	LabelStyle(char* font_name, int font_width, int font_height, int font_weight, D3DXCOLOR font_color, DWORD align);
	void CreateFont();
public:
	ID3DXFont* font;
	D3DXCOLOR color;
	DWORD alignFlag;
private:
	char* fontName;
	int fontWidth;
	int fontHeight;
	int fontWeight;
};

extern LabelStyle defaultLabelStyle;

class Label
{
public:
	Label(LPWSTR label_text, Rect label_rect, LabelStyle* label_style = NULL);
public:
	LPWSTR text;
	Rect rect;
	LabelStyle* style;
};

class ButtonStyle
{
public:
	ButtonStyle(LabelStyle* _labelStyle, char* _normalTexFile, char* _hoverTexFile, char* _activeTexFile);
	void CreateTextures();
public:
	IDirect3DTexture9 *normalTex;
	IDirect3DTexture9 *hoverTex;
	IDirect3DTexture9 *activeTex;
	LabelStyle* labelStyle;
private:
	char* normalTexFile;
	char* hoverTexFile;
	char* activeTexFile;
};

extern ButtonStyle defaultButtonStyle;

class Button
{
public:
	Button(LPWSTR button_text, Rect button_rect, ButtonStyle* button_style = NULL);
	void GetButtonState();
private:
	void CreateVertexBuffer();
public:
	LPWSTR text;
	Rect rect;
	ButtonStyle* style;
	IDirect3DVertexBuffer9 *vertexBuffer;

	ButtonState state;
};

class ToggleStyle
{
public:
	ToggleStyle(char* _offNormalTexFile, char* _offHoverTexFile, char* _onNormalTexFile, char* _onHoverTexFile);
	void CreateTextures();
public:
	IDirect3DTexture9 *offNormalTex;
	IDirect3DTexture9 *offHoverTex;
	IDirect3DTexture9 *onNormalTex;
	IDirect3DTexture9 *onHoverTex;
private:
	char* offNormalTexFile;
	char* offHoverTexFile;
	char* onNormalTexFile;
	char* onHoverTexFile;
};

extern ToggleStyle defaultToggleStyle;

class Toggle
{
public:
	Toggle(Rect toggle_rect, bool _value, ToggleStyle* toggle_style = NULL);
	void GetButtonState();
private:
	void CreateVertexBuffer();
public:
	Rect rect;
	ToggleStyle* style;
	IDirect3DVertexBuffer9 *vertexBuffer;

	ButtonState state;
	bool value;
}; 

class SliderStyle
{
public:
	SliderStyle(char* _sliderBarTexFile, char* _normalTexFile, char* _hoverTexFile, char* _activeTexFile, float _sliderBarSize);
	void CreateTextures();
public:
	IDirect3DTexture9 *sliderBarTex;
	IDirect3DTexture9 *normalTex;
	IDirect3DTexture9 *hoverTex;
	IDirect3DTexture9 *activeTex;
	float sliderBarSize;

private:
	char* sliderBarTexFile;
	char* normalTexFile;
	char* hoverTexFile;
	char* activeTexFile;
};

extern SliderStyle defaultSliderStyle;

class Slider
{
public:
	Slider(Rect slider_rect, float _minValue, float _maxValue, float _value, char* slider_id, SliderStyle* slider_style = NULL);
	void GetButtonState();
private:
	void CreateVertexBuffer();
public:
	Rect rect;
	SliderStyle* style;
	IDirect3DVertexBuffer9 *sliderBarVB;
	IDirect3DVertexBuffer9 *sliderThumbVB;

	float minValue;
	float maxValue;
	float value;

	char* id;

	ButtonState state;
};


class ListBoxStyle
{
public:
	ListBoxStyle(LabelStyle* _labelStyle, char* _buttonNormalTexFile, char* _buttonHoverTexFile, char* _itemNormalTexFile, char* _itemHoverTexFile);
	void CreateTextures();
public:
	IDirect3DTexture9 *buttonNormalTex;
	IDirect3DTexture9 *buttonHoverTex;
	IDirect3DTexture9 *itemNormalTex;
	IDirect3DTexture9 *itemHoverTex;
	LabelStyle* labelStyle;
private:
	char* buttonNormalTexFile;
	char* buttonHoverTexFile;
	char* itemNormalTexFile;
	char* itemHoverTexFile;
};

extern ListBoxStyle defaultListBoxStyle;

struct ListBoxItem
{
	LPWSTR text;
	IDirect3DVertexBuffer9 *vertexBuffer;
	int ix;
};

class ListBox
{
public:
	ListBox(vector<LPWSTR> _texts, Rect _rect, int _ix, char* _id, ListBoxStyle* listBox_style = NULL);
	void AddItem(LPWSTR _text);
	void GetState();
private:
	void CreateBodyVB();
	void CreateItemVB(int itemIx);
public:
	vector<ListBoxItem> items;
	IDirect3DVertexBuffer9 *labelVB;
	IDirect3DVertexBuffer9 *buttonVB;
	Rect rect;
	ListBoxStyle* style;

	bool isSelecting;
	int cursorState;
	int selectedIx;

	char* id;
};


class ColorPicker
{
public:
	ColorPicker(Rect _rect, D3DXCOLOR _rgb, char* _id);
private:
	void CreateVertexBuffer();
public:
	Rect rect;
	IDirect3DVertexBuffer9 *colorVB;
	IDirect3DVertexBuffer9 *hueVB;
	IDirect3DVertexBuffer9 *SLVB;
	Vector3 hsl;
	D3DXCOLOR rgb;

	bool isPickingColor;

	char* id;
};

class GUI
{
public:
	GUI() {}
	void clear();
	void GUILabel(char* label_text, Rect label_rect, LabelStyle* label_style = NULL);
	bool GUIButton(char* button_text, Rect button_rect, ButtonStyle* button_style = NULL);
	float GUISlider(Rect slider_rect, float _minValue, float _maxValue, float _value, char* slider_id, SliderStyle* slider_style = NULL);
	bool GUIToggle(Rect toggle_rect, bool _value, ToggleStyle* toggle_style = NULL);
	int GUIListBox(vector<LPWSTR> _texts, Rect _rect, int _ix, char* _id, ListBoxStyle* listBox_style = NULL);
	D3DXCOLOR GUIColor(Rect _rect, D3DXCOLOR _rgb, char* _id);
	void AddLabel(Label* label);
	void AddButton(Button* button);
	void AddSlider(Slider* slider);
	void AddToggle(Toggle* toggle);
	void AddListBox(ListBox* listBox);
	void AddColorPicker(ColorPicker* colorPicker);
	void Draw();
public:
	map<char*, ButtonState> sliderState; 
	map<char*, bool> colorPickerState;
	map<char*, bool> listBoxState;
private:
	list<Label*> labels;
	list<Button*> buttons;
	list<Slider*> sliders;
	list<Toggle*> toggles;
	list<ListBox*> listBoxes;
	list<ColorPicker*> colorPickers;
};

extern GUI GUISystem;


#endif