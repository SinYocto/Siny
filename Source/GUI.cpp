#include"GUI.h"
#include"Shader.h"

LabelStyle::LabelStyle(char *font_name, int font_width, int font_height, int font_weight, D3DXCOLOR font_color, DWORD align)
{
	fontName = font_name;
	fontWidth = font_width;
	fontHeight = font_height;
	fontWeight = font_weight;

	color = font_color;
	alignFlag = align;

	font = NULL;
}

void LabelStyle::CreateFont()
{
	SAFE_RELEASE(font);

	D3DXFONT_DESC fontDesc;
	ZeroMemory(&fontDesc, sizeof(D3DXFONT_DESC));
	strcpy(fontDesc.FaceName, fontName);
	fontDesc.Height = fontHeight;
	fontDesc.Width = fontWidth;
	fontDesc.Weight = fontWeight;
	fontDesc.MipLevels = D3DX_DEFAULT;
	fontDesc.Quality = CLEARTYPE_QUALITY;
	fontDesc.OutputPrecision = OUT_OUTLINE_PRECIS;

	D3DXCreateFontIndirect(D3DDevice, &fontDesc, &font);
}

Label::Label(LPWSTR label_text, Rect label_rect, LabelStyle* label_style)
{
	if(label_style == NULL)
		style = &defaultLabelStyle;
	else
		style = label_style;

	rect = label_rect;
	text = label_text;
}

ButtonStyle::ButtonStyle(LabelStyle *_labelStyle, char *_normalTexFile, char *_hoverTexFile, char *_activeTexFile)
{
	labelStyle = _labelStyle;
	normalTexFile = _normalTexFile;
	hoverTexFile = _hoverTexFile;
	activeTexFile = _activeTexFile;

	normalTex = NULL;
	hoverTex = NULL;
	activeTex = NULL;
}

void ButtonStyle::CreateTextures()
{
	if(normalTex)
		SAFE_RELEASE(normalTex);
	D3DXCreateTextureFromFile(D3DDevice, normalTexFile, &normalTex);
	
	if(hoverTex)
		SAFE_RELEASE(hoverTex);
	D3DXCreateTextureFromFile(D3DDevice, hoverTexFile, &hoverTex);
	
	if(activeTex)
		SAFE_RELEASE(activeTex);
	D3DXCreateTextureFromFile(D3DDevice, activeTexFile, &activeTex);
}

void Button::CreateVertexBuffer()
{
	VertexXYZRHWUV vertexData[4];
	vertexData[0] = VertexXYZRHWUV((float)rect.left, (float)rect.top, 0, 1, 0, 0);
	vertexData[1] = VertexXYZRHWUV((float)rect.right, (float)rect.top, 0, 1, 1, 0);
	vertexData[2] = VertexXYZRHWUV((float)rect.left, (float)rect.bottom, 0, 1, 0, 1);
	vertexData[3] = VertexXYZRHWUV((float)rect.right, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &vertexBuffer, vertexData, 4, XYZRHW_UV);
}

Button::Button(LPWSTR button_text, Rect button_rect, ButtonStyle *button_style)
{
	if(button_style == NULL)
		style = &defaultButtonStyle;
	else
		style = button_style;

	text = button_text;
	rect = button_rect;

	CreateVertexBuffer();
}

void Button::GetButtonState()
{
	if(Input::cursorPos.x > rect.left && Input::cursorPos.x < rect.right &&
		Input::cursorPos.y > rect.top && Input::cursorPos.y < rect.bottom)
	{
		if(Input::GetLeftButton())
			state = Active;
		else
			state = Hover;
	}
	else
		state = Normal;
}


ToggleStyle::ToggleStyle(char* _offNormalTexFile, char* _offHoverTexFile, char* _onNormalTexFile, char* _onHoverTexFile)
{
	offNormalTexFile = _offNormalTexFile;
	offHoverTexFile = _offHoverTexFile;
	onNormalTexFile = _onNormalTexFile;
	onHoverTexFile = _onHoverTexFile;

	offNormalTex = NULL;
	offHoverTex = NULL;
	onNormalTex = NULL;
	onHoverTex = NULL;
}

