//
//  Application.cpp
//  ouzel
//
//  Created by Elviss Strazdins on 20/12/15.
//  Copyright © 2015 Bool Games. All rights reserved.
//

#include "Application.h"

namespace ouzel
{    
    Application::~Application()
    {
        Engine::getInstance()->getEventDispatcher()->removeEventHandler(_eventHandler);
    }
    
    Settings Application::getSettings()
    {
        Settings settings;
        settings.size = ouzel::Size2(640.0f, 480.0f);
        settings.resizable = true;
        
        return settings;
    }
    
    void Application::begin()
    {
        _eventHandler = std::make_shared<EventHandler>();
        
        _eventHandler->keyDownHandler = std::bind(&Application::handleKeyDown, this, std::placeholders::_1, std::placeholders::_2);
        _eventHandler->mouseMoveHandler = std::bind(&Application::handleMouseMove, this, std::placeholders::_1, std::placeholders::_2);
        _eventHandler->touchBeginHandler = std::bind(&Application::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        _eventHandler->touchMoveHandler = std::bind(&Application::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        _eventHandler->touchEndHandler = std::bind(&Application::handleTouch, this, std::placeholders::_1, std::placeholders::_2);
        _eventHandler->gamepadButtonChangeHandler = std::bind(&Application::handleGamepadButtonChange, this, std::placeholders::_1, std::placeholders::_2);
        
        Engine::getInstance()->getEventDispatcher()->addEventHandler(_eventHandler);
        
        Engine::getInstance()->getRenderer()->setClearColor(Color(64, 0, 0));
        
        std::shared_ptr<Scene> scene(new Scene());
        Engine::getInstance()->getSceneManager()->setScene(scene);
        
        _layer = std::make_shared<Layer>();
        scene->addLayer(_layer);
        
        _sprite = std::make_shared<Sprite>();
        _sprite->initFromFile("run.json");
        _sprite->play(true);
        _layer->addChild(_sprite);
        
        std::shared_ptr<Sprite> fire(new Sprite());
        fire->initFromFile("fire.json");
        fire->play(true);
        fire->setPosition(Vector2(-100.0f, -100.0f));
        _layer->addChild(fire);
        
        std::shared_ptr<ParticleSystem> flame(new ParticleSystem());
        flame->initFromFile("flame.json");
        _layer->addChild(flame);
        
        _witch = std::make_shared<Sprite>();
        _witch->initFromFile("witch.png");
        _witch->setPosition(Vector2(100.0f, 100.0f));
        _witch->setColor(Color(128, 0, 255, 255));
        _layer->addChild(_witch);
        
        Engine::getInstance()->getInput()->startDiscovery();
    }
    
    void Application::handleKeyDown(const KeyboardEvent& event, std::shared_ptr<void> const& sender) const
    {
        Vector2 position = _sprite->getPosition();
        
        switch (event.key)
        {
            case KeyboardKey::UP:
                position.y += 10.0f;
                break;
            case KeyboardKey::DOWN:
                position.y -= 10.0f;
                break;
            case KeyboardKey::LEFT:
                position.x -= 10.0f;
                break;
            case KeyboardKey::RIGHT:
                position.x += 10.0f;
                break;
            case KeyboardKey::SPACE:
                _witch->setVisible(!_witch->getVisible());
                break;
            default:
                break;
        }
        
        _sprite->setPosition(position);
    }
    
    void Application::handleMouseMove(const MouseEvent& event, std::shared_ptr<void> const& sender) const
    {
        Vector2 worldLocation = _layer->screenToWorldLocation(event.position);
        _witch->setPosition(worldLocation);
    }
    
    void Application::handleTouch(const TouchEvent& event, std::shared_ptr<void> const& sender) const
    {
        Vector2 worldLocation = _layer->screenToWorldLocation(event.position);
        _witch->setPosition(worldLocation);
    }
    
    void Application::handleGamepadButtonChange(const GamepadEvent& event, std::shared_ptr<void> const& sender) const
    {
        Vector2 position = _layer->worldToScreenLocation(_witch->getPosition());
        
        switch (event.button)
        {
            case GamepadButton::DPAD_UP:
                position.y = event.value;
                break;
            case GamepadButton::DPAD_DOWN:
                position.y = -event.value;
                break;
            case GamepadButton::DPAD_LEFT:
                position.x = -event.value;
                break;
            case GamepadButton::DPAD_RIGHT:
                position.x = event.value;
                break;
            default:
                break;
        }
        
        Vector2 worldLocation = _layer->screenToWorldLocation(position);
        _witch->setPosition(worldLocation);
    }
}
