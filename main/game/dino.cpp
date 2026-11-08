#include "dino.h"

#include "assets/color.h"

#include <engine.h>
#include <esp_log.h>

#define TAG "Game"

float currentY = 0;
float velocity = 0;

int nextStep = 0;
bool buttonPressed = false;

// TODO Refactor
constexpr float scale = 1.f;

void Dino::handleJump(float deltaTime, bool buttonPressed) {
    if (isGrounded()) {
        if (buttonPressed) {
            m_Velocity = s_Force;
            m_JumpHeight += m_Velocity * deltaTime;
            ESP_LOGI(TAG, "Jump!");
        }
    } else {
        m_Velocity -= s_Gravity * deltaTime;
        m_JumpHeight += m_Velocity;

        if (isGrounded()) {
            m_JumpHeight = 0;
            m_Velocity = 0;
        }
    }

    m_CurrentY = ((Display::s_Height - DINO_HEIGHT * scale) - s_DinoScreenOffset) - m_JumpHeight;
}

bool Dino::isGrounded() {
    return (m_JumpHeight <= 0);
}

void Dino::updateStep() {
    m_CurrentStep++;
    if (m_CurrentStep == 12) {
        m_CurrentStep = 0;
    }
}

void Dino::drawDino() {
    GAL::draw_at(m_CurrentStep < 6 ? dino_rightstep : dino_leftstep, 0, 34, 36, 10, m_CurrentY, FOREGROUND_COLOR, BACKGROUND_COLOR, scale, true);
}

bool Dino::checkCollision(Device& device) {
    constexpr int bodyCount = 12;
    constexpr int x = 0;
    constexpr int y = 1;
    constexpr int displayWidth = Display::s_Width;
    constexpr int dinoScreenX = 10;
    constexpr int dinoBody[bodyCount][2] = {
        {1, 21},
        {3, 23},
        {4, 24},
        {5, 25},
        {6, 27},
        {7, 27},
        {8, 28},
        {21, 24},
        {22, 22},
        {26, 18},
        {29, 12},
        {32, 9},
    };

    constexpr int footCount = 4;

    constexpr int dinoLeftStepFoot[footCount][2] = {
        {9, 32},
        {11, 32},
        {17, 34},
        {19, 34},
    };

    constexpr int dinoRightStepFoot[footCount][2] = {
        {9, 34},
        {11, 34},
        {17, 32},
        {19, 32},
    };

    const auto toBufferIndex = [&](int screenX, int screenY) {
        return screenY * displayWidth + screenX;
    };

    const auto isInRangeUpTo = [&](int index) {
        return index / displayWidth >= Display::s_Height - 20;
    };

    for (int i = 0; i < bodyCount; ++i) {
        const int index = toBufferIndex(dinoScreenX + dinoBody[i][x], m_CurrentY + dinoBody[i][y]);
        if (isInRangeUpTo(index)) continue;
        if (device.getDisplay().getPixel(index) == FOREGROUND_COLOR) {
            //ESP_LOGI(TAG, "Body %d", index);
            return true;
        }
    }

    // TODO move to function
    // TODO magic number
    if (m_CurrentStep < 6) {
        // right
        for (int i = 0; i < footCount; ++i) {
            const int index = toBufferIndex(dinoScreenX + dinoRightStepFoot[i][x], m_CurrentY + dinoRightStepFoot[i][y]);
            if (isInRangeUpTo(index)) continue;
            if (device.getDisplay().getPixel(index) == FOREGROUND_COLOR) {
                //ESP_LOGI(TAG, "Right %d", index);
                return true;
            }
        }
    } else {
        for (int i = 0; i < footCount; ++i) {
            const int index = toBufferIndex(dinoScreenX + dinoLeftStepFoot[i][x], m_CurrentY + dinoLeftStepFoot[i][y]);
            if (isInRangeUpTo(index)) continue;
            if (device.getDisplay().getPixel(index) == FOREGROUND_COLOR) {
                //ESP_LOGI(TAG, "Left %d", index);
                return true;
            }
        }
    }
    return false;
}