void ToggleStyle::CreateTextures()
{
	if(offNormalTex)
		SAFE_RELEASE(offNormalTex);
	D3DXCreateTextureFromFile(D3DDevice, offNormalTexFile, &offNormalTex);
	
	if(offHoverTex)
		SAFE_RELEASE(offHoverTex);
	D3DXCreateTextureFromFile(D3DDevice, offHoverTexFile, &offHoverTex);
	
	if(onNormalTex)
		SAFE_RELEASE(onNormalTex);
	D3DXCreateTextureFromFile(D3DDevice, onNormalTexFile, &onNormalTex);
	
	if(onHoverTex)
		SAFE_RELEASE(onHoverTex);
	D3DXCreateTextureFromFile(D3DDevice, onHoverTexFile, &onHoverTex);
}

Toggle::Toggle(Rect toggle_rect, bool _value, ToggleStyle *toggle_style)
{
	if(toggle_style == NULL)
		style = &defaultToggleStyle;
	else
		style = toggle_style;

	rect = toggle_rect;
	value = _value;

	CreateVertexBuffer();
}

void Toggle::CreateVertexBuffer()
{
	VertexXYZRHWUV vertexData[4];
	vertexData[0] = VertexXYZRHWUV((float)rect.left, (float)rect.top, 0, 1, 0, 0);
	vertexData[1] = VertexXYZRHWUV((float)rect.right, (float)rect.top, 0, 1, 1, 0);
	vertexData[2] = VertexXYZRHWUV((float)rect.left, (float)rect.bottom, 0, 1, 0, 1);
	vertexData[3] = VertexXYZRHWUV((float)rect.right, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &vertexBuffer, vertexData, 4, XYZRHW_UV);
}

void Toggle::GetButtonState()
{
	if(Input::cursorPos.x > rect.left && Input::cursorPos.x < rect.right &&
		Input::cursorPos.y > rect.top && Input::cursorPos.y < rect.bottom)
	{
		if(Input::GetLeftButton())
			state = Active;
		else
			state = Hover;
	}
	else
		state = Normal;
}


SliderStyle::SliderStyle(char *_sliderBarTexFile, char *_normalTexFile, char *_hoverTexFile, char *_activeTexFile, float _sliderBarSize)
{
	sliderBarTexFile = _sliderBarTexFile;
	normalTexFile = _normalTexFile;
	hoverTexFile = _hoverTexFile;
	activeTexFile = _activeTexFile;
	sliderBarSize = _sliderBarSize;

	sliderBarTex = NULL;
	normalTex = NULL;
	hoverTex = NULL;
	activeTex = NULL;
}

void SliderStyle::CreateTextures()
{
	if(sliderBarTex)
		SAFE_RELEASE(sliderBarTex);
	D3DXCreateTextureFromFile(D3DDevice, sliderBarTexFile, &sliderBarTex);

	if(normalTex)
		SAFE_RELEASE(normalTex);
	D3DXCreateTextureFromFile(D3DDevice, normalTexFile, &normalTex);
	
	if(hoverTex)
		SAFE_RELEASE(hoverTex);
	D3DXCreateTextureFromFile(D3DDevice, hoverTexFile, &hoverTex);
	
	if(activeTex)
		SAFE_RELEASE(activeTex);
	D3DXCreateTextureFromFile(D3DDevice, activeTexFile, &activeTex);
}

Slider::Slider(Rect slider_rect, float _minValue, float _maxValue, float _value, char* slider_id, SliderStyle *slider_style)
{
	if(slider_style == NULL)
		style = &defaultSliderStyle;
	else
		style = slider_style;

	id = slider_id;
	rect = slider_rect;
	minValue = _minValue;
	maxValue = _maxValue;
	value = _value;
	if(value < minValue)
		value = minValue;
	if(value > maxValue)
		value = maxValue;

	CreateVertexBuffer();
}

