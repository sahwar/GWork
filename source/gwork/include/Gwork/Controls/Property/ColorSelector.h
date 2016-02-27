#pragma once
#ifndef GWEN_CONTROLS_PROPERTY_COLORSELECTOR_H
#define GWEN_CONTROLS_PROPERTY_COLORSELECTOR_H

#include "Gwen/Controls/Properties.h"
#include "Gwen/Controls/Menu.h"
#include "Gwen/Controls/HSVColorPicker.h"

#include <stdio.h>

namespace Gwen
{
    namespace Controls
    {
        namespace Internal
        {
            class ColourButton : public Button
            {
                GWEN_CONTROL_INLINE(ColourButton, Button)
                {
                    m_Color = Colors::Black;
                    SetText("");
                }

                void Render(Skin::Base* skin) override
                {
                    skin->GetRender()->SetDrawColor(m_Color);
                    skin->GetRender()->DrawFilledRect(GetRenderBounds());
                }

                void SetColor(const Gwen::Color& col)
                {
                    m_Color = col;
                }

                Gwen::Color m_Color;
            };


        }

        namespace Property
        {
            class ColorSelector : public Property::Text
            {
                GWEN_CONTROL_INLINE(ColorSelector, Property::Text)
                {
                    m_Button = new Controls::Internal::ColourButton(m_TextBox);
                    m_Button->Dock(Docking::Right);
                    m_Button->SetWidth(20);
                    m_Button->onPress.Add(this, &ThisClass::OnButtonPress);
                    m_Button->SetMargin(Margin(1, 1, 1, 2));
                }

                void OnButtonPress(Controls::Base* control)
                {
                    Gwen::Controls::Menu* pMenu = new Menu(GetCanvas());
                    pMenu->SetSize(256, 180);
                    pMenu->SetDeleteOnClose(true);
                    pMenu->SetDisableIconMargin(true);
                    
                    Gwen::Controls::HSVColorPicker* picker = new Gwen::Controls::HSVColorPicker(pMenu);
                    picker->Dock(Docking::Fill);
                    picker->SetSize(256, 128);
                    float defaultColor[3];
                    Gwen::Utility::Strings::To::Floats(m_TextBox->GetText(), defaultColor, 3);
                    picker->SetColor(Gwen::Color(defaultColor[0], defaultColor[1], defaultColor[2], 255),
                                     false, true);
                    picker->onColorChanged.Add(this, &ThisClass::ColorChanged);
                    
                    pMenu->Open(Docking::Right|Docking::Top);
                }

                void ColorChanged(Controls::Base* control)
                {
                    Gwen::Controls::HSVColorPicker* picker =
                        gwen_cast<Gwen::Controls::HSVColorPicker>(control);
                    Color col = picker->GetColor();
                    m_TextBox->SetText(Utility::Format("%u,%u,%u", col.r, col.g, col.b));
                    DoChanged();
                }

                virtual String GetPropertyValue() override
                {
                    return m_TextBox->GetText();
                }

                virtual void SetPropertyValue(const String& v, bool bFireChangeEvents) override
                {
                    m_TextBox->SetText(v, bFireChangeEvents);
                }

                virtual bool IsEditing() override
                {
                    return m_TextBox == Gwen::KeyboardFocus;
                }

                virtual void DoChanged() override
                {
                    ParentClass::DoChanged();
                    unsigned int r,g,b;
#ifdef _WIN32
                    if (sscanf_s(m_TextBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#else
                    if (sscanf(m_TextBox->GetText().c_str(), "%u,%u,%u", &r, &g, &b) == 3)
#endif
                    {
                        m_Button->SetColor(Gwen::Color(r,g,b));
                    }
                }

                Controls::Internal::ColourButton*       m_Button;
            };


        }
    }
}
#endif // ifndef GWEN_CONTROLS_PROPERTY_COLORSELECTOR_H