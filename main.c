#include <LedControl.h>

// LED Matrix Pins (for MAX7219)
#define DIN_PIN 12
#define CS_PIN 10
#define CLK_PIN 13
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1); // 1 device attached

// Joystick Pins
#define JOYSTICK_X_PIN A0
#define JOYSTICK_Y_PIN A1 // Not used for paddle control in this simple version
#define JOYSTICK_BUTTON_PIN 2 // Optional reset button

// Game Variables
int paddlePos = 3; // Initial paddle position (vertical, 0-7)
int ballX = 1;      // Ball X position (0-7)
int ballY = 3;      // Ball Y position (0-7)
int ballDirX = 1;   // Ball direction X (-1: left, 1: right)
int ballDirY = 1;   // Ball direction Y (-1: down, 1: up)

unsigned long lastTime = 0;
unsigned long gameSpeed = 200; // Milliseconds between frames (adjust for speed)

void setup() {
  // Initialize LED matrix
  lc.shutdown(0, false);       // Wake up display
  lc.setIntensity(0, 8);      // Set brightness (0=min, 15=max)
  lc.clearDisplay(0);         // Clear display

  // Joystick pins are input by default, but good practice to set
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP); // Use internal pull-up resistor for button
}

void loop() {
  if (millis() - lastTime > gameSpeed) {
    lastTime = millis();
    updateGame();
    drawGame();
  }

  // Optional: Reset game with joystick button
  if (digitalRead(JOYSTICK_BUTTON_PIN) == LOW) { // Button is pressed (LOW because of PULLUP)
    resetGame();
    delay(200); // Debounce delay
  }
}

void updateGame() {
  // Read joystick X-axis to control paddle
  int joystickXValue = analogRead(JOYSTICK_X_PIN);
  // Map joystick range (0-1023) to paddle movement (0-6 for paddle position, leaving space at top/bottom)
  paddlePos = map(joystickXValue, 0, 1023, 6, 0); // Invert range for joystick direction

  // Update ball position
  ballX += ballDirX;
  ballY += ballDirY;

  // Ball collision detection

  // Top and Bottom walls
  if (ballY <= 0 || ballY >= 7) {
    ballDirY = -ballDirY; // Reverse Y direction
  }

  // Right wall (Player side) - Paddle collision
  if (ballX >= 7) {
    if (ballY >= paddlePos && ballY <= paddlePos + 1) { // Simple 2-LED paddle height
      ballDirX = -ballDirX; // Bounce off paddle
      ballX = 7; // Keep ball at edge to avoid sticking
    } else {
      // Player Missed! For simplicity, just reset ball, no score in this basic version
      resetBall();
    }
  }

  // Left wall (AI or just bounce back) -  Simplified AI (just bounce off left wall for now)
  if (ballX <= 0) {
    ballDirX = -ballDirX; // Bounce off left wall
    ballX = 0; // Keep ball at edge
  }
}

void drawGame() {
  lc.clearDisplay(0);

  // Draw Paddle (2 LEDs vertical, right side)
  for (int i = 0; i < 2; i++) { // 2 LED paddle height
    if (paddlePos + i <= 7) { // Keep paddle within bounds
       lc.setLed(0, paddlePos + i, 7, true); // Column 7 (rightmost), rows based on paddlePos
    }
  }


  // Draw Ball
  lc.setLed(0, ballY, ballX, true);
}

void resetGame() {
  paddlePos = 3;
  resetBall();
}

void resetBall() {
  ballX = 1;
  ballY = 3;
  ballDirX = 1;
  ballDirY = 1;
}