void Slider::CreateVertexBuffer()
{
	VertexXYZRHWUV barVertexData[4];
	barVertexData[0] = VertexXYZRHWUV((float)rect.left, (float)rect.top, 0, 1, 0, 0);
	barVertexData[1] = VertexXYZRHWUV((float)rect.right, (float)rect.top, 0, 1, 1, 0);
	barVertexData[2] = VertexXYZRHWUV((float)rect.left, (float)rect.bottom, 0, 1, 0, 1);
	barVertexData[3] = VertexXYZRHWUV((float)rect.right, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &sliderBarVB, barVertexData, 4, XYZRHW_UV);

	VertexXYZRHWUV thumbVertexData[4];
	float offset = ((value - minValue)/(maxValue - minValue)) * (rect.right - style->sliderBarSize*(rect.bottom - rect.top) - rect.left);
	float width = style->sliderBarSize * (rect.bottom - rect.top);
	thumbVertexData[0] = VertexXYZRHWUV(rect.left + offset, (float)rect.top, 0, 1, 0, 0);
	thumbVertexData[1] = VertexXYZRHWUV(rect.left + offset + width, (float)rect.top, 0, 1, 1, 0);
	thumbVertexData[2] = VertexXYZRHWUV(rect.left + offset, (float)rect.bottom, 0, 1, 0, 1);
	thumbVertexData[3] = VertexXYZRHWUV(rect.left + offset + width, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &sliderThumbVB, thumbVertexData, 4, XYZRHW_UV);
}

void Slider::GetButtonState()
{
	if(state == Active && Input::GetLeftButton())
		return;

	float offset = ((value - minValue)/(maxValue - minValue)) * (rect.right - style->sliderBarSize*(rect.bottom - rect.top) - rect.left);
	float width = style->sliderBarSize * (rect.bottom - rect.top);

	if(Input::cursorPos.x > rect.left && Input::cursorPos.x < rect.right &&
		Input::cursorPos.y > rect.top && Input::cursorPos.y < rect.bottom && Input::GetLeftButton())
	{
		state = Active;
		return;
	}

	if(Input::cursorPos.x > rect.left + offset && Input::cursorPos.x < rect.left + offset + width &&
		Input::cursorPos.y > rect.top && Input::cursorPos.y < rect.bottom)
	{
		state = Hover;
	}
	else
		state = Normal;
}


ListBoxStyle::ListBoxStyle(LabelStyle* _labelStyle, char* _buttonNormalTexFile, char* _buttonHoverTexFile, char* _itemNormalTexFile, char* _itemHoverTexFile)
{
	labelStyle = _labelStyle;
	buttonNormalTexFile = _buttonNormalTexFile;
	buttonHoverTexFile = _buttonHoverTexFile;
	itemNormalTexFile = _itemNormalTexFile;
	itemHoverTexFile = _itemHoverTexFile;

	buttonNormalTex = NULL;
	buttonHoverTex = NULL;
	itemNormalTex = NULL;
	itemHoverTex = NULL;
}

void ListBoxStyle::CreateTextures()
{
	if(buttonNormalTex)
		SAFE_RELEASE(buttonNormalTex);
	D3DXCreateTextureFromFile(D3DDevice, buttonNormalTexFile, &buttonNormalTex);
	
	if(buttonHoverTex)
		SAFE_RELEASE(buttonHoverTex);
	D3DXCreateTextureFromFile(D3DDevice, buttonHoverTexFile, &buttonHoverTex);
	
	if(itemNormalTex)
		SAFE_RELEASE(itemNormalTex);
	D3DXCreateTextureFromFile(D3DDevice, itemNormalTexFile, &itemNormalTex);
	
	if(itemHoverTex)
		SAFE_RELEASE(itemHoverTex);
	D3DXCreateTextureFromFile(D3DDevice, itemHoverTexFile, &itemHoverTex);
}

