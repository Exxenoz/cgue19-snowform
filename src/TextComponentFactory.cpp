#include "TextComponentFactory.h"
#include "TextComponent.h"
#include "AssetManager.h"
#include "Material.h"
#include "Font.h"
#include "StringUtils.h"
#include "GameObject.h"
#include "tinyxml2.h"

TextComponentFactory::TextComponentFactory() :
    ComponentFactory::ComponentFactory()
{
}

void TextComponentFactory::Build(GameObject* gameObject, tinyxml2::XMLElement* element)
{
    TextComponent* textComponent = gameObject->AttachComponent<TextComponent>();

    if (element->Attribute("material"))
    {
        std::string materialName = std::string(element->Attribute("material"));

        if (Material* material = sAssetManager.GetMaterial(materialName))
        {
            textComponent->SetMaterial(material);
        }
        else
        {
            fprintf(stderr, "Could not find material '%s' for text component of game object '%s'!\n", materialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("font"))
    {
        std::string fontName = std::string(element->Attribute("font"));

        if (Font* font = sAssetManager.GetFont(fontName))
        {
            textComponent->SetFont(font);
        }
        else
        {
            fprintf(stderr, "Could not find font '%s' for text component of game object '%s'!\n", fontName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("fontSize"))
    {
        std::string fontSizeValue = std::string(element->Attribute("fontSize"));
        textComponent->SetFontSize(std::strtof(fontSizeValue.c_str(), 0));
    }

    if (element->Attribute("text"))
    {
        std::string textValue = std::string(element->Attribute("text"));
        textComponent->SetText(textValue);
    }

    if (element->Attribute("textAlignment"))
    {
        std::string textValue = std::string(element->Attribute("textAlignment"));

        if (textValue == "TopLeft")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_TOP_LEFT);
        }
        else if (textValue == "TopCenter")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_TOP_CENTER);
        }
        else if (textValue == "TopRight")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_TOP_RIGHT);
        }
        else if (textValue == "CenterLeft")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_CENTER_LEFT);
        }
        else if (textValue == "CenterCenter")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_CENTER_CENTER);
        }
        else if (textValue == "CenterRight")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_CENTER_RIGHT);
        }
        else if (textValue == "BottomLeft")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_BOTTOM_LEFT);
        }
        else if (textValue == "BottomCenter")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_BOTTOM_CENTER);
        }
        else if (textValue == "BottomRight")
        {
            textComponent->SetTextAlignment(TEXT_ALIGNMENT_BOTTOM_RIGHT);
        }
    }

    if (element->Attribute("textOverflowMode"))
    {
        std::string textValue = std::string(element->Attribute("textOverflowMode"));

        if (textValue == "Overflow")
        {
            textComponent->SetTextOverflowMode(TEXT_OVERFLOW_MODE_OVERFLOW);
        }
        else if (textValue == "Ellipsis")
        {
            textComponent->SetTextOverflowMode(TEXT_OVERFLOW_MODE_ELLIPSIS);
        }
        else if (textValue == "Truncate")
        {
            textComponent->SetTextOverflowMode(TEXT_OVERFLOW_MODE_TRUNCATE);
        }
        else if (textValue == "OverflowH")
        {
            textComponent->SetTextOverflowMode(TEXT_OVERFLOW_MODE_OVERFLOW_H);
        }
        else if (textValue == "OverflowV")
        {
            textComponent->SetTextOverflowMode(TEXT_OVERFLOW_MODE_OVERFLOW_V);
        }
    }

    if (element->Attribute("wordWrapping"))
    {
        textComponent->SetWordWrapping(std::string(element->Attribute("wordWrapping")) == "true");
    }

    if (element->Attribute("maxWidth"))
    {
        std::string maxWidthValue = std::string(element->Attribute("maxWidth"));
        textComponent->SetMaxWidth(std::strtof(maxWidthValue.c_str(), 0));
    }

    if (element->Attribute("maxHeight"))
    {
        std::string maxHeightValue = std::string(element->Attribute("maxHeight"));
        textComponent->SetMaxHeight(std::strtof(maxHeightValue.c_str(), 0));
    }

    if (element->Attribute("enabled"))
    {
        textComponent->SetEnabled(std::string(element->Attribute("enabled")) == "true");
    }

    if (element->Attribute("outline"))
    {
        textComponent->SetOutline(std::string(element->Attribute("outline")) == "true");
    }

    if (element->Attribute("outlineMaterial"))
    {
        std::string outlineMaterialName = std::string(element->Attribute("outlineMaterial"));

        if (Material* outlineMaterial = sAssetManager.GetMaterial(outlineMaterialName))
        {
            textComponent->SetOutlineMaterial(outlineMaterial);
        }
        else
        {
            fprintf(stderr, "Could not find outline material '%s' for text component of game object '%s'!\n", outlineMaterialName.c_str(), gameObject->GetName().c_str());
        }
    }

    if (element->Attribute("outlineThickness"))
    {
        std::string outlineThicknessValue = std::string(element->Attribute("outlineThickness"));
        textComponent->SetOutlineThickness(std::strtof(outlineThicknessValue.c_str(), 0));
    }

    if (element->Attribute("outlineOffset"))
    {
        std::string offsetString = std::string(element->Attribute("outlineOffset"));
        std::vector<std::string> offsetStringSplit = split(offsetString, ' ');

        glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f);

        switch (offsetStringSplit.size())
        {
            case 3:
                offset.z = (float)::atof(offsetStringSplit[2].c_str());
                // No break
            case 2:
                offset.y = (float)::atof(offsetStringSplit[1].c_str());
                // No break
            case 1:
                offset.x = (float)::atof(offsetStringSplit[0].c_str());
                break;
            default:
                fprintf(stderr, "Could not parse outline offset for text component of game object '%s'!\n", gameObject->GetName().c_str());
                break;
        }

        textComponent->SetOutlineOffset(offset);
    }
}
