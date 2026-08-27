#include <LiquidCrystal.h>

// ==================== STATES ====================

enum State
{
    OPEN_SEA,
    ANCHOR_DROPPED,
    STORM,
    CHARYBDIS,
    WRECKED
};

State state = OPEN_SEA;

// ==================== PINS ====================

const int LDR_PIN = A0;
const int BUTTON_PIN = 2;
const int LED_PIN = 3;
const int BUZZER_PIN = 4;

const int TRIG_PIN = 9;
const int ECHO_PIN = 10;

// ==================== LCD ====================

LiquidCrystal lcd(11, 12, 5, 6, 7, 8);

// ==================== THRESHOLDS ====================

const int LIGHT_THRESHOLD = 512;
const int DISTANCE_THRESHOLD = 100;

// ==================== TIMERS ====================

unsigned long dangerStartTime = 0;
unsigned long ledTimer = 0;

bool ledState = LOW;

// ==================== ANCHOR ====================

bool anchorDropped = false;

// Button:
// HIGH = not pressed
// LOW  = pressed
int lastButtonState = HIGH;

// ==================== LCD ====================

State previousState = OPEN_SEA;

void displayState()
{
    lcd.clear();

    if (state == OPEN_SEA)
        lcd.print("OPEN SEA");

    else if (state == ANCHOR_DROPPED)
        lcd.print("ANCHOR DROPPED");

    else if (state == STORM)
        lcd.print("STORM");

    else if (state == CHARYBDIS)
        lcd.print("CHARYBDIS");

    else if (state == WRECKED)
        lcd.print("WRECKED");
}

// ==================== SETUP ====================

void setup()
{
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    lcd.begin(16, 2);
    lcd.print("OPEN SEA");

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

// ==================== LOOP ====================

void loop()
{
    // WRECKED is permanent
    if (state == WRECKED)
    {
        return;
    }

    // ==================== READ SENSORS ====================

    int lightValue = analogRead(LDR_PIN);

    // Ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);

    int distance = duration * 0.034 / 2;

    // ==================== BUTTON ====================

    int buttonState = digitalRead(BUTTON_PIN);

    // Detect a new button press
    if (buttonState == LOW && lastButtonState == HIGH)
    {
        // Toggle anchor
        anchorDropped = !anchorDropped;

        if (anchorDropped)
        {
            state = ANCHOR_DROPPED;
            dangerStartTime = 0;
        }
        else
        {
            state = OPEN_SEA;
            dangerStartTime = 0;
        }
    }

    lastButtonState = buttonState;

    // ==================== ANCHOR ====================

    // Anchor protects the ship
    if (anchorDropped)
    {
        state = ANCHOR_DROPPED;
    }

    // ==================== OPEN SEA ====================

    if (!anchorDropped && state == OPEN_SEA)
    {
        // Storm detected
        if (lightValue < LIGHT_THRESHOLD)
        {
            state = STORM;
            dangerStartTime = millis();

            // Start LED blinking from the beginning
            ledTimer = millis();
            ledState = LOW;
            digitalWrite(LED_PIN, LOW);
        }

        // Charybdis detected
        else if (distance < DISTANCE_THRESHOLD)
        {
            state = CHARYBDIS;
            dangerStartTime = millis();
        }
    }

    // ==================== STORM ====================

    if (state == STORM)
    {
        // Storm ended
        if (lightValue >= LIGHT_THRESHOLD)
        {
            state = OPEN_SEA;
            dangerStartTime = 0;
        }

        // Storm lasted 5 seconds
        else if (millis() - dangerStartTime >= 5000)
        {
            state = WRECKED;

            displayState();

            return;
        }
    }

    // ==================== LED ====================

    if (state == STORM)
    {
        if (millis() - ledTimer >= 500)
        {
            ledTimer = millis();
            ledState = !ledState;

            digitalWrite(LED_PIN, ledState);
        }
    }
    else
    {
        ledState = LOW;
        digitalWrite(LED_PIN, LOW);
    }

    // ==================== CHARYBDIS ====================

    if (state == CHARYBDIS)
    {
        // Charybdis ended
        if (distance >= DISTANCE_THRESHOLD)
        {
            state = OPEN_SEA;
            dangerStartTime = 0;
        }

        // Charybdis lasted 5 seconds
        else if (millis() - dangerStartTime >= 5000)
        {
            state = WRECKED;

            displayState();

            return;
        }
    }

    // ==================== BUZZER ====================

    if (state == CHARYBDIS)
    {
        digitalWrite(BUZZER_PIN, HIGH);
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);
    }

    // ==================== LCD ====================

    if (state != previousState)
    {
        displayState();
        previousState = state;
    }
}