void ListBox::CreateBodyVB()
{
	int height= rect.bottom - rect.top;

	VertexXYZRHWUV labelVertexData[4];
	labelVertexData[0] = VertexXYZRHWUV((float)rect.left, (float)rect.top, 0, 1, 0, 0);
	labelVertexData[1] = VertexXYZRHWUV((float)rect.right - height, (float)rect.top, 0, 1, 1, 0);
	labelVertexData[2] = VertexXYZRHWUV((float)rect.left, (float)rect.bottom, 0, 1, 0, 1);
	labelVertexData[3] = VertexXYZRHWUV((float)rect.right - height, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &labelVB, labelVertexData, 4, XYZRHW_UV);
	
	VertexXYZRHWUV buttonVertexData[4];
	buttonVertexData[0] = VertexXYZRHWUV((float)rect.right - height, (float)rect.top, 0, 1, 0, 0);
	buttonVertexData[1] = VertexXYZRHWUV((float)rect.right, (float)rect.top, 0, 1, 1, 0);
	buttonVertexData[2] = VertexXYZRHWUV((float)rect.right - height, (float)rect.bottom, 0, 1, 0, 1);
	buttonVertexData[3] = VertexXYZRHWUV((float)rect.right, (float)rect.bottom, 0, 1, 1, 1);

	CreateVB(D3DDevice, &buttonVB, buttonVertexData, 4, XYZRHW_UV);
}

void ListBox::CreateItemVB(int itemIx)
{
	VertexXYZRHWUV vertexData[4];
	int height= rect.bottom - rect.top;
	vertexData[0] = VertexXYZRHWUV((float)rect.left, (float)rect.top + (itemIx+1)*height, 0, 1, 0, 0);
	vertexData[1] = VertexXYZRHWUV((float)rect.right, (float)rect.top + (itemIx+1)*height, 0, 1, 1, 0);
	vertexData[2] = VertexXYZRHWUV((float)rect.left, (float)rect.bottom + (itemIx+1)*height, 0, 1, 0, 1);
	vertexData[3] = VertexXYZRHWUV((float)rect.right, (float)rect.bottom + (itemIx+1)*height, 0, 1, 1, 1);

	CreateVB(D3DDevice, &(items[itemIx].vertexBuffer), vertexData, 4, XYZRHW_UV);
}

ListBox::ListBox(vector<LPWSTR> _texts, Rect _rect, int _ix, char* _id, ListBoxStyle* listBox_style)
{
	if(listBox_style == NULL)
		style = &defaultListBoxStyle;
	else
		style = listBox_style;

	rect = _rect;
	selectedIx = _ix;
	id = _id;
	isSelecting = false;
	cursorState = 0;
	CreateBodyVB();

	for(vector<LPWSTR>::iterator iter = _texts.begin(); iter != _texts.end(); ++iter){
		AddItem((*iter));
	}
}

void ListBox::AddItem(LPWSTR _text)
{
	ListBoxItem item;
	item.text = _text;
	item.ix = items.size();

	items.push_back(item);
	CreateItemVB(item.ix);
}

void ListBox::GetState()
{
	int height = rect.bottom - rect.top;

	if(Input::cursorPos.x < rect.left || Input::cursorPos.x > rect.right ||
		Input::cursorPos.y < rect.top || Input::cursorPos.y > rect.bottom + items.size()*height)
	{
		cursorState = 0;
		return;
	}

	cursorState = (int)(Input::cursorPos.y - rect.top) / height + 1;

	if(cursorState == 1){
		if(Input::GetLeftButtonUp())
			isSelecting = !isSelecting;
	}
	else{
		if(Input::GetLeftButtonUp()){
			selectedIx = cursorState - 2;
			isSelecting = false;
		}
	}
}


ColorPicker::ColorPicker(Rect _rect, D3DXCOLOR _rgb, char* _id)
{
	rect = _rect;
	rgb = _rgb;
	hsl = RGB2HSL(rgb);
	SLVB = NULL;
	id = _id;
	isPickingColor = false;
	
	CreateVertexBuffer();
}

