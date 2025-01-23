# Pong_game

Let's create a Pong game using Arduino Uno R3, a joystick, and an 8x8 LED matrix. Here's a step-by-step guide to help you build and code this project.

**Components You'll Need:**

1.  **Arduino Uno R3:** The microcontroller board.
2.  **8x8 LED Matrix (2.1 inch):**  Often based on the MAX7219 chip. Ensure it has the necessary pins for connection (VCC, GND, DIN, CS, CLK).
3.  **Joystick Module:** An analog joystick with X and Y axis outputs and a button.
4.  **Jumper Wires:** To connect components (male-to-male and male-to-female recommended).
5.  **Breadboard (Optional but Recommended):** For easier prototyping and connections.
6.  **5V Power Supply (USB from computer is sufficient for testing):** To power the Arduino.
7.  **Resistors (Potentially, check your LED Matrix and Joystick modules specifications, often built-in):**  Usually not required for basic operation of these modules, but good to have on hand if needed.

**Connecting the Components:**

Here's how to connect the joystick and LED matrix to your Arduino Uno.

**1. Joystick Connection:**

Most joysticks have 5 pins.  Let's identify them:

*   **VCC (or 5V):** Power supply for the joystick.
*   **GND (or G):** Ground.
*   **VRx (or X):**  Analog output for the X-axis.
*   **VRy (or Y):**  Analog output for the Y-axis.
*   **SW (or BTN):** Switch (button) - we might use this later, but for basic pong, we'll focus on X-axis.

Connect them to the Arduino as follows:

| Joystick Pin | Arduino Uno Pin |
| :----------- | :-------------- |
| VCC          | 5V              |
| GND          | GND             |
| VRx          | Analog A0        |
| VRy          | Analog A1        |
| SW           | Digital 2       |  *(Optional - we may use this as a reset/start button later)*

**Wiring Diagram (Joystick):**

```
Joystick       Arduino Uno
---------      -----------
VCC ---------- 5V
GND ---------- GND
VRx ---------- A0
VRy ---------- A1
SW  ---------- D2 (Optional)
```

**2. LED Matrix Connection (MAX7219 Example):**

For an 8x8 LED Matrix module based on MAX7219, you will typically find these pins:

*   **VCC:** Power supply for the LED matrix.
*   **GND:** Ground.
*   **DIN:** Data In (Serial Data Input).
*   **CS (or LOAD):** Chip Select (or Load).
*   **CLK:** Clock (Serial Clock Input).

Connect them to the Arduino as follows:

| LED Matrix Pin | Arduino Uno Pin |
| :------------- | :-------------- |
| VCC            | 5V              |
| GND            | GND             |
| DIN            | Digital 12 (MOSI) |
| CS             | Digital 10 (SS)   |
| CLK            | Digital 13 (SCK)  |

**Wiring Diagram (LED Matrix MAX7219):**

```
LED Matrix (MAX7219)  Arduino Uno
--------------------  -----------
VCC ---------------- 5V
GND ---------------- GND
DIN ---------------- D12
CS  ---------------- D10
CLK ---------------- D13
```

**Complete Breadboard Setup (Conceptual):**

If you are using a breadboard, you would place the Arduino, joystick module, and LED matrix module on the breadboard and use jumper wires to make the connections as described above. This is a good way to prototype before making a more permanent setup.

**Arduino Code for Pong Game:**

Now, let's write the Arduino code. We'll need a library to easily control the MAX7219 LED matrix. A popular and easy-to-use library is `LedControl`.

**Installation of `LedControl` Library (if needed):**

1.  Open your Arduino IDE.
2.  Go to `Sketch` > `Include Library` > `Manage Libraries...`
3.  In the Library Manager, search for "LedControl".
4.  Find the `LedControl by Eberhard Fahle` library and click "Install".

**Code Explanation:**

1.  **Libraries and Definitions:**
    *   Includes `LedControl.h` for the LED matrix.
    *   Defines pins for LED matrix and joystick.
    *   Creates `LedControl` object `lc`.
    *   Defines game variables like `paddlePos`, `ballX`, `ballY`, and ball directions `ballDirX`, `ballDirY`.
    *   `gameSpeed` controls the game speed (smaller value = faster game).

2.  **`setup()` Function:**
    *   Initializes the LED matrix (wake up, set brightness, clear display).
    *   Sets the joystick button pin as `INPUT_PULLUP` (using internal pull-up resistor, so button press will be `LOW`).

3.  **`loop()` Function:**
    *   Uses `millis()` for timing to control game speed. `updateGame()` and `drawGame()` are called at intervals defined by `gameSpeed`.
    *   Checks for joystick button press to reset the game.

4.  **`updateGame()` Function:**
    *   **Reads Joystick:** Reads the analog value from `JOYSTICK_X_PIN` (A0).
    *   **Paddle Control:** `map()` function is used to map the joystick analog input (0-1023) to the paddle position (0-6). The range is inverted (`6, 0`) to make joystick movement intuitive.
    *   **Ball Movement:** Updates `ballX` and `ballY` based on `ballDirX` and `ballDirY`.
    *   **Collision Detection:**
        *   Checks for collisions with the top and bottom walls (`ballY <= 0 || ballY >= 7`).
        *   Checks for collision with the right wall (player side). If the ball's Y position is within the paddle's range, the ball bounces back (`ballDirX = -ballDirX`). Otherwise, the player misses, and `resetBall()` is called.
        *   Simple bounce off the left wall is implemented as a basic opponent/wall.

5.  **`drawGame()` Function:**
    *   Clears the LED matrix display using `lc.clearDisplay(0)`.
    *   **Draws Paddle:** Draws a vertical 2-LED paddle on the right side (column 7) based on `paddlePos`.
    *   **Draws Ball:** Draws a single LED at the `ballX`, `ballY` position.

6.  **`resetGame()` and `resetBall()` Functions:**
    *   `resetGame()` resets both paddle position and ball position.
    *   `resetBall()` resets the ball to the starting position and initial direction.

**How to Use:**

1.  **Assemble Hardware:** Connect all the components as described in the connection guide.
2.  **Install Library (if needed):** Install the `LedControl` library in your Arduino IDE.
3.  **Upload Code:** Copy and paste the Arduino code into your Arduino IDE and upload it to your Arduino Uno.
4.  **Play:** Once uploaded, the game should start. Move the joystick horizontally to control the right paddle. The ball will bounce around, and you need to try to hit it with the paddle. Pressing the joystick button (if connected) will reset the game.
5.  **Adjust:**
    *   Change `gameSpeed` in the code to adjust the game speed (smaller value = faster).
    *   Modify paddle size or game logic in the `updateGame()` and `drawGame()` functions to customize the game.

**Important Notes:**

*   **Joystick Calibration:** Analog joysticks might have slight variations. You might need to adjust the `map()` function parameters or add calibration code if your joystick's range is significantly different.
*   **LED Matrix Type:** This code is written assuming a common MAX7219 based 8x8 LED matrix. If you have a different type of LED matrix, you might need to use a different library and adapt the code accordingly.
*   **Simple Pong:** This is a very basic version of Pong.  You can expand upon this by adding:
    *   Scoring.
    *   AI for the left paddle (opponent).
    *   Sound effects (using a buzzer).
    *   More complex game logic and ball physics.
*   **Power:** Ensure your Arduino and LED matrix are getting sufficient power. USB from a computer is usually enough for this project.

This comprehensive guide should get you started with your Arduino Pong game! Have fun building and playing!
