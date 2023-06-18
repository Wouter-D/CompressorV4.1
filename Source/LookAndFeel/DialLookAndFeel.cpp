/*
  ==============================================================================

    DialLookAndFeel.cpp
    Created: 16 Jun 2023 7:22:40pm
    Author:  Wouter Duprez

  ==============================================================================
*/

//Courtesy of the AudioPluginCodersGuild

#include "DialLookAndFeel.h"

// Constructor for the DialStyle class
DialLookAndFeel::DialLookAndFeel()
{
}

// Override the drawRotarySlider function to provide custom drawing for rotary sliders
void DialLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    /** Define color variables for customization. */
    //const auto outlineColor = juce::Colour::fromRGB(95, 95, 85).darker(0.5f);
    //const auto fillColor = juce::Colour::fromRGB(125, 154, 208);
    //const auto mainColor = juce::Colour::fromRGB(125, 154, 208).darker(0.8f);
    //const auto brighterColor = juce::Colour::fromRGB(202, 222, 220).withAlpha(0.2f);
    //const auto trackColor = juce::Colour::fromRGB(125, 154, 208).withAlpha(0.6f);
    //const auto dialOutlineColor = juce::Colour::fromRGB(125, 154, 208).darker(0.5f);

    //  Theme 1
    const juce::Colour outlineColor = juce::Colour::fromRGB(202, 222, 220).darker(0.9f);
    const juce::Colour fillColor = juce::Colour::fromRGB(202, 222, 220).brighter(0.6f);
    const juce::Colour mainColor = juce::Colour::fromRGB(202, 222, 220).darker(0.3f);
    const juce::Colour brighterColor = juce::Colour::fromRGB(202, 222, 220).withAlpha(0.2f);
    const juce::Colour trackColor = juce::Colour::fromRGB(202, 222, 220).withAlpha(0.6f);
    const juce::Colour dialOutlineColor = juce::Colour::fromRGB(202, 222, 220).brighter(0.2f);

    ////  Theme 2
    //const juce::Colour outlineColor = juce::Colour::fromRGB(107, 110, 33).darker(0.9f);
    //const juce::Colour fillColor = juce::Colour::fromRGB(107, 110, 33).brighter(0.6f);
    //const juce::Colour mainColor = juce::Colour::fromRGB(107, 110, 33).darker(0.3f);
    //const juce::Colour brighterColor = juce::Colour::fromRGB(107, 110, 33).withAlpha(0.2f);
    //const juce::Colour trackColor = juce::Colour::fromRGB(107, 110, 33).withAlpha(0.6f);
    //const juce::Colour dialOutlineColor = juce::Colour::fromRGB(107, 110, 33).brighter(0.2f);

    // Convert the bounds to float
    juce::Rectangle<float> dialBounds(x, y, width, height);
    // Calculate the center of the dial
    juce::Point<float> centre = dialBounds.getCentre();
    // Calculate the full radius of the dial
    float fullRadius = juce::jmin(dialBounds.getWidth() / 1.95f, dialBounds.getHeight() / 1.95f);

    m_sliderWidth = width;

    /** Dot color*/
    g.setColour(juce::Colour::fromRGB(202, 222, 220).darker(0.9f));
    centre = dialBounds.getCentre();

    /** Draw dots */
    /** How many dots to draw, works well as num dial intervals + 1 for small ranges, e.g. [0 - 10]*/
    for (int i = 0; i < 11; ++i)
    {
        auto dotSize = width * 0.025;

       // IF you change the number of dots, do i / (num dots - 1)
       // const auto angle = juce::jmap(i / 10.0f, rotaryStartAngle, rotaryEndAngle);
        const float angle = juce::jmap(i / 2.0f, rotaryStartAngle, rotaryEndAngle);

        // Dot distance from slider center
        const auto point = centre.getPointOnCircumference(fullRadius - width * 0.06f, angle);

        // Draw a dot at the calculated position
        g.fillEllipse(point.getX() - 3, point.getY() - 3, dotSize, dotSize);
    }

    fullRadius -= 10.0f;

    // Calculate the angle for the current slider position
    float toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // Calculate the line width based on the slider width
    float lineWidthMultiplier = width * 0.045;
    float lineWidth = juce::jmin(lineWidthMultiplier, fullRadius * 0.5f);
    // Calculate the radius of the arc track
    float arcRadius = fullRadius - lineWidth * 2.25;

    // Create a path for the background arc track
    juce::Path backgroundArc;
    backgroundArc.addCentredArc
    (
        dialBounds.getCentreX(),
        dialBounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        rotaryEndAngle,
        true
    );

    // Draw the background arc track
    g.setColour(outlineColor);
    g.strokePath(backgroundArc, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Calculate the dial radius
    auto dialRadius = std::max(fullRadius - 4.0f * lineWidth, 10.0f);
    {
        // Draw the dial center
        juce::Graphics::ScopedSaveState saved(g);
        if (slider.isEnabled())
        {
            // Create a color gradient for the dial center
            juce::ColourGradient fillGradient
            (
                brighterColor,
                centre.getX() + lineWidth * 0.2f,
                centre.getY() - lineWidth * 2.0f,
                mainColor,
                centre.getX() + dialRadius,
                centre.getY() + dialRadius,
                true
            );

            // Set the color gradient as the fill color
            g.setGradientFill(fillGradient);
        }

        // Draw the dial center ellipse
        g.fillEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * 2.0f, dialRadius * 2.0f);
    }

    //dialRadius = std:: max (dialRadius - 4.0f, 10.0f);

    // Set the color for the dial outline
    g.setColour(dialOutlineColor);

    auto scale = 2.0f;

    // Draw the dial outline
    g.drawEllipse(centre.getX() - dialRadius, centre.getY() - dialRadius, dialRadius * scale, dialRadius * scale, 3.5f);

    // Create a path for the value track of the dial
    juce::Path dialValueTrack;
    dialValueTrack.addCentredArc
    (
        dialBounds.getCentreX(),
        dialBounds.getCentreY(),
        arcRadius,
        arcRadius,
        0.0f,
        rotaryStartAngle,
        toAngle,
        true
    );

    // Draw the value track of the dial
    g.setColour(fillColor);
    g.strokePath(dialValueTrack, juce::PathStrokeType(lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    // Set the color for the dial tick
    g.setColour(dialOutlineColor);

    // Create a path for the dial tick
    juce::Path dialTick;
    dialTick.startNewSubPath(centre.getPointOnCircumference(dialRadius - lineWidth, toAngle));

    // Calculate the length of the dial tick
    dialTick.lineTo(centre.getPointOnCircumference((dialRadius - lineWidth) * .75f, toAngle));

    // Draw the dial tick
    g.strokePath(dialTick, juce::PathStrokeType(lineWidth * 0.5, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    //float diameter = fmin(width, height) * .7;
    //float radius = diameter * 0.5;
    //float centerX = x + width * 0.5;
    //float centerY = y + height * 0.5;
    //float rx = centerX - radius;
    //float ry = centerY - radius;
    //float angle = rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

    //juce::Rectangle<float> dialArea(rx, ry, diameter, diameter);
    //// g.fillAll(juce::Colours::antiquewhite);
    //g.setColour(juce::Colour::fromRGB(125, 154, 208)); //center
    //g.fillEllipse(dialArea);

    //g.setColour(juce::Colours::skyblue.withAlpha(0.8f).darker(0.8f)); //outline
    //g.drawEllipse(rx, ry, diameter, diameter, 10.f);

    //g.setColour(juce::Colours::whitesmoke.darker(0.5)); //tick color
    //dialTick.addRectangle(0, -radius + 6, 8.f, radius * 0.5);
    //g.fillPath(dialTick, juce::AffineTransform::rotation(angle).translated(centerX, centerY));

}

void DialLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    // Fill the graphics context with the background color of the label
    g.fillAll(label.findColour(juce::Label::backgroundColourId));

    // Set the label as editable
    label.setEditable(true);
    //label.setEditable(false);

    // Check if the label is not being edited
    if (!label.isBeingEdited())
    {
        // Calculate the alpha value based on the label's enabled state
        float alpha = label.isEnabled() ? 1.0f : 0.5f;

        // Create a font with the specified typeface, size, and style flags
        const juce::Font font(juce::LookAndFeel::getTypefaceForFont(
            juce::Font("Helvetica",
                m_sliderWidth*0.4,
                juce::Font::FontStyleFlags::bold)));

        // Set the text color with the multiplied alpha value
        g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));

        // Set the font for drawing the text
        g.setFont(font);

        // Calculate the area for drawing the text by subtracting the label's border size from its local bounds
        juce::Rectangle<int> textArea = getLabelBorderSize(label).subtractedFrom(label.getLocalBounds());

        // Draw the text within the calculated text area, using the label's justification type, height, and minimum horizontal scale
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
            juce::jmax(1, (int)((float)textArea.getHeight() / font.getHeight())),
            label.getMinimumHorizontalScale());

        // Set the outline color with the multiplied alpha value
        g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    }

    // If the label is being edited and enabled, set the outline color without modifying the alpha value
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }

    // Draw a rectangle around the label's local bounds
    g.drawRect(label.getLocalBounds());
}