void ColorPicker::CreateVertexBuffer()
{
	D3DCOLOR color32 = D3DCOLOR_XRGB((int)(255*rgb.r), (int)(255*rgb.g), (int)(255*rgb.b));
	VertexXYZRHWD colorVertexData[4];
	colorVertexData[0] = VertexXYZRHWD((float)rect.left, (float)rect.top, 0, 1, color32);
	colorVertexData[1] = VertexXYZRHWD((float)rect.right, (float)rect.top, 0, 1, color32);
	colorVertexData[2] = VertexXYZRHWD((float)rect.left, (float)rect.bottom, 0, 1, color32);
	colorVertexData[3] = VertexXYZRHWD((float)rect.right, (float)rect.bottom, 0, 1, color32);

	CreateVB(D3DDevice, &colorVB, colorVertexData, 4, XYZRHW_D);

	VertexXYZRHWUV SLVertexData[4];
	SLVertexData[0] = VertexXYZRHWUV((float)rect.right + 10.0f, (float)rect.top, 0, 1, 0, 0);
	SLVertexData[1] = VertexXYZRHWUV((float)rect.right + 110.0f, (float)rect.top, 0, 1, 1, 0);
	SLVertexData[2] = VertexXYZRHWUV((float)rect.right + 10.0f, (float)rect.top + 100.0f, 0, 1, 0, 1);
	SLVertexData[3] = VertexXYZRHWUV((float)rect.right + 110.0f, (float)rect.top + 100.0f, 0, 1, 1, 1);

	CreateVB(D3DDevice, &SLVB, SLVertexData, 4, XYZRHW_UV);
	
	VertexXYZRHWUV hueVertexData[4];
	hueVertexData[0] = VertexXYZRHWUV((float)rect.right + 115.0f, (float)rect.top, 0, 1, 0, 0);
	hueVertexData[1] = VertexXYZRHWUV((float)rect.right + 125.0f, (float)rect.top, 0, 1, 1, 0);
	hueVertexData[2] = VertexXYZRHWUV((float)rect.right + 115.0f, (float)rect.top + 100.0f, 0, 1, 0, 1);
	hueVertexData[3] = VertexXYZRHWUV((float)rect.right + 125.0f, (float)rect.top + 100.0f, 0, 1, 1, 1);

	CreateVB(D3DDevice, &hueVB, hueVertexData, 4, XYZRHW_UV);
}


void GUI::AddLabel(Label *label)
{
	labels.push_back(label);
}

void GUI::AddButton(Button* button)
{
	buttons.push_back(button);
}

void GUI::AddSlider(Slider* slider)
{
	sliders.push_back(slider);
}

void GUI::AddToggle(Toggle* toggle)
{
	toggles.push_back(toggle);
}

void GUI::AddListBox(ListBox* listBox)
{
	listBoxes.push_back(listBox);
}

void GUI::AddColorPicker(ColorPicker* colorPicker)
{
	colorPickers.push_back(colorPicker);
}

void GUI::clear()
{
	for(list<Label*>::iterator iter = labels.begin(); iter != labels.end(); ++iter){
		free((*iter)->text);
		delete (*iter);
	}
	labels.clear();

	for(list<Button*>::iterator iter = buttons.begin(); iter != buttons.end(); ++iter){
		free((*iter)->text);
		SAFE_RELEASE((*iter)->vertexBuffer);
		delete (*iter);
	}
	buttons.clear();
	
	for(list<Toggle*>::iterator iter = toggles.begin(); iter != toggles.end(); ++iter){
		SAFE_RELEASE((*iter)->vertexBuffer);
		delete (*iter);
	}
	toggles.clear();

	for(list<Slider*>::iterator iter = sliders.begin(); iter != sliders.end(); ++iter){
		SAFE_RELEASE((*iter)->sliderBarVB);
		SAFE_RELEASE((*iter)->sliderThumbVB);
		delete (*iter);
	}
	sliders.clear();
	
	for(list<ListBox*>::iterator iter = listBoxes.begin(); iter != listBoxes.end(); ++iter){
		SAFE_RELEASE((*iter)->buttonVB);
		SAFE_RELEASE((*iter)->labelVB);
		for(vector<ListBoxItem>::iterator i = (*iter)->items.begin(); i != (*iter)->items.end(); ++i){
			free((*i).text);
			SAFE_RELEASE((*i).vertexBuffer);
		}
		delete (*iter);
	}
	listBoxes.clear();
	
	for(list<ColorPicker*>::iterator iter = colorPickers.begin(); iter != colorPickers.end(); ++iter){
		SAFE_RELEASE((*iter)->SLVB);
		SAFE_RELEASE((*iter)->hueVB);
		SAFE_RELEASE((*iter)->colorVB);
		delete (*iter);
	}
	colorPickers.clear();
}

