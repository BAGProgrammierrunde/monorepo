#include "MainScene.hpp"

#include "Core/Structure/Device.hpp"

MainScene::MainScene() : Scene() {}

void MainScene::update() {
    if (Device::get().button1.isPressed()) Device::get().setScene<TestScene>();
}