// Copyright (C) 2016 Elviss Strazdins
// This file is part of the Ouzel engine.

#include "PerspectiveSample.h"
#include "MainMenu.h"

using namespace std;
using namespace ouzel;

PerspectiveSample::PerspectiveSample(Samples& aSamples):
    samples(aSamples),
    backButton("button.png", "button_selected.png", "button_down.png", "", "Back", Color::BLACK, "arial.fnt")
{
    eventHandler.keyboardHandler = bind(&PerspectiveSample::handleKeyboard, this, placeholders::_1, placeholders::_2);
    eventHandler.mouseHandler = bind(&PerspectiveSample::handleMouse, this, placeholders::_1, placeholders::_2);
    eventHandler.touchHandler = bind(&PerspectiveSample::handleTouch, this, placeholders::_1, placeholders::_2);
    eventHandler.gamepadHandler = bind(&PerspectiveSample::handleGamepad, this, placeholders::_1, placeholders::_2);
    eventHandler.uiHandler = bind(&PerspectiveSample::handleUI, this, placeholders::_1, placeholders::_2);

    sharedEngine->getEventDispatcher()->addEventHandler(&eventHandler);

    camera.setType(scene::Camera::Type::PERSPECTIVE);
    camera.setFarPlane(1000.0f);
    camera.setPosition(Vector3(0.0f, 0.0f, -500.0f));
    layer.addCamera(&camera);
    addLayer(&layer);

    // floor
    floorSprite.initFromFile("floor.jpg");

    floor.addComponent(&floorSprite);
    layer.addChild(&floor);
    floor.setPosition(Vector2(0.0f, -100.0f));
    floor.setRotation(Vector3(TAU_4, 0.0f, 0.0f));

    // character
    characterSprite.initFromFile("run.json");
    characterSprite.play(true);

    character.addComponent(&characterSprite);
    layer.addChild(&character);
    character.setPosition(Vector2(10.0f, 0.0f));

    rotate.reset(new scene::Rotate(10.0f, Vector3(0.0f, TAU, 0.0f)));
    character.animate(rotate.get());

    guiCamera.setScaleMode(scene::Camera::ScaleMode::SHOW_ALL);
    guiCamera.setTargetContentSize(Size2(800.0f, 600.0f));
    guiLayer.addCamera(&guiCamera);
    addLayer(&guiLayer);

    guiLayer.addChild(&menu);

    backButton.setPosition(Vector2(-200.0f, -200.0f));
    menu.addWidget(&backButton);
}

bool PerspectiveSample::handleUI(ouzel::Event::Type type, const ouzel::UIEvent& event)
{
    if (type == Event::Type::UI_CLICK_NODE)
    {
        if (event.node == &backButton)
        {
            samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
        }
    }

    return true;
}

bool PerspectiveSample::handleKeyboard(ouzel::Event::Type type, const ouzel::KeyboardEvent& event)
{
    if (type == Event::Type::KEY_DOWN)
    {
        Quaternion rotation;

        switch (event.key)
        {
            case input::KeyboardKey::UP:
                rotation.setEulerAngles(Vector3(-TAU / 100.0f, 0.0f, 0.0f));
                break;
            case input::KeyboardKey::DOWN:
                rotation.setEulerAngles(Vector3(TAU / 100.0f, 0.0f, 0.0f));
                break;
            case input::KeyboardKey::LEFT:
                rotation.setEulerAngles(Vector3(0.0f, -TAU / 100.0f, 0.0f));
                break;
            case input::KeyboardKey::RIGHT:
                rotation.setEulerAngles(Vector3(0.0f, TAU / 100.0f, 0.0f));
                break;
            case input::KeyboardKey::ESCAPE:
                samples.setScene(std::unique_ptr<scene::Scene>(new MainMenu(samples)));
                break;
            default:
                break;
        }

        camera.setRotation(camera.getRotation() * rotation);
    }

    return true;
}

bool PerspectiveSample::handleMouse(ouzel::Event::Type type, const ouzel::MouseEvent& event)
{
    if (event.modifiers & LEFT_MOUSE_DOWN)
    {
        Quaternion rotation;

        rotation.setEulerAngles(Vector3(event.difference.y() / 2.0f,
                                        -event.difference.x() / 2.0f,
                                        0.0f));

        camera.setRotation(camera.getRotation() * rotation);
    }

    return true;
}

bool PerspectiveSample::handleTouch(ouzel::Event::Type type, const ouzel::TouchEvent& event)
{
    if (type == Event::Type::TOUCH_MOVE)
    {
        Quaternion rotation;

        rotation.setEulerAngles(Vector3(event.difference.y() / 2.0f,
                                        -event.difference.x() / 2.0f,
                                        0.0f));

        camera.setRotation(camera.getRotation() * rotation);
    }

    return true;
}

bool PerspectiveSample::handleGamepad(ouzel::Event::Type type, const ouzel::GamepadEvent& event)
{
    return true;
}