void GUI::GUILabel(char* label_text, Rect label_rect, LabelStyle* label_style)
{
	Label* label = new Label(str2wstr(label_text), label_rect, label_style);
	AddLabel(label);
}

bool GUI::GUIButton(char* button_text, Rect button_rect, ButtonStyle *button_style)
{
	Button* button = new Button(str2wstr(button_text), button_rect, button_style);
	AddButton(button);

	button->GetButtonState();
	if(Input::GetLeftButtonUp() && button->state == Hover)
		return true;
	return false;
}

bool GUI::GUIToggle(Rect toggle_rect, bool _value, ToggleStyle *toggle_style)
{
	Toggle* toggle = new Toggle(toggle_rect, _value, toggle_style);
	AddToggle(toggle);

	toggle->GetButtonState();
	if(toggle->state != Normal && Input::GetLeftButtonUp())
		toggle->value = !toggle->value;

	return toggle->value;
}

float GUI::GUISlider(Rect slider_rect, float _minValue, float _maxValue, float _value, char* slider_id, SliderStyle* slider_style)
{ 
	Slider* slider = new Slider(slider_rect, _minValue, _maxValue, _value, slider_id, slider_style);
	AddSlider(slider);

	if(sliderState.count(slider_id))
		slider->state = sliderState[slider_id];
	else
		slider->state = Normal;

	slider->GetButtonState();
	sliderState[slider_id] = slider->state;

	if(slider->state == Active){
		float width = slider->style->sliderBarSize*(slider->rect.bottom - slider->rect.top);
		slider->value = slider->minValue + (slider->maxValue - slider->minValue)*(Input::cursorPos.x - width/2 - slider->rect.left)/
			(slider->rect.right - slider->rect.left - width);

		if(slider->value < slider->minValue)
			slider->value = slider->minValue;
		if(slider->value > slider->maxValue)
			slider->value = slider->maxValue;
	}
	return slider->value;
}

int GUI::GUIListBox(vector<LPWSTR> _texts, Rect _rect, int _ix, char* _id, ListBoxStyle* listBox_style)
{
	ListBox* listBox = new ListBox(_texts, _rect, _ix, _id, listBox_style);
	AddListBox(listBox);

	if(listBoxState.count(_id))
		listBox->isSelecting = listBoxState[_id];
	else
		listBox->isSelecting = false;

	listBox->GetState();
	listBoxState[_id] = listBox->isSelecting;
	
	return listBox->selectedIx;
}

D3DXCOLOR GUI::GUIColor(Rect _rect, D3DXCOLOR _rgb, char* _id)
{
	ColorPicker* colorPicker = new ColorPicker(_rect, _rgb, _id);
	AddColorPicker(colorPicker);

	if(colorPickerState.count(_id))
		colorPicker->isPickingColor = colorPickerState[_id];
	else
		colorPicker->isPickingColor = false;

	if(Input::cursorPos.x > colorPicker->rect.left && Input::cursorPos.x < colorPicker->rect.right &&
		Input::cursorPos.y > colorPicker->rect.top && Input::cursorPos.y < colorPicker->rect.bottom && Input::GetLeftButtonUp())
	{
		colorPicker->isPickingColor = !colorPicker->isPickingColor;
		colorPickerState[_id] = colorPicker->isPickingColor;

		return _rgb;
	}

	if(colorPicker->isPickingColor){
		if(Input::cursorPos.x > colorPicker->rect.right + 10 && Input::cursorPos.x < colorPicker->rect.right + 110 &&
			Input::cursorPos.y > colorPicker->rect.top && Input::cursorPos.y < colorPicker->rect.top + 100 && Input::GetLeftButton() && colorPicker->isPickingColor)
		{
			float s = (Input::cursorPos.x - (colorPicker->rect.right + 10.0f)) / 100.0f;
			float l = (colorPicker->rect.top + 100.0f - Input::cursorPos.y) / 100.0f;

			return HSL2RGB(Vector3(colorPicker->hsl.x, s, l));
		}
		else if(Input::cursorPos.x > colorPicker->rect.right + 115 && Input::cursorPos.x < colorPicker->rect.right + 125 &&
			Input::cursorPos.y > colorPicker->rect.top && Input::cursorPos.y < colorPicker->rect.top + 100 && Input::GetLeftButton() && colorPicker->isPickingColor)
		{
			float h = (colorPicker->rect.top + 100.0f - Input::cursorPos.y) / 100.0f;

			return HSL2RGB(Vector3(h, colorPicker->hsl.y, colorPicker->hsl.z));
		}
	}

	return _rgb;
}

