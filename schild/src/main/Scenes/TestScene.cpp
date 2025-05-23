#include "TestScene.hpp"

TestScene::TestScene(String pName) : Scene() {
    Serial.println("Created TestScene: " + pName);
}

void TestScene::update() {
    Serial.println("Helloo from TestScene update :)");
}