void GUI::Draw()
{
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	D3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	D3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	D3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	D3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	for(list<Label*>::iterator iter = labels.begin(); iter != labels.end(); ++iter){
		RECT rect;
		rect.left = (*iter)->rect.left;
		rect.top = (*iter)->rect.top;
		rect.right = (*iter)->rect.right;
		rect.bottom = (*iter)->rect.bottom;

		(*iter)->style->font->DrawTextW(NULL, (*iter)->text, -1, &rect, (*iter)->style->alignFlag, (*iter)->style->color);
	}

	for(list<Button*>::iterator iter = buttons.begin(); iter != buttons.end(); ++iter){
		D3DDevice->SetStreamSource(0, (*iter)->vertexBuffer, 0, sizeof(VertexXYZRHWUV));
		D3DDevice->SetFVF(VertexXYZRHWUV::FVF);

		switch((*iter)->state){
			case Normal:
				D3DDevice->SetTexture(0, (*iter)->style->normalTex);
				break;
			case Hover:
				D3DDevice->SetTexture(0, (*iter)->style->hoverTex);
				break;
			case Active:
				D3DDevice->SetTexture(0, (*iter)->style->activeTex);
				break;
		}

		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		RECT rect;
		rect.left = (*iter)->rect.left;
		rect.top = (*iter)->rect.top;
		rect.right = (*iter)->rect.right;
		rect.bottom = (*iter)->rect.bottom;

		(*iter)->style->labelStyle->font->DrawTextW(NULL, (*iter)->text, -1, &rect, (*iter)->style->labelStyle->alignFlag, (*iter)->style->labelStyle->color);
	}

	for(list<Toggle*>::iterator iter = toggles.begin(); iter != toggles.end(); ++iter){
		D3DDevice->SetStreamSource(0, (*iter)->vertexBuffer, 0, sizeof(VertexXYZRHWUV));
		D3DDevice->SetFVF(VertexXYZRHWUV::FVF);

		if((*iter)->value == false){
			if((*iter)->state == Normal)
				D3DDevice->SetTexture(0, (*iter)->style->offNormalTex);
			else
				D3DDevice->SetTexture(0, (*iter)->style->offHoverTex);
		}
		else{
			if((*iter)->state == Normal)
				D3DDevice->SetTexture(0, (*iter)->style->onNormalTex);
			else
				D3DDevice->SetTexture(0, (*iter)->style->onHoverTex);
		}

		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	for(list<Slider*>::iterator iter = sliders.begin(); iter != sliders.end(); ++iter){
		D3DDevice->SetFVF(VertexXYZRHWUV::FVF);

		D3DDevice->SetStreamSource(0, (*iter)->sliderBarVB, 0, sizeof(VertexXYZRHWUV));
		D3DDevice->SetTexture(0, (*iter)->style->sliderBarTex);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		D3DDevice->SetStreamSource(0, (*iter)->sliderThumbVB, 0, sizeof(VertexXYZRHWUV));
		switch((*iter)->state){
			case Normal:
				D3DDevice->SetTexture(0, (*iter)->style->normalTex);
				break;
			case Hover:
				D3DDevice->SetTexture(0, (*iter)->style->hoverTex);
				break;
			case Active:
				D3DDevice->SetTexture(0, (*iter)->style->activeTex);
				break;
		}
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	
	for(list<ListBox*>::iterator iter = listBoxes.begin(); iter != listBoxes.end(); ++iter){
		D3DDevice->SetStreamSource(0, (*iter)->labelVB, 0, sizeof(VertexXYZRHWUV));
		D3DDevice->SetFVF(VertexXYZRHWUV::FVF);

		if((*iter)->cursorState == 1)
			D3DDevice->SetTexture(0, (*iter)->style->itemHoverTex);
		else
			D3DDevice->SetTexture(0, (*iter)->style->itemNormalTex);

		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		
		D3DDevice->SetStreamSource(0, (*iter)->buttonVB, 0, sizeof(VertexXYZRHWUV));
		if((*iter)->cursorState == 1)
			D3DDevice->SetTexture(0, (*iter)->style->buttonHoverTex);
		else
			D3DDevice->SetTexture(0, (*iter)->style->buttonNormalTex);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		RECT rect;
		rect.left = (*iter)->rect.left;
		rect.top = (*iter)->rect.top;
		rect.right = (*iter)->rect.right - ((*iter)->rect.bottom - (*iter)->rect.top);
		rect.bottom = (*iter)->rect.bottom;

		(*iter)->style->labelStyle->font->DrawTextW(NULL, (*iter)->items[(*iter)->selectedIx].text, -1, &rect, (*iter)->style->labelStyle->alignFlag, (*iter)->style->labelStyle->color);

		if((*iter)->isSelecting){
			for(int i = 0; i < (*iter)->items.size(); ++i){
				D3DDevice->SetStreamSource(0, (*iter)->items[i].vertexBuffer, 0, sizeof(VertexXYZRHWUV));

				if((*iter)->cursorState == (*iter)->items[i].ix + 2)
					D3DDevice->SetTexture(0, (*iter)->style->itemHoverTex);
				else
					D3DDevice->SetTexture(0, (*iter)->style->itemNormalTex);

				D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				
				RECT rect;
				int height = (*iter)->rect.bottom - (*iter)->rect.top;
				rect.left = (*iter)->rect.left;
				rect.top = (*iter)->rect.top + (i+1)*height;
				rect.right = (*iter)->rect.right;
				rect.bottom = (*iter)->rect.bottom + (i+1)*height;

				(*iter)->style->labelStyle->font->DrawTextW(NULL, (*iter)->items[i].text, -1, &rect, (*iter)->style->labelStyle->alignFlag, (*iter)->style->labelStyle->color);
			}
		}
	}
	
	for(list<ColorPicker*>::iterator iter = colorPickers.begin(); iter != colorPickers.end(); ++iter){

		// color
		D3DDevice->SetFVF(VertexXYZRHWD::FVF);
		D3DDevice->SetStreamSource(0, (*iter)->colorVB, 0, sizeof(VertexXYZRHWD));
		D3DDevice->SetTexture(0, NULL);
		D3DDevice->SetRenderState(D3DRS_LIGHTING, false);
		D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		if((*iter)->isPickingColor){
			// SL
			D3DDevice->SetFVF(VertexXYZRHWUV::FVF);
			D3DDevice->SetStreamSource(0, (*iter)->SLVB, 0, sizeof(VertexXYZRHWUV));
			colorPickerSLShader.effect->SetTechnique("ColorPickerSL");
			colorPickerSLShader.effect->SetFloat("hue", (*iter)->hsl.x);
			
			colorPickerSLShader.effect->Begin(0, 0);
			colorPickerSLShader.effect->BeginPass(0);

			D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			colorPickerSLShader.effect->EndPass();
			colorPickerSLShader.effect->End();

			// hue
			D3DDevice->SetStreamSource(0, (*iter)->hueVB, 0, sizeof(VertexXYZRHWUV));
			colorPickerHueShader.effect->SetTechnique("ColorPickerHue");
			
			colorPickerHueShader.effect->Begin(0, 0);
			colorPickerHueShader.effect->BeginPass(0);

			D3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

			colorPickerHueShader.effect->EndPass();
			colorPickerHueShader.effect->End();
		}
	}
	
	D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}